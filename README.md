# keil
不通过STM32CUBE，直接用HAL建立的工程，具体工程描述如下：

## 1.GPIO实验
通过按键控制LED灯。
-  key0：控制同时控制DS0和DS1，按一次，两个灯反转一次
-  key1：控制DS1反转
-  key2：控制DS0反转
-  wkup：控制DS1和DS2互斥点亮
