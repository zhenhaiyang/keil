/**
  ******************************************************************************
  * @file    usart.h
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   这是与串口相关的C文件。
  *             主要实现了重构printf函数。
  *             串口1的配置
  ******************************************************************************
  * @version 1.1
  * @date    2021-05-27
  * @brief   这是与串口相关的C文件。
  *             将串口的参数初始化
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "usart.h"
#include "queue.h"

/*------------------------------------------------------------------------------*/

Queue queueRx; //串口接收队列

/*------------------------------------------------------------------------------*/

/** 
 * @brief 重构函数 
 * @note 在printf中自动调用
 * @param {int} ch  发送的数据
 * @param {FILE} *f 此处用不到
 * @retval 当前发送值
 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & USART_SR_TXE) == 0)
        ;                     //判断是否发送完成
    USART1->DR = (uint8_t)ch; //发送数据
    return ch;
}

/** 
 * @brief   串口初始化
 * @note  无
 * @param   无
 * @retval  无
 */
void Uart1Init(void)
{
    /* 1.启动时钟 */
    __HAL_RCC_USART1_CLK_ENABLE(); //开启串口1时钟

    /* 2.配置外设相关参数 */
    UART_HandleTypeDef huart = {0};                //串口句柄
    huart.Instance = USART1;                       //串口1
    huart.Init.BaudRate = 115200;                  //波特率
    huart.Init.WordLength = UART_WORDLENGTH_8B;    //字长
    huart.Init.StopBits = UART_STOPBITS_1;         //1位停止位
    huart.Init.Parity = UART_PARITY_NONE;          //非校验
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;    //无硬件控制
    huart.Init.Mode = UART_MODE_TX_RX;             //收发模式
    huart.Init.OverSampling = UART_OVERSAMPLING_8; //8倍过采样
    HAL_UART_Init(&huart);                         //串口1初始化

    /* 3.接收中断相关 */
    __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE); //使能接收中断
}

/** 
 * @brief   串口底层初始化 
 * @note    此函数会在HAL_UART_Init中被调用
 * @param {UART_HandleTypeDef} *huart 串口句柄
 * @retval 无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if (USART1 == huart->Instance) //若是串口1
    {
        /* 1.启动底层时钟 */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* 2.配置gpio底层 */
        GPIO_InitTypeDef gpioInit = {0};
        gpioInit.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        gpioInit.Mode = GPIO_MODE_AF_PP;
        gpioInit.Pull = GPIO_PULLUP;
        gpioInit.Speed = GPIO_SPEED_FAST;
        gpioInit.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &gpioInit);

        /* 3.配置接收中断 */
        InitQueue(&queueRx);                     //使能接收队列
        HAL_NVIC_EnableIRQ(USART1_IRQn);         //使能中断
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3); //设置抢断优先级与子优先级
    }
}

/** 
 * @brief   串口1中断服务函数
 * @note    在此处，只开放接收中断。那么当中断触发时，必然是接收中断。
 *          因为中断函数时间越短越好，采用直接操作寄存器的方式。
 * @param {*}无
 * @retval  无
 */
void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE)) //当接收中断置位且有中断标记
    {
        InQueue(&queueRx, USART1->DR); //将接收的内容放入队列，读取寄存器，自动清除中断标记位
    }
}
