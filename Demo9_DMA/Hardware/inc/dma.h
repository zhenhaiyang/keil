/**
  ******************************************************************************
  * @file    dma.h
  * @author  zhy
  * @version 1.0
  * @date    2021-05-26
  * @brief   dma.c的对外接口
  ******************************************************************************
  */
#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>

typedef enum
{
    DMA_NONE,
    DMA_ONGOING,
    DMA_FINISH,
} DmaType;

extern void DmaInit(void);
extern void DmaStart(uint32_t memory, uint32_t length);

#endif
