

/**
*Classe de dicionário para a dlb_utils.
*É baseado no objeto dicionário da linguagem de script bgt.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em junho de 2021
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_DICTIONARY_H
#define DLB_DICTIONARY_H

#include<string>
#include<shared_mutex>
#include<unordered_map>
#include<list>
#include<vector>
#include"dlb_key.h"
#include"dlb_value.h"

namespace dlb
{

//Hasher...
struct DictionaryHasher
{
uint32 operator()(const dlb_key& dk) const
  {
    return std::hash<std::string>()(dk.keyname);
  }
};

class dlb_key;
class dlb_value;
class dlb_dictionary
{
private:
std::unordered_map<dlb_key, dlb_value, DictionaryHasher> items;
mutable std::shared_mutex mtx;
public:
dlb_dictionary();
dlb_dictionary(const dlb_dictionary& dt);
dlb_dictionary& operator=(const dlb_dictionary& dt);
uint32 get_size()const;
uint32 get_type(const dlb_key& dk)const;
bool is_empty()const;
bool exists(const dlb_key& dk)const;
bool delete_key(const dlb_key& dk);
void delete_all();
uint32 get_keys(std::vector<dlb_key>& keys)const;
uint32 get_keys(std::list<dlb_key>& keys)const;
//set and get
void set(const dlb_key& key_name, int8 value);
void set(const dlb_key& key_name, uint8 value);
void set(const dlb_key& key_name, int16 value);
void set(const dlb_key& key_name, uint16 value);
void set(const dlb_key& key_name, int32 value);
void set(const dlb_key& key_name, uint32 value);
void set(const dlb_key& key_name, int64 value);
void set(const dlb_key& key_name, uint64 value);
void set(const dlb_key& key_name, float value);
void set(const dlb_key& key_name, double value);
void set(const dlb_key& key_name, bool value);
void set(const dlb_key& key_name, const char* value);
void set(const dlb_key& key_name, const std::string& value);
void set(const dlb_key& key_name, const wchar_t* value);
void set(const dlb_key& key_name, const std::wstring& value);
void set(const dlb_key& key_name, const dlb_value& value);
//get
bool get(const dlb_key& key_name, int8& value)const;
bool get(const dlb_key& key_name, uint8& value)const;
bool get(const dlb_key& key_name, int16& value)const;
bool get(const dlb_key& key_name, uint16& value)const;
bool get(const dlb_key& key_name, int32& value)const;
bool get(const dlb_key& key_name, uint32& value)const;
bool get(const dlb_key& key_name, int64& value)const;
bool get(const dlb_key& key_name, uint64& value)const;
bool get(const dlb_key& key_name, float& value)const;
bool get(const dlb_key& key_name, double& value)const;
bool get(const dlb_key& key_name, bool& value)const;
bool get(const dlb_key& key_name, std::string& value)const;
bool get(const dlb_key& key_name, std::wstring& value)const;
};

uint32 dlb_serialize_dictionary(const dlb_dictionary& dt, const std::string& filename, std::string* output=NULL);
uint32 dlb_deserialize_dictionary(dlb_dictionary& dt, const std::string& indata, bool fromfile);
}
#endif
