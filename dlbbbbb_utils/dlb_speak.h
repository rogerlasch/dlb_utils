
/**
*Funções para fazerem leitores de tela falarem.
*Atualmente é usado a biblioteca Tolk para isto, mas pretendo escrever algo nativo da dlb futuramente.
*Escrito por Róger (rogerlasch@gmail.com) em março de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_SPEAK_H
#define DLB_SPEAK_H

#include<string>
#include"dlb_format.h"

namespace dlb
{
void dlb_Speak_init();
void dlb_Speak_unload();
bool dlb_Speak_stop();
bool dlb_speak(const std::string& str, bool interrupt=false);
bool dlb_speak_w(const std::wstring& str, bool interrupt=false);

#define speak(str, ...) dlb_speak(dlb_format(str, __VA_ARGS__), false)
#define speak_interrupt(str, ...) dlb_speak(dlb_format(str, __VA_ARGS__), true)
#if defined(_DLB_CPP_20)
#define wspeak(wstr, ...) dlb_speak_w(dlb_wformat(wstr, __VA_ARGS__), false)
#define wspeak_interrupt(wstr, ...) dlb_speak_w(dlb_wformat(wstr, __VA_ARGS__), true)
#endif
}
#endif
