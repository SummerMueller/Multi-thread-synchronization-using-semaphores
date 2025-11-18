//
// Example from: http://www.amparo.net/ce155/sem-ex.c
//
// Adapted using some code from Downey's book on semaphores
//
// Compilation:
//
//       g++ main.cpp -lpthread -o main -lm
// or 
//      make
//

#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */
#include <iostream>
using namespace std;

/*
 This wrapper class for semaphore.h functions is from:
 http://stackoverflow.com/questions/2899604/using-sem-t-in-a-qt-project
 */
class Semaphore {
public:
    // Constructor
    Semaphore(int initialValue)
    {
        sem_init(&mSemaphore, 0, initialValue);
    }
    // Destructor
    ~Semaphore()
    {
        sem_destroy(&mSemaphore); /* destroy semaphore */
    }
    
    // wait
    void wait()
    {
        sem_wait(&mSemaphore);
    }
    // signal
    void signal()
    {
        sem_post(&mSemaphore);
    }
    
    
private:
    sem_t mSemaphore;
};

class Lightswitch {
private:
    int counter;
    Semaphore mutex;

public:
    Lightswitch(): counter(0), mutex(1) {}

    void lock(Semaphore &semaphore) {
        mutex.wait();
        counter++;
        if (counter == 1) {
            semaphore.wait();
        }
        mutex.signal();
    }

    void unlock(Semaphore &semaphore) {
        mutex.wait();
        counter --;
        if (counter == 0) {
            semaphore.signal();
        }
        mutex.signal();
    }
};



/* global vars */
const int bufferSize = 5;
const int numConsumers = 3; 
const int numProducers = 3; 

// Example globals
Semaphore Mutex(1);
Semaphore Spaces(bufferSize);
Semaphore Items(0);

// Problem 1 globals
Lightswitch readSwitch;
Semaphore roomEmpty(1);
Semaphore turnstile(1);

void *Writer1 (void *threadID) {

    int x = (long)threadID;

    while (1) {
        sleep(3);
        turnstile.wait();
        roomEmpty.wait();
            printf("Reader %d: Reading.\n", x);
            fflush(stdout);
        turnstile.signal();
        roomEmpty.signal();
    }
}

void *Reader1 (void *threadID) {

    int x = (long)threadID;

    while (1) {
        turnstile.wait();
        turnstile.signal();
        readSwitch.lock(roomEmpty);
            printf("Writer %d: Writing.\n", x);
            fflush(stdout);
        readSwitch.unlock(roomEmpty);
        sleep(5);
    }
}

/*
    Producer function 
*/
void *Producer ( void *threadID )
{
    // Thread number 
    int x = (long)threadID;

    while( 1 )
    {
        sleep(3); // Slow the thread down a bit so we can see what is going on
        Spaces.wait();
        Mutex.wait();
            printf("Producer %d adding item to buffer \n", x);
            fflush(stdout);
        Mutex.signal();
        Items.signal();
    }

}

/*
    Consumer function 
*/
void *Consumer ( void *threadID )
{
    // Thread number 
    int x = (long)threadID;
    
    while( 1 )
    {
        Items.wait();
        Mutex.wait();
            printf("Consumer %d removing item from buffer \n", x);
            fflush(stdout);
        Mutex.signal();
        Spaces.signal();
        sleep(5);   // Slow the thread down a bit so we can see what is going on
    }

}

void problem0() {
    pthread_t producerThread[ numProducers ];
    pthread_t consumerThread[ numConsumers ];

    // Create the producers 
    for( long p = 0; p < numProducers; p++ )
    {
        int rc = pthread_create ( &producerThread[ p ], NULL, 
                                  Producer, (void *) (p+1) );
        if (rc) {
            printf("ERROR creating producer thread # %d; \
                    return code from pthread_create() is %d\n", p, rc);
            exit(-1);
        }
    }

    // Create the consumers 
    for( long c = 0; c < numConsumers; c++ )
    {
        int rc = pthread_create ( &consumerThread[ c ], NULL, 
                                  Consumer, (void *) (c+1) );
        if (rc) {
            printf("ERROR creating consumer thread # %d; \
                    return code from pthread_create() is %d\n", c, rc);
            exit(-1);
        }
    }

    printf("Main: program completed. Exiting.\n");


    // To allow other threads to continue execution, the main thread 
    // should terminate by calling pthread_exit() rather than exit(3). 
    pthread_exit(NULL); 

}

void problem1() {
    pthread_t writerThread[5];
    pthread_t readerThread[5];

    // Create the writers
    for( long p = 0; p < 5; p++ )
    {
        int rc = pthread_create ( &writerThread[ p ], NULL, 
                                  Writer1, (void *) (p+1) );
        if (rc) {
            printf("ERROR creating writer thread # %d; \
                    return code from pthread_create() is %d\n", p, rc);
            exit(-1);
        }
    }

    // Create the readers
    for( long c = 0; c < 5; c++ )
    {
        int rc = pthread_create ( &readerThread[ c ], NULL, 
                                  Reader1, (void *) (c+1) );
        if (rc) {
            printf("ERROR creating reader thread # %d; \
                    return code from pthread_create() is %d\n", c, rc);
            exit(-1);
        }
    }

    //printf("Main: program completed. Exiting.\n");


    // To allow other threads to continue execution, the main thread 
    // should terminate by calling pthread_exit() rather than exit(3). 
    pthread_exit(NULL); 
}


int main(int argc, char **argv )
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <number>" << endl;
        return 1;
    }

    int choice = atoi(argv[1]);

    switch(choice) {
        case 0:
            problem0();
            break;
        case 1:
            problem1();
            break;
    }
}
