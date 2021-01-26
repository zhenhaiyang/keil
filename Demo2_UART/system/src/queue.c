/**
  ******************************************************************************
  * @file    
  * @author  zhy
  * @version 
  * @date    2021-01-25
  * @brief   
  ******************************************************************************
  */
#include "queue.h"
/** 
 * @brief 初始化队列 
 * @note 无
 * @param {Queue} *q 被初始化队列的指针
 * @retval 无
 */
void InitQueue(Queue *q)
{
    q->front = q->mem;
    q->rear = q->mem;
}

/** 
 * @brief   进队列 
 * @note    无
 * @param {Queue} *q    进入的队列
 * @param {uint8_t} item 进入的元素
 * @retval 无
 */
void InQueue(Queue *q, uint8_t item)
{
    *(q->front) = item;                                   //将值写入队头
    q->front = q->mem + ((q->front + 1 - q->mem) & 0xff); //队头自增
    if (q->front == q->rear)                              //若队头和队尾相等——队列满，队尾被覆盖
    {
        q->rear = q->mem + ((q->rear + 1 - q->mem) & 0xff); //队尾自增
    }
}
/** 
 * @brief 出队列 
 * @note 无
 * @param {Queue} *q 从该队列出
 * @param {uint8_t} *item 获得的数据
 * @retval 队伍状态
 */
uint8_t DeQueue(Queue *q, uint8_t *item)
{
    if (q->front != q->rear) //队伍不空
    {
        *item = *(q->rear);                                 //获得队尾值
        q->rear = q->mem + ((q->rear + 1 - q->mem) & 0xff); //队尾自增
        return 1;                                           //正确得到数据
    }
    return 0; //队伍空
}
