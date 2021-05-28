/**
  ******************************************************************************
  * @file    dma.c
  * @author  zhy
  * @version 1.0
  * @date    2021-05-26
  * @brief   dma驱动文件
  ******************************************************************************
  */

#include "dma.h"
#include "stm32f4xx_hal.h"

/** 
 * @brief DMA初始化
 * @note 无
 * @param {*}无
 * @retval 无
 */
void DmaInit(void)
{
    /* 1.RCC时钟使能 */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* 2.DMA初始化 */
    DMA_HandleTypeDef hdma;
    hdma.Instance = DMA2_Stream7;                        //DMA2数据流7
    hdma.Init.Channel = DMA_CHANNEL_4;                   //第4通道
    hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;          //内存到外设
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;              //外设地址不自增
    hdma.Init.MemInc = DMA_MINC_ENABLE;                  //内存地址自增
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //外设数据宽度为字节
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    //内存数据宽度为字节
    hdma.Init.Mode = DMA_NORMAL;                         //普通模式
    hdma.Init.Priority = DMA_PRIORITY_HIGH;              //优先级：高
    hdma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;           //禁用FIFO
    HAL_DMA_Init(&hdma);                                 //hdma初始化

    /* 3.关联外设DMA */
    hdma.Instance->PAR = (uint32_t) & (USART1->DR);
    USART1->CR3 |= USART_CR3_DMAT; //串口发送DMA使能
}

/** 
 * @brief DMA
 * @note 无
 * @param {uint32_t} memory 内存的地址
 * @param {uint32_t} length 发送数据的长度
 * @retval 无
 */
void DmaStart(uint32_t memory, uint32_t length)
{
    DMA2_Stream7->M0AR = memory;
    DMA2_Stream7->NDTR = length;
    DMA2_Stream7->CR |= DMA_SxCR_EN;
}
