

/**
*Algumas fun��es http b�sicas para pegar coisas rapidamente.
*Depende do subsistema libcurl.
*Escrito por R�ger (rogerlasch@gmail.com) em maio de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/
#ifndef DLB_HTTP_H
#define DLB_HTTP_H

namespace dlb
{
bool dlb_curl_url_get(const std::string& url, std::string& data);
}
#endif
