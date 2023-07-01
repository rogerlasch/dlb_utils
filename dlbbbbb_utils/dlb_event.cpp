

#include<list>
#include<shared_mutex>
#include<mutex>
#include"dlb_types.h"
#include"dlb_timer.h"
#include"dlb_worker.h"
#include"dlb_event.h"


using namespace std;
namespace dlb
{
static list<dlb_event*> dlb_events;
static shared_mutex mtx_event;

dlb_event::dlb_event()
{
this->reset();
}

dlb_event::dlb_event(const dlb_event& dev)
{
*this=dev;
}

dlb_event& dlb_event::operator=(const dlb_event& dev)
{
this->id=dev.id;
this->type=dev.type;
this->timestamp=dev.timestamp;
this->data=dev.data;
return *this;
}

dlb_event::~dlb_event()
{
}

void dlb_event::reset()
{
this->id=0;
this->type=dlb_event_default;
this->timestamp=0;
this->worker=NULL;
this->data="";
}


//Functions

void dlb_event_send(uint32 type, uint32 id, const string& data)
{
dlb_event* ev=new dlb_event();
ev->id=id;
ev->type=type;
ev->data=data;
dlb_event_send(ev);
}

void dlb_event_send(dlb_event* ev)
{
if(ev==NULL)
{
return;
}
unique_lock<shared_mutex> lck(mtx_event);
ev->timestamp=dlb_gettimestamp();
dlb_events.push_back(ev);
//despertar os workers para cuidarem dos eventos...
if(dlb_events.size()==1)
{
dlb_worker_set_can_state(dlb_worker_can_work);
}
}

bool dlb_event_get(dlb_event** ev)
{
if(ev==NULL)
{
return false;
}
unique_lock<shared_mutex> lck(mtx_event);
if(dlb_events.size()==0)
{
return false;
}
*ev=*dlb_events.begin();
dlb_events.erase(dlb_events.begin());
//Se n�o existir mais eventos, avise os workers para ibernarem at� surgir mais trabalho.
if(dlb_events.size()==0)
{
dlb_worker_set_can_state(dlb_worker_can_pause);
}
return true;
}

uint32 dlb_event_count()
{
shared_lock<shared_mutex> lck(mtx_event);
return dlb_events.size();
}

void dlb_event_cleanup()
{
unique_lock<shared_mutex> lck(mtx_event);
for(auto it=dlb_events.begin(); it!=dlb_events.end(); ++it)
{
delete (*it);
}
dlb_events.clear();
}

}
