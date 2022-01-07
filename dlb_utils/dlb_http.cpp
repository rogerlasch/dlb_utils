

#define CURL_STATICLIB
#include<curl/curl.h>
#include"dlb_main.h"

using namespace std;

namespace dlb
{

static size_t dlb_http_callback_pushmemory(void* ptr, size_t size, size_t nmemb, void *userp)
{
(*(string*)userp).append((char*)ptr, nmemb);
  return nmemb;
}

bool dlb_curl_url_get(const string& url, string& data)
{
if(dlb_subsystem_isloaded(dlb_sub_curl))
{
if(!dlb_subsystem_load(dlb_sub_curl))
{
return false;
}
}
CURL* handle=NULL;
handle=curl_easy_init();
if(handle==NULL)
{
return false;;
}
curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dlb_http_callback_pushmemory);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
int32 res=curl_easy_perform(handle);
  curl_easy_cleanup(handle);
return res==CURLE_OK;
}
}
