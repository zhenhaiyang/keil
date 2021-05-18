/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-05-13
  * @brief   通过SRAM控制LCD
  *             1.输出汉字
  *             2.输出英文字符串
  *             3.输出数字
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "gpio.h"
#include "led.h"
#include "fmc.h"
#include "sys.h"
#include "delay.h"

int main()
{
    HAL_Init();
    SystemClock_Config();
    LedInit();
    GpioInit();
    SramInit();
    LcdInit();

    const uint8_t encode[] = {0, 1, 2, 255, 253};
    LcdShowChinese(encode);
    LcdShowNumber(123);
    LcdShowString("\nHello, zhy!");
    
    while (1)
    {
        delay_ms(500);
        LED0 = !LED0;
        LED1 = !LED0;
    }
}
