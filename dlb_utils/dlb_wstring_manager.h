
//Algumas funções úteis de string.
#ifndef DLB_WSTRING_MANAGER_H
#define DLB_WSTRING_MANAGER_H

#include<string>
#include<vector>

namespace dlb
{
std::wstring dlb_string_to_upper_case(const std::wstring& str);
std::wstring dlb_string_to_lower_case(const std::wstring& str);
std::wstring dlb_string_smash(const std::wstring& str, const std::wstring& char_set);
std::wstring dlb_string_verife(const std::wstring& str);
std::wstring dlb_string_replace(const std::wstring& str, const std::wstring& original, const std::wstring& new_str, bool replace_all);
dwparam dlb_string_split(const std::wstring& str, const std::wstring& separator, std::vector<std::wstring>& result);
std::string dlb_string_utf8_encode(const std::wstring& str);
std::string dlb_wstring_to_string(const std::wstring& str);
}
#endif
