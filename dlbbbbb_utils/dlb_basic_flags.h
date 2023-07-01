

/**
*Escrito por R�ger Matheus Lasch em Maio de 2022.
*Biblioteca de manipula��o de flags threadsafe...
*Apesar de ter outro nome, esta biblioteca foi utilizada para desenvolver o trabalho de threads de SO2.
*https://github.com/rogerlasch/SO2_Phone.git
**/
#ifndef DLB_BASIC_FLAGS_H
#define DLB_BASIC_FLAGS_H

#include<mutex>
#include<shared_mutex>
#include<stdint.h>

/**
*Em vez de criar uma vari�vel booleana para cada comportamento, eu crio constantes e uso uma opera��o bit a bit para armazenar tudo em um inteiro n�o assinado.
*Como cada vari�vel booleana ocupa 1 �nico bit, e um unsigned int, na maioria das plataformas tem 32 bits, consigo armazenar at� trinta e duas flags em um �nico inteiro.
*As flags devem seguir um padr�o para que isso seja poss�vel.
*Elas sempre devem ser o dobro da anterior como por exemplo: 1, 2, 4, 8, 16, 32, etc...
*Tomando como exemplo um inteiro de 4 bits, internamente se tudo estiver desativado, ele ser� isso:
*0000
*Se minha constante tiver valor 4, ele ir� ativar o terceiro bit, ficando a sim:
*0100
***
*Sempre que uma opera��o for feita, seja de consulta, ou defini��o ou remo��o de determinada flag, um acesso de leitura ou grava��o � adquirido. Isso � feito por meio da classe shared_mutex.
**/
namespace dlb
{
class dlb_basic_flags
{
private:
//Armazena as flags...
uint32_t flags;
/*Objeto que controlar� o acesso a vari�vel... O mutable significa que ele poder� ser mut�vel com o qualificador const em algumas situa��es.*/
mutable std::shared_mutex mtx_flags;
public:
//Construtor, construtor de c�pia, operador de atribui��o e destruidor virtual.
inline dlb_basic_flags(uint32_t flags=0);
inline dlb_basic_flags(const dlb_basic_flags& bs);
inline dlb_basic_flags& operator=(const dlb_basic_flags& bs);
virtual inline ~dlb_basic_flags();
//Substitue as flags usando um operador de atribui��o...
inline void replace_flags(uint32_t masq);
//Recupera todas flags contidas de uma vez...
inline uint32_t get_flags()const;
//Define uma, ou mais flags...
inline bool setflag(uint32_t masq);
//Remove uma ou mais flags...
inline bool removeflag(uint32_t masq);
//Verifica se uma, ou v�rias flags est�o definidas...
inline bool flag_contains(uint32_t masq)const;
};
}
#endif

#ifndef DLB_BASIC_FLAGS_IMPLEMENTATION
#define DLB_BASIC_FLAGS_IMPLEMENTATION
namespace dlb
{
dlb_basic_flags::dlb_basic_flags(uint32_t flags)
{
this->flags=flags;
}

dlb_basic_flags::dlb_basic_flags(const dlb_basic_flags& bs)
{
*this=bs;
}

dlb_basic_flags& dlb_basic_flags::operator=(const dlb_basic_flags& bs)
{
this->replace_flags(bs.get_flags());
return *this;
}

dlb_basic_flags::~dlb_basic_flags()
{
}

void dlb_basic_flags::replace_flags(uint32_t masq)
{
//Adquire permiss�o de escrita, ningu�m mais poder� acessar este m�todo enquanto a opera��o atual n�o for conclu�da.
std::unique_lock<std::shared_mutex> lck(this->mtx_flags);
this->flags=masq;
}

uint32_t dlb_basic_flags::get_flags()const
{
//Adquire permiss�o de leitura... Um ou mais threads podem ler ao mesmo tempo.
std::shared_lock<std::shared_mutex> lck(this->mtx_flags);
return this->flags;
}

/**
*Define uma ou mais flags com os operadores bit a bit.
*Se uma flag j� estiver definida, a opera��o toda retornar� false.
**/
bool dlb_basic_flags::setflag(uint32_t masq)
{
if(!this->flag_contains(masq))
{
std::unique_lock<std::shared_mutex> lck(this->mtx_flags);
this->flags|=masq;
return true;
}
return false;
}

/**
*Remove uma ou mais flags. Caso a flag n�o exista, n�o fa�a nada e retorne false.
**/
bool dlb_basic_flags::removeflag(uint32_t masq)
{
if(this->flag_contains(masq))
{
std::unique_lock<std::shared_mutex> lck(this->mtx_flags);
this->flags^=masq;
return true;
}
return false;
}

/**
*Verifica se uma ou mais flags est� definida...
*Retorna true se todas as flags existirem, false em falha.
**/
bool dlb_basic_flags::flag_contains(uint32_t masq)const
{
std::shared_lock<std::shared_mutex> lck(this->mtx_flags);
return (this->flags&masq)==masq;
}
}
#endif
