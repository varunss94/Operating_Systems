/*
Varun Subramanya
*/

#include "sched_sim.hpp"
#define DELTA_Q_SIZE 140

using namespace std;

// push to the tail in the queue
void push( Q_struct *queue, int data) {
	queue->Q_space[queue->tail++] = data;
}

// pop from the head of the queue 
int pop(Q_struct *queue) {
	int x = queue->Q_space[queue->head++];	
	return x;
}

// find the process in the queue which has the minimum burst time
int MinBurst(Q_struct *Q, vector<Process> &Processes){
   int minB = 1000;  // random maximum burst time number to get started
   int minB_Position = 0;

	for(int Q_location = Q->head; Q_location <= Q->tail; Q_location++) {  
		if (Processes[Q->Q_space[Q_location]].CPUburst < minB ) {
			minB = Processes[Q->Q_space[Q_location]].CPUburst;
			minB_Position = Q_location;    
		}
	} 
	return Q->Q_space[minB_Position];    // return PID with lowest Burst time
}


// check if a new process arrived based on the arrival time
int Arrived_Flag(int time, int Total_Processes_Num, vector<Process> &Processes){
	int arrived_PID = 0;
	for(int t = 0; t < Total_Processes_Num; t++){
		if (time == Processes[t].arrivalTime ){    
			arrived_PID=t;
			break;
		}
	}
	return arrived_PID;
}


// allocating memory for the queue and initializing the queue 
void Queue_Init(Q_struct *Q, int Total_Processes_Num, vector<Process> &Processes){
	Q->head = 0;
	Q->tail = 0;
	Q->Q_space = (int *)malloc(sizeof(int) * (Total_Processes_Num + DELTA_Q_SIZE));

	for(int t = Q->head; t < Total_Processes_Num; t++){
		if(Processes[t].arrivalTime == 0) {    
			push(Q, Processes[t].pid);
	}} 
}


// sorting the process IDs in the queue with the corresponding bursts
void SortQ(Q_struct *queue, vector<Process> &Processes){
   int swap = 0;
   int b_swap = 0;
   int size = queue->tail-queue->head;
   int Burst[size+10];

	if (size > 1) { // do sort if you have more than one element in the queue
		for(int k = queue->head; k<queue->tail; k++ ){
			Burst[k] = Processes[queue->Q_space[k]].CPUburst; 
		}
        // sorting operation 
		for (int r = queue->head; r<queue->tail-1;   r++){
			for (int j = queue->head; j<queue->tail-1; j++){
				
				if( Burst[j] > Burst[j+1] ){ 
				   
				    // sort Burst time     
					b_swap = Burst[j];
					Burst[j] = Burst[j+1];
					Burst[j+1] = b_swap;
                
                    // sort PID
					swap = queue->Q_space[j];
					queue->Q_space[j] = queue->Q_space[j+1];
					queue->Q_space[j+1] =swap;
		}}}
	} // end of size check
}

// print the content of the current queue
void Q_print(Q_struct *queue, vector<Process> &Processes, char *outputFile){
	ofstream output;
	output.open(outputFile, fstream::app);
	output << "Ready queue: ";
	for (int a = queue->head; a<queue->tail; a++){  
		if (a == queue->head || a == queue->tail) {
			output << queue->Q_space[a];
		} else {
			output << "-" << queue->Q_space[a];
		}       
	}
	output << endl;
}

// get the sum of all the processes burst time 
int sum_processes_burst(int numLine, vector<Process> &Processes){
    int sum_line_countr = numLine;
    int sum_countr = 0;
    int sum_cpu_burst = 0;

    while(sum_line_countr > 0) {
      sum_cpu_burst = sum_cpu_burst + Processes[sum_countr].CPUburst;
      sum_countr++;
      sum_line_countr--; 
    }
   return sum_cpu_burst;
}

// get the number of the remaining processes
int number_process_with_bursts(int number_lines, vector<Process> &Processes){
	int process_counter = 0;
	for(int i = 0; i < number_lines ; i++){
		if(Processes[i].CPUburst > 0) process_counter++;
	}
	return process_counter;
}

// count the waiting cycle for each process
void TimeLogger(int total_Processes, int curr_PID, vector<Process> &Processes){
	int loopCount;
	for(loopCount = 0; loopCount < total_Processes; loopCount++ ){
		if(( Processes[curr_PID].pid != loopCount )&&(Processes[loopCount].CPUburst>0)){
			Processes[loopCount].waitingTime++;
	}}
}

