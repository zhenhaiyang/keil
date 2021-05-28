/**
  ******************************************************************************
  * @file    key.c
  * @author  zhy
  * @version 1.0
  * @date    2021-05-18
  * @brief   按键处理程序
  ******************************************************************************
  */

#include "sys.h"
#include "key.h"
#include "stm32f4xx_hal.h"

//{
/*-----------------------------------私有域:开始--------------------------------------*/

static uint8_t edgeActive = 0;  //有效边沿
static uint8_t levelActive = 0; //有效电平

/*-----------------------------------私有域:结束--------------------------------------*/
//}

/** 
 * @brief 按键边沿初始化 
 * @note 无
 * @param {*}无
 * @retval 无
 */
void KeyInit(void)
{
    /* 1.RCC时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* 2.设置GPIO参数 */
    GPIO_InitTypeDef initGpio;
    initGpio.Pin = GPIO_PIN_0;
    initGpio.Mode = GPIO_MODE_INPUT;
    initGpio.Pull = GPIO_PULLUP;
    initGpio.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &initGpio);

    initGpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOH, &initGpio);

    initGpio.Pull = GPIO_PULLDOWN;
    initGpio.Pull = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOC, &initGpio);
}

/** 
 * @brief 查询按键功能 
 * @note 无
 * @param {*}无
 * @retval 无
 */
void KeyScan(void)
{
    static uint8_t edge = 0;                                                             //触发边缘
    static uint8_t level = 0;                                                            //有效电平
    uint8_t key = get_key_0 | (get_key_1 << 1) | (get_key_2 << 2) | (get_key_wkup << 3); //用后四位作为处理
    key ^= 0x07;                                                                         //将常高的按键置反，取下降沿
    edgeActive = edge & key;                                                             //上一次为有效触发，且当前还是有效电平
    levelActive = level & key;                                                           //上一次为有效电平，且当前还是有效电平
    edge = key & (key ^ level);                                                          //键值发生变化，且当前位有效电平
    level = key;                                                                         //保留上一次有效电平
}

/** 
 * @brief 获得有效电平 
 * @note 无
 * @param {*}无
 * @retval 无
 */
uint8_t KeyGetLevelActive(void)
{
    return levelActive;
}

/** 
 * @brief 获得有效边沿
 * @note 无
 * @param {*}无
 * @retval 无
 */
uint8_t KeyGetEdgeActive(void)
{
    return edgeActive;
}

/** 
 * @brief 有效边沿清空
 * @note 无
 * @param {*} 无
 * @retval 无
 */
void KeyResetEdge(void)
{
    edgeActive = 0;
}
