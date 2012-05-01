//implementation file of buffer
#include "buffer.h"

// default consturctor 
buffer:: buffer()
	{
		// initialize front and rear indices to -1
			front = -1;
			rear = -1;
		// initialize itemBuffer to BUFFER_PROD_SIZE (5 for this assignment)
			itemBuffer = new buffer_item[BUFFER_PROD_SIZE];
	}

buffer:: buffer(int size)
	{
		// initialize front and rear indices to -1
			front = -1;
			rear = -1;
			BUFFER_SIZE = size;
		// initialize itemBuffer to user defined size
			itemBuffer = new buffer_item[size];
	}

// insert item function
int buffer:: insert_item(buffer_item item)
	{				
		// check if buffer is full before inserting
			if((front == 0 && rear == BUFFER_SIZE - 1) || (rear + 1 == front))
				// return -1 for error
					return -1;

		// else insert the item into buffer
			else
			{
				// if at end of queue wrap around to index 0
		   		if(rear == BUFFER_SIZE - 1)
			   		rear = 0;
				// else increment insert index and insert 
					else
						rear++;
		   		itemBuffer[rear] = item;

				// if insert is called first time, also increment remove index
				if(front == -1)					
	  				front = 0;

				// return 0 for sucess										
					return 0;
		 	}	
	}

// remove item function
int buffer:: remove_item(buffer_item &item)
	{		
		// check if buffer is empty before removing
		// no item has been inserted if front is still -1
			if(front == -1)
				// return -1 for error
					return -1;

		//	else return next item in buffer
		 	else
		 	{
				// return item 
					item = itemBuffer[front];

				// if front == rear, indices must be reset
					if(front == rear)
					{
			   		front = -1;	
						rear = -1;
						// return 0 for sucess since item was removed
							return 0;
					}

				// else increment remove index 
					else
					{
						// if at end of queue wrap around to index 0
			 				if(front == BUFFER_SIZE-1)				  			
								front = 0;

						// otherwise just increment
			   			else
							{
								front++;
								// return 0 for sucess since item was removed
									return 0;
							}
						// return 0 for sucess since item was removed
							return 0;
					}
		 	}		
	}
