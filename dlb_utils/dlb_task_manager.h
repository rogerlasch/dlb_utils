
/**
*Algumas funções para lidar com tarefas assíncronas.
*Na verdade, é mais um pool de tarefas.
*Ainda pode ser melhorado bastante.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em maio de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_TASK_MANAGER_H
#define DLB_TASK_MANAGER_H

namespace dlb
{

uint32 dlb_task_create();
uint32 dlb_task_create(uint32 task_type, TASKFUNC taskfunc, TASKFINISHED taskfinished, const dlb_dictionary& data, bool start_work=false);
bool dlb_task_exists(uint32 handle);
uint32 dlb_task_destroy(uint32 handle);
uint32 dlb_task_cancel(uint32 handle);
bool dlb_task_waiting_for_cancelation(uint32 handle);
uint32 dlb_task_work(uint32 handle);
bool dlb_task_is_working(uint32 handle);
bool dlb_task_is_finished(uint32 handle);
uint32 dlb_task_get_error(uint32 handle);
uint32 dlb_task_get_type(uint32 handle);
uint32 dlb_task_get_state(uint32 handle);
uint32 dlb_task_get_data(uint32 handle, dlb_dictionary* outdata);
uint32 dlb_task_set_func(uint32 handle, TASKFUNC taskfunc);
uint32 dlb_task_set_finished(uint32 handle, TASKFINISHED taskfinished);
uint32 dlb_task_set_data(uint32 handle, const dlb_dictionary& data);
uint32 dlb_task_set_type(uint32 handle, uint32 task_type);
uint32 dlb_task_count();
uint32 dlb_task_work_all();
uint32 dlb_task_destroy_all();
uint32 dlb_task_cancel_all();
uint32 dlb_task_count_working();
}
#endif
