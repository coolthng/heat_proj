//#include "heat.h"

//#include "stdint.h"
#include "queue.h"
#define bool char
#define false 0
#define true 1
#define NULL 0

CircleQueue aaa;
bool InitCircleQueue(CircleQueue *pCQ)
{
	//pCQ = (CircleQueue *)malloc(sizeof(CircleQueue));//不需要新建
	if (pCQ == NULL)
		return false;
	else
	{
		pCQ->front = 0;
		pCQ->tail = 0;
		return true;
	}
}

bool IsQueueEmpty(CircleQueue *pCQ)
{
	if (pCQ->front == pCQ->tail)
		return true;
	else
		return false;
}

bool IsQueueFull(CircleQueue *pCQ)
{
	if ((pCQ->tail + 1) % QUEUESIZE == pCQ->front)
		return true;
	else
		return false;
}

bool PushElement(CircleQueue *pCQ, DataType dData)
{
	if (IsQueueFull(pCQ))
		return false;

	pCQ->tail = (pCQ->tail + 1) % QUEUESIZE;
	pCQ->data[pCQ->tail] = dData;
	return true;
}

bool PopElement(CircleQueue *pCQ, DataType *pData)
{
	if (IsQueueEmpty(pCQ))
		return false;

	pCQ->front = (pCQ->front + 1) % QUEUESIZE;
	*pData = pCQ->data[pCQ->front];
	return true;
}

bool GetHeadElement(CircleQueue *pCQ, DataType *pData)
{
	if (IsQueueEmpty(pCQ))
		return false;

	*pData = pCQ->data[(pCQ->front + 1) % QUEUESIZE];
	return true;
}