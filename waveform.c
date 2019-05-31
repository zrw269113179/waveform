#include "HeaderInclude.h"

static waveform *head = 0x00;
/**
 * @brief 波形对象初始化
 * 
 * @param wave 波形对象
 * @param pin 输出引脚
 */
void waveform_init(waveform *wave, unsigned char pin)
{
    wave->front = 0;
    wave->deep = 0;
    wave->pin = pin;
    wave->tick_cnt = 0;
    if (head == 0x00)
    {
        head = wave;
    }
    else
    {
        wave->next = head;
        head = wave;
    }

}
wave_queue_obj *get_first_wave(waveform *wave)
{
    if (wave->deep != wave->front)
    {
        return &wave->queue[wave->front];
    }
    return 0;
}
/**
 * @brief 是否有波形在输出
 * 
 * @param wave 
 * @return unsigned char 
 */
unsigned char wave_queue_is_empty(waveform *wave)
{
    if (wave->deep != wave->front)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
/**
 * @brief 波形生成函数
 * 
 * @param wave 波形
 * @param level 高低电平输出
 * @param tick 输出时间
 */
void waveform_generate(waveform *wave, unsigned char level, unsigned short tick)
{


    if ((wave->deep + 1) % WAVE_DEEP_LEN != wave->front)
    {
        wave->queue[wave->deep].tick = tick;
        wave->queue[wave->deep].level = level;
        wave->deep++;
        if (wave->deep >= WAVE_DEEP_LEN)
        {
            wave->deep -= WAVE_DEEP_LEN;
        }
    }
}
wave_queue_obj *waveform_quit_queue(waveform *wave)
{
    if (wave->deep != wave->front)
    {
        wave_queue_obj *temp = &wave->queue[wave->front];
        wave->front++;
        if (wave->front >= WAVE_DEEP_LEN)
        {
            wave->front -= WAVE_DEEP_LEN;
        }
        return temp;
    }
    return 0;
}
/**
 * @brief 波形轮询代码，在定时器中断中调用
 * 
 */
void waveform_loop()
{
    waveform *iterator = head;
    while (iterator != 0)
    {
        if (iterator->deep != iterator->front)
        {
            wave_queue_obj *temp = &iterator->queue[iterator->front];
            pin_write(iterator->pin,temp->level);
            iterator->tick_cnt++;
            if (iterator->tick_cnt > temp->tick)
            {
                waveform_quit_queue(iterator);
                iterator->tick_cnt=0;
            }
        }
        iterator = iterator->next;
    }
}
/**
 * @brief 波形定时器初始化，初始化定时器，根据不同芯片更改
 * 
 */
void waveform_timer_init()
{
    TIM2_DeInit();

    /* Configure a 100us tick */
    /* TIM2_Period = 0.0001*(8000000/prescaler) */
    TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, CLK_FREQUENCY/10000);
    TIM2_ARRPreloadConfig(ENABLE);
    /* Generate an interrupt on timer count overflow */
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);
    /* Enable TIM1 */
    TIM2_Cmd(ENABLE);
}