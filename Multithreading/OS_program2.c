/*
Varun Subramanya
*/



#include "prod_cons_MT.h"

// ********external and global variables and structs***************
int PRODUCER_NUMBER, CONSUMER_NUMBER = 0;
int BUFFER_SIZE;
queue Queue;               




// ********** Main Loop *****************
int main ( int argc, char *argv[]) 
{
    int FD =0;
    int lastID,cons_i,prod_i,P_i,C_i, I=0;  // used for loops 
	BUFFER_SIZE = atoi(argv[1]);    
	PRODUCER_NUMBER   = atoi(argv[2]);      // user inputs Producer, 	Consumer and Queue size 
	CONSUMER_NUMBER  = atoi(argv[3]);
  

    init(&Queue , (size_t)BUFFER_SIZE) ;    // initialization of global struct 
    math_buffer(BUFFER_SIZE*2,CONSUMER_NUMBER );     // math func - to distribute data to given consumer threads  

    printf("Buffer Size: %d ", BUFFER_SIZE);                           
    printf("Producers: %d , Consumers: %d\n\n", PRODUCER_NUMBER, CONSUMER_NUMBER);    
   
    // Initialize thread IDs
    pthread_t ConsumerID[CONSUMER_NUMBER];
    pthread_t ProducerID[PRODUCER_NUMBER];
 


    for(prod_i=0; prod_i<PRODUCER_NUMBER; prod_i++){
       
       printf("Main: Started Producer %d\n",prod_i);
       sleep(0.5);                                                                // general sleep func for delay  
       FD = pthread_create( &ProducerID[prod_i], NULL, Producer_thread, &prod_i); // create producer threads 
       if(FD!=0){ perror(" Producer Thread Creation Failed"); exit(EXIT_FAILURE); } // Failure checker for failed thread action
    }
  
   // ----------  Create Thread  --------------------------
   for(cons_i=0; cons_i<(CONSUMER_NUMBER-1); cons_i++){

        printf("Main: Started Consumer %d\n",cons_i);
        FD = pthread_create( &ConsumerID[cons_i], NULL, Consumer_thread, &cons_i);  // create consumer threads 
        if(FD!=0){ perror(" Consumer Thread Creation Failed"); exit(EXIT_FAILURE); } // Failure checker for failed thread action
        sleep(0.5);                                                                 // delay
    }
        sleep(0.5);  
        lastID = CONSUMER_NUMBER-1;                                       					// last consumer thread with different loop times
        printf("Main: Started Consumer %d\n",lastID);
        FD = pthread_create( &ConsumerID[lastID], NULL, Consumer_thread, &lastID);  // create the last consumer thread that takes care the remaining data in the Queue
        if(FD!=0){ perror("Consumer Thread Creation Failed"); exit(EXIT_FAILURE); }
        

  
   // ----------  Thread Finished  -------------------------
     for(int P_i=0; P_i<PRODUCER_NUMBER; P_i++){
        printf("joined loop %d\n ",P_i);
       FD = pthread_join(ProducerID[P_i], NULL);                                          // wait for each consumer thread to be joined
       if(FD!=0){ perror("Failed producer Thread to Join"); exit(EXIT_FAILURE); }
       printf("MAIN: producer %d joined\n", P_i);
    }


      for(C_i=0; C_i<CONSUMER_NUMBER; C_i++){
       FD = pthread_join(ConsumerID[C_i], NULL);                                         // wait for each consumer thread to be joined
       if(FD!=0){ perror("Failed consumer Thread to Join"); exit(EXIT_FAILURE);}
       printf("MAIN: consumer %d joined\n", C_i);
    }

 
    free(Queue.dataSpace);         // release memory back to the system
    pthread_exit(NULL);            // exits remaining threads
 

    // ------ end of main code ------
    printf("Main: Program Completed\n");

  return 0;
}







