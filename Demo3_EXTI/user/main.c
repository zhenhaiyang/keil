/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-01-29
  * @brief   验证外部中断
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "sys.h"
#include "exti.h"
#include "led.h"
#include "usart.h"

int main()
{
    HAL_Init();
    SystemClock_Config();
    LedInit();
    UartInit();
    ExtiInit();

    printf("hello,zhy!");
    while(1)
    {
        
    }

}