bool sortByArrivalTime(const Process &lhs, const Process &rhs) { 
	return lhs.arrivalTime < rhs.arrivalTime; 

}
bool sortByCPUburst(const Process &lhs, const Process &rhs) { 
	return lhs.CPUburst < rhs.CPUburst; 

}
bool sortByPriority(const Process &lhs, const Process &rhs) { 
	return lhs.priority < rhs.priority; 
}
bool sortByValue(const Comparison &lhs, const Comparison &rhs) { 
	return lhs.value < rhs.value; 
}
void runFCFS(vector<Process> processes, int timeStep, int numProc, char *outputFile, float stats[]) {
	ofstream output;
	output.open(outputFile, fstream::out);

	// variable to hold the end time of the previous process
	int lastEnd = 0;
	// variable to keep track of the process number
	int procNum = 0;
	// simulation time
	int time = 0;
	// variables for algorithm summary
	float AvgWaitTime = 0;
	float AvgTurnArndTime = 0;
	// vector to hold pids for ready queue
	vector<int> readyQueue;

	// calculate start and end times for FCFS
	for (int i = 0; i < numProc; i++) {
		if (i == 0) {
			processes[i].startTime = processes[i].arrivalTime;
			processes[i].endTime = processes[i].CPUburst;
			lastEnd = processes[i].endTime;
		} else {
			processes[i].startTime = lastEnd + 1;
			processes[i].endTime = processes[i].startTime + processes[i].CPUburst - 1;
			lastEnd = processes[i].endTime;
		}
		// calculate turn around time
		processes[i].turnAroundtime = processes[i].endTime - processes[i].arrivalTime;
	}

	// print the header for the FCFS algorithm
	output << "***** FCFS Scheduling *****" << endl;

	// run the simulation
	while (1) {
		// if all processes have been evaluated, break out of the loop
		if (procNum == numProc) {
			break;
		}
		// loop through the remaining processes to see what needs to be put on the ready queue
		for (int i = 0; i < numProc; i++) {
			// if they have arrived place them in the ready queue for sorting
			if (time == processes[i].arrivalTime) {
				readyQueue.push_back(i);
			}
			// if the process hasn't arrived yet and the end time has not elapsed, add it to ready queue
			 else if ((procNum != i) && (time < processes[i].endTime) && (time >= processes[i].arrivalTime)) {
				// add it to the ready queue			
				readyQueue.push_back(i);
			}

		}

		if (!(time % timeStep)) {
			// print the time
			output << "t = " << time << endl;

			// print out start of a new process
			if (time == processes[procNum].startTime - 1) {
				output << "CPU: Loading process " << procNum << " (CPU burst = " << processes[procNum].CPUburst << ")" << endl;
			}

			// otherwise print the status of the current process or next process to be started
			if (time == processes[procNum].endTime && procNum != numProc - 1) {
				output << "CPU: Finishing process " << procNum << "; loading process " << procNum + 1 << " (CPU burst = " << processes[procNum + 1].CPUburst << ")" << endl;
			}
			else if (time >= processes[procNum].startTime) {
				if (processes[procNum].endTime - time == 0) {
					output << "CPU: Finishing process " << procNum << endl;
						
				} else {
					output << "CPU: Running process " << procNum << " (CPU burst = remaining " << processes[procNum].endTime - time << ")" << endl;
				}
			}
		}


		if (!(time % timeStep)) {
			// print ready queue contents
			if (!readyQueue.empty()) {
				output << "Ready queue: ";
			} else {
				output << "Ready queue: empty";
				output << endl;
			}

			// loop through the ready queue
			for (int j = 0; j < int(readyQueue.size()); j++) {
				if (j + 1 != int(readyQueue.size())) {
					output << readyQueue[j] << '-';
				}
				else {
					output << readyQueue[j] << endl;
				}
			}

			output << endl;

		}
		// reset the ready queue for next iteration
		readyQueue.clear();

		// if the process is finished evaluate the next process
		if (time == processes[procNum].endTime) {
			procNum++;
		}

		// increment the time
		time++;

	}

	// print out algorithm summary
	output << "*********************************************************" << endl;
	output << "FCFS Summary (WT = wait time, TT = turnaround time):" << endl;
	output << "\nPID\t\tWT\t\tTT" << endl;
	
	for (int i = 0; i < numProc; i++) {
		AvgWaitTime = (processes[i].startTime - processes[i].arrivalTime - 1) + AvgWaitTime;
		AvgTurnArndTime = processes[i].turnAroundtime + AvgTurnArndTime;
		output << processes[i].pid << "\t\t" << processes[i].startTime - processes[i].arrivalTime << "\t\t" << processes[i].turnAroundtime << endl;
	}
	// calculate the averages
	AvgTurnArndTime = AvgTurnArndTime / numProc;
	stats[0] = AvgTurnArndTime;
	AvgWaitTime = AvgWaitTime / numProc;
	stats[1] = AvgWaitTime;
	// print out the averages
	output << "AVG\t\t" << AvgWaitTime << "\t" << AvgTurnArndTime << "\n" << endl;

	output << "Process sequence: ";
	// loop through the process vector and print out id numbers, since its FCFS arrival order is the only thing that matters
	stats[2] = numProc;
	for (int j = 0; j < numProc; j++) {
		if (j + 1 != numProc) {
			output << processes[j].pid << '-';
		} else {
			output << processes[j].pid << endl;
		}
	}

	output << "Context switches: " << numProc << endl;
	output.close();
}

