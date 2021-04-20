/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-04-07
  * @brief   捕获功能验证
  *             1.testInput：改变PWM波的占空比
  *             2.testResultPulse：捕获正脉冲时钟数，精度为180MHz
  *             3.testPulseInUS：捕获正脉冲的实际时间，单位为us。
  *             4.testResultPeriod: 捕获PWM一个周期时钟数，精度为180MHz
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "pwm.h"
#include "capture.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"

uint16_t testInput = 9000;
uint32_t testResultPulse = 0;
uint32_t testResultPeriod = 0;
double testPulseInUS = 0;

int main()
{

    HAL_Init();
    SystemClock_Config();
    UartInit();
    PwmInit();
    CaptureInit();

    while (1)
    {
        PwmSetCompare(testInput);                                                    //改变PWM波的占空比
        testResultPulse = (captureTim8.times[0] << 16) + captureTim8.counter[0] + 2; //加2用于补偿RESULT固定误差
        testResultPeriod = (captureTim8.times[1] << 16) + captureTim8.counter[1] + 2;
        testPulseInUS = testResultPulse / 180.0;
    }
}
