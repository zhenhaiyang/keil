/**
  ******************************************************************************
  * @file    main.c
  * @author  zhy
  * @version 1.0
  * @date    2021-01-22
  * @brief   实现与串口的通信
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "queue.h"

int main(void)
{
    //1.系统初始化
    HAL_Init();           //初始化HAL库
    SystemClock_Config(); //设置时钟,180Mhz
    GpioInit();           //GPIO相关配置
    UartInit();           //串口配置初始化

    printf("hello, zhy!\n"); //输出初始化信息
    /* while循环 */
    while (1)
    {
        uint8_t temp = 0;
        while (DeQueue(&queueRx, &temp)) //接收结构体非空，说明已经接收到数据
        {
            while ((USART1->SR & USART_SR_TXE) == 0)
                ; //循环发送,直到发送完毕
            USART1->DR = temp;
        }

        //流水灯功能
        PBout.bit0 = 0;
        PBout.bit1 = 1;

        delay_ms(1000);

        PBout.bit0 = 1;
        PBout.bit1 = 0;
        delay_ms(1000);
    }
}
