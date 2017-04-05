
#include "queue.h"
#include "assert.h"
#include "stdio.h"
#include "stm32_includes.h"

#define QUEUE_SIZE  100
#define ARRAY_SIZE  (QUEUE_SIZE + 1)

static QUEUE_TYPE  queue[ARRAY_SIZE];
static volatile size_t front = 1;
static volatile size_t rear = 0;

int queue_put(QUEUE_TYPE value)
{
	if(is_full())
    {
        //debug(info, "queue is full!");
        return -1;
    }
	rear = (rear + 1) % ARRAY_SIZE;
	queue[rear] = value;	
}

int queue_get(QUEUE_TYPE *value)
{
	if(is_empty())
    {
        //debug(info, "queue is empty!");
        return -1;
    }
    *value = queue[front];
	front = (front + 1) % ARRAY_SIZE;
    return 0;
}

int is_empty(void)
{
	return ((rear + 1) % ARRAY_SIZE) == front;	
}

int is_full(void)
{
	return ((rear + 2) % ARRAY_SIZE) == front;	
}

