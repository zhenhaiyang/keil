/**
  ******************************************************************************
  * @file    usart.h
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   与串口相关配置，USART1
  ******************************************************************************
  */

#ifndef __USART_H__
#define __USART_H__


/* 外部结构体类型声明 */
typedef struct Queue Queue;

/* 对外接口 */
extern void UartInit(void);
extern Queue queueRx;            //串口接收队列

#endif
