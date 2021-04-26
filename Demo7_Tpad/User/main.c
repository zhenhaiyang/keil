/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-04-26
  * @brief   电容按键使用：
  *             1.通过按下电容按键，改变LED1的状态
  *             2.LED0按照1s的频率改变其状态
  *          管脚分配：
  *             PA5：用于捕获电容充电时间
  *          使用资源：
  *             TIM2CH1
  ******************************************************************************
  */

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "tpad.h"
#include "capture.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"

int main()
{
    /* 1.变量初始化 */
    TpadStatus tpadStatus = TPAD_HIGH;
    uint32_t timeInMs = HAL_GetTick();
    uint32_t timeNewMS = 0;
    
    /* 2.硬件初始化 */
    HAL_Init();
    SystemClock_Config();
    UartInit();
    LedInit();
    CaptureInit();
    TpadInit();

    /* 3.while循环 */
    while (1)
    {
        timeNewMS = HAL_GetTick();
        TpadScan(&tpadStatus);
        if (tpadStatus == TPAD_RISING)
        {
            LED1 = !LED1;
        }
        if (timeNewMS - timeInMs >= 1000)
        {
            timeInMs = timeNewMS;
            LED0 = !LED0;
        }
    }
}
