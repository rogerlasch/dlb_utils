
#include<mutex>
#include<map>
#include"dlb_types.h"
#include"dlb_constantes.h"
#include"dlb_dictionary.h"
#include"dlb_state.h"
#include"dlb_task.h"
#include"dlb_task_manager.h"

using namespace std;

namespace dlb
{
static map<uint32, dlb_task*> tasks;
static mutex mtx_task;

static uint32 dlb_task_generate_handle()
{
static uint32 current_handle=0;
current_handle+=1;
return current_handle;
}

uint32 dlb_task_create()
{
uint32 handle=dlb_task_generate_handle();
dlb_task* ts=new dlb_task();
ts->sethandle(handle);
tasks.insert(make_pair(handle, ts));
return handle;
}

/**
*Esta função cria e inicializa uma nova tarefa assíncrona.
*Se start_work for true, a tarefa começará a ser executada imediatamente.
*A função irá falhar se work_type for task_none, ou se taskfunc for inválido.
*O valor de retorno é diferente de 0 se teve sucesso
*O work_type é responsabilidade do usuário final definir. Isto é apenas para identificar o que o thread está fazendo.
*Por exemplo, task_count_1m pode significar que o thread está contando até mil ou 1 milhão
*Divirta-se com isso
**/
uint32 dlb_task_create(uint32 task_type, TASKFUNC taskfunc, TASKFINISHED taskfinished, const dlb_dictionary& data, bool start_work)
{
uint32 handle=dlb_task_create();
if(handle==0)
{
return 0;
}
if(dlb_task_set_type(handle, task_type)!=dlb_error_ok)
{
dlb_task_destroy(handle);
return 0;
}
if(dlb_task_set_func(handle, taskfunc)!=dlb_error_ok)
{
dlb_task_destroy(handle);
return 0;
}
if(dlb_task_set_finished(handle, taskfinished)!=dlb_error_ok)
{
dlb_task_destroy(handle);
return 0;
}
if(dlb_task_set_data(handle, data)!=dlb_error_ok)
{
dlb_task_destroy(handle);
return 0;
}
if(start_work==true)
{
dlb_task_work(handle);
}
return handle;
}

bool dlb_task_exists(uint32 handle)
{
return tasks.count(handle)>0;
}

/**
*Remove uma tarefa da lista.
*Se a tarefa não existir dlb_error_not_found é retornado.
*A tarefa só pode ser destruída depois que a mesma terminou de executar.
*Caso ainda não possa ser destruída, dlb_error_working é retornado. *Task_error_ok é retornado caso a tarefa tenha sido destruída.
*Note que esta função não altera o erro interno da tarefa.
*Por tanto, dlb_task_get_error poderá retornar dlb_error_ok mesmo que esta falhe.
**/
uint32 dlb_task_destroy(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
if(tasks.at(handle)->getstate()==dlb_state_working)
{
tasks.at(handle)->cancel(false);
}
delete tasks.at(handle);
tasks.erase(handle);
return dlb_error_ok;
}

uint32 dlb_task_cancel(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
tasks.at(handle)->cancel(false);
return dlb_error_ok;
}

bool dlb_task_waiting_for_cancelation(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return false;
}
return tasks.at(handle)->getstate()==dlb_state_waiting_for_cancelation;
}
/**
*Esta função irá iniciar a tarefa, caso a mesma ainda não tenha sido iniciada.
*Ela irá falhar se o estado interno for diferente de task_ready_to_work
**/
uint32 dlb_task_work(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->work();
}

bool dlb_task_is_working(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return false;
}
return tasks.at(handle)->is_working();
}

bool dlb_task_is_finished(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return false;
}
return tasks.at(handle)->is_finished();
}

uint32 dlb_task_get_error(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->geterror();
}

uint32 dlb_task_get_type(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->gettype();
}

uint32 dlb_task_get_state(uint32 handle)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->getstate();
}

uint32 dlb_task_get_data(uint32 handle, dlb_dictionary* outdata)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->getdata(outdata);
}

uint32 dlb_task_set_func(uint32 handle, TASKFUNC taskfunc)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->setfunc(taskfunc);
}

uint32 dlb_task_set_finished(uint32 handle, TASKFINISHED taskfinished)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->setfinished(taskfinished);
}

uint32 dlb_task_set_data(uint32 handle, const dlb_dictionary& data)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->setdata(data);
}

uint32 dlb_task_set_type(uint32 handle, uint32 task_type)
{
if(!dlb_task_exists(handle))
{
return dlb_error_handle;
}
return tasks.at(handle)->settype(task_type);
}

uint32 dlb_task_count()
{
return tasks.size();
}

uint32 dlb_task_work_all()
{
if(tasks.size()==0)
{
return 0;
}
uint32 starteds=0;
for(auto it=tasks.begin(); it!=tasks.end(); ++it)
{
if(it->second->work()==dlb_error_ok)
{
starteds++;
}
}
return starteds;
}

uint32 dlb_task_destroy_all()
{
if(tasks.size()==0)
{
return 0;
}
uint32 destroyeds=0;
for(auto it=tasks.begin(); it!=tasks.end(); ++it)
{
dlb_task_cancel(it->first);
delete it->second;
destroyeds++;
}
tasks.clear();
return destroyeds;
}

uint32 dlb_task_cancel_all()
{
if(tasks.size()==0)
{
return 0;
}
uint32 canceleds=0;
for(auto it=tasks.begin(); it!=tasks.end(); ++it)
{
if(dlb_task_is_working(it->first))
{
dlb_task_cancel(it->first);
canceleds++;
}
}
return canceleds;
}

uint32 dlb_task_count_working()
{
if(tasks.size()==0)
{
return 0;
}
uint32 workings=0;
for(auto it=tasks.begin(); it!=tasks.end(); ++it)
{
if(dlb_task_is_working(it->first))
{
workings++;
}
}
return workings;
}
}
