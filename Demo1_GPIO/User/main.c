/**
 * @file    main.c  
 * @author  zhy
 * @version 1.0
 * @date    2020-11-12
 * @brief   主函数，代码的入口
 **/
/**实现的功能为：
 * * key0：控制同时控制DS0和DS1，按一次，两个灯反转一次
 * * key1：控制DS1反转
 * * key2：控制DS0反转
 * * wkup：控制DS1和DS2互斥点亮
 * **/
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "led.h"
#include "key.h"

/**
 * @brief   按键控制LED灯
 * @note    无  
 * @param   无 
 * @retval  无
 **/
int main()
{
    
    HAL_Init();                      //库函数初始化
    Stm32_Clock_Init(360, 25, 2, 8); //时钟设置

    delay_init(180); //延时初始化
    LED_init();      //LED初始化
    KEY_init();      //按键初始化

    KeyNum key = nothing;   
    while (1)
    {
        key = scanKey();
        switch (key) //当前状态
        {
        case zero:
            led0 = led0 ^ 0x1; //位异或
            led1 = led0;
            break;
        case one:
            led0 = 0x1;
            led1 = led1 ^ 0x1;
            break;

        case two:
            led0 = led0 ^ 0x1;
            led1 = 0x1;
            break;
        case wkup:             //只要当前按键是该按键
            led0 = led0 ^ 0x1; //位异或
            led1 = led0 ^ 0x1;
            break;
        default:

            break;
        }
    }
}
