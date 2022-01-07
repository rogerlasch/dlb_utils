

#include<safe_ptr.h>
#include"dlb_types.h"
#include"dlb_timer.h"
#include"dlb_event.h"
#include"dlb_worker.h"
#include"safe_ptr.h"


using namespace std;

namespace dlb
{
static safe_ptr<map<uint32, dlb_worker*>> dlb_workers;

//The dlb_worker_info class
dlb_worker_info::dlb_worker_info()
{
this->reset();
}

dlb_worker_info::dlb_worker_info(const dlb_worker_info& info)
{
*this=info;
}

dlb_worker_info& dlb_worker_info::operator=(const dlb_worker_info& info)
{
this->events=info.events;
this->total_executiontime=info.total_executiontime;
this->average_executiontime=info.average_executiontime;
return *this;
}

void dlb_worker_info::reset()
{
this->events=0;
this->total_executiontime=0;
this->average_executiontime=0;
}

//The dlb_worker class

dlb_worker::dlb_worker(uint32 id, const string& Name, uint32 lags, dlb_event_callback evcall)
{
this->id.store(id);
name->resize(0);
name->append(Name);
this->flags.store(flags);
info->reset();
unique_lock<mutex> lck(mtx);
this->evcall=evcall;
if((flags.load()&dlb_worker_paused)==0)
{
handle=async(&dlb_worker::worker_loop, this);
}
}

dlb_worker::~dlb_worker()
{
this->stop(100);
}

uint32 dlb_worker::get_id()const
{
return this->id.load();
}

uint32 dlb_worker::getflags()const
{
return this->flags.load();
}

string dlb_worker::getname()const
{
return name->substr(0, name->size());
}

bool dlb_worker::getinfo(dlb_worker_info* winfo)const
{
if(winfo==NULL)
{
return false;
}
winfo->events=info->events;
winfo->total_executiontime=info->total_executiontime;
winfo->average_executiontime=info->average_executiontime;
return true;
}

void dlb_worker::set_id(uint32 id)
{
this->id.store(id);
}

void dlb_worker::setname(const string& name)
{
this->name->clear();
this->name->append(name);
}

void dlb_worker::set_evcall(dlb_event_callback evcall)
{
unique_lock<mutex> lck(mtx);
evcall=evcall;
}

uint32 dlb_worker::stop(uint32 timeout)
{
if(this->isstopped())
{
return dlb_worker_ready;
}
if((this->flags.load()&dlb_worker_stop_work)==0)
{
this->flags.store(this->flags.load()|dlb_worker_stop_work);
}
return static_cast<uint32>(this->handle.wait_for(chrono::milliseconds(timeout)));
}

uint32 dlb_worker::pause()
{
if((this->ispaused())||((this->flags.load()&dlb_worker_stop_work)>0))
{
return dlb_worker_ready;
}
this->flags.store(this->flags.load()|dlb_worker_paused);
uint32 result=static_cast<uint32>(this->handle.wait_for(chrono::milliseconds(20)));
if(result!=dlb_worker_ready)
{
this->flags.store(this->flags.load()^dlb_worker_paused);
}
return result;
}

bool dlb_worker::resume()
{
if(!this->ispaused())
{
return false;
}
if((this->flags.load()&dlb_worker_stop_work)>0)
{
return false;
}
this->flags.store(this->flags.load()^dlb_worker_paused);
unique_lock<mutex> lck(mtx);
this->handle=async(&dlb_worker::worker_loop, this);
return true;
}

bool dlb_worker::ispaused()const
{
return (this->flags.load()&dlb_worker_paused)>0;
}

bool dlb_worker::isstopped()const
{
return ((ispaused()==false)&&((flags.load()&dlb_worker_active)==0));
}

bool dlb_worker::continue_loop()const
{
return (((this->flags.load()&dlb_worker_paused)>0||(flags.load()&dlb_worker_stop_work)>0||(flags.load()&dlb_worker_active)==0) ? false : true);
}

void dlb_worker::worker_loop()
{
this->flags.store(this->flags.load()|dlb_worker_active);
dlb_event* ev=NULL;
while(continue_loop())
{
this_thread::sleep_for(chrono::microseconds(5));
while(dlb_event_get(&ev))
{
unique_lock<mutex> lck(mtx);
info->events++;
if(evcall)
{
int64 start=dlb_gettimestamp();
evcall(ev);
int64 end=dlb_gettimestamp();
info->total_executiontime+=(end-start);
info->average_executiontime=(info->total_executiontime/info->events);
}
lck.unlock();
delete ev;
ev=NULL;
if(!continue_loop())
{
break;
}
}
}
}


//Functions

static uint32 dlb_worker_generate_id()
{
static int x=0;
x++;
return x;
}

uint32 dlb_worker_create(const std::string& name, uint32 flags, dlb_event_callback evcall)
{
uint32 id=dlb_worker_generate_id();
dlb_worker* wk=new dlb_worker(id, name, flags, evcall);
lock_timed_any_infinity locked(dlb_workers);
dlb_workers->insert(make_pair(id, wk));
return id;
}

uint32 dlb_worker_create(uint32 n_workers, uint32 flags, dlb_event_callback evcall)
{
for(uint32 i=0; i<n_workers; i++)
{
dlb_worker_create("", flags, evcall);
}
return n_workers;
}

uint32 dlb_worker_create(uint32 n_workers, uint32 flags, dlb_event_callback
evcall, std::vector<uint32>& ids)
{
ids.clear();
ids.reserve(n_workers);
for(uint32 i=0; i<n_workers; i++)
{
ids.push_back(dlb_worker_create("", flags, evcall));
}
return n_workers;
}

bool dlb_worker_exists(uint32 id)
{
lock_timed_any_infinity locked(dlb_workers);
return dlb_workers->count(id)>0;
}

dwparam dlb_worker_count()
{
return dlb_workers->size();
}

uint32 dlb_worker_stop(uint32 id)
{
if(!dlb_worker_exists(id))
{
return dlb_worker_handle;
}
lock_timed_any_infinity locked(dlb_workers);
dlb_worker* wk=dlb_workers->at(id);
uint32 res=wk->stop(20);
if(res==dlb_worker_ready)
{
dlb_workers->erase(id);
delete wk;
}
return res;
}

uint32 dlb_worker_stop_n(uint32 n_workers)
{
if((n_workers==0)||(dlb_worker_count()==0))
{
return 0;
}
uint32 x=0;
if(dlb_worker_count()<n_workers)
{
n_workers=dlb_worker_count();
}
for(uint32 i=0; i<n_workers; i++)
{
if(dlb_worker_count()>0)
{
if(dlb_worker_isstopped(dlb_workers->begin()->first))
{
dlb_worker_stop(dlb_workers->begin()->first);
if(i>0)
{
i--;
}
continue;
}
if(dlb_worker_stop(dlb_workers->begin()->first)==dlb_worker_ready)
{
x++;
}
}
}
return x;
}

uint32 dlb_worker_pause(uint32 id)
{
if(!dlb_worker_exists(id))
{
return dlb_worker_handle;
}
lock_timed_any_infinity locked(dlb_workers);
dlb_worker* wk=dlb_workers->at(id);
return wk->pause();
}

bool dlb_worker_resume(uint32 id)
{
if(!dlb_worker_exists(id))
{
return false;
}
lock_timed_any_infinity locked(dlb_workers);
dlb_worker* wk=dlb_workers->at(id);
return wk->resume();
}

bool dlb_worker_ispaused(uint32 id)
{
if(!dlb_worker_exists(id))
{
return false;
}
return dlb_workers->at(id)->ispaused();
}

bool dlb_worker_isstopped(uint32 id)
{
if(!dlb_worker_exists(id))
{
return false;
}
return dlb_workers->at(id)->isstopped();
}

uint32 dlb_worker_stop_all()
{
uint32 x=0;
while(dlb_worker_count()>0)
{
if(dlb_worker_stop(dlb_workers->begin()->first)==dlb_worker_ready)
{
x++;
}
}
return x;
}

uint32 dlb_worker_stop_all(vector<dlb_worker_info>& winfo)
{
uint32 x=0;
while(dlb_worker_count()>0)
{
dlb_worker_info wf;
dlb_workers->begin()->second->getinfo(&wf);
if(dlb_worker_stop(dlb_workers->begin()->first)==dlb_worker_ready)
{
x++;
winfo.push_back(wf);
}
}
return x;
}

uint32 dlb_worker_pause_all()
{
uint32 x=0;
for(auto it=dlb_workers->begin(); it!=dlb_workers->end(); ++it)
{
if(dlb_worker_pause(it->first)==dlb_worker_ready)
{
x++;
}
}
return x;
}

uint32 dlb_worker_resume_all()
{
uint32 x=0;
for(auto it=dlb_workers->begin(); it!=dlb_workers->end(); ++it)
{
if(dlb_worker_resume(it->first))
{
x++;
}
}
return x;
}

bool dlb_worker_getinfo(uint32 id, dlb_worker_info*winfo)
{
if(!dlb_worker_exists(id))
{
return false;
}
return dlb_workers->at(id)->getinfo(winfo);
}
}
