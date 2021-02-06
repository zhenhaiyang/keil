/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-02-04
  * @brief   实现定时器中断
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main()
{
    /* 1.初始化配置 */
    HAL_Init();           //HAL初始化
    SystemClock_Config(); //系统时钟初始化
    LedInit();            //LED初始化
    Tim6Init();           //TIM6初始化

    /* 2.while循环 */
    while (1)
    {
        LED0 = !LED0;   //LED反转
        delay_ms(1000); //延时1秒
    }
}
