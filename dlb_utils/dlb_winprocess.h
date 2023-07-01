

#ifndef DLB_WINPROCESS_H
#define DLB_WIINPROCESS_H

#include<atomic>
#include<future>
#include<sstream>

namespace dlb
{

enum dlb_winprocess_states
{
dlb_win_default=0,
dlb_win_active,
dlb_win_finished,
dlb_win_canceling
};

class dlb_winprocess
{
private:
std::atomic<uint32> hstate;
uint32 pid;
uint32 tid;
int32 exit_code;
void* hinput;
void* houtput;
void* hprocess;
void* hthread;
std::string cmdline;
mutable std::stringstream buffer;
std::future<void> handle;
public:
dlb_winprocess();
dlb_winprocess(const dlb_winprocess& dp)=delete;
dlb_winprocess& operator=(const dlb_winprocess& dp)=delete;
~dlb_winprocess();
void cleanup();
private:
void setState(uint32 state);
public:
uint32 getState()const;
uint32 getPid()const;
uint32 getTid()const;
int32 getExitCode()const;
std::string getCmdLine()const;
std::string getLine()const;
std::string getOutput()const;
bool closeProcess();
bool run(const std::string& cmdline);
void wait();
uint32 wait_for(uint32 ms);
private:
void onLoop();
};
}
#endif
