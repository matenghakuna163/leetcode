#include <stdlib.h>

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

typedef struct {
    int size;
    int head;
    int tail;
    int *data;    
} MyCircularQueue;


/** Initialize your data structure here. Set the size of the queue to be k. */

MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue *queue = NULL;
    if (k > 1000 || k < 0) {
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

int main(int argc, char *argv[])
{
  int k = 3;
  int value = 29;
  MyCircularQueue* obj = myCircularQueueCreate(k);
  bool param_1 = myCircularQueueEnQueue(obj, 1);
 
  param_1 = myCircularQueueEnQueue(obj, 2);
  param_1 = myCircularQueueEnQueue(obj, 3);
  param_1 = myCircularQueueEnQueue(obj, 4);
  int param_4 = myCircularQueueRear(obj);
  bool param_6 = myCircularQueueIsFull(obj);
  bool param_2 = myCircularQueueDeQueue(obj);
 
  param_1 = myCircularQueueEnQueue(obj, value);
 
  param_2 = myCircularQueueDeQueue(obj);
 
 
  myCircularQueueFree(obj);
  return 0;
}
