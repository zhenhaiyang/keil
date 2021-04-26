/**
  ******************************************************************************
  * @file    capture.c
  * @author  zhy
  * @version 1.0
  * @date    2021-04-07
  * @brief   捕获功能相关配置
  ******************************************************************************
  */

#include "capture.h"
#include "stm32f4xx_hal.h"

/*------------------------------------------------------------------------------*/

uint16_t timeTim2Up = 0;
CaptureResult captureTim2 = {0};

/*------------------------------------------------------------------------------*/

/** 
 * @brief 捕获初始化 
 * @note 无
 * @param {*}无
 * @retval 无
 */
void CaptureInit(void)
{
    /* 1.RCC时钟初始化 */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* 2.基本时钟初始化 */
    TIM_HandleTypeDef htim = {0};
    htim.Instance = TIM2;                                     //时钟为TIM2
    htim.Init.Prescaler = 1 - 1;                              //预分频为0
    htim.Init.Period = 0xFFFFFFFF;                            //使用最大值
    htim.Init.RepetitionCounter = 0;                          //无重复模式
    htim.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE; //支持影子寄存器
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;         //不分频
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;               //向上计数模式
    HAL_TIM_IC_Init(&htim);                                   //初始化TIM2

    /* 3.输入通道配置设置 */
    TIM_IC_InitTypeDef initTim2Ic = {0};
    initTim2Ic.ICFilter = 0;                                     //不用滤波
    initTim2Ic.ICPrescaler = TIM_ICPSC_DIV1;                     //不分频
    initTim2Ic.ICPolarity = TIM_ICPOLARITY_RISING;               //捕捉上升沿
    initTim2Ic.ICSelection = TIM_ICSELECTION_DIRECTTI;           //使用默认捕捉源
    HAL_TIM_IC_ConfigChannel(&htim, &initTim2Ic, TIM_CHANNEL_1); //初始化TIM2通道1

    /* 4.启动定时器 */
    TIM_CCxChannelCmd(htim.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE); //启动通道1
    __HAL_DBGMCU_FREEZE_TIM2();
    __HAL_TIM_ENABLE_IT(&htim, TIM_IT_UPDATE); //启动更新中断
}

/** 
 * @brief TIM8底层配置
 * @note 该函数自动被HAL_TIM_IC_Init()调用
 *       TIM8CH1->PC6
 * @param {TIM_HandleTypeDef} *htim 定时器句柄
 * @retval 无
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    /* 1.初始化时钟 */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* 2.配置GPIO */
    GPIO_InitTypeDef initGpio;
    initGpio.Pin = GPIO_PIN_5;          //pin6
    initGpio.Pull = GPIO_PULLDOWN;      //下拉
    initGpio.Speed = GPIO_SPEED_HIGH;   //速度快
    initGpio.Mode = GPIO_MODE_AF_PP;    //推挽复用
    initGpio.Alternate = GPIO_AF3_TIM8; //复用为TIM8
    HAL_GPIO_Init(GPIOA, &initGpio);    //初始化PC6

    /* 3.配置中断 */
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/** 
 * @brief TIM8自动更新计数器中断服务函数
 * @note 自动触发
 * @param {*} 无
 * @retval 无
 */
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & TIM_SR_UIF) && (TIM2->DIER & TIM_DIER_UIE)) //判断是否为更新中断
    {
        TIM2->SR &= ~TIM_SR_UIF; //写0清除中断标记
        timeTim2Up++;
    }
}
