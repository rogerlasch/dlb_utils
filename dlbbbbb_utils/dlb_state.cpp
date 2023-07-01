
#include"dlb_state.h"
#include"dlb_timer.h"

using namespace std;

namespace dlb
{
dlb_state::dlb_state()
{
this->reset();
}

dlb_state::dlb_state(const dlb_state& dbs)
{
*this=dbs;
}

dlb_state& dlb_state::operator=(const dlb_state& dbs)
{
this->error_code.store(dbs.error_code.load());
this->current_state.store(dbs.current_state.load());
this->cancel_task.store(this->cancel_task.load());
return *this;
}

bool dlb_state::operator==(const dlb_state& dbs)const
{
return this->current_state.load()==dbs.current_state.load();
}

dlb_state::~dlb_state()
{
this->setstate(dlb_state_hidle);
this->seterror(dlb_error_ok);
this->cancel_task.store(false);
}

void dlb_state::reset(uint32 new_state, uint32 error_code)
{
this->setstate(new_state);
this->seterror(error_code);
this->cancel_task.store(false);
}

uint32 dlb_state::geterror()const
{
return this->error_code.load();
}

uint32 dlb_state::getstate()const
{
return this->current_state.load();
}

void dlb_state::seterror(uint32 error_code)
{
this->error_code.store(error_code);
}

void dlb_state::setstate(uint32 current_state)
{
this->current_state.store(current_state);
}

/**
*Muda o estado interno para dlb_state_waiting_for_cancelation e sinaliza que a tarefa deve ser finalizada a sim que possível.
*Se return_immediately for false, a função esperará até que o estado seja diferente de dlb_state_waiting_for_cancelation.
*Se return_immediately for true, o método sinaliza que a tarefa deve ser finalizada e retorna sem esperar que seja realmente cancelada. Com isso, basta monitorar  o estado com getstate.
*Caso o estado interno já seja dlb_state_hidle, dlb_state_error, dlb_state_waiting_for_cancelation, dlb_state_canceled, ou dlb_state_finished a função retornará imediatamente apenas definindo o sinalizador de cancelamento para true por garantia.
**/
void dlb_state::cancel(bool return_immediately)
{
uint32 internal_state=getstate();
if((internal_state==dlb_state_hidle)||(internal_state==dlb_state_waiting_for_cancelation)||(internal_state==dlb_state_canceled)||(internal_state==dlb_state_error)||(internal_state==dlb_state_finished))
{
this->cancel_task.store(true);
return;
}
this->cancel_task.store(true);
setstate(dlb_state_waiting_for_cancelation);
if(return_immediately==true)
{
return;
}
dlb_timer tmr;
while(getstate()==dlb_state_waiting_for_cancelation)
{
tmr.wait(5);
}
}

bool dlb_state::is_cancel()const
{
return this->cancel_task.load();
}

}
