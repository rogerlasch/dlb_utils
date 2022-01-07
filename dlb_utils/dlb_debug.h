
/**
*Fun��es para debugs internos da dlb. Sem muita utilidade no momento.
*Escrito por R�ger (rogerlasch@gmail.com) em julho de 2021.
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
#define _dl_null(fn, pr) dlb_debug(dlb_format("Ponteiro NULL na fun��o: {}, com o par�metro {}.", fn, std::typeid(pr).name()))
#define _dl_illparam(fn, pr) dlb_debug(dlb_format("Par�metro ilegal na fun��o {}. Com o par�metro {}, Valor do par�metro: {}", fn, std::typeid(pr).name(), pr))
#define _dl_except(f, w) dlb_debug(dlb_format("Exce��o na fun��o: {}, Descri��o: {}", f, w))
}
#endif
