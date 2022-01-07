

/**
*Classe de timer e algumas funções de tempo...
*Provavelmente não é tão preciso quanto poderia ser, mas é fácil de usar e fornece alguns recursos legais.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_TIMER_H
#define DLB_TIMER_H

#include<chrono>

namespace dlb
{
enum dlb_timer_durations
{
dlb_timer_invalid=0,
dlb_timer_nano,//Nanosegundos
dlb_timer_micro,//Microsegundos
dlb_timer_ms,//Milisegundos
dlb_timer_seg,//segundos
dlb_timer_min//minutos
};

class dlb_timer
{
private:
uint32 timer_type;
int64 timeout;
int64 min_timeout;
int64 max_timeout;
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
bool paused;
public:
dlb_timer();
dlb_timer(const dlb_timer& tmr);
dlb_timer& operator=(const dlb_timer& tmr);
bool ispaused()const;
bool isrunning()const;
void pause();
void resume();
void restart();
int64 elapsed(uint32 dlb_timer_type=dlb_timer_ms);
bool force(int64 time_count, uint32 dlb_timer_type=dlb_timer_ms);
void wait(int64 time_wait, uint32 dlb_timer_type=dlb_timer_ms);
uint32 get_type()const;
int64 get_timeout()const;
bool set_timeout(int64 timeout, uint32 timer_type=dlb_timer_ms);
void set_timeout_range(int64 min_timeout, int64 max_timeout, uint32 timer_type=dlb_timer_ms);
void get_timeout_range(int64* min_timeout, int64* max_timeout);
bool is_timeout();
};

//functions...
void dlb_wait(int64 timeout, uint32 dlb_time=dlb_timer_ms);
int64 dlb_gettimestamp(uint32 time_type=dlb_timer_ms);
}
#endif
