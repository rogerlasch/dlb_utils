
#include<windows.h>
#include<locale>
#include<string>
#include"dlb_types.h"
#include"dlb_wstring_manager.h"

using namespace std;

namespace dlb
{
wstring dlb_string_to_upper_case(const wstring& str)
{
if(str.size()==0)
{
return str;
}
wstring str2=L"";
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

wstring dlb_string_to_lower_case(const wstring& str)
{
if(str.size()==0)
{
return str;
}
wstring str2=L"";
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

wstring dlb_string_smash(const wstring& str, const wstring& char_set)
{
if((str.size()==0)||(char_set.size()==0))
{
return str;
}
wstring str2=L"";
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

wstring dlb_string_verife(const wstring& str)
{
if(str.size()==0)
{
return str;
}
wstring char_set=L" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÁÃÂÀÉÊËÍÏÓÔÕÖÚÜÇ0123456789-_ãáâáéëêíïõóôöúü";
wstring str2=L"";
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

wstring dlb_string_replace(const wstring& str, const wstring& original, const wstring& new_str, bool replace_all)
{
if((str.size()==0)||(original.size()==0))
{
return str;
}
wstring str2=L"";
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
wstring txt=str.substr(i, original.size());
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

uint32 dlb_string_split(const wstring& str, const wstring& separator, vector<wstring>& result)
{
if((str.size()==0)||(separator.size()==0))
{
return 0;
}
result.clear();
wstring chunk=L"";
chunk.reserve(1024);
for(dwparam i=0; i<str.size(); i++)
{
if(str[i]!=separator[0])
{
chunk+=str[i];
continue;
}
wstring txt=str.substr(i, separator.size());
if(separator==txt)
{
if(chunk.size()>0)
{
result.push_back(chunk);
}
chunk=L"";
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

string dlb_string_utf8_encode(const wstring& str)
{
string final="";
int32 res=WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
if(res==0)
{
return "";
}
final.resize(res);
WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), &final[0], final.size(), NULL, NULL);
return final;
}

string dlb_wstring_to_string(const wstring& str)
{
locale loc;
const auto& _Facet=use_facet<codecvt<char, wchar_t, mbstate_t>>(loc);
string final="";
final.resize(str.size());
mbstate_t _State{};
const wchar_t* istart=str.c_str();
const wchar_t* iend=(str.c_str()+str.size());
const wchar_t* inext=NULL;
char* ostart=(char*)final.c_str();
char* oend=(char*)(final.c_str()+final.size());
char* onext=NULL;
const auto Result=_Facet.in(_State, istart, iend, inext, ostart, oend, onext);
if(Result==codecvt_base::ok)
{
final.resize(static_cast<size_t>(onext-ostart));
return final;
}
return "";
}

}
