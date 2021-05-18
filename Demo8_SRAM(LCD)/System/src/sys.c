/**
  ******************************************************************************
  * @file    sys.c
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   系统初始化相关，配置RCC时钟，中断分组
  ******************************************************************************
  * @version 1.1
  * @date    2021-04-07
  * @brief   改变位带定义地址写法，提高程序的兼容性
  ******************************************************************************
  */
#include "sys.h"
#include "stm32f4xx.h"

/*------------------------------------------------------------------------------*/

BitBand PAin __attribute__((at((unsigned long)&(GPIOA->IDR))));
BitBand PBin __attribute__((at((unsigned long)&(GPIOB->IDR))));
BitBand PCin __attribute__((at((unsigned long)&(GPIOC->IDR))));
BitBand PDin __attribute__((at((unsigned long)&(GPIOD->IDR))));
BitBand PEin __attribute__((at((unsigned long)&(GPIOE->IDR))));
BitBand PFin __attribute__((at((unsigned long)&(GPIOF->IDR))));
BitBand PGin __attribute__((at((unsigned long)&(GPIOG->IDR))));
BitBand PHin __attribute__((at((unsigned long)&(GPIOH->IDR))));
BitBand PIin __attribute__((at((unsigned long)&(GPIOI->IDR))));



BitBand PAout __attribute__((at((unsigned long)&(GPIOA->ODR))));
BitBand PBout __attribute__((at((unsigned long)&(GPIOB->ODR))));
BitBand PCout __attribute__((at((unsigned long)&(GPIOC->ODR))));
BitBand PDout __attribute__((at((unsigned long)&(GPIOD->ODR))));
BitBand PEout __attribute__((at((unsigned long)&(GPIOE->ODR))));
BitBand PFout __attribute__((at((unsigned long)&(GPIOF->ODR))));
BitBand PGout __attribute__((at((unsigned long)&(GPIOG->ODR))));
BitBand PHout __attribute__((at((unsigned long)&(GPIOH->ODR))));
BitBand PIout __attribute__((at((unsigned long)&(GPIOI->ODR))));

/*------------------------------------------------------------------------------*/

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
