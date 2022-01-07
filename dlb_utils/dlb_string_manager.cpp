
#include<windows.h>
#include<locale>
#include<string>
#include"dlb_types.h"
#include"dlb_string_manager.h"
#include"third-party/base64.h"


using namespace std;

namespace dlb
{
string dlb_string_to_upper_case(const string& str)
{
if(str.size()==0)
{
return str;
}
string str2="";
str2.resize(str.size());
for(dwparam i=0; i<str.size(); i++)
{
str2[i]=str[i];
if((str2[i]>='a')&&(str2[i]<='z'))
{
str2[i]=str2[i]-32;
}
}
return str2;
}

string dlb_string_to_lower_case(const string& str)
{
if(str.size()==0)
{
return str;
}
string str2="";
str2.resize(str.size());
for(dwparam i=0; i<str.size(); i++)
{
str2[i]=str[i];
if((str2[i]>='A')&&(str2[i]<='Z'))
{
str2[i]=str2[i]+32;
}
}
return str2;
}

string dlb_string_smash(const string& str, const string& char_set)
{
if((str.size()==0)||(char_set.size()==0))
{
return str;
}
string str2="";
str2.reserve(str.size());
for(dwparam i=0; i<str.size(); i++)
{
if(char_set.find_first_of(str[i], 0)==string::npos)
{
str2+=str[i];
}
}
return str2;
}

string dlb_string_verife(const string& str)
{
if(str.size()==0)
{
return str;
}
string
char_set=" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÁÃÂÀÉÊËÍÏÓÔÕÖÚÜÇ0123456789-_ãáâáéëêíïõóôöúü";
string str2="";
str2.reserve(str.size());
for(dwparam i=0; i<str.size(); i++)
{
if(char_set.find_first_of(str[i], 0)!=string::npos)
{
str2+=str[i];
}
}
return str2;
}

string dlb_string_replace(const string& str, const string& original, const string& new_str, bool replace_all)
{
if((str.size()==0)||(original.size()==0))
{
return str;
}
string str2="";
str2.reserve(str.size());
bool replaced_first=false;
for(dwparam i=0; i<str.size(); i++)
{
if(str[i]!=original[0])
{
str2+=str[i];
continue;
}
if((replaced_first==true)&&(replace_all==false))
{
str2+=str[i];
continue;
}
string txt=str.substr(i, original.size());
if(original==txt)
{
str2.append(new_str);
i=(i+original.size())-1;
replaced_first=true;
continue;
}
str2+=str[i];
}
return str2;
}

dwparam dlb_string_split(const string& str, const string& separator, vector<string>& result)
{
if((str.size()==0)||(separator.size()==0))
{
return 0;
}
result.clear();
string chunk="";
chunk.reserve(1024);
for(dwparam i=0; i<str.size(); i++)
{
if(str[i]!=separator[0])
{
chunk+=str[i];
continue;
}
string txt=str.substr(i, separator.size());
if(separator==txt)
{
if(chunk.size()>0)
{
result.push_back(chunk);
}
chunk="";
i=(i+separator.size())-1;
continue;
}
}
if(chunk.size()>0)
{
result.push_back(chunk);
}
return result.size();
}

wstring dlb_string_utf8_decode(const std::string& str)
{
wstring final=L"";
int32 res=MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
if(res==0)
{
return L"";
}
final.resize(res);
MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), &final[0], final.size());
return final;
}

wstring dlb_string_to_wstring(const string& str)
{
locale loc;
const auto& _Facet=use_facet<codecvt<wchar_t, char, mbstate_t>>(loc);
wstring final=L"";
final.resize(str.size());
mbstate_t _State{};
const char* istart=str.c_str();
const char* iend=(str.c_str()+str.size());
const char* inext=NULL;
wchar_t* ostart=(wchar_t*)final.c_str();
wchar_t* oend=(wchar_t*)(final.c_str()+final.size());
wchar_t* onext=NULL;
const auto Result=_Facet.in(_State, istart, iend, inext, ostart, oend, onext);
if(Result==codecvt_base::ok)
{
final.resize(static_cast<size_t>(onext-ostart));
return final;
}
return L"";
}

string dlb_string_base64_encode(const string& str)
{
return base64_encode(str);
}

string dlb_string_base64_decode(const string& str)
{
return base64_decode(str);
}
}
