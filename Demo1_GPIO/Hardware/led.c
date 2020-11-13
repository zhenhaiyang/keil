/**
 * @file    delay.c
 * @author  zhy
 * @version 1.0
 * @date    2020-11-12
 * @brief   提供LED灯初始化函数以及控制宏
 **/

#include "sys.h"
#include "led.h"


/**
 * @brief   LED初始化 
 * @note    PB0-DS0，PB1-DS1
 * @param   无
 * @retval  无
 **/
void LED_init()
{
    //1.打开时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    //2.GPIO初始化
    GPIO_InitTypeDef gpioinit;
    gpioinit.Pin = GPIO_PIN_0 | GPIO_PIN_1; //管脚0和管脚1
    gpioinit.Pull = GPIO_PULLUP;            //上拉
    gpioinit.Mode = GPIO_MODE_OUTPUT_PP;    //推挽输出
    gpioinit.Speed = GPIO_SPEED_FAST;       //高速

    HAL_GPIO_Init(GPIOB,&gpioinit);
    
    //3.设置GPIO初始电平
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0|GPIO_PIN_1,GPIO_PIN_SET);

}
