#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int bool;

#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define EMPTY_CUR (-1)

typedef struct
{
    int size;
    int head;
    int tail;
    int *data;
} MyCircularQueue;

/** Initialize your data structure here. Set the size of the queue to be k. */

MyCircularQueue *myCircularQueueCreate(int k)
{
    MyCircularQueue *queue = NULL;
    if (k < 0)
    {
        return NULL;
    }
    queue = malloc(sizeof(MyCircularQueue));
    if (NULL == queue)
    {
        return NULL;
    }
    queue->data = malloc(sizeof(int) * k);
    if (NULL == queue->data)
    {
        free(queue);
        queue = NULL;
        return NULL;
    }
    queue->size = k;
    queue->head = EMPTY_CUR;
    queue->tail = EMPTY_CUR;
    return queue;
}

/** Checks whether the circular queue is empty or not. */
bool myCircularQueueIsEmpty(MyCircularQueue *obj)
{
    if (NULL == obj)
    {
        return TRUE;
    }
    if (EMPTY_CUR == obj->head)
    {
        return TRUE;
    }
    return FALSE;
}

/** Checks whether the circular queue is full or not. */
bool myCircularQueueIsFull(MyCircularQueue *obj)
{
    if (NULL == obj)
    {
        return FALSE;
    }
    if (myCircularQueueIsEmpty(obj))
    {
        return FALSE;
    }

    int next_pos = (obj->tail + 1) % obj->size;
    return next_pos == obj->head;
}

/** Insert an element into the circular queue. Return true if the operation is successful. */
bool myCircularQueueEnQueue(MyCircularQueue *obj, int value)
{
    if (myCircularQueueIsFull(obj))
    {
        return FALSE;
    }

    if (myCircularQueueIsEmpty(obj))
    {
        obj->head = 0;
        obj->tail = 0;
    }
    else
    {
        int cur_tail = obj->tail;
        cur_tail++;
        obj->tail = cur_tail % obj->size;
    }
    obj->data[obj->tail] = value;
    return TRUE;
}

/** Delete an element from the circular queue. Return true if the operation is successful. */
bool myCircularQueueDeQueue(MyCircularQueue *obj)
{
    if (myCircularQueueIsEmpty(obj))
    {
        return FALSE;
    }
    if (obj->head == obj->tail)
    {
        obj->head = EMPTY_CUR;
        obj->tail = EMPTY_CUR;
    }
    else
    {
        int cur_head = obj->head;
        cur_head++;
        obj->head = cur_head % obj->size;
    }
    return TRUE;
}

/** Get the front item from the queue. */
int myCircularQueueFront(MyCircularQueue *obj)
{
    if (myCircularQueueIsEmpty(obj))
    {
        return -1;
    }
    return obj->data[obj->head];
}

/** Get the last item from the queue. */
int myCircularQueueRear(MyCircularQueue *obj)
{
    if (myCircularQueueIsEmpty(obj))
    {
        return -1;
    }
    return obj->data[obj->tail];
}

void myCircularQueueFree(MyCircularQueue *obj)
{
    if (NULL == obj)
    {
        return;
    }
    if (obj->data != NULL)
    {
        free(obj->data);
    }
    free(obj);
}

/////////////////////////////
#define TURNTABLE_COUNT (4)

typedef struct turntable
{
    int table[TURNTABLE_COUNT];
} turntable_t;

void printLock(char **deadends, int deadendsSize, char *target)
{
    printf("target=%s\n", target);
    printf("deadends:\n");
    for (int index = 0; index < deadendsSize; index++)
    {
        printf("%s\n", deadends[index]);
    }
}

static int convertStrToTurntable(const char *str, turntable_t *table)
{
    if (NULL == str || NULL == table)
    {
        return -1;
    }
    int index = 0;
    for (index = 0; index < TURNTABLE_COUNT; index++)
    {
        char num[TURNTABLE_COUNT] = {0};
        num[0] = str[index];
        table->table[index] = atoi(num);
    }
    return 0;
}

static int convertTruntableToStr(const turntable_t *table, char *str)
{
    if (NULL == str || NULL == table)
    {
        return -1;
    }
    int index = 0;
    for (index = 0; index < TURNTABLE_COUNT; index++)
    {
        str[index] = (char)(table->table[index] + 48);
    }
    return 0;
}

int openLock(char **deadends, int deadendsSize, char *target)
{
    int step = 0;
    turntable_t startTable;
    turntable_t targetTable;
    convertStrToTurntable("0000", &startTable);
    convertStrToTurntable(target, &targetTable);

    return step;
}

/*
 deadends = ["0201","0101","0102","1212","2002"],
 target = "0202", 
 start = "0000",
 output step=6
*/
int main(int argc, char *argv[])
{
    int step = 0;
    char *deadends[5] = {"0201", "0101", "0102", "1212", "2002"};
    char *target = "0202";
    step = openLock(deadends, 5, target);
    printf("step=%d\n", step);
    return 0;
}