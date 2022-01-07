
/**
*Funções para debugs internos da dlb. Sem muita utilidade no momento.
*Escrito por Róger (rogerlasch@gmail.com) em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_DEBUG_H
#define DLB_DEBUG_H

#include<typeinfo>
#include"dlb_constantes.h"
#include"dlb_config.h"
#include"dlb_format.h"

namespace dlb
{
void dlb_debug(const std::string& str);

#define _dl_debug(str, ...) dlb_debug(dlb_format(str, __VA_ARGS__))
#define _dl_null(fn, pr) dlb_debug(dlb_format("Ponteiro NULL na função: {}, com o parâmetro {}.", fn, std::typeid(pr).name()))
#define _dl_illparam(fn, pr) dlb_debug(dlb_format("Parâmetro ilegal na função {}. Com o parâmetro {}, Valor do parâmetro: {}", fn, std::typeid(pr).name(), pr))
#define _dl_except(f, w) dlb_debug(dlb_format("Exceção na função: {}, Descrição: {}", f, w))
}
#endif
