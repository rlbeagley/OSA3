#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;// TA is mutex (1 spot)
sem_t availableWaitingSpots;// queue is semaphore of 3 spots
pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER;//to ensure only one thread at a time changes studentsInLine
int studentsInLine = 0;//can't see if waiting line has no running threads or one (both returns 0)
int NUM_STUDENTS = 5; // const that can be changed for number of student threads

// Students cycle between programming and asking for help
// Program will end once students get help (just so it doesn't infinitely run)

// get helped between 1 and 3 seconds
void getHelp() {
    sleep(rand() % 1 + 3);
}

// program for between 1 and 10 seconds
void program(int id) {
    sleep(rand() % 10 + 1);
    printf("Student %d: I am done programming\n", id);
}


void *help(void *arg) {
    // INVOKED BY TA
    for (;;) {
        // check if someone in queue
        pthread_mutex_lock(&counterMutex);
        if (studentsInLine > 0) {
            studentsInLine--;
            pthread_mutex_unlock(&counterMutex);

            printf("TA: Helping a student. %d remaining in queue.\n", studentsInLine);
            getHelp();

            printf("TA: I am done helping the student.\n");
            sem_post(&mutex);// TA is free again
            sem_post(&availableWaitingSpots); // release the student, increment available spots

        } else {
            pthread_mutex_unlock(&counterMutex);
            printf("TA: No students, going to sleep\n");
            sem_wait(&mutex); // TA sleeps if nobody in line
        }
    }
}

void *askForHelp(void *arg) {
    int id = *(int *)arg;
    // INVOKED BY STUDENT
    
    for (;;) {
        program(id);

        printf("Student %d: I want help from the TA\n", id);

        pthread_mutex_lock(&counterMutex);
        if (studentsInLine == 3) {
            pthread_mutex_unlock(&counterMutex);
            printf("Student %d: No chairs available, going back to programming\n", id);
            continue;
        }

        studentsInLine++;
        

        // wake TA if I'm the first in line
        if (studentsInLine == 1) {
            printf("Student %d: TA is sleeping, waking them up\n", id);
            sem_post(&mutex);
        }
        pthread_mutex_unlock(&counterMutex);

        sem_wait(&availableWaitingSpots); // take a spot in waitin room

        return NULL;

    }
}

int main(){
    srand(time(NULL));

    sem_init(&mutex, 0, 0);
    sem_init(&availableWaitingSpots, 0, 3);

    // create TA thread that runs help
    pthread_t ta;
    pthread_create(&ta, NULL, help, NULL);

    // create student threads that run ask for help, number them
    pthread_t students[NUM_STUDENTS];
    int ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; i++) {
        ids[i] = i + 1;
        pthread_create(&students[i], NULL, askForHelp, &ids[i]);
    }

    for (int i = 0; i < NUM_STUDENTS; i++)
        pthread_join(students[i], NULL);

    sleep(10); // allow threads to finish

    return 0;
}

