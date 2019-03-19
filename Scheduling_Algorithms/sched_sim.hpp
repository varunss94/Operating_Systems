/*
Varun Subramanya
*/

#ifndef SCHED_SIM_HPP
#define SCHED_SIM_HPP

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>    // to get the vector class definition
#include <algorithm>
#include <iomanip> 

//#define QUEUE_SIZE 100

using namespace std;


typedef struct   {
    int *Q_space; 
    int head;
    int tail;
} Q_struct;

typedef struct   {
    float value;
    string type;
} Comparison;


class Process {
public:
	// class variables
	int pid;
	int CPUburst;
	int priority;
	int arrivalTime;
	int startTime;
	int endTime;
	int waitingTime;
	int turnAroundtime;

	// class constructor
	Process(int id, int CPU, int prior, int arrv) {
		pid = id;
		CPUburst = CPU;
		priority = prior;
		arrivalTime = arrv;
		startTime = 0;
		endTime = 0;
		waitingTime = 0;
		turnAroundtime = 0;
	}
};

// structure for counting turn arround and waiting timmings
typedef struct {
    float TT_time;
    float WT_time;
    int   Context_c;
} P_metrics;

// structure for grouping the performance timmings for each algorithm
typedef struct {
    P_metrics SJF_alg;
    P_metrics STCF_alg;
    P_metrics RR_alg;
    P_metrics FCFS_alg;
    P_metrics Prior_alg;
}Summary;


//extern "C" void test(void);
extern Q_struct Queue; 
extern P_metrics timmings;
extern Summary Sum_Timings; 


// ----- Prototypes -----
void push(Q_struct *queue, int data);
int pop(Q_struct *queue);
int MinBurst(Q_struct *Q, vector<Process> &Processes);
int Arrived_Flag(int time, int Total_Processes_Num, vector<Process> &Processes);
void Queue_Init(Q_struct *Q, int Total_Processes_Num, vector<Process> &Processes);
void SortQ(Q_struct *queue, vector<Process> &Processes);
void Q_print(Q_struct *queue, vector<Process> &Processes, char *outputFile);
int sum_processes_burst( int numLine ,  vector<Process> &Processes);
int number_process_with_bursts(int number_lines, vector<Process> &Processes);
void TimeLogger(int total_Processes, int curr_PID, vector<Process> &Processes);

bool sortByArrivalTime(const Process &lhs, const Process &rhs);
bool sortByCPUburst(const Process &lhs, const Process &rhs);
bool sortByPriority(const Process &lhs, const Process &rhs);
bool sortByValue(const Comparison &lhs, const Comparison &rhs);

void runFCFS(vector<Process> processes, int timeStep, int numProc, char *outputFile, float stats[]);
void STCF(Q_struct *queue, vector<Process> &Processes, int total_Processes, char *outputFile, int int_bursts[], int snapStep, Summary *Sum_Timings);
void SJF(Q_struct * queue, vector<Process> &Processes, int total_Processes, char *outputFile, int int_bursts[], int snapStep, Summary *Sum_Timings);
void RoundRobin(Q_struct * queue, vector<Process> &Processes, int total_processes, char *outputFile, int int_bursts[], int snapStep, Summary *Sum_Timings);
void runPriority(vector<Process> processes, int timeStep, int numProc, char *outputFile, float stats[]);

void Statistic_Logs(vector<Process> &Processes, vector<int> &Context, int total_Processes, char *outputFile, int Burst_Array[], int context_Count, P_metrics *timmings);
void Performace_report(Summary * sum_time, char *outputFile, float stats[]);

#endif
