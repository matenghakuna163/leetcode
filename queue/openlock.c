#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define EMPTY_CUR (-1)

#define TURNTABLE_COUNT (4)

typedef struct turntable
{
    int table[TURNTABLE_COUNT];
} turntable_t;

typedef turntable_t QueueDataType;

typedef struct
{
    int size;
    int head;
    int tail;
    QueueDataType *data;
    int dataNum;
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
    queue->data = (QueueDataType *)malloc(sizeof(QueueDataType) * k);
    if (NULL == queue->data)
    {
        free(queue);
        queue = NULL;
        return NULL;
    }
    queue->size = k;
    queue->head = EMPTY_CUR;
    queue->tail = EMPTY_CUR;
    queue->dataNum = 0;
    return queue;
}

int myCircularQueueGetDataNum(MyCircularQueue *obj)
{
    if (NULL == obj)
        return -1;
    return obj->dataNum;
}

/** Checks whether the circular queue is empty or not. */
bool myCircularQueueIsEmpty(MyCircularQueue *obj)
{
    if (NULL == obj)
    {
        return true;
    }
    if (EMPTY_CUR == obj->head)
    {
        return true;
    }
    return false;
}

/** Checks whether the circular queue is full or not. */
bool myCircularQueueIsFull(MyCircularQueue *obj)
{
    if (NULL == obj)
    {
        return false;
    }
    if (myCircularQueueIsEmpty(obj))
    {
        return false;
    }

    int next_pos = (obj->tail + 1) % obj->size;
    return next_pos == obj->head;
}

/** Insert an element into the circular queue. Return true if the operation is successful. */
bool myCircularQueueEnQueue(MyCircularQueue *obj, QueueDataType value)
{
    if (myCircularQueueIsFull(obj))
    {
        return false;
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
    obj->dataNum++;
    return true;
}

/** Delete an element from the circular queue. Return true if the operation is successful. */
bool myCircularQueueDeQueue(MyCircularQueue *obj)
{
    if (myCircularQueueIsEmpty(obj))
    {
        return false;
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
    obj->dataNum--;
    return true;
}

/** Get the front item from the queue. */
QueueDataType myCircularQueueFront(MyCircularQueue *obj)
{
    if (myCircularQueueIsEmpty(obj))
    {
        QueueDataType empty;
        return empty;
    }
    return obj->data[obj->head];
}

/** Get the last item from the queue. */
QueueDataType myCircularQueueRear(MyCircularQueue *obj)
{
    if (myCircularQueueIsEmpty(obj))
    {
        QueueDataType empty;
        return empty;
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

static void setupDeadNums(bool *deadNums, char **deadends, int deadendsSize)
{
    int index;
    for (index = 0; index < deadendsSize; index++) {
        int num = atoi(deadends[index]);
        deadNums[num] = true;
    }
}

static int convertTurntableToNum(const turntable_t *table)
{
    char tabelStr[TURNTABLE_COUNT+1] = {0};
    convertTruntableToStr(table, tabelStr);
    return atoi(tabelStr);
}

static void getAroundNums(const turntable_t *current, int *aroundNums, int aroundNumsSize)
{
    int index = 0;
    for (index = 0; index < TURNTABLE_COUNT; index++)
    {
        turntable_t nextLeft = *current;
        turntable_t nextRight = *current;
        nextLeft.table[index] =  (current->table[index] + 10 - 1) % 10;
        nextRight.table[index] =  (current->table[index] + 10 + 1) % 10;
        aroundNums[index*2] = convertTurntableToNum(&nextLeft);
        aroundNums[index*2+1] = convertTurntableToNum(&nextRight);
    }
}

static void convertNumToTurnTable(int num, turntable_t *turnNum)
{
    char turnStr[TURNTABLE_COUNT+1] = {0};
    sprintf(turnStr, "%04d", num);
    convertStrToTurntable(turnStr, turnNum);
}

static void addNextNum(bool *deadNums, 
                       bool *visitedNum, 
                       MyCircularQueue *queue,
                       int *nextNums,
                       int nextNumsSize)
{
    int index = 0;
    for (index = 0; index < nextNumsSize; index++)
    {
        int next = nextNums[index];
        if (deadNums[next] || visitedNum[next]) 
        {
            continue;
        }
        visitedNum[next] = true;
        turntable_t nextTable;
        convertNumToTurnTable(next, &nextTable);
        myCircularQueueEnQueue(queue, nextTable);
    }
}

static int caclStep(const char *targetStr, 
                    bool *deadNums, 
                    bool *visitedNum, 
                    MyCircularQueue *queue)
{
    int step = 0;
    turntable_t start;
    turntable_t target;

    convertStrToTurntable("0000", &start);
    convertStrToTurntable(targetStr, &target);
    int targetNum = convertTurntableToNum(&target);
    if (deadNums[0])
    {
        return -1;
    }

    myCircularQueueEnQueue(queue, start);

    while (!myCircularQueueIsEmpty(queue))
    {
        int dataNum = myCircularQueueGetDataNum(queue);
        int index = 0;
        for (index = 0; index < dataNum; index++)
        {
            turntable_t current = myCircularQueueFront(queue);
            int nextNums[TURNTABLE_COUNT*2] = {0};
            getAroundNums(&current, nextNums, TURNTABLE_COUNT*2);
            myCircularQueueDeQueue(queue);
            int currentNum = convertTurntableToNum(&current);
            if (currentNum == targetNum)
            {
                return step;
            }
            addNextNum(deadNums, visitedNum, queue, nextNums, TURNTABLE_COUNT*2);
        }
        step++;
    }
    
    return -1;
}

int openLock(char **deadends, int deadendsSize, char *target)
{
    int step = 0;
    bool *deadNums = NULL;
    bool *visitedNums = NULL;

    deadNums = (bool *)malloc(10000 * sizeof(bool));
    memset(deadNums, 0, sizeof(bool) * 10000);
    visitedNums = (bool *)malloc(10000 * sizeof(bool));
    memset(visitedNums, 0, sizeof(bool) * 10000);
    if (NULL == deadNums || NULL == visitedNums)
    {
        printf("can not malloc.");
        return -2;
    }
    setupDeadNums(deadNums, deadends, deadendsSize);

    MyCircularQueue *queue = myCircularQueueCreate(2000);

    step = caclStep(target, deadNums, visitedNums, queue);

    myCircularQueueFree(queue);
    queue = NULL;
    free(deadNums);
    deadNums = NULL;
    free(visitedNums);
    visitedNums = NULL;
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
    char *deadends[1] = {"0000"};
    char *target = "8888";
    step = openLock(deadends, 1, target);
    printf("step=%d\n", step);
    return 0;
}