/**
  ******************************************************************************
  * @file    tpad.c
  * @author  zhy
  * @version 1.0
  * @date    2021-04-26
  * @brief   触摸按键
  *             该功能需要配合TIM2CH1使用
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "tpad.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"

/*-----------------------------------公有域--------------------------------------*/

/*------------------------------------------------------------------------------*/

/*-----------------------------------私有域--------------------------------------*/
uint16_t timeUntouched = 0;

void GetTimeCharge(uint16_t *time);
void GetTimeUntoched(void);
/*------------------------------------------------------------------------------*/

/** 
 * @brief 触摸按键初始化——将按键放电
 * @note 无
 * @param {*}无
 * @retval 无
 */
void TpadInit(void)
{
    /* 1.时钟初始化 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* 2.GPIO初始化 */
    GPIO_InitTypeDef initGpio;
    initGpio.Pin = GPIO_PIN_5;           //pin5
    initGpio.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    initGpio.Speed = GPIO_SPEED_FAST;    //速度：快
    HAL_GPIO_Init(GPIOA, &initGpio);     //初始化PA5
    /* 3.初始化 */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //PA5输出低电平
    /* 4.获取为触摸时的电容充电时间 */
    GetTimeUntoched();
}

/** 
 * @brief 判断按键是否有按下
 * @note 无
 * @param {TpadStatus} *tpadStatus 按键状态
 * @retval 无
 */
void TpadScan(TpadStatus *tpadStatus)
{
    static uint8_t timeHigh = 0;
    uint16_t timeCharge = 0;

    GetTimeCharge(&timeCharge);

    if (timeCharge > timeUntouched * 5 >> 2) //若是平均未触碰充电时间的5/4，则判断是按键按下
    {
        timeHigh = 3;
        switch (*tpadStatus)
        {
        case TPAD_LOW:
            *tpadStatus = TPAD_RISING;
            break;
        case TPAD_RISING:
            *tpadStatus = TPAD_HIGH;
            break;
        default:
            break;
        }
    }
    else //若检测到时间不足
    {
        *tpadStatus = TPAD_HIGH;
        if (timeHigh > 0)
        {
            timeHigh--;
        }
        else
        {
            *tpadStatus = TPAD_LOW;
        }
    }
}

/** 
 * @brief 获取捕获时间
 * @note 无
 * @param {uint32_t} *time 捕获时间
 * @retval 无
 */
void GetTimeCharge(uint16_t *time)
{
    /* PA5：推挽输出 */
    GPIOA->MODER &= ~GPIO_MODER_MODE5;
    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;
    /* 输出低电平 */
    PAout.bit5 = 0;

    delay_us(50);

    /* PA5：复用为TIM2CH1 */
    GPIOA->MODER &= ~GPIO_MODER_MODE5;
    GPIOA->MODER |= GPIO_MODER_MODE5_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5;
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0; //0b0001:AF1

    TIM2->SR = 0x0000;        //清空中断标志
    TIM2->CR1 |= TIM_CR1_CEN; //打开计数器

    while (!(TIM2->SR & TIM_SR_CC1IF)) //等待充电完成
        ;
    TIM2->SR ^= TIM_SR_CC1IF;  //清除中断标记
    TIM2->CNT = 0;             //清空计数器
    TIM2->CR1 &= ~TIM_CR1_CEN; //关闭计数器

    *time = TIM2->CCR1;         //获取捕获值
    printf("time:%d\n", *time); //发送时间值
}

/** 
 * @brief 获取初始时间
 * @note 无
 * @param {*}无
 * @retval 无
 */
void GetTimeUntoched(void)
{
    int16_t temp[10] = {0};
    for (uint8_t i = 0; i < 10; i++)
    {
        GetTimeCharge((uint16_t *)(void *)temp + i);
    }

    int16_t min = 0;
    int16_t max = 0;
    int16_t sub = 0;
    int16_t average = 0;
    for (uint8_t i = 1; i < 10; i++)
    {
        sub = temp[i] - temp[0];     //获取差值
        min = min > sub ? sub : min; //差值的最小值
        max = max < sub ? sub : max; //差值的最大值
        average += sub;              //差值的和
    }
    average -= min + max;                        //剔除最小与最大值
    average >>= 3;                               //差值的平均
    timeUntouched = average + temp[0];           //实际的平均值
    printf("timeUntouched:%d\n", timeUntouched); //输出未按下按键时的充电时间
}
