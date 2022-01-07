
/**
*Classe worker, é similar a dlb_task,mas este trabalha especificamente em conjunto com dlb_event.
*Também tem funções para manipular um pool de workers.
*Eles são úteis para lidar com a fila de eventos da dlb. Você informa uma função para processar os eventos e quantos workers quer que cuidem da fila, e deixe o resto por conta deles.
*Escrito por Róger (rogerlasch@gmail.com) em setembro de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_WORKER_H
#define DLB_WORKER_H

#include<future>
#include<functional>
#include"safe_ptr.h"

namespace dlb
{

#ifndef DLB_EVENT_H
class dlb_event;
#endif

typedef std::function<void(dlb_event* ev)> dlb_event_callback;

enum dlb_worker_flags
{
dlb_worker_paused=(1<<0),
dlb_worker_active=(1<<1),
//reservado, não usar...
dlb_worker_stop_work=(1<<2),
};

enum dlb_worker_stop_result
{
dlb_worker_deferred=std::future_status::deferred,
dlb_worker_ready=std::future_status::ready,
dlb_worker_timeout=std::future_status::timeout,
dlb_worker_handle//Handle inválido
};

class dlb_worker_info
{
public:
uint64 events;//Número de eventos que processou...
int64 total_executiontime;//O tempo total em que de fato processou eventos.
int64 average_executiontime;//O tempo médio de execução de cada evento
dlb_worker_info();
dlb_worker_info(const dlb_worker_info& info);
dlb_worker_info& operator=(const dlb_worker_info& info);
void reset();
};


class dlb_worker
{
private:
std::atomic<uint32> id;
std::atomic<uint32> flags;
safe_ptr<std::string> name;
safe_ptr<dlb_worker_info> info;
dlb_event_callback evcall;
std::future<void> handle;
std::mutex mtx;
public:
dlb_worker(uint32 id=0, const std::string& name="", uint32 flags=0, dlb_event_callback evcall=NULL);
dlb_worker(const dlb_worker& dw)=delete;
dlb_worker& operator=(const dlb_worker& dw)=delete;
virtual ~dlb_worker();
uint32 get_id()const;
uint32 getflags()const;
std::string getname()const;
bool getinfo(dlb_worker_info* winfo)const;
void set_id(uint32 id);
void setname(const std::string& name);
void set_evcall(dlb_event_callback evcall);
uint32 stop(uint32 timeout=0);
uint32 pause();
bool resume();
bool ispaused()const;
bool isstopped()const;
private:
bool continue_loop()const;
void worker_loop();
};

uint32 dlb_worker_create(const std::string& name, uint32 flags, dlb_event_callback evcall);
uint32 dlb_worker_create(uint32 n_workers, uint32 flags, dlb_event_callback evcall);
uint32 dlb_worker_create(uint32 n_workers, uint32 flags, dlb_event_callback evcall, std::vector<uint32>& ids);
bool dlb_worker_exists(uint32 id);
dwparam dlb_worker_count();
uint32 dlb_worker_stop_n(uint32 id);
uint32 dlb_worker_stop(uint32 n_workers);
uint32 dlb_worker_pause(uint32 id);
bool dlb_worker_resume(uint32 id);
bool dlb_worker_ispaused(uint32 id);
bool dlb_worker_isstopped(uint32 id);
uint32 dlb_worker_stop_all();
uint32 dlb_worker_stop_all(std::vector<dlb_worker_info>& winfo);
uint32 dlb_worker_pause_all();
uint32 dlb_worker_resume_all();
bool dlb_worker_getinfo(uint32 id, dlb_worker_info* winfo);

}
#endif