void SJF (Q_struct * queue, vector<Process> &Processes, int total_Processes, char *outputFile, int int_bursts[], int snapStep, Summary *Sum_Timings){
	ofstream output;
	output.open(outputFile, fstream::app);
	output << "\n\n***** SJF Scheduling *****" << endl;
	vector<int> context;
	int curr_process = 0, Arrived_PID = 0,shortest_PID, X_time = 0;
	int Total_Burst = sum_processes_burst(total_Processes, Processes);
	Queue_Init(queue, total_Processes, Processes);

	for(X_time = 0; X_time <= Total_Burst; X_time++ ){	// check for the last process burst time ot be 0 
	    	SortQ(queue, Processes);                    // sort the queue for any execution cycle
		if (Processes[curr_process].CPUburst == 0 ){	// check if the execution of the current process ended
			context.push_back(curr_process);
			SortQ(queue, Processes);           

			if(X_time%snapStep == 0){ 
				output << "t = " << X_time << endl;
				output << "CPU: Finishing process " << curr_process;
			}
		
			if(X_time%snapStep == 0){  
		 
				if (X_time ==Total_Burst) output << " "<< endl;
				else output << "; loading process " << Processes[queue->Q_space[queue->head]].pid << " (CPU Burst = " << Processes[queue->Q_space[queue->head]].CPUburst << " )" <<endl;

				if (queue->head == queue->tail) output << "Ready queue: empty" << endl;
				else Q_print(queue, Processes, outputFile);            
			  	output << endl; 
			    }
		
				if (queue->head != queue->tail) shortest_PID = pop(queue);	// POP the shortest Process 
				curr_process = Processes[shortest_PID].pid;
			//}           
		} else {
			// condition to manage the queue print outs regarding the snapshot interval value given by the user
			   if(X_time%snapStep == 0){

				output << "t = " << X_time << endl;
			    }

				if (X_time > 0){   // if we running after the first cycle
					if(X_time%snapStep == 0) output << "CPU: Running process " << curr_process << " (remaining CPU burst = " << Processes[curr_process].CPUburst << ")" << endl;  
				} else {
					if(X_time%snapStep == 0) {
				    output << "CPU: Loading process " << curr_process << " (CPU burst = " << Processes[curr_process].CPUburst << ")" << endl;
					Q_print(queue, Processes, outputFile);                 // print the content of the queue
				    }
					SortQ(queue, Processes);                               // sort the queue initially 
					curr_process = Processes[pop(queue)].pid;              // pop a process from the queue and do context switch with the current running process
				} 
                // if new processs arrives we push it in the queue
				if( (Arrived_PID = Arrived_Flag(X_time, total_Processes, Processes)) > 0 ) push(queue, Arrived_PID);
				SortQ(queue, Processes);  

				if(X_time%snapStep == 0) {                                // sort the queue after pushing a new one in the qu
				if ((queue->head == queue->tail)&&(X_time>0)) output << "Ready queue: empty" << endl;
				else { if(X_time !=0)Q_print(queue, Processes, outputFile); }   
				output << endl; 
			}
		}

		Processes[curr_process].CPUburst--;                   //  process the burst time 
		TimeLogger(total_Processes, curr_process, Processes); //  calculating the waiting cycles for later use to calculate the waiting and turn around time
	}
	output << "*********************************************************" << endl;
	output << "SJF Summary (WT = wait time, TT = turnaround time):" << endl;
	Statistic_Logs(Processes, context, total_Processes, outputFile, int_bursts ,context.size(), &Sum_Timings->SJF_alg); // calculating the waiting and turn around time for the currnent algorithm 
	Processes.clear();
	free(queue->Q_space);    // freeing the memory
}

