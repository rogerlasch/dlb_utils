
/**
*Escrito por R�ger em abril de 2021
*Esta classe representa o estado de um objeto.
* Por exemplo, para ler um arquivo primeiro voc� precisa verificar se ele existe, tentar abrir, verificar o tamano e ent�o ler os dados.
*Ent�o voc� com esta classe pode ir mapeando cada passo dessa tarefa. Isto � mais �til em multithreading
*Ela tamb�m guarda um c�digo de erro especificado.
*Tamb�m inclui dois m�todos adicionais para sinalizar que uma tarefa deve ser cancelada imediatamente, e um is_cancel que pode ser usado em um while.
*Modo de uso: voc� pode instanciar um objeto dentro de sua classe e usar os m�todos. Apesar desse m�todo n�o ser o recomendado, ele funciona relativamente bem.
*Recomendo que sua classe herde da classe dlb_state, com isso voc� n�o precisa estanciar em sua classe e tamb�m n�o precisa definir os seters e geters dela.
*Esta classe � segura para threads para a altera��o de seus membros. As duas propriedades modific�veis s�o definidas como atomic, s�o um tanto mais lentas, mas garantem a seguran�a.
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
