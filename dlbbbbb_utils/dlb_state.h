
/**
*Escrito por Róger em abril de 2021
*Esta classe representa o estado de um objeto.
* Por exemplo, para ler um arquivo primeiro você precisa verificar se ele existe, tentar abrir, verificar o tamano e então ler os dados.
*Então você com esta classe pode ir mapeando cada passo dessa tarefa. Isto é mais útil em multithreading
*Ela também guarda um código de erro especificado.
*Também inclui dois métodos adicionais para sinalizar que uma tarefa deve ser cancelada imediatamente, e um is_cancel que pode ser usado em um while.
*Modo de uso: você pode instanciar um objeto dentro de sua classe e usar os métodos. Apesar desse método não ser o recomendado, ele funciona relativamente bem.
*Recomendo que sua classe herde da classe dlb_state, com isso você não precisa estanciar em sua classe e também não precisa definir os seters e geters dela.
*Esta classe é segura para threads para a alteração de seus membros. As duas propriedades modificáveis são definidas como atomic, são um tanto mais lentas, mas garantem a segurança.
*Requer alguma melhoria ainda.
**/

#ifndef DLB_STATE_H
#define DLB_STATE_H

#include<atomic>
#if !defined(DLB_CONSTANTES_H)
#include"dlb_constantes.h"
#endif

namespace dlb
{
class dlb_state
{
private:
std::atomic<uint32> error_code;
std::atomic<uint32> current_state;
std::atomic<bool> cancel_task;
public:
dlb_state();
dlb_state(const dlb_state& dbs);
dlb_state& operator=(const dlb_state& dbs);
bool operator==(const dlb_state& dbs)const;
virtual ~dlb_state();
void reset(uint32 new_state=dlb_state_hidle, uint32 error_code=dlb_error_ok);
uint32 geterror()const;
uint32 getstate()const;
void seterror(uint32 error_code);
void setstate(uint32 current_state);
void cancel(bool return_immediately=false);
bool is_cancel()const;
};
}
#endif
