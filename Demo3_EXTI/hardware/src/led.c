/**
  ******************************************************************************
  * @file    gpio.c
  * @author  zhy
  * @version 1.0
  * @date    2021-01-23
  * @brief   用于GPIO的配置
  ******************************************************************************
  */

#include "led.h"
#include "stm32f4xx.h"
/** 
 * @brief   GPIO初始化
 * @note    此处用于LED灯，当管脚为低电平时，LED亮2
 * @param {*}无
 * @retval  无
 */
void LedInit(void)
{
    /* 1.开启时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 2.LED初始化 */
    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin = GPIO_PIN_0 | GPIO_PIN_1; //led对应的端口
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;    //推挽输出
    gpioInit.Pull = GPIO_PULLUP;            //上拉
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;  //电平切换速度快

    HAL_GPIO_Init(GPIOB, &gpioInit); //对gpio进行初始化

    /* 3.配置GPIOB的默认状态 */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}
