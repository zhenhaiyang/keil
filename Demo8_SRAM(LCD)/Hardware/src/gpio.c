/**
  ******************************************************************************
  * @file    
  * @author  zhy
  * @version 
  * @date    2021-05-13
  * @brief   
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "gpio.h"

/** 
 * @brief GPIO初始化
 * @note 无
 * @param {*}无
 * @retval 无
 */
void GpioInit(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef initGpio;
    initGpio.Pin = GPIO_PIN_5;
    initGpio.Mode = GPIO_MODE_OUTPUT_PP;
    initGpio.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOB, &initGpio);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}