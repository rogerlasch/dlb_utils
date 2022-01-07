
#include<thread>
#include"dlb_types.h"
#include"dlb_math.h"
#include"dlb_timer.h"

using namespace std;

namespace dlb
{
dlb_timer::dlb_timer()
{
this->set_timeout(dlb_timer_invalid, dlb_timer_invalid);
this->restart();
}

dlb_timer::dlb_timer(const dlb_timer& tmr)
{
*this=tmr;
}

dlb_timer& dlb_timer::operator=(const dlb_timer& tmr)
{
this->timer_type=tmr.timer_type;
this->timeout=tmr.timeout;
this->min_timeout=tmr.min_timeout;
this->max_timeout=tmr.max_timeout;
this->start=tmr.start;
this->end=tmr.end;
this->paused=tmr.paused;
return *this;
}

bool dlb_timer::ispaused()const
{
return this->paused;
}

bool dlb_timer::isrunning()const
{
return paused==false;
}

void dlb_timer::pause()
{
if(this->ispaused())
{
return;
}
end=chrono::high_resolution_clock::now();
paused=true;
}

void dlb_timer::resume()
{
if(this->isrunning())
{
return;
}
chrono::high_resolution_clock::time_point tp=chrono::high_resolution_clock::now();
chrono::milliseconds milis(this->elapsed(dlb_timer_ms));
tp-=milis;
this->start=tp;
paused=false;
}

void dlb_timer::restart()
{
if(timer_type!=dlb_timer_invalid)
{
if(min_timeout!=max_timeout)
{
timeout=dlb_random_int64(min_timeout, max_timeout);
}
}
this->start=chrono::high_resolution_clock::now();
this->end=start;
paused=false;
}

int64 dlb_timer::elapsed(uint32 dlb_timer_type)
{
if(this->isrunning())
{
this->end=chrono::high_resolution_clock::now();
}
switch(dlb_timer_type)
{
case dlb_timer_nano:
{
return static_cast<int64>(std::chrono::duration_cast<std::chrono::nanoseconds>(this->end-this->start).count());
}
case dlb_timer_micro:
{
return static_cast<int64>(std::chrono::duration_cast<std::chrono::microseconds>(this->end-this->start).count());
}
case dlb_timer_ms:
{
int64 x=static_cast<int64>(std::chrono::duration_cast<std::chrono::milliseconds>(this->end-this->start).count());
return x;
}
case dlb_timer_seg:
{
return static_cast<int64>(std::chrono::duration_cast<std::chrono::seconds>(this->end-this->start).count());
}
case dlb_timer_min:
{
return static_cast<int64>(std::chrono::duration_cast<std::chrono::minutes>(this->end-this->start).count());
}
}
return -1;
}

bool dlb_timer::force(int64 timer_count, uint32 dlb_timer_type)
{
switch(dlb_timer_type)
{
case dlb_timer_nano:
{
std::chrono::nanoseconds sf(timer_count);
this->start=std::chrono::high_resolution_clock::now();
this->start-=sf;
return true;
}
case dlb_timer_micro:
{
std::chrono::microseconds sf(timer_count);
this->start=std::chrono::high_resolution_clock::now();
this->start-=sf;
return true;
}
case dlb_timer_ms:
{
std::chrono::milliseconds sf(timer_count);
this->start=std::chrono::high_resolution_clock::now();
this->start-=sf;
return true;
}
case dlb_timer_seg:
{
std::chrono::seconds sf(timer_count);
this->start=std::chrono::high_resolution_clock::now();
this->start-=sf;
return true;
}
case dlb_timer_min:
{
std::chrono::minutes sf(timer_count);
this->start=std::chrono::high_resolution_clock::now();
this->start-=sf;
return true;
}
}
return false;
}

void dlb_timer::wait(int64 wait_time, uint32 dlb_timer_type)
{
switch(dlb_timer_type)
{
case dlb_timer_nano:
{
this_thread::sleep_for(chrono::nanoseconds(wait_time));
break;
}
case dlb_timer_micro:
{
this_thread::sleep_for(chrono::microseconds(wait_time));
break;
}
case dlb_timer_ms:
{
this_thread::sleep_for(chrono::milliseconds(wait_time));
break;
}
case dlb_timer_seg:
{
this_thread::sleep_for(chrono::seconds(wait_time));
break;
}
case dlb_timer_min:
{
this_thread::sleep_for(chrono::minutes(wait_time));
break;
}
}
}

uint32 dlb_timer::get_type()const
{
return this->timer_type;
}

int64 dlb_timer::get_timeout()const
{
return this->timeout;
}

bool dlb_timer::set_timeout(int64 timeout, uint32 timer_type)
{
if(timeout<0)
{
timeout=dlb_timer_invalid;
min_timeout=dlb_timer_invalid;
max_timeout=dlb_timer_invalid;
}
if(timer_type>dlb_timer_min)
{
return false;
}
this->timeout=timeout;
this->min_timeout=this->timeout;
this->max_timeout=this->min_timeout;
this->timer_type=timer_type;
return true;
}

void dlb_timer::set_timeout_range(int64 min_timeout, int64 max_timeout, uint32 timer_type)
{
if(timer_type>dlb_timer_min)
{
return;
}
this->timer_type=timer_type;
if(min_timeout>max_timeout)
{
int64 x=min_timeout;
min_timeout=max_timeout;
max_timeout=x;
}
this->min_timeout=min_timeout;
this->max_timeout=max_timeout;
}

void dlb_timer::get_timeout_range(int64* min_timeout, int64* max_timeout)
{
if(min_timeout!=NULL)
{
*min_timeout=this->min_timeout;
}
if(max_timeout!=NULL)
{
*max_timeout=this->max_timeout;
}
}

bool dlb_timer::is_timeout()
{
if((this->timer_type==dlb_timer_invalid)||(this->timeout<=0))
{
return true;
}
return this->elapsed(this->timer_type)>this->timeout;
}

//Outras funções

void dlb_wait(int64 timeout, uint32 dlb_time)
{
switch(dlb_time)
{
case dlb_timer_nano:
{
this_thread::sleep_for(chrono::nanoseconds(timeout));
break;
}
case dlb_timer_micro:
{
this_thread::sleep_for(chrono::microseconds(timeout));
break;
}
case dlb_timer_ms:
{
this_thread::sleep_for(chrono::milliseconds(timeout));
break;
}
case dlb_timer_seg:
{
this_thread::sleep_for(chrono::seconds(timeout));
break;
}
case dlb_timer_min:
{
this_thread::sleep_for(chrono::minutes(timeout));
break;
}
}
}

//Functions...

int64 dlb_gettimestamp(uint32 time_type)
{
chrono::system_clock::time_point tp=chrono::system_clock::now();
chrono::system_clock::duration dtn=tp.time_since_epoch();
switch(time_type)
{
case dlb_timer_nano:
{
return chrono::duration_cast<chrono::nanoseconds>(dtn).count();
}
case dlb_timer_micro:
{
return chrono::duration_cast<chrono::microseconds>(dtn).count();
}
case dlb_timer_ms:
{
return chrono::duration_cast<chrono::milliseconds>(dtn).count();
}
case dlb_timer_seg:
{
return chrono::duration_cast<chrono::seconds>(dtn).count();
}
case dlb_timer_min:
{
return chrono::duration_cast<chrono::minutes>(dtn).count();
}
}
return 0;
}
}
