/**
  ******************************************************************************
  * @file    sys.c
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   系统初始化相关，配置RCC时钟，中断分组
  ******************************************************************************
  */
#include "sys.h"
#include "stm32f4xx.h"

/* 全局变量 */
BitBand PAin __attribute__((at(GPIOA_BASE + 16)));
BitBand PBin __attribute__((at(GPIOB_BASE + 16)));
BitBand PCin __attribute__((at(GPIOC_BASE + 16)));
BitBand PDin __attribute__((at(GPIOD_BASE + 16)));
BitBand PEin __attribute__((at(GPIOE_BASE + 16)));
BitBand PFin __attribute__((at(GPIOF_BASE + 16)));
BitBand PGin __attribute__((at(GPIOG_BASE + 16)));
BitBand PHin __attribute__((at(GPIOH_BASE + 16)));
BitBand PIin __attribute__((at(GPIOI_BASE + 16)));


BitBand PAout __attribute__((at(GPIOA_BASE + 20)));
BitBand PBout __attribute__((at(GPIOB_BASE + 20)));
BitBand PCout __attribute__((at(GPIOC_BASE + 20)));
BitBand PDout __attribute__((at(GPIOD_BASE + 20)));
BitBand PEout __attribute__((at(GPIOE_BASE + 20)));
BitBand PFout __attribute__((at(GPIOF_BASE + 20)));
BitBand PGout __attribute__((at(GPIOG_BASE + 20)));
BitBand PHout __attribute__((at(GPIOH_BASE + 20)));
BitBand PIout __attribute__((at(GPIOI_BASE + 20)));


/** 
 * @brief       系统初始化
 * @note        使用外部晶振，且晶振频率为25Mhz
 * @param {*}   无
 * @retval      无 
 */
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON; //无源晶振的选项
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    HAL_PWREx_EnableOverDrive();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0); //设置systick时钟的中断
}

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
__asm void WFI_SET(void)
{
    WFI;
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
    CPSID I
        BX LR
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
    CPSIE I
        BX LR
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr)
{
    MSR MSP, r0 //set Main Stack value
                 BX r14
}
