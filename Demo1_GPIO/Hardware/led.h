/*
 * @file    led.h
 * @author  zhy
 * @version 1.0
 * @date    2020-11-12
 * @brief   提供led宏定义和函数声明
 */

#ifndef __LED__H__
#define __LED__H__

#include "sys.h"

#define led0 (PBout(0)) //led0
#define led1 (PBout(1)) //led1

extern void LED_init(void);

#endif