void STCF (Q_struct * queue, vector<Process> &Processes, int total_Processes, char *outputFile, int int_bursts[], int snapStep, Summary *Sum_Timings) {
	ofstream output;
	output.open(outputFile, fstream::app);
	output << "\n***** STCF Scheduling *****" << endl;
	vector<int> context;
	
	int curr_process = 0, Arrived_PID = 0, popedID = 0, X_time = 0;
	int Total_Burst = sum_processes_burst(total_Processes, Processes);
	Queue_Init(queue, total_Processes, Processes);                  // Store the first data in the Queue

	for(X_time = 0; X_time <= Total_Burst; X_time++){               // check for the last process burst time ot be 0 
		SortQ(queue, Processes);	                                // Sort the Queue on every cycle
		
		if (X_time == 0) curr_process = Processes[pop(queue)].pid;	// sort the queue initially 
	   
	   if( (Arrived_PID = Arrived_Flag(X_time, total_Processes, Processes)) > 0 ){	// something arrived    
				push(queue, Arrived_PID);              // Push in the queue the incomming process 
				SortQ(queue, Processes);               // sort the queue 
				
				if (Processes[curr_process].CPUburst > Processes[Arrived_PID].CPUburst){
					if(X_time%snapStep == 0){ 
						output << "t = " << X_time << endl;
						output << "CPU: Preempting process " << curr_process << " (remaining CPU burst = " << Processes[curr_process].CPUburst << ");"; 
					}
					// condition to manage the queue print outs regarding the snapshot interval value given by the user
					if(X_time%snapStep == 0)output << "loading process " << Processes[queue->Q_space[queue->head]].pid <<"; (CPU burst = "<<   Processes[queue->Q_space[queue->head]].CPUburst << ")" <<endl;
						if(X_time%snapStep == 0){ 
							if (queue->head == queue->tail)output << "Ready queue: empty" << endl;
								else Q_print(queue, Processes, outputFile);       
							output << endl; 
					}
					// check remaining burst against to arrived burst  
					push(queue, curr_process);	        // if no , keep the current Process for the next X-cycle
					context.push_back(curr_process);    //  pushing process ID in the vector when the process finishes so we can calculate the context switch
					int shortest_PID = pop(queue);	    // if yes ,POP  Process with the Shortest Burst time       
					curr_process = shortest_PID;        // contect switch happens here, replacing the current process ID with the shortest PID burst process form the queue
				} else {
					// condition to manage the queue print outs regarding the snapshot interval value given by the user
					if((X_time%snapStep == 0)&&(Processes[curr_process].CPUburst >0)){  
						output << "t = " << X_time << endl;
						if(X_time >0)output << "CPU: Running process " << curr_process << " (remaining CPU burst = " << Processes[curr_process].CPUburst << ")" << endl;  
							if (queue->head == queue->tail)output << "Ready queue: empty" << endl;
								else Q_print(queue, Processes, outputFile);      
						output << endl; 
					} 
				}
		} else {
			
			if((X_time%snapStep == 0)&&(Processes[curr_process].CPUburst > 0)){       
				output << "t = " << X_time << endl;
			
				if(X_time >0) {  // if this is not the first execution cycle 
					 output << "CPU: Running process " << curr_process << " (remaining CPU burst = " << Processes[curr_process].CPUburst << ")" << endl;  
				     if (queue->head == queue->tail)output << "Ready queue: empty" << endl;
				     else Q_print(queue, Processes, outputFile);  
				}else { 
					 output << "CPU: Loading process " << curr_process << " (CPU burst = " << Processes[curr_process].CPUburst << ")" << endl;
				     output << "Ready queue: "<< curr_process <<endl;
				     }			
				output << endl; 
			} 
		}

		if (Processes[curr_process].CPUburst == 0){         //  check if the execution of the current process ended
			context.push_back(curr_process);                //  pushing process ID in the vector when the process finishes so we can calculate the context switch
			SortQ(queue, Processes);                        //  if the current process ended sort again the queue
        
			if(X_time % snapStep == 0){ 
				output << "t = " << X_time << endl;       // print the currnet execution cycle
				output << "CPU: Finishing process " << curr_process;
				if (X_time ==Total_Burst) output << " "<< endl; 
					else output << "; loading process " << Processes[queue->Q_space[queue->head]].pid << "; (CPU burst = " << Processes[queue->Q_space[queue->head]].CPUburst << ");" << endl; 
			}

			if(X_time%snapStep == 0){ 
				if (queue->head == queue->tail)output << "Ready queue: empty" << endl;  // if tail = head means empty queue
					else Q_print(queue, Processes, outputFile);  // print the current queue      
				output << endl; 
			}
			popedID = pop(queue);                         // POP the shortest Process
			curr_process = Processes[popedID].pid;        // loading new process
              
		} 

		Processes[curr_process].CPUburst--;
		TimeLogger(total_Processes, curr_process, Processes);  // calculating the waiting cycles of each process
	}
	output << "*********************************************************" << endl;
	output << "STCF Summary (WT = wait time, TT = turnaround time):" << endl;
	Statistic_Logs(Processes, context, total_Processes, outputFile, int_bursts ,context.size(), &Sum_Timings->STCF_alg);  // calculating the waiting and turn around time for the currnent algorithm 
	Processes.clear();     // clear the vecotor processes
	free(queue->Q_space);
}

