

/**
*Algumas funções http básicas para pegar coisas rapidamente.
*Depende do subsistema libcurl.
*Escrito por Róger (rogerlasch@gmail.com) em maio de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/
#ifndef DLB_HTTP_H
#define DLB_HTTP_H

namespace dlb
{
bool dlb_curl_url_get(const std::string& url, std::string& data);
bool dlb_curl_url_get(const std::string& url, const std::string& agente, std::string& data);
bool dlb_curl_http_post(const std::string& url, const std::string& postfields, std::vector<std::string> the_headers, std::string& data);
bool dlb_curl_http_post(const std::string& url, const std::string& agent, const std::string& postfields, std::vector<std::string> the_headers, std::string& data);
std::string dlb_curl_url_encode(const std::string& url);
std::string dlb_curl_url_decode(const std::string& url);
}
#endif
