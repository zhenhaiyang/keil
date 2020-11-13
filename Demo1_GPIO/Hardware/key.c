/**
 * @file    key.c
 * @author  zhy
 * @version 1.0
 * @date    2020-11-12
 * @brief   用于控制按键输入
 **/

#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

/**
 * @brief   KEY初始化
 * @note    None
 * @param   None
 * @retval  None
 **/

void KEY_init()
{
    /*******************1.打开时钟*********************************/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /*******************2.初始化按键*********************************/
    GPIO_InitTypeDef gpioinit;
    //配置PH2和PH1
    gpioinit.Pin = GPIO_PIN_2 | GPIO_PIN_3; //PH2对应key1，PH0对应key0
    gpioinit.Mode = GPIO_MODE_INPUT;        //两者为输入
    gpioinit.Pull = GPIO_PULLUP;            //低电平有效配置成上拉
    gpioinit.Speed = GPIO_SPEED_LOW;        //按键输入

    HAL_GPIO_Init(GPIOH, &gpioinit); //配置key0和key1

    //配置PC13
    gpioinit.Pin = GPIO_PIN_13;      //PC13对应key2
    HAL_GPIO_Init(GPIOC, &gpioinit); //配置key2

    //配置PA0
    gpioinit.Pin = GPIO_PIN_0;       //PA0对应WK_UP
    gpioinit.Pull = GPIO_PULLDOWN;   //该按键是高电平有效，需要下拉电阻
    HAL_GPIO_Init(GPIOA, &gpioinit); //配置按键WK_UP
}
/**
 * @brief   按键查询 
 * @note    消抖的时间为100ms，按键支持连续按
 * @param   无
 * @retval  枚举类型，返回按键值
 **/
KeyNum scanKey()
{
    KeyNum result = nothing;
    if (HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_3) == GPIO_PIN_RESET) //key0低电平有效
    {
        delay_ms(100);
        if (HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_3) == GPIO_PIN_RESET) //再次判断以消抖
        {
            result = zero;
        }
    }
    if (HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_2) == GPIO_PIN_RESET) //key1低电平有效
    {
        delay_ms(100);
        if (HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_2) == GPIO_PIN_RESET) //再次判断以消抖
        {
            result = one;
        }
    }
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) //key2低电平有效
    {
        delay_ms(100);
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) //再次判断以消抖
        {
            result = two;
        }
    }
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) //key3低电平有效
    {
        delay_ms(100);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) //再次判断以消抖
        {
            result = wkup;
        }
    }
    return result;
}
