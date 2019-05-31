# waveform
方波生成器，适用于任何单片机，调用简单，非阻塞
## 使用方法
将
```c
waveform_loop()
```
在定时器中断中调用
定义一个波形对象
```c
static waveform wave1; 
waveform_init(&wave1,23);// 绑定输出端口为23号引脚
waveform_generate(&wave1,0,1000);// 先输出低电平1000tick
waveform_generate(&wave1,1,2000);// 然后输出高电平2000tick
waveform_generate(&wave1,0,3000);// 最后输出低电平3000tick
```
上述代码实现了mcu的23引脚先低电平输出1000tick，然后高电平2000tick，最后低电平输出3000tick，tick的单位为定时器中断触发一次的时间。

调用
```c
wave_queue_is_empty(&wave1);
```
可以知道波形输出是否完成。
更改
```c
#define WAVE_DEEP_LEN       20
```
来更改波形队列最大深度，即每次波形输出调用waveform_generate函数最大的次数。
## 注意
本代码关于底层的操作都调用https://github.com/zrw269113179/drv_pin 内的函数实现硬件解耦合，如需更换芯片请替换drv_pin.c或直接更改本waveform.c中所有有关gpio的操作。