void RoundRobin(Q_struct * queue , vector<Process> &Processes, int total_processes ,char *outputFile, int int_bursts[],int snapStep, Summary *Sum_Timings ){
	ofstream output;
	output.open(outputFile, fstream::app);
	output << "\n\n***** Round Robin Scheduling *****";
	vector<int> context;
	int time_quanta = 2;
	int flag = 0;  /////variable that shows if the process is executing or not
	int time_quanta_countr = 0;
	int sum_cpu_burst = sum_processes_burst(total_processes, Processes);
	int curr_process = 0;
	int Arrived_PID = 0;

	// Store the first data in the Queue
	Queue_Init(queue, total_processes, Processes);                            // Store the first data in the Queue
	int processes_in_queue = 0;  ///need to add

	for (int i = 0; i <= sum_cpu_burst; i++) {
		if(i % snapStep == 0){ 
			output<<" "<< endl;
			output << "t = " << i << endl;
		}

		Arrived_PID = Arrived_Flag(i, total_processes, Processes);
		if (Arrived_PID > 0) {
			push(queue, Arrived_PID);
		}

		//// change this location to the place of decreementing the counter

		if ( i == 0){
			if(i%snapStep == 0){ 
				output << "CPU: Loading process " << queue->head << " (CPU burst = " << Processes[queue->Q_space[queue->head]].CPUburst << ")" << endl;
				Q_print(queue, Processes, outputFile);
			}
		}
		processes_in_queue = number_process_with_bursts(total_processes, Processes);

		if (flag == 1 || processes_in_queue > 0 ) {
			if (flag == 0 && processes_in_queue > 0) {
				curr_process = Processes[pop(queue)].pid;
				flag = 1;
				time_quanta_countr = time_quanta;  //give time quanta to process
			}

			if (i != 0) {
				Processes[curr_process].CPUburst--;
				time_quanta_countr = time_quanta_countr - 1;
				TimeLogger(total_processes, curr_process, Processes); 
			}

			if ((i != 0) && (Processes[curr_process].CPUburst >= 1) && time_quanta_countr ==1){
				if(i%snapStep == 0){ 
					output << "CPU: Running process " << curr_process << " (CPU burst = " << Processes[curr_process].CPUburst << ")" << endl; 
					
						if(queue->head == queue->tail) output << "Ready queue: empty" << endl;
						else { Q_print(queue, Processes, outputFile); }   
						output << endl; 

					    //Q_print(queue, Processes, outputFile);

				}
			}

			if (Processes[curr_process].CPUburst == 0) {
				flag = 0;
				context.push_back(curr_process);

				if(i%snapStep == 0){ 

					output << "CPU: Finishing process " << curr_process << " (remaining CPU burst = " << Processes[curr_process].CPUburst << ")";
					if(queue->head != queue->tail)
					output << "CPU: Loading process "   << queue->Q_space[queue->head] << "(CPU burst = " << Processes[queue->Q_space[queue->head]].CPUburst << ")" << endl;
					else output<<" "<< endl;

						if(queue->head == queue->tail) output << "Ready queue: empty" << endl;
						else { Q_print(queue, Processes, outputFile); }   
						output << endl; 

					//Q_print(queue, Processes, outputFile);


				}
			}

			if (time_quanta_countr == 0 && Processes[curr_process].CPUburst != 0) {
				/// put a if condition if the head and tail of the process is same 
				if(queue->head != queue->tail){
					context.push_back(curr_process);
					if(i%snapStep == 0){ 
						output << "CPU: Preempting process " << curr_process << " (remaining CPU burst = " << Processes[curr_process].CPUburst << ")" ;
						output << "CPU: Loading process " << queue->Q_space[queue->head] << " (CPU burst = " << Processes[queue->Q_space[queue->head]].CPUburst << ")" << endl;
						Q_print(queue, Processes, outputFile);
					}
				}

				/// changed from else to if  with condition other that before  condition = queue->head == queue->tail
				if(queue->head == queue->tail){ 
					if(i % snapStep == 0){ 
						output << "CPU: Running process " << curr_process << " (CPU burst = " << Processes[curr_process].CPUburst << ")" << endl;  
						//Q_print(queue, Processes, outputFile);

						output << "Ready Queue: ";
       				    output << "empty\n";
          			    output << endl; 

					}
				}    

				flag = 0;
				push(queue, curr_process);
			}   
		}
	}
	output << "\n*********************************************************" << endl;
	output << "Round robin Summary (WT = wait time, TT = turnaround time):" << endl;
	Statistic_Logs(Processes, context, total_processes, outputFile, int_bursts ,context.size(), &Sum_Timings->RR_alg);
    Processes.clear();
    free(queue->Q_space);
}

