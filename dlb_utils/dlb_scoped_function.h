

/**
*Classe que loga quando uma fun��o se inicia, e quando a mesma � terminada.
*� �til em alguns debugs. No encerramento, ela fornece uo tempo aproximado da dura��o da fun��o.
*� recomend�vel instanciar uma classe disto ao entrar na fun��o.
*Escrito por R�ger (rogerlasch@gmail.com) em Janeiro de 2022.
**/
#ifndef DLB_SCOPED_FUNCTION_H
#define DLB_SCOPED_FUNCTION_H

namespace dlb
{
class dlb_scoped_function
{
private:
std::string func_name;
int64 start_time;
public:
dlb_scoped_function(const std::string& func_name);
dlb_scoped_function(const dlb_scoped_function& sc)=delete;
dlb_scoped_function& operator=(const dlb_scoped_function& sc)=delete;
virtual ~dlb_scoped_function();
void write(const std::string& str, dlb_format_args&& args=dlb_make_format_args());
};
#define _scoped_function dlb_scoped_function(__FUNCTION__)
#define _flog(SCOPED_FUNCTION, str, ...) SCOPED_FUNCTION.write(str, dlb_make_format_args(__FUNCTION__))
}
#endif
