
/**
*Classe para executar um processo filho, com opção de recuperar o stdout.
*Vide https://stackoverflow.com/questions/10866944/how-can-i-read-a-child-processs-output
*Escrito por Róger (rogerlasch@gmail.com) em julho de 2022.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#include<windows.h>
#include<sstream>
#include<stdlib.h>
#include"dlb_types.h"
#include"dlb_timer.h"
#include"dlb_object.h"
#include"dlb_winprocess.h"

using namespace std;

namespace dlb
{
dlb_winprocess::dlb_winprocess()
{
hprocess=NULL;
this->replace_flags(0);
}

dlb_winprocess::~dlb_winprocess()
{
internal_cleanup();
}

void dlb_winprocess::cleanup()
{
this->internal_cleanup();
this->setexitcode(0);
this->cmdline="";
}

bool dlb_winprocess::isactive()
{
if(this->internal_isactive())
{
this->internal_read();
}
else
{
this->internal_read(true);
}
return this->flag_contains(dlb_win_active);
}

bool dlb_winprocess::isread()const
{
return this->flag_contains(dlb_win_read);
}

uint32 dlb_winprocess::getpid()const
{
auto lck=this->read_lock();
return this->pid;
}

uint32 dlb_winprocess::gettid()const
{
auto lck=this->read_lock();
return this->tid;
}

int32 dlb_winprocess::getexitcode()const
{
auto lck=this->read_lock();
return this->exit_code;
}

void dlb_winprocess::setexitcode(int32 exit_code)
{
auto lck=this->write_lock();
this->exit_code=exit_code;
}

string dlb_winprocess::get_cmdline()const
{
auto lck=this->read_lock();
return this->cmdline;
}

string dlb_winprocess::getline()const
{
dlb_read_lock lck(this->mtx_io);
string str="";
std::getline(hbuffer, str);
return str;
}

string dlb_winprocess::getoutput()const
{
dlb_read_lock lck(this->mtx_io);
return hbuffer.str();
}

bool dlb_winprocess::run(const string& cmdline)
{
if((this->flag_contains(dlb_win_active))||(cmdline.size()==0))
{
return false;
}
auto lck=this->write_lock();
this->setflag(dlb_win_active);
this->removeflag(dlb_win_read);
                               this->removeflag(dlb_win_cansel);
PROCESS_INFORMATION pinfo;
STARTUPINFOA startinfo;
SECURITY_ATTRIBUTES saAttr;
memset(&saAttr, 0, sizeof(saAttr));
saAttr.nLength=sizeof(SECURITY_ATTRIBUTES);
saAttr.bInheritHandle=TRUE;
saAttr.lpSecurityDescriptor=NULL;
if(!CreatePipe(&hinput, &houtput, &saAttr, 5000))
    {
this->removeflag(dlb_win_active);
return false;
    }
if(!SetHandleInformation(hinput, HANDLE_FLAG_INHERIT, 0))
{
CloseHandle(hinput);
CloseHandle(houtput);
this->removeflag(dlb_win_active);
return false;
}
memset(&startinfo, 0, sizeof(startinfo));
startinfo.cb= sizeof(startinfo);
startinfo.hStdError= houtput;
startinfo.hStdOutput= houtput;
startinfo.hStdInput= GetStdHandle(STD_INPUT_HANDLE);
startinfo.dwFlags|=STARTF_USESTDHANDLES;
if(!CreateProcessA(NULL, (char*)cmdline.c_str(), NULL, NULL, TRUE,         CREATE_NO_WINDOW, NULL, 0, &startinfo, &pinfo))
{
CloseHandle(hinput);
CloseHandle(houtput);
this->removeflag(dlb_win_active);
return false;
}
    CloseHandle(houtput);
this->pid=pinfo.dwProcessId;
this->tid=pinfo.dwThreadId;
this->hprocess=pinfo.hProcess;
this->hthread=pinfo.hThread;
this->cmdline=cmdline;
return true;
}

void dlb_winprocess::wait()
{
while(this->isactive())
{
this_thread::sleep_for(chrono::milliseconds(5));
}
}

bool dlb_winprocess::wait_for(uint32 ms)
{
int64 start=dlb_gettimestamp();
while(this->isactive())
{
this_thread::sleep_for(chrono::microseconds(100));
if((dlb_gettimestamp()-start)>ms)
{
break;
}
}
return this->flag_contains(dlb_win_active);
}

bool dlb_winprocess::internal_isactive()
{
uint32 res=WaitForSingleObject(this->hprocess, 5);
switch(res)
{
case WAIT_ABANDONED:
case WAIT_FAILED:
{
this->removeflag(dlb_win_active);
this->setflag(dlb_win_read);
internal_cleanup();
break;
}
case WAIT_OBJECT_0:
{
int32 x=0;
GetExitCodeProcess(hprocess, (LPDWORD)&x);
this->setexitcode(x);
internal_cleanup();
this->removeflag(dlb_win_active);
this->setflag(dlb_win_read);
break;
}
case WAIT_TIMEOUT:
{
break;
}
}
return this->flag_contains(dlb_win_active);
}

void dlb_winprocess::internal_read(bool read_all)
{
dlb_write_lock lck(this->mtx_io);
if(read_all==false)
{
string str="";
str.resize(1024*4);
uint32 size=0;
if((!ReadFile(hinput, &str[0], str.size(), (LPDWORD)&size, NULL))||(size==0))
{
return;
}
hbuffer.write(str.c_str(), size);
}
else
{
string str="";
str.resize(1024*4);
uint32 size=0;
while(ReadFile(hinput, &str[0], str.size(), (LPDWORD)&size, NULL))
{
if(size==0)
{
continue;
}
hbuffer.write(str.c_str(), size);
}
}
}

void dlb_winprocess::internal_cleanup()
{
auto lck=this->write_lock();
CloseHandle(hprocess);
CloseHandle(hthread);
CloseHandle(hinput);
hprocess=NULL;
hthread=NULL;
hinput=NULL;
houtput=NULL;
pid=0;
tid=0;
}
}
