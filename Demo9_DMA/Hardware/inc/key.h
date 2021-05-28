/**
  ******************************************************************************
  * @file    key.h
  * @author  zhy
  * @version 1.0
  * @date    2021-05-18
  * @brief   key.c的对外接口
  ******************************************************************************
  */

#ifndef __KEY_H__
#define __KEY_H__

#include <stdint.h>

typedef enum
{
    KEY_0 = 0X01,
    KEY_1 = 0X02,
    KEY_2 = 0X04,
    KEY_WKUP = 0X08
} KeyValue;

extern void KeyInit(void);              //按键初始化
extern void KeyScan(void);              //按键扫描函数
extern void KeyResetEdge(void);         //单次触发清0
extern uint8_t KeyGetLevelActive(void); //获得有效电平
extern uint8_t KeyGetEdgeActive(void);  //获得有效边沿

#endif
