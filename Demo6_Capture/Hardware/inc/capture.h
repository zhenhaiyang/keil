/**
  ******************************************************************************
  * @file    capture.h
  * @author  zhy
  * @version 1.0
  * @date    2021-04-07
  * @brief   capture.c的接口
  ******************************************************************************
  */
#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include "stdint.h"

typedef enum
{
    CAPTURE_NONE,
    CAPTURE_RASING,
    CAPTURE_FALLING,
    CAPTURE_ENDING
} StateCapture; //捕获状态结构体

typedef struct
{
    uint16_t counter[100];
    uint8_t length;
    StateCapture state;
    uint16_t times[100];
} CaptureResult;

extern CaptureResult captureTim8;

extern void CaptureInit(void);


#endif
