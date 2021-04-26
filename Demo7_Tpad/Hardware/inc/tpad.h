/**
  ******************************************************************************
  * @file    tpad.h
  * @author  zhy
  * @version 1.0
  * @date    2021-04-26
  * @brief   tpad.c的对外接口
  ******************************************************************************
  */

#ifndef __TPAD_H__
#define __TPAD_H__

typedef enum
{
    TPAD_HIGH,    //高电平
    TPAD_LOW,     //低电平
    TPAD_FALLING, //下降沿
    TPAD_RISING   //上升沿
} TpadStatus;

extern void TpadInit(void);
extern void TpadScan(TpadStatus *tpadStatus);
#endif
