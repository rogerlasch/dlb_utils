
/**
*Biblioteca de inicializa��o e descarregamento de subsistemas.
*Carrega bass, libcurl e tolk.
*Se lembre de chamar dlb_unload sempre antes de sair.
*Caso chame alguma fun��o como dlb_speak, ou fun��es http e o subsistema apropriado n�o tiver cido carregado, o mesmo tentar� ser carregado dinamicamente.
*Mas, � bom inicializar no in�cio do programa para evitar problemas.
*Escrito por R�ger (rogerlasch@gmail.com) em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_MAIN_H
#define DLB_MAIN_H

#include<list>
#include<map>
#include<string>
#include<vector>
#include"dlb_types.h"
#include"dlb_state.h"
#include"dlb_config.h"

namespace dlb
{
uint32 dlb_init(uint32 dlb_subsystems=dlb_sub_all);
bool dlb_subsystem_load(uint32 dlb_subsystem);
bool dlb_subsystem_isloaded(uint32 subsystem);
bool dlb_subsystem_unload(uint32 dlb_subsystem);
uint32 dlb_subsystem_get();
void dlb_unload();
}
#endif
