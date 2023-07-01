

/**
*Algumas funções para simplificar algumas coisas.
*Escrito por Róger (rogerlasch@gmail.com) em 2020.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_MATH_H
#define DLB_MATH_H

namespace dlb
{
int32 dlb_random_int32(int32 min, int32 max);
int64 dlb_random_int64(int64 min, int64 max);
double dlb_round(double number, uint32 decimal_places);
bool dlb_getlucky(int32 percentage);
}
#endif
