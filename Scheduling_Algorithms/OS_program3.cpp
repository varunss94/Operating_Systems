/*
Varun Subramanya
*/

#include "sched_sim.hpp"
 
using namespace std;

int main(int argc, char* argv[]) {
	// array for other alg stats
	float stats[6];
	int context_switch = 0;    
	Q_struct Queue; 
	Summary Sum_Timings; 
   	int numProcesses = 0;
	
	vector<Process> processFCFS;
	vector<Process> processSJF;
	vector<Process> processSTCF;
	vector<Process> processRoundRobin;
	vector<Process> processPriority;                          
	
	//  --  get user's input arguments
	char *inputFile = argv[1];
	char *outputFile = argv[2];
	int SnapshotStep = atoi(argv[3]);                    // time interval between printing snapshots
  	

	// variables to hold process info
	int CPUburst, priority, arrivalTime;
	string line;
	
	// create a new file stream to loop through file
	ifstream input;
	input.open(inputFile, fstream::in);

	// loop through the file and get text line by line
	while (getline(input, line)) {
		istringstream iss(line);
		if (!(iss >> CPUburst >> priority >> arrivalTime)) {
			// if we're here something went wrong
			break;
		}

		// create the new object and push it
		processFCFS.push_back(Process(numProcesses, CPUburst, priority, arrivalTime));
		processSJF.push_back(Process(numProcesses, CPUburst, priority, arrivalTime));
		processSTCF.push_back(Process(numProcesses, CPUburst, priority, arrivalTime));
		processRoundRobin.push_back(Process(numProcesses, CPUburst, priority, arrivalTime));
		processPriority.push_back(Process(numProcesses, CPUburst, priority, arrivalTime));

		// increment the counter
		numProcesses++;
	}

	int Buffer_Burst[numProcesses];                     // temporary copy from burst timmings
	for(int t=0; t<numProcesses; t++) Buffer_Burst[t] = processFCFS[t].CPUburst;  // coping in the temp buffer

// ---- Scheduling ALgorithms ----------
	runFCFS(processFCFS, SnapshotStep, numProcesses, outputFile, stats); //FCFS Algorithm
	SJF(&Queue, processSJF, numProcesses, outputFile, Buffer_Burst, SnapshotStep, &Sum_Timings);        // SJF Algorithm
	STCF(&Queue, processSTCF, numProcesses, outputFile, Buffer_Burst, SnapshotStep, &Sum_Timings);       // STCF Algorithm
	RoundRobin(&Queue, processRoundRobin, numProcesses, outputFile, Buffer_Burst, SnapshotStep, &Sum_Timings); // RR Algorithm
	runPriority(processPriority, SnapshotStep, numProcesses, outputFile, stats);

// -------------------------------------

	Performace_report(&Sum_Timings, outputFile, stats);
	return 0;
}
