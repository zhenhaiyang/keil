/**
  ******************************************************************************
  * @file    queue.h
  * @author  zhy
  * @version 1.0
  * @date    2021-01-25
  * @brief   与队列相关的结构体
  ******************************************************************************
  */
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>

typedef struct Queue
{
    uint8_t mem[256]; //255个长度的队列
    uint8_t *front;  //队首
    uint8_t *rear;   //队尾
} Queue;

/** 
 * @brief 判断队列是否为空 
 * @note    注意括号的使用
 * @param q：队列指针
 * @retval 0：队列非空，1：队列空
 */
#define isQueueEmpty(q) ((q)->front == (q)->rear)

extern void InitQueue(Queue *q);
extern void InQueue(Queue *q, uint8_t item);
extern uint8_t DeQueue(Queue *q, uint8_t *item);

#endif
