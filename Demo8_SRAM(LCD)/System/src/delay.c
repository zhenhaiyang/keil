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
  */

#include "stm32f4xx.h" //包含该头文件不会出现引用错误
#include "delay.h"

/**
  * @brief   延时函数微秒
  * @note    当nus取值比较小时，该延时函数的误差比较大
  * @param   nus：延时的微秒数，0~2^32/fac_us
  * @retval  None
  */
void delay_us(uint32_t nus)
{
    uint32_t told = SysTick->VAL;                          //记录当前时间值：放在第一句
    uint32_t fac_us = HAL_RCC_GetSysClockFreq() / 1000000; //us延时倍乘数
    uint32_t ticks = nus * fac_us;                         //存储需要节拍数
    uint32_t tnow, tcnt = 0;                               //told：上一次存储的时间，tnew：当前时间；tcent：累计的时间差
    uint32_t reload = SysTick->LOAD;                       //读取sysTick装载值

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            /*获得上一次记录的时间和当前时间之间的差值*/
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;

            /*将当前时间记录下来*/
            told = tnow;
            if (tcnt >= ticks)
                break; //时间超过/等于要延迟的时间,则退出.
        }
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