void runPriority(vector<Process> processes, int timeStep, int numProc, char *outputFile, float stats[]) {
	ofstream output;
	output.open(outputFile, fstream::app);
	// print the header for the FCFS algorithm
	output << "\n***** Priority Scheduling *****" << endl;

	// variable to keep track of the process number
	int procNum = 0;
	// simulation time
	int time = 0;
	// current running process
	int CPUProc = -1;
	// variable to hold current processes remaining time
	int timeRemain = 0;
	// vector for context switches
	vector<int> context;
	// vector to hold contents of ready queue
	vector<Process> readyQueue;
	// variables for algorithm summary
	float AvgWaitTime = 0;
	float AvgTurnArndTime = 0;

	while (1) {
		// check the time step for output
		if (!(time % timeStep)) {
			output << "t = " << time << endl;
		}
		// check to see which processes have arrived
		for (int j = 0; j < numProc; j++) {
			// if they have arrived place them in the ready queue for sorting
			if (time == processes[j].arrivalTime) {
				readyQueue.push_back(processes[j]);
			}
		}
		
		// sort the list by priority 
		sort(readyQueue.begin(), readyQueue.end(), sortByPriority);

		if (CPUProc != -1 && timeRemain == 0) {
			// note the context switch
			context.push_back(CPUProc);
			// note the end time
			processes[CPUProc].endTime = time;
			// calculate turn around time
			processes[CPUProc].turnAroundtime = processes[CPUProc].endTime - processes[CPUProc].arrivalTime;

			if (numProc == int(context.size())) {
				// print out last 
				if (!(time % timeStep)) {
					output << "CPU: Finishing process " << CPUProc << endl;
					output << "Ready queue: empty" << endl;
				}
				break;
			}
			if (!(time % timeStep)) {
				output << "CPU: Finishing process " << CPUProc << "; loading process " << readyQueue[0].pid << " (CPU burst = " << readyQueue[0].CPUburst << ")" << endl;
			}
			// reset flags and counter
			CPUProc = readyQueue[0].pid;

			timeRemain = readyQueue[0].CPUburst;
			// note the start time
			processes[CPUProc].startTime = time;
			// take it off the ready queue
			readyQueue.erase(readyQueue.begin());
			// bring in the next process
			++procNum;
		} else if (CPUProc != -1 && timeRemain > 0) {
			if (!(time % timeStep)) {
				output << "CPU: Running process " << CPUProc << " (CPU burst = remaining " << timeRemain << ")" << endl;
			}
		}

		// first iteration
		if (CPUProc == -1){
			CPUProc = readyQueue[0].pid;
			timeRemain = readyQueue[0].CPUburst;

			if (!(time % timeStep)) {
				if (time == 0) {
					output << "CPU: Loading process " << CPUProc << " (CPU burst = " << timeRemain << ")" << endl;
				} else {
					output << "CPU: Running process " << CPUProc << " (CPU burst = remaining " << timeRemain << ")" << endl;
				}
			}
			// remove process from ready queue
			readyQueue.erase(readyQueue.begin());
		} 
		

		if (!(time % timeStep)) {
			// print out ready queue
			if (!readyQueue.empty()) {
				output << "Ready queue: ";
			} else {
				output << "Ready queue: empty";
				output << endl;
			}

			// loop through the ready queue
			for (int j = 0; j < int(readyQueue.size()); j++) {
				if (j + 1 != int(readyQueue.size())) {
					output << readyQueue[j].pid << '-';
				}
				else {
					output << readyQueue[j].pid << endl;
				}
			}

			output << endl;
		}
		// change the counters
		timeRemain--;
		time++;
	}

	// print out algorithm summary
	output << "\n*********************************************************" << endl;
	output << "Priority Summary (WT = wait time, TT = turnaround time):" << endl;
	output << "\nPID\t\tWT\t\tTT" << endl;
	for (int i = 0; i < numProc; i++) {
		AvgWaitTime = (processes[i].startTime - processes[i].arrivalTime) + AvgWaitTime;
		AvgTurnArndTime = processes[i].turnAroundtime + AvgTurnArndTime;
		output << processes[i].pid << "\t\t" << processes[i].startTime - processes[i].arrivalTime << "\t\t" << processes[i].turnAroundtime << endl;
	}
	// calculate the averages
	AvgTurnArndTime = AvgTurnArndTime / numProc;
	stats[3] = AvgTurnArndTime;
	AvgWaitTime = AvgWaitTime / numProc;
	stats[4] = AvgWaitTime;
	// print out the averages
	output << "AVG\t\t" << AvgWaitTime << "\t" << AvgTurnArndTime << "\n" << endl;

	output << "Process sequence: ";
	// loop through the process vector and print out id numbers, since its FCFS arrival order is the only thing that matters
	for (int j = 0; j < int(context.size()); j++) {
		if (j + 1 != numProc) {
			output << context[j] << '-';
		} else {
			output << context[j] << endl;
		}
	}
	stats[5] = int(context.size());
	output << "Context switches: " << context.size() << endl << endl;
}




