#ifndef _WAVEFORM_H_
#define _WAVEFORM_H_

#define WAVE_DEEP_LEN       20

typedef struct
{
    unsigned short tick;
    unsigned char level:1;
}wave_queue_obj;
typedef struct waveform_t
{
    unsigned char pin;
    unsigned char deep;
    unsigned char front;
    unsigned short tick_cnt;
    struct waveform_t* next;
    wave_queue_obj queue[WAVE_DEEP_LEN];
}waveform;


void waveform_init(waveform* wave,unsigned char pin);
void waveform_generate(waveform* wave,unsigned char level,unsigned short tick);
void waveform_loop();
void waveform_timer_init();
unsigned char wave_queue_is_empty(waveform *wave);
#endif