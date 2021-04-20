/**
  ******************************************************************************
  * @file    pwm.c
  * @author  zhy
  * @version 1.0
  * @date    2021-02-22
  * @brief   通过高级定时器配置双通道PWM
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "pwm.h"

/** 
 * @brief PWM配置
 * @note 高级寄存器TIM1的通道1
 * @param {*}无
 * @retval 无
 */
void PwmInit(void)
{
    /* 1.使能TIM1的时钟 */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* 2.PWM时钟初始化 */
    TIM_HandleTypeDef htim1 = {0};                              //定时器1句柄
    htim1.Instance = TIM1;                                      //定时器1
    htim1.Init.Prescaler = 1 - 1;                               //此处不分频，提高PWM的精度
    htim1.Init.Period = 18000 - 1;                              //PWM的频率为10K
    htim1.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE; //启用ARR影子寄存器
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;          //死区发生器与采样频率的分频数
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;                //向上计数模式
    htim1.Init.RepetitionCounter = 0;                           //重复次数1
    HAL_TIM_PWM_Init(&htim1);                                   //初始化定时器1的通道1

    /* 3.PWM输出通道初始化 */
    TIM_OC_InitTypeDef ocInit;
    ocInit.OCMode = TIM_OCMODE_PWM1;              //pwm1模式
    ocInit.OCPolarity = TIM_OCPOLARITY_HIGH;      //高电平有效
    ocInit.OCNPolarity = TIM_OCPOLARITY_HIGH;     //高电平有效
    ocInit.OCIdleState = TIM_OCIDLESTATE_RESET;   //闲置状态：低
    ocInit.OCNIdleState = TIM_OCNIDLESTATE_RESET; //闲置状态：低
    ocInit.Pulse = 9000;                          //占空比50%
    ocInit.OCFastMode = TIM_OCFAST_DISABLE;       //不启动快速响应
    HAL_TIM_PWM_ConfigChannel(&htim1, &ocInit, TIM_CHANNEL_1);

    /* 4.配置死区与断路 */
    TIM_BreakDeadTimeConfigTypeDef timBreakDead;
    timBreakDead.OffStateRunMode = TIM_OSSR_DISABLE;           //禁止OSSR
    timBreakDead.OffStateIDLEMode = TIM_OSSI_ENABLE;           //使能OSSI
    timBreakDead.LockLevel = TIM_LOCKLEVEL_OFF;                //关闭上锁等级
    timBreakDead.DeadTime = 0;                                 //死区时间为0
    timBreakDead.BreakState = TIM_BREAK_ENABLE;                //使能断路
    timBreakDead.BreakPolarity = TIM_BREAKPOLARITY_HIGH;       //高电平有效
    timBreakDead.BreakFilter = 0;                              //不使用滤波器
    timBreakDead.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; //自动断路使能
    HAL_TIMEx_ConfigBreakDeadTime(&htim1, &timBreakDead);

    /* 5.启动PWM */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
}

/** 
 * @brief PWM底层驱动配置
 * @note    1.此函数在HAL_TIM_PWM_Init中调用
 *          2.PA7-->PWMN PA8-->PWM PA6-->Break
 * @param {*} 定时器句柄
 * @retval 无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    /* 1.启动时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 2.GPIOA初始化 */
    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8; //pin6，7，8
    gpioInit.Pull = GPIO_PULLDOWN;                       //上拉
    gpioInit.Speed = GPIO_SPEED_HIGH;                    //快速
    gpioInit.Mode = GPIO_MODE_AF_PP;                     //复用推挽
    gpioInit.Alternate = GPIO_AF1_TIM1;                  //复用为TIM1通道接口
    HAL_GPIO_Init(GPIOA, &gpioInit);                     //PA6，PA7，PA8
}

/** 
 * @brief 设置比较值
 * @note 用于Tim1的通道1
 * @param {uint16_t} c 比较寄存器的值
 * @retval 无
 */
void PwmSetCompare(uint16_t c)
{
    TIM1->CCR1 = c;
}