// acquiring timmings for any algorithm calls this function and store it in the performance metrics structure
void Statistic_Logs(vector<Process> &Processes, vector<int> &Context, int total_Processes, char *outputFile, int Burst_Array[], int context_Count, P_metrics *timmings){ 
	ofstream output;
	output.open(outputFile, fstream::app);
	int waitc = 0;
	float avgWT = 0, avgTT = 0;
	

	output << "\nPID\t\tWT\t\tTT" << endl;

	for(int c_count = 0; c_count < total_Processes; c_count++){
		waitc = Processes[c_count].waitingTime - Processes[c_count].arrivalTime;  // calculating current process waiting time 
		avgWT = avgWT + waitc;                         // Average Waiting timming
		avgTT = avgTT + waitc + Burst_Array[c_count];  // Turn Around timming

		output << Processes[c_count].pid << "\t\t" << waitc << "\t\t" <<  waitc + Burst_Array[c_count] << endl; 
    }
	timmings->WT_time = avgWT/total_Processes;
	timmings->TT_time = avgTT/total_Processes;    // store timmings in the sctrucutre
	output << "AVG" << "\t\t" << timmings->WT_time << "\t" << timmings->TT_time << endl; 

	output << "\nProcess sequence:  ";
	// loop through the process vector and print out id numbers, since its FCFS arrival order is the only thing that matters
	for (int j = 0; j < int(Context.size()); j++) {    
		output<< Context[j] << '-'; 
    }
	output << endl;
	timmings->Context_c = context_Count;  // contex switch counter stored in the metrics structure
	output << "Context switches:  " << timmings->Context_c << endl; 
}




