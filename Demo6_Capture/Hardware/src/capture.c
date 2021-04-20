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

uint16_t timeTim8Up = 0;
CaptureResult captureTim8 = {0};

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
    __HAL_RCC_TIM8_CLK_ENABLE();

    /* 2.基本时钟初始化 */
    TIM_HandleTypeDef htim8 = {0};
    htim8.Instance = TIM8;                                     //时钟为TIM8
    htim8.Init.Prescaler = 1 - 1;                              //预分频为0
    htim8.Init.Period = 0xFFFF;                                //使用最大值
    htim8.Init.RepetitionCounter = 0;                          //无重复模式
    htim8.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE; //支持影子寄存器
    htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;         //不分频
    htim8.Init.CounterMode = TIM_COUNTERMODE_UP;               //向上计数模式
    HAL_TIM_IC_Init(&htim8);                                   //初始化TIM8

    /* 3.输入通道配置设置 */
    TIM_IC_InitTypeDef initTim8Ic = {0};
    initTim8Ic.ICFilter = 0;                                      //不用滤波
    initTim8Ic.ICPrescaler = TIM_ICPSC_DIV1;                      //不分频
    initTim8Ic.ICPolarity = TIM_ICPOLARITY_RISING;                //捕捉上升沿
    initTim8Ic.ICSelection = TIM_ICSELECTION_DIRECTTI;            //使用默认捕捉源
    HAL_TIM_IC_ConfigChannel(&htim8, &initTim8Ic, TIM_CHANNEL_1); //初始化TIM8通道1
    initTim8Ic.ICPolarity = TIM_ICPOLARITY_FALLING;               //捕捉下降沿
    initTim8Ic.ICSelection = TIM_ICSELECTION_INDIRECTTI;          //使用替换捕捉源
    HAL_TIM_IC_ConfigChannel(&htim8, &initTim8Ic, TIM_CHANNEL_2); //初始化TIM8通道2

    /* 4.设置SLAVE模式 */
    TIM_SlaveConfigTypeDef configTim8Slave = {0};
    configTim8Slave.SlaveMode = TIM_SLAVEMODE_RESET;              //复位模式
    configTim8Slave.TriggerFilter = 0;                            //不滤波
    configTim8Slave.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1; //不预分频
    configTim8Slave.InputTrigger = TIM_TS_TI1FP1;                 //触发源为TI1FP1
    configTim8Slave.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING; //上升沿触发
    HAL_TIM_SlaveConfigSynchro(&htim8, &configTim8Slave);         //配置TIM8的通道1位触发源

    /* 5.启动定时器 */
    HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1); //启动通道1
    HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2); //启动通道2
    __HAL_TIM_ENABLE_IT(&htim8, TIM_IT_UPDATE); //启动更新中断
    __HAL_TIM_URS_ENABLE(&htim8);               //更新中断只有溢出时触发
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
    initGpio.Pin = GPIO_PIN_6;          //pin6
    initGpio.Pull = GPIO_PULLDOWN;      //下拉
    initGpio.Speed = GPIO_SPEED_HIGH;   //速度快
    initGpio.Mode = GPIO_MODE_AF_PP;    //推挽复用
    initGpio.Alternate = GPIO_AF3_TIM8; //复用为TIM8
    HAL_GPIO_Init(GPIOC, &initGpio);    //初始化PC6

    /* 3.配置中断 */
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
    HAL_NVIC_SetPriority(TIM8_CC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
}

/** 
 * @brief TIM8自动更新计数器中断服务函数
 * @note 自动触发
 * @param {*} 无
 * @retval 无
 */
void TIM8_UP_TIM13_IRQHandler(void)
{
    if ((TIM8->SR & TIM_SR_UIF) && (TIM8->DIER & TIM_DIER_UIE)) //判断是否为更新中断
    {
        TIM8->SR &= ~TIM_SR_UIF; //写0清除中断标记
        timeTim8Up++;
    }
}

/** 
 * @brief TIM8通道中断服务函数
 * @note  自动触发
 * @param {*} 无
 * @retval 无
 */
void TIM8_CC_IRQHandler(void)
{
    if ((TIM8->SR & TIM_SR_CC1IF) && (TIM8->DIER & TIM_DIER_CC1IE)) //通道1：上升沿
    {
        captureTim8.times[1] = timeTim8Up;   //保存溢出次数
        timeTim8Up = 0;                      //一个周期完成，清空溢出此处
        captureTim8.counter[1] = TIM8->CCR1; //保存上升沿锁存值，且清除中断标记
    }
    else if ((TIM8->SR & TIM_SR_CC2IF) && (TIM8->DIER & TIM_DIER_CC2IE)) //通道2：下降沿
    {
        captureTim8.times[0] = timeTim8Up;   //保存溢出次数
        captureTim8.counter[0] = TIM8->CCR2; //保存下降沿锁存值，且清除中断标记
    }
}
