

//#define CURL_STATICLIB
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
if(!dlb_subsystem_isloaded(dlb_sub_curl))
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
  curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.164 Safari/537.36");
curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
int32 res=curl_easy_perform(handle);
  curl_easy_cleanup(handle);
return res==CURLE_OK;
}

bool dlb_curl_http_post(const string& url, const string& postfields, vector<string> the_headers, string& data)
{
if(!dlb_subsystem_isloaded(dlb_sub_curl))
{
if(!dlb_subsystem_load(dlb_sub_curl))
{
return false;
}
}
CURL* handle=curl_easy_init();
if(handle==NULL)
{
return false;
}
curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dlb_http_callback_pushmemory);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.164 Safari/537.36");
curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postfields.c_str());
struct curl_slist* headers=NULL;
for(auto it=the_headers.begin(); it!=the_headers.end(); ++it)
{
headers=curl_slist_append(headers, it->c_str());
}
curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
int res=curl_easy_perform(handle);
curl_slist_free_all(headers);
curl_easy_cleanup(handle);
return res==CURLE_OK;
}

string dlb_curl_url_encode(const string& url)
{
if(!dlb_subsystem_isloaded(dlb_sub_curl))
{
if(!dlb_subsystem_load(dlb_sub_curl))
{
return "";
}
}
CURL* handle=curl_easy_init();
char* ptr=curl_easy_escape(handle, url.c_str(), url.size());
string final="";
final.append(ptr);
curl_free(ptr);
curl_easy_cleanup(handle);
return final;
}

string dlb_curl_url_decode(const string& url)
{
if(!dlb_subsystem_isloaded(dlb_sub_curl))
{
if(!dlb_subsystem_load(dlb_sub_curl))
{
return "";
}
}
CURL* handle=curl_easy_init();
int32 size=0;
char* ptr=curl_easy_unescape(handle, url.c_str(), url.size(), &size);
string final="";
final.append(ptr, size);
curl_free(ptr);
curl_easy_cleanup(handle);
return final;
}
}
