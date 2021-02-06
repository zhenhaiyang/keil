/**
  ******************************************************************************
  * @file    led.c
  * @author  zhy
  * @version 1.0
  * @date    2021-02-04
  * @brief   用于LED的初始化
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "led.h"

/** 
 * @brief LED初始化
 * @note 无
 * @param {*}无
 * @retval 无
 */
void LedInit(void)
{
    /* 1.时钟使能 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 2.外设配置 */
    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin = GPIO_PIN_0 | GPIO_PIN_1; //pin0,pin1
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;    //推挽输出
    gpioInit.Pull = GPIO_PULLUP;            //上拉
    gpioInit.Speed = GPIO_SPEED_LOW;        //慢速
    HAL_GPIO_Init(GPIOB, &gpioInit);        //PB0,PB1

    /* 3.输出管脚初始化 */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);
}
