//circular queue representation of buffer
#ifndef BUFFER_H
#define BUFFER_H

typedef int buffer_item;
#define BUFFER_PROD_SIZE 5
#define BUFFER_MOVE_SIZE 10

class buffer {
	public:
	// buffer functions
	// default constructor (allocates a buffer size of 5)
		buffer();
	// parameterized constructor (user defines size of buffer)
		buffer(int);
		int insert_item(buffer_item);
		int remove_item(buffer_item&);

	// buffer data members
		int front;
		int rear;
		int BUFFER_SIZE;
		buffer_item *itemBuffer;
	};

#endif
