/**
  ******************************************************************************
  * @file    exti.h
  * @author  zhy
  * @version 1.0
  * @date    2021-01-29
  * @brief   对外接口
  ******************************************************************************
  */
#ifndef __EXTI_H__
#define __EXTI_H__
#include "stdint.h"

extern uint8_t flagExti0;
extern uint8_t flagExti2;
extern uint8_t flagExti3;
extern uint8_t flagExti15_10;

extern void ExtiInit(void);

#endif
