/**
  ******************************************************************************
  * @file    
  * @author  zhy
  * @version 
  * @date    2021-05-13
  * @brief   
  ******************************************************************************
  */
#include "stm32f4xx_hal.h"
#include "fmc.h"

/** 
 * @brief SRAM初始化
 * @note 无
 * @param {*}无
 * @retval 无
 */
void SramInit(void)
{
    __HAL_RCC_FMC_CLK_ENABLE();

    SRAM_HandleTypeDef hsram1 = {0};
    hsram1.Instance = FMC_NORSRAM_DEVICE;                           //设置SRAM的寄存器地址
    hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;                  //设置SRAM拓展寄存器地址
    hsram1.Init.NSBank = FMC_NORSRAM_BANK1;                         //设置存储区域的地址
    hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;      //设置数据与地址
    hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;                  //存储类型
    hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;     //总线长度
    hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;    //不支持突发模式
    hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_HIGH; //等待信号极性：高
    hsram1.Init.WrapMode = FMC_WRAP_MODE_DISABLE;                   //禁止回卷模式
    hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;       //等待状态一个数据周期NWAIT有效
    hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;        //写操作使能
    hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;               //禁用等待信号
    hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;            //打开拓展模式
    hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;   //同步等待
    hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;               //写突发禁止
    hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;   //连续时钟同步
    hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;                      //页大小无

    FMC_NORSRAM_TimingTypeDef readTiming = {0};
    readTiming.AccessMode = FMC_ACCESS_MODE_A; //读取模式为：A
    readTiming.AddressSetupTime = 15;          //地址建立时间(15/180M=83ns)
    readTiming.DataSetupTime = 9;              //数据建立时间(9/180M=50ns)
    readTiming.BusTurnAroundDuration = 8;      //总线反转时间((15+9+8)/180M=177ns)

    FMC_NORSRAM_TimingTypeDef writeTiming = {0};
    writeTiming.AccessMode = FMC_ACCESS_MODE_A; //写入模式为：A
    writeTiming.AddressSetupTime = 3;           //地址建立时间:(3/180M=16.6ns)
    writeTiming.DataSetupTime = 3;              //数据建立时间:(3/180M=16.6ns)
    writeTiming.BusTurnAroundDuration = 1;      //总线反转时间:(7/180M=38.8ns)

    HAL_SRAM_Init(&hsram1, &readTiming, &writeTiming); //SRAM初始化
}

/** 
 * @brief SRAM初始化
 * @note 无
 * @param {SRAM_HandleTypeDef} *hsram SRAM的句柄
 * @retval 无
 */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef initGpio;
    initGpio.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 |
                   GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5;
    initGpio.Speed = GPIO_SPEED_HIGH;
    initGpio.Mode = GPIO_MODE_AF_PP;
    initGpio.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &initGpio);

    initGpio.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                   GPIO_PIN_14 | GPIO_PIN_15;

    HAL_GPIO_Init(GPIOE, &initGpio);
}