void Performace_report(Summary * sum_time, char *outputFile, float stats[]){
	ofstream output;
	output.open(outputFile, fstream::app);
	// create the vectors to hold the data
	vector <Comparison> waitTimeComparison;
	vector <Comparison> turnaroundTimeComparison;
	vector <Comparison> contextComparison;
	
	Comparison WT;
	Comparison TT;
	Comparison CS;

	// SJF
	WT.value = sum_time->SJF_alg.WT_time;
	WT.type = "SJF";
	
	CS.value = sum_time->SJF_alg.Context_c;
	CS.type = "SJF";
	
	TT.value = sum_time->SJF_alg.TT_time;
	TT.type = "SJF";
	
	waitTimeComparison.push_back(WT);
	contextComparison.push_back(CS);
	turnaroundTimeComparison.push_back(TT);
	
	// STCF
	WT.value = sum_time->STCF_alg.WT_time;
	WT.type = "STCF";
	
	CS.value = sum_time->STCF_alg.Context_c;
	CS.type = "STCF";
	
	TT.value = sum_time->STCF_alg.TT_time;
	TT.type = "STCF";
	
	waitTimeComparison.push_back(WT);
	contextComparison.push_back(CS);
	turnaroundTimeComparison.push_back(TT);
	
	// round robin
	WT.value = sum_time->RR_alg.WT_time;
	WT.type = "Round robin";
	
	CS.value = sum_time->RR_alg.Context_c;
	CS.type = "Round robin";
	
	TT.value = sum_time->RR_alg.TT_time;
	TT.type = "Round robin";
	
	waitTimeComparison.push_back(WT);
	contextComparison.push_back(CS);
	turnaroundTimeComparison.push_back(TT);
	
	// priority
	WT.value = stats[4];
	WT.type = "Priority";
	
	CS.value = stats[5];
	CS.type = "Priority";
	
	TT.value = stats[3];
	TT.type = "Priority";
	
	waitTimeComparison.push_back(WT);
	contextComparison.push_back(CS);
	turnaroundTimeComparison.push_back(TT);
	
	// FCFS
	WT.value = stats[1];
	WT.type = "FCFS";
	
	CS.value = stats[2];
	CS.type = "FCFS";
	
	TT.value = stats[0];
	TT.type = "FCFS";
	
	waitTimeComparison.push_back(WT);
	contextComparison.push_back(CS);
	turnaroundTimeComparison.push_back(TT);	
	
	sort(waitTimeComparison.begin(),waitTimeComparison.end(), sortByValue);
	sort(contextComparison.begin(),contextComparison.end(), sortByValue);
	sort(turnaroundTimeComparison.begin(),turnaroundTimeComparison.end(), sortByValue);
	 
	output<< "\n***** OVERALL SUMMARY *****\n" << endl;
	output<< "Wait Time Comparison" << endl;
	int setSpace = 0;
	for (int i = 0; i < int(waitTimeComparison.size()); i++) {
		if (waitTimeComparison[i].type.length() == 11) {
			setSpace = 8;
		} else if(waitTimeComparison[i].type.length() == 8)  {
			setSpace = 11;
		} else if(waitTimeComparison[i].type.length() == 4) {
			setSpace = 15;
		} else {
			setSpace = 16;
		}
		output << i + 1 << " " << waitTimeComparison[i].type << setw(setSpace) << waitTimeComparison[i].value << endl;
	}

	output<<"Turnaround Time Comparison" << endl;
	for (int j = 0; j < int(turnaroundTimeComparison.size()); j++) {
		if (turnaroundTimeComparison[j].type.length() == 11) {
			setSpace = 8;
		} else if(turnaroundTimeComparison[j].type.length() == 8)  {
			setSpace = 11;
		} else if(turnaroundTimeComparison[j].type.length() == 4) {
			setSpace = 15;
		} else {
			setSpace = 16;
		}
		output << j + 1 << " " << turnaroundTimeComparison[j].type << setw(setSpace) << turnaroundTimeComparison[j].value << endl;
	}

	output<<"Context Switch Comparison" << endl;
	for (int k = 0; k < int(contextComparison.size()); k++) {
		if (contextComparison[k].type.length() == 11) {
			setSpace = 8;
		} else if(contextComparison[k].type.length() == 8)  {
			setSpace = 11;
		} else if(contextComparison[k].type.length() == 4) {
			setSpace = 15;
		} else {
			setSpace = 16;
		}
		output << k + 1 << " " << contextComparison[k].type << setw(setSpace) << contextComparison[k].value << endl;
	}
}
