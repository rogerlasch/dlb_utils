

/**
*Apenas uma macro para usar std::format nativo (c++20 em diante) ou fmt que ? para vers?es mais antigas.
*Escrito por R?ger (rogerlasch@gmail.com) em Outubro de 2021.
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
typedef std::format_args dlb_format_args;
#define dlb_format(str, ...) std::format(str, __VA_ARGS__)
#define dlb_wformat(wstr, ...) std::format(wstr, __VA_ARGS__)
#define dlb_make_format_args(...) std::make_format_args(__VA_ARGS__)
#define dlb_make_wformat_args(...) std::make_wformat_args(__VA_ARGS__)
#define dlb_vformat(str, _args) std::vformat(str, _args)
#else
typedef fmt::format_args dlb_format_args;
#define dlb_format(str, ...) fmt::format(str, __VA_ARGS__)
#define dlb_make_format_args(...) fmt::make_format_args(__VA_ARGS__)
#define dlb_vformat(str, _args) fmt::vformat(str, _args)
#endif
}
#endif
