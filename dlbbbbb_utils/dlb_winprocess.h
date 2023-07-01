
/**
*Classe para executar um processo filho, com opção de recuperar o stdout.
*Vide https://stackoverflow.com/questions/10866944/how-can-i-read-a-child-processs-output
*Escrito por Róger (rogerlasch@gmail.com) em julho de 2022.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_WINPROCESS_H
#define DLB_WINPROCESS_H

#include<string>
#include<sstream>

namespace dlb
{

typedef void* WinHandle;

enum dlb_winprocess_flags
{
dlb_win_active=(1<<0),
dlb_win_read=(1<<2),
dlb_win_cansel=(1<<3)
};

class dlb_winprocess : public dlb_object<uint32>
{
private:
uint32 pid;
uint32 tid;
int32 exit_code;
WinHandle hinput;
WinHandle houtput;
WinHandle hprocess;
WinHandle hthread;
std::string cmdline;
mutable std::stringstream hbuffer;
mutable std::shared_timed_mutex mtx_io;
public:
dlb_winprocess();
dlb_winprocess(const dlb_winprocess& ps)=delete;
dlb_winprocess& operator=(const dlb_winprocess& ps)=delete;
~dlb_winprocess();
void cleanup();
bool isactive();
bool isread()const;
uint32 getpid()const;
uint32 gettid()const;
int32 getexitcode()const;
void setexitcode(int32 exit_code);
std::string get_cmdline()const;
std::string getline()const;
std::string getoutput()const;
bool run(const std::string& cmdline);
void wait();
bool wait_for(uint32 ms);
private:
bool internal_isactive();
void internal_read(bool read_all=false);
void internal_cleanup();
};
}
#endif
