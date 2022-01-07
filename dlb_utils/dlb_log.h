

/**
*Biblioteca de log da dlb.
*Fornece algumas fun��es para logar facilmente coisas.
*Escrito por R�ger (rogerlasch@gmail.com) em maio de 2021 e reformulado em outubro de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_LOG_H
#define DLB_LOG_H

#include<string>
#include"dlb_format.h"

namespace dlb
{
void dlb_log_open(const std::string& filename);
void dlb_log_open(const std::wstring& filename);
void dlb_log_close();
void dlb_log_set_stdout(bool dlb_stdout);
void dlb_log_write(const std::string& str);
void dlb_log_write_w(const std::wstring& str);

//Define macros para suportar argumentos vari�veis com std::format.
#define _log(str, ...) dlb_log_write(dlb_format(str, __VA_ARGS__))
#define _log_except(f, w) dlb_log_write(dlb_format("Exce��o na fun��o {}: Descri��o: {}", f, w))
}
#endif
