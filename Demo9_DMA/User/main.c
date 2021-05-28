/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-05-26
  * @brief   DMA实验
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "sys.h"
#include "fmc.h"
#include "gpio.h"
#include "dma.h"
#include "lcd.h"
#include "key.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"

#define LENGTH 8000
static uint8_t testTxDma[LENGTH] = {0}; //因为数组过长，所以放置在此处

int main()
{
    /* 1.变量初始化 */
    unsigned int i = 0;
    unsigned int j = 0;

    uint32_t timer = 0;
    uint32_t timerLed = 0;
    uint32_t timerKey = 0;

    uint8_t keyEdge = 0;

    const LcdCoordinate focusTips = {64, 0};
    const LcdCoordinate focusPercent = {80, 0};

    const uint8_t *temp = "hello, word!\n";

    DmaType flagDma = DMA_NONE;

    /* 2.硬件初始化 */
    HAL_Init();
    SystemClock_Config();
    SramInit();
    GpioInit();
    LcdInit();
    Uart1Init();
    KeyInit();
    LedInit();
    DmaInit();

    /* 3.发送内容初始化 */
    for (i = 0; i < LENGTH; i++)
    {
        if (temp[j] == 0)
        {
            j = 0;
        }
        testTxDma[i] = temp[j];
        j++;
    }

    /* 4.lcd显示初始化 */
    LcdSetColor(LCD_RED, LCD_WHITE);
    LcdShowString("Demo9 DMA\n");
    LcdShowString("Zhen Haiyang\n");
    LcdShowString("2021/05/27\n");
    LcdShowString("--KEY0:Start--\n");
    LcdSetColor(LCD_BLUE, LCD_WHITE);

    /* 5.while循环 */
    while (1)
    {
        timer = HAL_GetTick();

        /* key */
        keyEdge = 0; //将有效边沿清0
        if (timer - timerKey >= 10)
        {
            timerKey = timer;             //时间复位
            KeyScan();                    //按键扫描
            keyEdge = KeyGetEdgeActive(); //获得有效按键边沿
        }

        /* usart */
        if (keyEdge & KEY_0) //key0按下
        {
            DmaStart((uint32_t)&testTxDma, LENGTH);
            flagDma = DMA_ONGOING;
        }
        if (DMA2->HISR & DMA_HISR_TCIF7) //传输完成
        {
            flagDma = DMA_FINISH;
            DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
        }

        /* lcd */
        if (flagDma == DMA_FINISH)
        {
            flagDma = DMA_NONE;
            LcdSetFocus(focusTips);
            LcdShowString("Trassmit Finish!");
            LcdSetFocus(focusPercent);
            LcdShowNumber(100, 3);
            LcdShowString("%");
        }
        else if (flagDma == DMA_ONGOING)
        {
            uint16_t temp = LENGTH - DMA2_Stream7->NDTR;
            temp = temp * 100 / LENGTH;
            LcdSetFocus(focusTips);
            LcdShowString("Trassmiting...  ");
            LcdSetFocus(focusPercent);
            LcdShowNumber(temp, 3);
            LcdShowString("%");
        }

        /* led */
        if (timer - timerLed >= 500) //500ms
        {
            timerLed = timer;
            set_led_0 = !set_led_0;
        }
        if (keyEdge & KEY_1)
        {
            set_led_1 = !set_led_1;
        }
    }
}
