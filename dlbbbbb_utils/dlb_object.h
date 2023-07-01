

/**
*Classe base para objetos.
*Fornece manipulação de bitflags threadsafe, e métodos de bloqueio e de leitura.
*Escrito por Róger (rogerlasch@gmail.com) em Julho de 2022.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_OBJECT_H
#define DLB_OBJECT_H

#include<chrono>
#include<thread>
#include<shared_mutex>
#include<mutex>
#include<exception>
#include<stdexcept>

namespace dlb
{

typedef std::unique_lock<std::shared_timed_mutex> dlb_write_lock;
typedef std::shared_lock<std::shared_timed_mutex> dlb_read_lock;

template<class T>
class dlb_object
{
protected:
T flags;
mutable std::thread::id dlb_tid;
mutable std::shared_timed_mutex mtx;
mutable std::shared_timed_mutex mtx_flags;
public:
dlb_object();
dlb_object(const dlb_object& obj)=delete;
dlb_object& operator=(const dlb_object& dl)=delete;
virtual ~dlb_object();
bool setflag(T masq);
bool removeflag(T masq);
void replace_flags(T value);
T get_flags()const;
bool flag_contains(T masq)const;
dlb_write_lock write_lock()const;
dlb_read_lock read_lock()const;
};
}
#endif

#ifndef DLB_OBJECT_IMPLEMENTATION_H
#define DLB_OBJECT_IMPLEMENTATION_H

namespace dlb
{
template<class T>
dlb_object<T>::dlb_object()
{

}

template<class T>
dlb_object<T>::~dlb_object()
{
}

template<class T>
bool dlb_object<T>::setflag(T masq)
{
if(this->flag_contains(masq))
{
return false;
}
dlb_write_lock lck(this->mtx_flags);
this->flags|=masq;
return true;
}

template<class T>
bool dlb_object<T>::removeflag(T masq)
{
if(!this->flag_contains(masq))
{
return false;
}
dlb_write_lock lck(this->mtx_flags);
this->flags^=masq;
return true;
}

template<class T>
bool dlb_object<T>::flag_contains(T masq)const
{
dlb_read_lock lck(this->mtx_flags);
return (this->flags&masq)==masq;
}

template<class T>
void dlb_object<T>::replace_flags(T value)
{
dlb_write_lock lck(this->mtx_flags);
this->flags=value;
}

template<class T>
T dlb_object<T>::get_flags()const
{
dlb_read_lock lck(this->mtx_flags);
return this->flags;
}

template<class T>
dlb_write_lock dlb_object<T>::write_lock()const
{
/*
if(this->tid==std::this_thread::get_id())
{
throw std::runtime_error("Deadhlock detectado!");
return dlb_write_lock();
}
*/
dlb_write_lock lck(this->mtx, std::defer_lock);
while(!lck.try_lock())
{
std::this_thread::sleep_for(std::chrono::milliseconds(5));
}
//this->tid=std::this_thread::get_id();
return lck;
}

template<class T>
dlb_read_lock dlb_object<T>::read_lock()const
{
/*
if(this->tid==std::this_thread::get_id())
{
throw std::runtime_error("Deadhlock detectado!");
return dlb_read_lock();
}
*/
dlb_read_lock lck(this->mtx, std::defer_lock);
while(!lck.try_lock())
{
std::this_thread::sleep_for(std::chrono::milliseconds(5));
}
//this->tid=std::this_thread::get_id();
return lck;
}
}
#endif
