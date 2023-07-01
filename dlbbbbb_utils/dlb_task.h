
/**
*Biblioteca de gerenciamento de tarefas assíncronas
*permite invocar funções especiais passando parâmetros e processar os resultados.
*Ainda é bem beta, então coisas esquisitas podem acontecer...
*Escrito por Róger (rogerlasch@gmail.com) em maio de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/
#ifndef DLB_TASK_H
#define DLB_TASK_H

#include<atomic>
#include<functional>
#include<future>

namespace dlb
{
//Mapeie as tarefas aqui
enum task_types
{
task_none=0,//Não mecha nisto, enumere apartir daqui
};

/**
*Todas as funções devem ter esta assinatura para usar a biblioteca.
*O dicionário conterá os parâmetros e 1 ou mais valores de retorno.
**/
typedef std::function<void(dlb_dictionary&)> TASKFUNC;
typedef std::function<void(uint32, uint32, const dlb_dictionary&)> TASKFINISHED;

class dlb_task : public dlb_state
{
private:
std::atomic<uint32> handle;
std::atomic<uint32> task_type;
std::future<void> task_handle;
std::future<void> task_monitor;
TASKFUNC taskfunc;
TASKFINISHED taskfinished;
dlb_dictionary data;
public:
dlb_task();
dlb_task(uint32 task_type, TASKFUNC taskfunc, const dlb_dictionary& data);
dlb_task(const dlb_task& ts)=delete;
dlb_task& operator=(const dlb_task& ts)=delete;
~dlb_task();
uint32 gettype()const;
uint32 gethandle()const;
uint32 getdata(dlb_dictionary* outdata);
uint32 reset();
uint32 setfunc(TASKFUNC taskfunc);
uint32 setfinished(TASKFINISHED taskfinished);
uint32 setdata(const dlb_dictionary& data);
uint32 settype(uint32 type);
uint32 sethandle(uint32 handle);
uint32 work();
bool is_working();
bool is_finished()const;
private:
void monitor();
};
}
#endif
