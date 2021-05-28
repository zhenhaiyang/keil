/**
  ******************************************************************************
  * @name    delay.c
  * @author  zhy
  * @version 1.0
  * @date    2020.11.11
  * @brief   通过时钟窃取法，得到延时时钟的功能。
  ******************************************************************************
  * @version 1.1
  * @date    2021.04.07
  * @brief   更新delay_us函数，减小delay误差
  ******************************************************************************
  * @version 1.2
  * @date    2021.05.21
  * @brief   更新delay_us函数，减小delay误差
  ******************************************************************************
  */

#include "stm32f4xx.h" //包含该头文件不会出现引用错误
#include "delay.h"

//{
/*-----------------------------------私有域:开始--------------------------------------*/

uint32_t fac_us = 0; //us延时倍乘数
uint32_t reload = 0; //读取sysTick装载值

/*-----------------------------------私有域:结束--------------------------------------*/
//}

/** 
 * @brief delay函数初始化
 * @note 使用delay函数必须要初始化
 * @param {*}无
 * @retval 无
 */
void delay_init(void)
{
    fac_us = HAL_RCC_GetSysClockFreq() / 1000000; //系统主频/10^6
    reload = SysTick->LOAD;                       //过边界点
}

/**
  * @brief   延时函数微秒
  * @note    当nus取值比较小时，该延时函数的误差比较大
  * @param   nus：延时的微秒数，0~2^32/fac_us
  * @retval  None
  */
void delay_us(uint32_t nus)
{
    uint32_t told = SysTick->VAL;  //记录当前时间值：放在第一句
    uint32_t ticks = nus * fac_us; //存储需要节拍数
    uint32_t tnow, tcnt = 0;       //told：上一次存储的时间，tnew：当前时间；tcent：累计的时间差

    while (1)
    {
        tnow = SysTick->VAL;                                                    //时钟时递减时钟
        tcnt = tnow <= told ? tcnt + told - tnow : tcnt + reload - tnow + told; //获得上一次记录的时间和当前时间之间的差值
        if (tcnt >= ticks)
            return;  //时间超过/等于要延迟的时间,则退出.
        told = tnow; //将当前时间记录下来
    }
}

/**
  * @brief   延时函数毫秒
  * @note    None
  * @param   nms：延时的毫秒数
  * @retval  None
  */
void delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++)
        delay_us(1000);
}

