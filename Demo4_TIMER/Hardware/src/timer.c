/**
  ******************************************************************************
  * @file    timer.c
  * @author  zhy
  * @version 1.0
  * @date    2021-02-04
  * @brief   与定时器相关
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "timer.h"
#include "sys.h"
/** 
 * @brief TIM6的初始化
 * @note 无
 * @param {*}无
 * @retval 无
 */
void Tim6Init(void)
{
    /* 1.时钟使能 */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* 2.参数配置 */
    TIM_HandleTypeDef htim6;
    htim6.Instance = TIM6;                                     //tim6
    htim6.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE; //自动加载寄存器影子寄存器使能
    htim6.Init.Prescaler = 9000 - 1;                           //预分频系数为9000，分频后的时钟为10KHz
    htim6.Init.Period = 5000 - 1;                              //计数周期为5000，即500ms触发一次中断
    htim6.Lock = HAL_UNLOCKED;                                 //不上锁
    htim6.State = HAL_TIM_STATE_RESET;                         //状态初值
    HAL_TIM_Base_Init(&htim6);

    /* 3.初始化 */
    HAL_TIM_Base_Start_IT(&htim6); //开启基本定时器的中断计时
}

/** 
 * @brief 定时器的底层设置
 * @note 在HAL_TIM_Base_Init自动调用，设置底层外设，中断等
 * @param {TIM_HandleTypeDef} *htim 时钟句柄
 * @retval 无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 1); //设置中断优先级
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);         //使能中断
    }
}

/** 
 * @brief TIM6中断响应函数
 * @note 中断发生时，自动调用
 * @param {*}无
 * @retval 无
 */
void TIM6_DAC_IRQHandler(void)
{
    if ((TIM6->SR | TIM_SR_UIF) && (TIM6->DIER | TIM_DIER_UIE)) //判断中断类别
    {
        TIM6->SR = ~TIM_SR_UIF; //清除中断标记
        LED1 = !LED1;           //功能反转
    }
}
