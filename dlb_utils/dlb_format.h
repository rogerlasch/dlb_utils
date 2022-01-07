

/**
*Apenas uma macro para usar std::format nativo (c++20 em diante) ou fmt que é para versões mais antigas.
*Escrito por Róger (rogerlasch@gmail.com) em Outubro de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_FORMAT_H
#define DLB_FORMAT_H

#include<string>
#if defined(_DLB_CPP_20)
#include<format>
#else
#include<fmt/core.h>
#endif

namespace dlb
{
#if defined(_DLB_CPP_20)
#define dlb_format(str, ...) std::format(str, __VA_ARGS__)
#define dlb_wformat(wstr, ...) std::format(wstr, __VA_ARGS__)
#else
#define dlb_format(str, ...) fmt::format(str, __VA_ARGS__)
#endif
}
#endif
