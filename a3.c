#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

SEMAPHORE mutex;// TA is mutex (1 spot)
// queue is semaphore of 3 spots


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
        // wait for TA to be available wait(mutex)
        // getHelp()
        // signal TA is free
        // signal queue up (one less person in queue)
    }
}

askForHelp() {
    // INVOKED BY STUDENT
    // if queue full, program()
    // otherwise join queue
    // if at front of queue, check if TA awake
    // if awake, wait(mutex)
    // if asleep, signal(mutex)

    //yay!
}





// each thread would need to have a set amount of time/progress that
// is need to be done, and it goes down until done

// QUESTIONS:
// what is help()
// how do we use actual threads in this like lol