/**
  ******************************************************************************
  * @file    sys.h
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   系统初始化相关的头文件
  ******************************************************************************
  */

#ifndef __SYS_H__
#define __SYS_H__

#include <stdint.h>

/* 位带操作 */
typedef struct
{
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int bit4 : 1;
    unsigned int bit5 : 1;
    unsigned int bit6 : 1;
    unsigned int bit7 : 1;
    unsigned int bit8 : 1;
    unsigned int bit9 : 1;
    unsigned int bit10 : 1;
    unsigned int bit11 : 1;
    unsigned int bit12 : 1;
    unsigned int bit13 : 1;
    unsigned int bit14 : 1;
    unsigned int bit15 : 1;
    unsigned int rsv : 16;

} BitBand __attribute__((bitband));

extern BitBand PAin;
extern BitBand PBin;
extern BitBand PCin;
extern BitBand PDin;
extern BitBand PEin;
extern BitBand PFin;
extern BitBand PGin;
extern BitBand PHin;
extern BitBand PIin;

extern BitBand PAout;
extern BitBand PBout;
extern BitBand PCout;
extern BitBand PDout;
extern BitBand PEout;
extern BitBand PFout;
extern BitBand PGout;
extern BitBand PHout;
extern BitBand PIout;

/* 硬件对应信息 */
#define LED0 PBout.bit1
#define LED1 PBout.bit0

#define WKUP PAin.bit0
#define KEY0 PHin.bit3
#define KEY1 PHin.bit2
#define KEY2 PCin.bit13

/* 函数 */
extern void SystemClock_Config(void); //时钟系统配置

#endif
