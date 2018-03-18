//#ifndef __QUEUE_H
//#define __QUEUE_H
//extern typedef struct CircleQueue;

#define QUEUESIZE 100 //定义队列的大小  
typedef int DataType; //定义队列元素类型  

typedef struct
{
	DataType data[QUEUESIZE];
	int front; //指向队头的索引，这个所指的空间不存放元素  
	int tail; //指向队尾的索引，存放最后一个元素  
}CircleQueue;

//#endif
