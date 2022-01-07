
#include<chrono>
#include<future>
#include"dlb_types.h"
#include"dlb_constantes.h"
#include"dlb_dictionary.h"
#include"dlb_state.h"
#include"dlb_task.h"

using namespace std;
namespace dlb
{

dlb_task::dlb_task() : dlb_state()
{
this->reset();
}

dlb_task::dlb_task(uint32 task_type, TASKFUNC taskfunc, const dlb_dictionary& data) : dlb_state()
{
this->settype(task_type);
this->setfunc(taskfunc);
this->setdata(data);
}

dlb_task::~dlb_task()
{
if(this->is_working())
{
this->cancel(false);
this_thread::sleep_for(chrono::milliseconds(11));
}
this->reset();
}

uint32 dlb_task::gettype()const
{
return this->task_type.load();
}

uint32 dlb_task::gethandle()const
{
return this->handle.load();
}

uint32 dlb_task::getdata(dlb_dictionary* outdata)
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
if(outdata==NULL)
{
this->seterror(dlb_error_null);
return this->geterror();
}
*outdata=this->data;
return this->geterror();
}

uint32 dlb_task::reset()
{
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
this->sethandle(0);
this->setstate(dlb_state_hidle);
this->settype(task_none);
this->seterror(dlb_error_ok);
this->taskfunc=NULL;
this->taskfinished=NULL;
this->data.delete_all();
return this->geterror();
}

uint32 dlb_task::setfunc(TASKFUNC taskfunc)
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_incomplet_task);
return this->geterror();
}
this->taskfunc=taskfunc;
if(this->taskfunc)
{
this->setstate(dlb_stat_read_for_work);
}
else
{
this->setstate(dlb_state_hidle);
}
return this->geterror();
}

uint32 dlb_task::setfinished(TASKFINISHED taskfinished)
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
this->taskfinished=taskfinished;
return this->geterror();
}

uint32 dlb_task::setdata(const dlb_dictionary& data)
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
this->data=data;
return this->geterror();
}

uint32 dlb_task::settype(uint32 type)
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
this->task_type.store(type);
return this->geterror();
}

uint32 dlb_task::sethandle(uint32 handle)
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
this->handle.store(handle);
return this->geterror();
}

uint32 dlb_task::work()
{
this->seterror(dlb_error_ok);
if(this->is_working())
{
this->seterror(dlb_error_invalid_state);
return this->geterror();
}
switch(this->getstate())
{
case dlb_state_hidle:
{
this->seterror(dlb_error_func);
return this->geterror();
}
case dlb_stat_read_for_work:
{
this->setstate(dlb_state_working);
this->data.set("handle_task", this->gethandle());
this->task_handle=async(this->taskfunc, std::ref(this->data));
this->task_monitor=async(&dlb_task::monitor, this);
return this->geterror();
}
default:
{
this->seterror(dlb_error_reset);
return this->geterror();
}
}
}

bool dlb_task::is_working()
{
return this->getstate()==dlb_state_working;
}

bool dlb_task::is_finished()const
{
return this->getstate()==dlb_state_finished;
}

void dlb_task::monitor()
{
if(!this->is_working())
{
return;
}
while((this->getstate()==dlb_state_working)||(this->getstate()==dlb_state_waiting_for_cancelation))
{
this_thread::sleep_for(chrono::milliseconds(5));
switch(this->task_handle.wait_for(chrono::milliseconds(5)))
{
case future_status::ready:
{
if(this->is_cancel())
{
this->setstate(dlb_state_canceled);
break;
}
if(this->taskfinished)
{
taskfinished(this->gethandle(), this->gettype(), this->data);
}
this->setstate(dlb_state_finished);
break;
}
default:
{
continue;
}
}
}
}
}
