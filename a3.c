#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;// TA is mutex (1 spot)
sem_t waitingLine;// queue is semaphore of 3 spots
pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER;//to ensure only one thread at a time changes studentsInLine
int studentsInLine = 0;//can't see if waiting line has no running threads or one (both returns 0)


// Perhaps the best option for simulating students programming—as well as the TA 
// providing help to a student—is to have the appropriate threads sleep for a random period 
// of time.

// set amount of time for each student thread to 'live'
// random sleep (programming) time
// try to join queue -> get helped till finish

help() {
    // INVOKED BY TA
    // this the the TA's loop
    for (;;) {
        // check if someone in queue
        if (studentsInLine == 0) {
        // wait for TA to be available wait(mutex): decrements the semaphore by 1. If the value is 0, the thread blocks until it can decrement.
            sem_wait(&mutex);
            pthread_mutex_lock(&counterMutex);
            getHelp()
            studentsInLine--;
            pthread_mutex_unlock(&counterMutex);
        // signal TA is free: increments the semaphore by 1. 
            sem_post(&mutex);
        // signal queue up (one less person in queue)
            sem_wait(&waitingLine);
        }else{
            sem_wait(&mutex);
        }
    }
}

askForHelp() {
    // INVOKED BY STUDENT
    // if queue full, program()
    if (studentsInLine == 3){
        program();
    }
    else{
    // otherwise join queue
        sem_post(&waitingLine);
        pthread_mutex_lock(&counterMutex);
        studentsInLine++;
        pthread_mutex_unlock(&counterMutex);
    // if at front of queue, wake TA up
        if(studentsInLine == 1){
    // if awake, wait(mutex)
            sem_wait(&mutex);
    // if asleep, signal(mutex)
            sem_post(&mutex);
        }
    }

    //yay!
}

getHelp() {
}

program() {
}

int main(){
    sem_init(&mutex, 0, 1);              
    sem_init(&waitingLine, 0, 3);
}



// each thread would need to have a set amount of time/progress that
// is need to be done, and it goes down until done

// QUESTIONS:
// what is help()
// how do we use actual threads in this like lol