#include "stdint.h"
#define bool int
#define false 0
#define true 1
typedef int DataType;

typedef struct qnode
{
	DataType data;
	struct qnode *next;
}Node, *pNode;

typedef struct
{
	pNode front;
	pNode rear;
}LinkQueue;

bool InitLinkQueue(LinkQueue *pLQ)
{
	pNode p;
	p = (pNode)malloc(sizeof(Node));
	if (p == NULL)
		return false;
	else
	{
		p->next = NULL;
		pLQ->front = p;
		pLQ->rear = p;
		return true;
	}
}

bool IsQueueEmpty(LinkQueue *pLQ)
{
	if (pLQ->front == pLQ->rear)
		return true;
	else
		return false;
}

bool PushElement(LinkQueue *pLQ, DataType dData)
{
	pNode p;
	p = (pNode)malloc(sizeof(Node));
	if (p == NULL)
		return false;

	p->data = dData;
	p->next = NULL;
	pLQ->rear->next = p;
	pLQ->rear = p;
	return true;
}

bool PopElement(LinkQueue *pLQ, DataType *pData)
{
	pNode p;
	if (IsQueueEmpty(pLQ))
		return false;

	p = pLQ->front->next;
	*pData = p->data;
	pLQ->front->next = p->next;
	if (pLQ->front->next == NULL)
		pLQ->rear = pLQ->front;

	free(p);
		return true;
}

bool GetHeadElement(LinkQueue *pLQ, DataType *pData)
{
	if (IsQueueEmpty(pLQ))
		return false;

	*pData = pLQ->front->next->data;
	return true;
}