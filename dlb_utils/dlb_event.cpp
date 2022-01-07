

#include<safe_ptr.h>
#include"dlb_types.h"
#include"dlb_event.h"

using namespace std;
namespace dlb
{
static safe_ptr<list<dlb_event*>> dlb_events;

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
}


//Functions

void dlb_event_send(uint32 type, uint32 id)
{
dlb_event* ev=new dlb_event();
ev->id=id;
ev->type=type;
dlb_event_send(ev);
}

void dlb_event_send(dlb_event* ev)
{
try
{
if(ev==NULL)
{
return;
}
lock_timed_any_infinity locked(dlb_events);
dlb_events->push_back(ev);
} catch(const exception& e) {
cout<<e.what()<<endl;
}
}

bool dlb_event_get(dlb_event** ev)
{
try
{
if(ev==NULL)
{
return false;
}
lock_timed_any_infinity locked(dlb_events);
if(dlb_events->size()==0)
{
return false;
}
*ev=*dlb_events->begin();
dlb_events->erase(dlb_events->begin());
} catch(const exception& e) {
cout<<e.what()<<endl;
}
return true;
}

dwparam dlb_event_count()
{
return dlb_events->size();
}

void dlb_event_cleanup()
{
lock_timed_any_infinity locked(dlb_events);
for(auto it=dlb_events->begin(); it!=dlb_events->end(); ++it)
{
delete (*it);
}
dlb_events->clear();
}

}
