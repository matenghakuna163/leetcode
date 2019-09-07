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

#ifndef ISLAND
#define ISLAND ('1')
#endif

#define EMPTY_CUR (-1)

typedef struct {
    int size;
    int head;
    int tail;
    int *data;    
} MyCircularQueue;

typedef struct {
    int landx;
    int landy;
}LandPos;


/** Initialize your data structure here. Set the size of the queue to be k. */

MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue *queue = NULL;
    if (k < 0) {
         return NULL;
    }
    queue = malloc(sizeof(MyCircularQueue));
    if (NULL == queue) {
         return NULL;
    }
    queue->data = malloc(sizeof(int)*k);
    if (NULL == queue->data) {
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
bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    if (NULL == obj) {
        return TRUE;
    }
    if (EMPTY_CUR == obj->head) {
        return TRUE;
    }
    return FALSE;
}

/** Checks whether the circular queue is full or not. */
bool myCircularQueueIsFull(MyCircularQueue* obj) {
    if (NULL == obj) {
        return FALSE;
    }
    if (myCircularQueueIsEmpty(obj)) {
        return FALSE;
    }
    
    int next_pos = (obj->tail + 1) % obj->size;
    return next_pos == obj->head;
}

/** Insert an element into the circular queue. Return true if the operation is successful. */
bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    if (myCircularQueueIsFull(obj)) {
        return FALSE;
    }

    if (myCircularQueueIsEmpty(obj)) {
        obj->head = 0;
        obj->tail = 0;
    } 
    else {
        int cur_tail = obj->tail;
        cur_tail++;
        obj->tail = cur_tail % obj->size;
    }
    obj->data[obj->tail] = value;
    return TRUE; 
}

/** Delete an element from the circular queue. Return true if the operation is successful. */
bool myCircularQueueDeQueue(MyCircularQueue* obj) {
     if (myCircularQueueIsEmpty(obj)) {
        return FALSE; 
     }
     if (obj->head == obj->tail) {
         obj->head = EMPTY_CUR;
         obj->tail = EMPTY_CUR;
     }  
     else {
     	int cur_head = obj->head;
     	cur_head++;
     	obj->head = cur_head % obj->size;
     }
     return TRUE;
}

/** Get the front item from the queue. */
int myCircularQueueFront(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }
    return obj->data[obj->head];
}

/** Get the last item from the queue. */
int myCircularQueueRear(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }
    return obj->data[obj->tail];
}

void myCircularQueueFree(MyCircularQueue* obj) {
    if (NULL == obj) {
        return;
    }
    if (obj->data != NULL) {
        free(obj->data);
    }
    free(obj);    
}

void printGrid(char** grid, int gridSize, int theGridColSize) {
    for (int gridIndex = 0; gridIndex < gridSize; gridIndex++) {
        for (int gridColIndex = 0; gridColIndex < theGridColSize; gridColIndex++) {
            printf("%c", grid[gridIndex][gridColIndex]);
        }
        printf("\n");
    }
}

static void freeGrid(char **grid, int gridSize) {
    if (NULL == grid)
        return;
    for (int index; index < gridSize; index++){
        if(grid[index] != NULL){
            free(grid[index]);
            grid[index] = NULL;
        }
    }
    free(grid);
}

static char **buildEmptyGrid(int gridSize, int theGridColSize) {
    char **grid = NULL;
    grid = malloc(sizeof(char *) * gridSize);
    if (NULL == grid)
        return NULL;
    memset(grid, 0, sizeof(char *) * gridSize);
    for (int index = 0; index < gridSize; index++) {
        grid[index] = malloc(sizeof(char)*theGridColSize);
        if (NULL == grid[index]) {
            freeGrid(grid, gridSize);
            grid = NULL;
            break;
        }
        else{
            memset(grid[index], 0, sizeof(char)*theGridColSize);
        }
        
    }
    return grid;
}

static bool isNewLand(char **grid, 
                      char **flagGrid,
                      int gridIndex,
                      int gridColIndex)
{
    if (ISLAND == grid[gridIndex][gridColIndex]){
        if (flagGrid[gridIndex][gridColIndex] != ISLAND){
            return TRUE;
        }
    }
    return FALSE;
}

static int getLandPosValue(LandPos *pos)
{
    int value = pos->landx;
    value = (value << 16) | pos->landy;
    return value;
}

static LandPos getLandPos(int value)
{
    LandPos pos;
    pos.landx = (0xFFFF0000 & value) >> 16;
    pos.landy = 0x0000FFFF & value;
    return pos;
}

