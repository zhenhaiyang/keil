# keil
不通过STM32CUBE，直接用HAL建立的工程，具体工程描述如下：

## 1.GPIO实验
通过按键控制LED灯。
-  key0：控制同时控制DS0和DS1，按一次，两个灯反转一次
-  key1：控制DS1反转
-  key2：控制DS0反转
-  wkup：控制DS1和DS2互斥点亮



## 2. 串口实验

通过USART1实现串口自发自收。

- 程序初始化会自动发送 hello，zhy！
- LED灯(PB0和PB1)控制，会来回闪动。
- 串口配置为115200，8位字长，无校验，无数据流控制。
- 向串口1发送数据，其会按照原数据返回。
- 对正点原子原有的system文件夹中的文件进行重构(仅支持无系统模式)。
- 实现一个长度为255的无符号型8位整型队列来存储接收数据。
- 发送方式为阻塞发送，接收方式为中断接收。



## 3.外部中断实验

实验效果等同于实验1，但是是通过外部中断形式实现！



## 4.时钟中断实验

自实验开始，HAL库版本升级为STM32Cube_FW_F4_V1.25.0。实验实现不同频率点亮两个LED灯。

- 基本时钟TIM6的时钟频率为500ms。——LED1
- 延时函数延时时间为1000ms。——LED0



## 5.PWM

该实验对外有三个输出管脚：

- PA6 —— 断路管脚，高电平有效。
- PA7—— OC1N输出管脚。
- PA8—— OC1输出管脚

该实验通过TIM1的通道1实现，可以在调试修改compare值来调节PWM输出占空比，且通过改变PA6电平来控制PWM是否输出。



## 6.捕获实验

该实验通过TIM8实现捕获功能，其捕获管脚为PC6。

1. testInput：改变PWM波的占空比
2. testResultPulse：捕获正脉冲时钟数，精度为180MHz
3. testPulseInUS：捕获正脉冲的实际时间，单位为us。
4. testResultPeriod: 捕获PWM一个周期时钟数，精度为180MHz

在调试模式中，通过修改PWM波的占空比，比较ARM硬件捕获的能力。