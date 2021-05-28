/**
  ******************************************************************************
  * @file    sys.h
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   系统初始化相关的头文件
  ******************************************************************************
  * @version 1.1
  * @date    2021-05-26
  * @brief   系统初始化相关的头文件
  *             硬件名称的修改
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
#define set_led_0 PBout.bit1
#define set_led_1 PBout.bit0

#define get_key_wkup PAin.bit0
#define get_key_0 PHin.bit3
#define get_key_1 PHin.bit2
#define get_key_2 PCin.bit13

/* 函数 */
extern void SystemClock_Config(void); //时钟系统配置

#endif
