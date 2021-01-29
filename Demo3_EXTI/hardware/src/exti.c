/**
  ******************************************************************************
  * @file    exti.c
  * @author  zhy
  * @version 1.0
  * @date    2021-01-29
  * @brief   与外部中断相关的源文件:
  *             WKUP->PA0
  *             key0->PH3
  *             key1->PH2
  *             key2->PC13
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "exti.h"

/*------------------------------------------------------------------------------*/
uint8_t flagExti0 = 0;
uint8_t flagExti2 = 0;
uint8_t flagExti3 = 0;
uint8_t flagExti15_10 = 0;
/*------------------------------------------------------------------------------*/

/** 
 * @brief 外部中断初始化
 * @note  无
 * @param {*}无
 * @retval 无 
 */
void ExtiInit()
{
    /* 1.gpio时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* 2.gpio初始化 */
    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin = GPIO_PIN_0;           //Pin0
    gpioInit.Mode = GPIO_MODE_IT_RISING; //上升沿
    gpioInit.Pull = GPIO_PULLDOWN;       //下拉
    gpioInit.Speed = GPIO_SPEED_FAST;    //快速
    HAL_GPIO_Init(GPIOA, &gpioInit);     //PA0

    gpioInit.Pin = GPIO_PIN_2 | GPIO_PIN_3; //PIN2和PIN3
    gpioInit.Mode = GPIO_MODE_IT_FALLING;   //下降沿
    gpioInit.Pull = GPIO_PULLUP;            //上拉
    HAL_GPIO_Init(GPIOH, &gpioInit);        //PH2,PH3

    gpioInit.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &gpioInit); //PC13

    /* 3.中断优先级与使能 */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/** 
 * @brief 外部中断0响应函数 
 * @note  无
 * @param {*}无
 * @retval 无
 */
void EXTI0_IRQHandler()
{
    flagExti0 = 1;
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/** 
 * @brief 外部中断2响应函数 
 * @note  无
 * @param {*}无
 * @retval 无
 */
void EXTI2_IRQHandler()
{
    flagExti2 = 1;
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/** 
 * @brief 外部中断3响应函数 
 * @note  无
 * @param {*}无
 * @retval 无
 */
void EXTI3_IRQHandler()
{
    flagExti3 = 1;
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

/** 
 * @brief 外部中断15-10响应函数 
 * @note  无
 * @param {*}无
 * @retval 无
 */
void EXTI15_10_IRQHandler()
{
    flagExti15_10 = 1;
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

/** 
 * @brief 外部中断回调函数 
 * @note  在HAL_GPIO_EXTI_IRQHandler()函数中自动调用
 *             WKUP->PA0
 *             key0->PH3
 *             key1->PH2
 *             key2->PC13
 * @param {uint16_t} GPIO_Pin
 * @retval 无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(100); //消抖
    switch (GPIO_Pin)
    {
    case GPIO_PIN_0:
        if (WKUP == 1) //延时后依然按下
        {
            LED1 = LED0;
            LED0 = !LED0;
        }
        break;
    case GPIO_PIN_2:
        if (KEY1 == 0)
        {
            LED1 = !LED1;
        }
        break;
    case GPIO_PIN_3:
        if (KEY0 == 0)
        {
            LED1 = !LED1;
            LED0 = LED1;
        }
        break;
    case GPIO_PIN_13:
        if (KEY2 == 0)
        {
            LED0 = !LED0;
        }
        break;
    }
}
