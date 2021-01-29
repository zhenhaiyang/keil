/**
  ******************************************************************************
  * @name    delay.h
  * @author  zhy
  * @version 1.0
  * @date    2020.11.11
  * @brief   提供延时函数的对外接口。
  ******************************************************************************
  */
#ifndef __DELAY_H
#define __DELAY_H

#include "stdint.h"

extern void delay_us(uint32_t nus );

extern void delay_ms( uint16_t nms );

#endif
