#include <iostream>
#include "buffer.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;

// global constants
	// initialize num produced/consumed counts
		int itemsProduced = 0;
		int itemsConsumed = 0;	

	// initialize mutex and semaphore constants
		pthread_mutex_t mutex;
		sem_t produce, consume;

	// initialize buffers
		buffer myBuffer(BUFFER_PROD_SIZE);

	// initialize command line constants
		int SLEEP_TIME = 0;	
		int NUM_PRODUCERS = 0;	
		int NUM_CONSUMERS = 0;	
	
// function prototypes for thread functions
void *producer(void *param);
void *consumer(void *param);

// main function
int main(int argc, char *argv[])
	{	
		// parse command line arguments								
			// exit program if command line arguments are invalid
				if((argc != 4) || (SLEEP_TIME < 0) || (NUM_PRODUCERS < 0) || (NUM_CONSUMERS < 0))
					{
						cout << "\nPlease enter 3 valid arguments after the file name (int int int)\nProgram Exited\n";				
						return 0;
					}

			// set sleep time					
				SLEEP_TIME = atoi(argv[1]);

			// set number of producer threads				
				NUM_PRODUCERS = atoi(argv[2]);

			// set number of consumer threads				
				NUM_CONSUMERS = atoi(argv[3]);

		// initialize random seed					 					
			srand(time(NULL));
  		
		// initialize mutex and semaphores
			pthread_mutex_init(&mutex, NULL);
			sem_init(&consume, 0, 0);
			sem_init(&produce, 0, 5);

		// initialize unique thread ID for producer and consumer threads
			int proId = 0;
			int conId = 0;												

		// create producer thread(s)
			pthread_t threadPro[NUM_PRODUCERS];						
			for(int i = 0; i < NUM_PRODUCERS; i++, proId++)											
				pthread_create(&(threadPro[i]), NULL, producer, &proId);														
		
		// create consumer thread(s)							
			pthread_t threadCon[NUM_CONSUMERS];			
			for(int j = 0; j < NUM_CONSUMERS; j++, conId++)											
				pthread_create(&(threadCon[j]), NULL, consumer, &conId);					

		// sleep for predetermined amount of time			
			sleep(SLEEP_TIME);	
		
		// acquire mutex lock to stop producer and consumer threads from printing 									
			pthread_mutex_lock(&mutex);

		// print out number of producers and consumers			
			cout << "\n\nItems produced: " << itemsProduced << endl;
			cout << "Items removed: " << itemsConsumed << endl;				

		// terminate program
			return 0;		
	}

// producer thread function
void *producer(void *param)
	{
		// initialize temporary variables
			buffer_item item;
			int ID = *((int *) param);	

		while(true)
			{
				// sleep for a random period of time
					sleep(rand() % 5);	

				// generate a random item	
					item = rand();		
							
				// enter critical region				
					sem_wait(&produce);
					pthread_mutex_lock(&mutex);

				// insert item into buffer & report if failed or not
					if(myBuffer.insert_item(item))
						cout <<  "BUFFER FULL, item not inserted" << endl;

				// display info to screen if insert is sucessful
					else
						{
							// display which thread produced what item												
								cout << "Producer " << ID << " produced: " << item << endl;					
							// increment items produced
								itemsProduced++;
						}

				// exit critical region
					sem_post(&consume);															
					pthread_mutex_unlock(&mutex);	
			}	
	}

//	consumer thread function
void *consumer(void *param)
	{
		// initialize temporary variables
			buffer_item item;
			int ID = *((int *) param);
		
		while(true)
			{
				// sleep for a random period of time
					sleep(rand() % 5);

				// enter critical region
					sem_wait(&consume);			
					pthread_mutex_lock(&mutex);

				// consume item from buffer & report if failed or not		
					if(myBuffer.remove_item(item))
						cout << "\t\tBUFFER EMPTY, item not removed" << endl;

				// display info to screen if remove was sucessful
					else
						{							
							// display which thread consumed which item
								cout << "\t\tConsumer " << ID << " consumed: " << item << endl;	
							// increment items consumed
								itemsConsumed++;								
						}

				// exit critical region	
					sem_post(&produce);								
					pthread_mutex_unlock(&mutex);					
			}				
	}


