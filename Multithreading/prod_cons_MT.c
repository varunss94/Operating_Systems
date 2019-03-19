/*
Varun Subramanya
*/


#include "prod_cons_MT.h"

// *********extenral and global variables and structs*******
extern queue Queue; 
extern int BUFFER_SIZE,CONSUMER_NUMBER;
int Data_Counter=0;
int norm_cons, last_cons, consumedHead , pushedTail=0; 




//******Calculating Random values based on CPU Clock ticks*********
void init(queue *queuePtr, size_t size) {
    queuePtr->size = size;                             // set queue size
    queuePtr->head = 0;  
    queuePtr->tail = 0;   
    queuePtr->count = 0;                               // initialize count
    queuePtr->dataSpace = malloc(sizeof(int) * size);  // size to dataSpace type
    pthread_cond_init(&(queuePtr->Empty_Q), NULL);     // initialize mutex 
    pthread_mutex_init(&(queuePtr->Q_locker), NULL);   // initialize condition variable
  }




// if full return 1 (true)
int isFull(void) {
    if (Data_Counter == BUFFER_SIZE) {
        return 1;  // Full Buffer
    } else {
        return 0;  // Not full buffer
    }
}

// if empty return 1 (true)
int isEmpty(void) {
    if (Data_Counter == 0) {
        return 1;  // Empty Buffer
    } else {
        return 0;  // Not empty buffer
    }
}



// ***********Store Data in Queue****************
int Push_Q(queue *queuePtr, int data) {
    
    if (isFull()) {  // check to see if queue full
        printf("queue full!\n");
        return 0;
    } else {

       if(queuePtr->tail==(BUFFER_SIZE)) {
         queuePtr->tail=0;
       } 

       queuePtr->dataSpace[queuePtr->tail] = data;   // store data
       Data_Counter++;
       pushedTail = queuePtr->tail;
       queuePtr->tail++;
}}




// ************ Get Data from Queue*****************
int Pop_Q(queue *queuePtr) {
    int dataEl;
    if (isEmpty()) {
        printf("No items in queue!\n");
        return 0;
    } else {
        consumedHead = queuePtr->head;
        dataEl = queuePtr->dataSpace[queuePtr->head];   // getting the data element from queue

          if(queuePtr->head == (BUFFER_SIZE -1)) {
             queuePtr->head =0;

          } else {
              queuePtr->head++;
          } 
        Data_Counter--;
}}

// ------------- Consumer Thread ---------------------
void *Consumer_thread( void * ptr ){

  int ThreadID_C = *(int*)ptr;             // getting thread ID
  int loopCount_C = norm_cons; 
  int val,wakeupFlag=0;

  if (ThreadID_C == (CONSUMER_NUMBER-1))loopCount_C = last_cons;
  printf("C%d: Consuming %d values\n",ThreadID_C, loopCount_C);   
 
 sleep(1);
for(int C0=0; C0<loopCount_C; C0++){      // infinte loop so make it for for loopCount_C
  
   pthread_mutex_lock(&Queue.Q_locker);                                    // Lock access to the Queue
   while (Data_Counter == 0) {                                             // queue is empty
     printf ("C%d: Blocked due to empty buffer\n",ThreadID_C);
     wakeupFlag = pthread_cond_wait(&Queue.Empty_Q,&Queue.Q_locker);       // Sleep and wait if the queue is empty
     if(wakeupFlag ==0)printf("C%d: Done waiting on empty buffer\n", ThreadID_C);
   }
  
  val = Pop_Q(&Queue);
  printf("C%d: Reading %d from position %d \n",ThreadID_C,val, consumedHead);
  
  pthread_cond_signal(&Queue.Empty_Q);                               // Signal Producer threads that there is available space
  pthread_mutex_unlock(&Queue.Q_locker);                             // Unlock queue access 
  
  }
  printf("C%d: Exiting\n",ThreadID_C);
  pthread_exit((void*)ptr);
  
}

// ------------- Producer Thread ---------------------
void *Producer_thread( void *ptr ){

  int ThreadID_P = *(int*)ptr;       // getting thread ID
  ThreadID_P--;
  int loopCount_P = BUFFER_SIZE*2; 
  int Data, output, wakeupFlag =0;

  printf("P%d: Producing %d values\n", ThreadID_P ,loopCount_P );  // debugging

  for(int C1=0; C1<loopCount_P; C1++){   // infinte loop  -> make it "for loop" to run  loopCount_P  times
  
     pthread_mutex_lock(&Queue.Q_locker);                                            // lock access to the queue
     while (Data_Counter == BUFFER_SIZE){                                            // queue is full
        printf ("P%d: Blocked due to full buffer\n",ThreadID_P);
        wakeupFlag = pthread_cond_wait(&Queue.Empty_Q,&Queue.Q_locker);              // Sleep and wait if the queue is FULL
        if(wakeupFlag==0)printf("P%d: Done waiting on full buffer\n", ThreadID_P); 
      }    

  Data = randNum();
  output = Push_Q(&Queue,Data);
  printf("P%d: Writing %d to position %d \n",ThreadID_P, Data, pushedTail);
  pthread_cond_signal(&Queue.Empty_Q);                    // Signal Consumer threads that the buffer is filled up
  pthread_mutex_unlock(&Queue.Q_locker);                  // Unlock queue access 
}
 printf("P%d: Exiting\n",ThreadID_P);
 pthread_exit((void*)ptr);

}


// --- consumer computations ------- 
int roundNo(float num) {
    return num < 0 ? num - 0.5 : num + 0.5;
}


void math_buff(float buffer_size,float consumer_num )
{

float   division;
int     round_division;
float   extra_share;
int     final_value;

division       = buffer_size/consumer_num;
round_division = buffer_size/consumer_num;
extra_share    = division - round_division;

float  final_value1    =  extra_share*consumer_num;
int final_value2 = roundNo(final_value1);
final_value = final_value2+ round_division;

norm_cons= round_division;
last_cons= final_value;

printf("norm_cons  %d,  last_cons: %d\n",norm_cons, last_cons );   // debugging 
}


int randNum(void ) {
  srand(clock());                  // seed random number generator from system clock ticks
  int rNum = rand() % (10 + 1);    // choose random values from 1 to 10
  return rNum;
}

