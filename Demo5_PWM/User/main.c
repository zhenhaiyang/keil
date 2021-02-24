/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-02-22
  * @brief   用于生成pwm波
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "sys.h"
#include "pwm.h"

uint16_t compare = 9000;
int main()
{
    /* 1.系统初始化 */
    HAL_Init();
    SystemClock_Config();
    PwmInit();

    /* 2.设置占空比 */
    while (1)
    {
        PwmSetCompare(compare);
    }
}
