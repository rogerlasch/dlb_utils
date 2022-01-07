
/**
*Typos que são usados pela dlb.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e atualizado em julho de 2021
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_TYPES_H
#define DLB_TYPES_H

#include<string>
#include<stdlib.h>
#include<stdint.h>

namespace dlb
{
#define myatoi(f)strtoul(f, NULL, 0)

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
#if defined(_DLB_64)
typedef uint64 dwparam;
typedef int64 dlparam;
#else
typedef uint32 dwparam;
typedef int32 dlparam;
#endif
}
#endif
