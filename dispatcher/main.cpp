// AUTHOR: Nitish Narala //
// Dispatcher Simulation //
#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;

// global variables
// struct to represent a process
struct process{
	int arvTime;
	int priority;
	int cpuTime;	
	int finishTime;
	int serviceTime;
	int waitingTime;
	int turnAroundTime;
	int normTurnTime;
};

// pList is a list of all processes read from file
	vector<process> pList;

// initialize time quantum value
	const int timeQuant = 1;

// function prototypes 

// readData populates the dispatcher list from file
	bool readData(char *);	

// demoteProcess checks if a single process has finished running and demotes if needed
	void demoteProcess(queue<process*> &, queue<process*> &, int);

// checkCpuDone returns true if all processes in the dispatch list have been run to completion
	bool checkCpuDone();

// calcTimes loops through the process list and calculates times for each process and global averages
	void calcTimes(double&, double&, double&); 

// main function
int main(int argc, char *argv[])
	{
		// initialize variables
			// final average values
				double avgTurnTime, avgNormTurnTime, avgWaitTime;
		
		// initialize queues 
			queue<process*> realList, highList, medList, lowList;

			// temporary process variable
				process temp;

			// initialize dispatcher time unit
				int timeTick = 0;		

			// initialize bool variable to check if processes have finished executing
				bool DONE = false;
		
		// read data from file or exit if no file is found
			if(!readData(argv[1]))
				return 0;
	
		// initialize stuff
			for(int index = 0; index < pList.size(); index++)				
				{
					pList[index].finishTime = 0;
					pList[index].serviceTime = pList[index].cpuTime;					
				}	
				
		// begin loop while there are processes to be run
			while(!DONE)
				{
					// parse process list
						for(int index = 0; index < pList.size(); index++)
							{
								// check if a job has "arrived" yet
									if(timeTick == pList[index].arvTime)
										{
											// place process in appropriate queue
				 								// check if process belongs in real-time list
													if(pList[index].priority == 0 )
           																												
														realList.push(&pList[index]);																										
																						   										
												// check if process belongs in high-priority list
													else if(pList[index].priority == 1)														
														highList.push(&pList[index]);														

				 								// check if process belongs in medium-priority list
													else if(pList[index].priority == 2)
														medList.push(&pList[index]);

											 	// else process belongs in low-priority list
													else 																								
														lowList.push(&pList[index]);														
										}							
							}				
									
							// finish running all real-time priority processes
								if(!realList.empty())					
									{						
										// run process in realList
											realList.front()->cpuTime -= timeQuant;

										// check if process has finished running and pop
											if(realList.front()->cpuTime == 0)
												{
													// set finished time stamp
														realList.front()->finishTime = timeTick+1;
													// if process has 0 cpuTime remaining pop it
														realList.pop();
												}
												
									}			
																													
							// check for high-priority processes
								else if(!highList.empty())
									{						
										// run process in highList
											highList.front()->cpuTime -= timeQuant;
											
										// check if process has finished running and set time stamp										
											demoteProcess(highList, medList, timeTick);
									}								
							
							// check for med-priority processes
								else if(!medList.empty())
									{
										// run process in medList
											medList.front()->cpuTime -= timeQuant;
								
										// check if process has finished running and set time stamp
											demoteProcess(medList, lowList, timeTick);
									}
								
							// check for low-priority processes							
								else if(!lowList.empty())
									{
										// run process in lowList
											lowList.front()->cpuTime -= timeQuant;
								
										// check if process has finished running and set time stamp
											demoteProcess(lowList, lowList, timeTick);
									}
				
					// increment timeTick 
						timeTick++;
					
					// check if processes have finished running and break loop if yes
						DONE = checkCpuDone();									
				}

		// calculate averages and output to screen
			calcTimes(avgTurnTime, avgNormTurnTime, avgWaitTime);
		return 0; 	
	}


// function implementations
void demoteProcess(queue<process*> &currentQueue, queue<process*> &demotedQueue, int timeTick)
	{
		// check if process has finished running and set time stamp
			if(currentQueue.front()->cpuTime == 0)
				{
					currentQueue.front()->finishTime = timeTick+1;																							
					currentQueue.pop();
				}
		// otherwise demote process to next priority queue
			else
				{
					demotedQueue.push(currentQueue.front());	
					currentQueue.pop();
				}
	}

bool checkCpuDone()
	{
		// initialize a temporary bool 
			bool done = false;

		// loop through process list
			for(int i = 0; i < pList.size(); i++)
				{
					// check if process at index i has finished running
						if(pList[i].cpuTime == 0)		
							// if process finished running, set done to true to indicate exit
								done = true;

					// else there is a process still running
						else
							{
								// set done to false and break
									done = false;	
									break;
							}
				}
		return done;
	}

void calcTimes(double &avgTurnTime, double &avgNormTurnTime, double &avgWaitTime)
	{
		// loop through process list and calculate times for each process
			for(int i = 0; i < pList.size(); i++)			
				{
					// calculate turnaround time for each process and total
						pList[i].turnAroundTime = (pList[i].finishTime - pList[i].arvTime);
						avgTurnTime += pList[i].turnAroundTime;

					// calculate normalized turnaround time for each process and total
						pList[i].normTurnTime = (pList[i].turnAroundTime / pList[i].serviceTime);
						avgNormTurnTime += pList[i].normTurnTime;

					// calculate waiting time for each process and total
						pList[i].waitingTime = (pList[i].finishTime - pList[i].arvTime - pList[i].serviceTime);
						avgWaitTime += pList[i].waitingTime;							
				}

		// calculate all averages
		   avgTurnTime /= pList.size();
			avgNormTurnTime /= pList.size();
			avgWaitTime /= pList.size();

		// set precision and display global averages
			cout << fixed << endl;
			cout << "Average turnaround time = " << setprecision(2) << avgTurnTime << endl;
			cout << "Average normalized turnaround time = " << setprecision(2) << avgNormTurnTime << endl;
			cout << "Average waiting time = " << setprecision(2) << avgWaitTime << endl << endl;
	}

bool readData(char *file)
	{
		// temp variables
			char dummy;
			process temp;

		// declare file stream
			ifstream fin;
			fin.open(file);

		// check if file opened and return
			if(!fin)
				{
					cout << "Can not open file!\n";
					return false;
				}

		// prime loop and set first element in list
			fin >> temp.arvTime >> dummy >> temp.priority >> dummy >> temp.cpuTime;
				
		// read file
			while(fin.good())
				{					
					// insert element into vector 
						pList.insert(pList.end(), temp);
					
					// read and set next element			
						fin >> temp.arvTime >> dummy >> temp.priority >> dummy >> temp.cpuTime;
				}

		// return true for file opened and close file			
			fin.close();
			return true;
	}
