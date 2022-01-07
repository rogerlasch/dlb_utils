
//Algumas funções úteis de string.
//A biblioteca original de base64 pode ser encontrada aqui:
//https://github.com/ReneNyffenegger/cpp-base64

#ifndef DLB_STRING_MANAGER_H
#define DLB_STRING_MANAGER_H

#include<string>
#include<vector>


namespace dlb
{
std::string dlb_string_to_upper_case(const std::string& str);
std::string dlb_string_to_lower_case(const std::string& str);
std::string dlb_string_smash(const std::string& str, const std::string& char_set);
std::string dlb_string_verife(const std::string& str);
std::string dlb_string_replace(const std::string& str, const std::string& original, const std::string& new_str, bool replace_all);
dwparam dlb_string_split(const std::string& str, const std::string& separator, std::vector<std::string>& result);
std::wstring dlb_string_utf8_decode(const std::string& str);
std::wstring dlb_string_to_wstring(const std::string& str);
std::string dlb_string_base64_encode(const std::string& the_string);
std::string dlb_string_base64_decode(const std::string& the_string);
}
#endif
