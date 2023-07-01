
#include<windows.h>
#include<stdlib.h>
#include"dlb_types.h"
#include"dlb_timer.h"
#include"dlb_log.h"
#include"dlb_math.h"
#include"dlb_winprocess.h"

using namespace std;
namespace dlb
{
dlb_winprocess::dlb_winprocess()
{
pid=0;
tid=0;
exit_code=0;
hstate.store(dlb_win_default);
cmdline="";
buffer.clear();
hinput=NULL;
houtput=NULL;
hprocess=NULL;
hthread=NULL;
}

dlb_winprocess::~dlb_winprocess()
{
switch(getState())
{
case dlb_win_active:
case dlb_win_canceling:
{
this->closeProcess();
handle.get();
break;
}
}
}

void dlb_winprocess::cleanup()
{
pid=0;
tid=0;
exit_code=0;
hstate.store(dlb_win_default);
cmdline="";
buffer.clear();
hinput=NULL;
houtput=NULL;
hprocess=NULL;
hthread=NULL;
}

void dlb_winprocess::setState(uint32 state)
{
this->hstate.store(state);
}

uint32 dlb_winprocess::getState()const
{
return this->hstate.load();
}

uint32 dlb_winprocess::getPid()const
{
return this->pid;
}

uint32 dlb_winprocess::getTid()const
{
return this->tid;
}

int32 dlb_winprocess::getExitCode()const
{
return this->exit_code;
}

string dlb_winprocess::getCmdLine()const
{
return this->cmdline;
}

string dlb_winprocess::getLine()const
{
string ss;
std::getline(buffer, ss);
return ss;
}

string dlb_winprocess::getOutput()const
{
return buffer.str();
}

bool dlb_winprocess::closeProcess()
{
switch(getState())
{
case dlb_win_default:
case dlb_win_finished:
{
return true;
}
case dlb_win_active:
case dlb_win_canceling:
{
if(TerminateProcess(hprocess, 1))
{
CloseHandle(hinput);
CloseHandle(hprocess);
CloseHandle(hthread);
hprocess=NULL;
hthread=NULL;
hinput=NULL;
return true;
}
return false;
}
}
return false;
}

bool dlb_winprocess::run(const string& cmdline)
{
if(cmdline.size()==0)
{
return false;
}
uint32 h=getState();
bool onrun=false;
if((h==dlb_win_default)||(h==dlb_win_finished))
{
onrun=true;
}
if(!onrun)
{
return false;
}
this->cleanup();
this->setState(dlb_win_active);
PROCESS_INFORMATION pinfo;
STARTUPINFOA startinfo;
SECURITY_ATTRIBUTES saAttr;
memset(&saAttr, 0, sizeof(saAttr));
saAttr.nLength=sizeof(SECURITY_ATTRIBUTES);
saAttr.bInheritHandle=TRUE;
saAttr.lpSecurityDescriptor=NULL;
if(!CreatePipe(&hinput, &houtput, &saAttr, 5000))
    {
this->setState(dlb_win_default);
return false;
    }
if(!SetHandleInformation(hinput, HANDLE_FLAG_INHERIT, 0))
{
CloseHandle(hinput);
CloseHandle(houtput);
this->setState(dlb_win_default);
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
this->setState(dlb_win_default);
return false;
}
    CloseHandle(houtput);
this->pid=pinfo.dwProcessId;
this->tid=pinfo.dwThreadId;
this->hprocess=pinfo.hProcess;
this->hthread=pinfo.hThread;
this->cmdline=cmdline;
try {
this->handle=async(&dlb_winprocess::onLoop, this);
} catch(const exception& e) {
CloseHandle(hinput);
CloseHandle(houtput);
hprocess=NULL;
TerminateProcess(hprocess, 1);
this->cleanup();
this->setState(dlb_win_default);
_log_except(__FUNCTION__, e.what());
return false;
}
return true;
}

void dlb_winprocess::wait()
{
while(true)
{
uint32 h=getState();
if((h==dlb_win_default)||(h==dlb_win_finished))
{
break;
}
this_thread::sleep_for(chrono::milliseconds(5));
}
}

uint32 dlb_winprocess::wait_for(uint32 ms)
{
int64 start=dlb_gettimestamp();
while((dlb_gettimestamp()-start)<ms)
{
if((getState()==dlb_win_default)||(getState()==dlb_win_finished))
{
break;
}
dlb_wait(dlb_random_int32(1, 10));
}
return getState();
}

void dlb_winprocess::onLoop()
{
while(((getState()==dlb_win_active)||(this->getState()==dlb_win_canceling)))
{
uint32 res=WaitForSingleObject(this->hprocess, 2);
if(this->getState()==dlb_win_canceling)
{
this->closeProcess();
break;
}
switch(res)
{
case WAIT_ABANDONED:
case WAIT_FAILED:
{
this->closeProcess();
setState(dlb_win_finished);
break;
}
case WAIT_OBJECT_0:
{
int32 x=0;
GetExitCodeProcess(hprocess, (LPDWORD)&x);
this->exit_code=x;
this->closeProcess();
this->setState(dlb_win_finished);
break;
}
default:
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
buffer.write(str.c_str(), size);
}
break;
}
}
}
}
}