static bool findNewLand(char **grid,
                        char **flagGrid, 
                        int gridSize,
                        int theGridColSize,
                        MyCircularQueue *queue)
{
    int gridIndex = 0;
    int gridColIndex = 0;

    for (gridIndex = 0; gridIndex < gridSize; gridIndex++){
        for (gridColIndex = 0; gridColIndex < theGridColSize; gridColIndex++){
            if (isNewLand(grid, flagGrid, gridIndex, gridColIndex)){
                flagGrid[gridIndex][gridColIndex] = ISLAND;
                LandPos pos;
                pos.landx = gridIndex;
                pos.landy = gridColIndex;
                int landValue = getLandPosValue(&pos);
                myCircularQueueEnQueue(queue, landValue);
                return TRUE;
            }
        }
    }
    return FALSE;
}

static void travleLandPos(char **grid,
                          char **flagGrid, 
                          int gridSize,
                          int theGridColSize,
                          MyCircularQueue *queue,
                          LandPos *landPos)
{
    if (landPos->landx < gridSize 
        && landPos->landy < theGridColSize
        && landPos->landx >= 0 && landPos->landy >= 0)
    {
        if (isNewLand(grid, flagGrid, landPos->landx, landPos->landy))
        {
            flagGrid[landPos->landx][landPos->landy] = ISLAND;
            myCircularQueueEnQueue(queue, getLandPosValue(landPos));
        }
    }
}

static void travleLand(char **grid,
                       char **flagGrid, 
                       int gridSize,
                       int theGridColSize,
                       MyCircularQueue *queue)
{
    while (!myCircularQueueIsEmpty(queue))
    {
        int landValue = myCircularQueueFront(queue);
        LandPos pos = getLandPos(landValue);
    
        myCircularQueueDeQueue(queue);

        LandPos rightPos = pos;
        rightPos.landx++;
        LandPos downPos = pos;
        downPos.landy++;
        LandPos upPos = pos;
        upPos.landy--;
        LandPos leftPos = pos;
        leftPos.landx--;
        travleLandPos(grid, flagGrid, gridSize, theGridColSize, queue, &rightPos);
        travleLandPos(grid, flagGrid, gridSize, theGridColSize, queue, &downPos);
        travleLandPos(grid, flagGrid, gridSize, theGridColSize, queue, &upPos);
        travleLandPos(grid, flagGrid, gridSize, theGridColSize, queue, &leftPos);
    }
}

static int caclIslands(char **grid,
                       char **flagGrid, 
                       int gridSize,
                       int theGridColSize,
                       MyCircularQueue *queue)
{
    int num = 0;

    while (findNewLand(grid, flagGrid, gridSize, theGridColSize, queue)){
        travleLand(grid, flagGrid, gridSize, theGridColSize, queue);
        num++;
    };
    return num;
}

int numIslands(char** grid, int gridSize, int* gridColSize){
    int ret = 0;
    if (NULL == grid || NULL == gridColSize) {
        return 0;
    }
    if (0 == gridSize || 0 == *gridColSize) {
        return 0;
    }

    int theGridColSize = *gridColSize;
    char **flagGrid = buildEmptyGrid(gridSize, theGridColSize);
    if (NULL == flagGrid)
    {
        printf("buildEmptyGrid failed. gridSize(%d) theGridColSize(%d)\n", gridSize, theGridColSize);
        return -1;
    }
    MyCircularQueue *queue = myCircularQueueCreate(gridSize * theGridColSize);
    if (NULL == queue){
        printf("myCircularQueueCreate failed. size(%d)", gridSize * theGridColSize);
        freeGrid(flagGrid, gridSize);
        return -1;
    }

    ret = caclIslands(grid, flagGrid, gridSize, theGridColSize, queue);

    freeGrid(flagGrid, gridSize);
    myCircularQueueFree(queue);
    flagGrid = NULL;
    return ret;
}

int main(int argc, char *argv[])
{
    /*
    11000
    11000
    00100
    00011
    */
    int gridSize = 3;
    int gridColSize = 3;
    char *grid[gridSize];
    char gridBuf[3][3] = {
        {'1','1','1'},
        {'0','1','0'},
        {'1','1','1'},
    };
    for (int index = 0; index < gridSize; index++) {
        grid[index] = gridBuf[index];
    }
    printGrid(grid, gridSize, gridColSize);
    int num = numIslands(grid, gridSize, &gridColSize);
    printf("num=%d\n", num);
    
    return 0;
}
