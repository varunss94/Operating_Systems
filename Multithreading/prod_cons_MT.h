/*
Varun Subramanya
*/


#ifndef PROD_CONS_MT_H
#define PROD_CONS_MT_H


#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

// *********** Memory orgnanization: Queue structure is defined here*********

typedef struct {
    size_t size;    // size of each dataSpace
    int head;       // position in the queue that program can read
    int tail;       // position in the queue that program can write
    int count;      // number of dataSpaces in queue
  int *dataSpace;   // array of integers
pthread_mutex_t Q_locker;  // mutex 
pthread_cond_t  Empty_Q ;  // condition variables
} queue;



//************Prototype Functions are defined here*****************
void * Consumer_thread(void * ptr);                        
void * Producer_thread(void * ptr);
void init(queue *queuePtr, size_t size);      // initialization of the queue struct
int randNum(void );                           // calculating random values based on CPU Clock ticks
int isFull(void);                             // confirms if the queue is full returning 1 for full and 0 for not full
int isEmpty(void);                            // confirms if the queue is empty returning 1 for empty and 0 for not empty
int Push_Q(queue *queuePtr, int data);        // Pushing a data element in the  data queue (circular buffer)
int Pop_Q(queue *queuePtr);                   // Poping a data element from the data queue (circular buffer)
int roundNo(float num);						  // standard round() function
void math_buffer(float buffer_size,float consumer_num ); // custom math function - distribute data to given consumer threads  



#endif
