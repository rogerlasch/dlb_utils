
/**
*Classe que representa uma chave para ser usado em conjunto com o dlb_dictionary.
*No momento não existe nenhuma outra utilidade para ela.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/
#ifndef DLB_KEY_H
#define DLB_KEY_H

#include"dlb_types.h"
#include<string>
namespace dlb
{

enum dlb_key_flags
{
dlb_key_valid=(1<<0),
dlb_key_num=(1<<1),
};

class dlb_key
{
private:
uint8 flags;
std::string keyname;
public:
dlb_key();
explicit dlb_key(int32 value);
explicit dlb_key(uint32 value);
explicit dlb_key(int64 value);
explicit dlb_key(uint64 value);
explicit dlb_key(double value);
dlb_key(const char* value);
dlb_key(const std::string& value);
dlb_key(const dlb_key& value);
//Operator= overloads
dlb_key& operator=(int32 value);
dlb_key& operator=(uint32 value);
dlb_key& operator=(int64 value);
dlb_key& operator=(uint64 value);
dlb_key& operator=(const double keyname);
dlb_key& operator=(const char* keyname);
dlb_key& operator=(const std::string& keyname);
dlb_key& operator=(const dlb_key& dk);
//The operator== overloads;
bool operator<(int32 value)const;
bool operator<(uint32 value)const;
bool operator<(int64 value)const;
bool operator<(uint64 value)const;
bool operator<(double keyname)const;
bool operator<(const char* keyname)const;
bool operator<(const std::string& keyname)const;
bool operator<(const dlb_key& dk)const;
bool operator<=(int32 value)const;
bool operator<=(uint32 value)const;
bool operator<=(int64 value)const;
bool operator<=(uint64 value)const;
bool operator<=(double keyname)const;
bool operator<=(const char* keyname)const;
bool operator<=(const std::string& keyname)const;
bool operator<=(const dlb_key& dk)const;
bool operator==(int32 value)const;
bool operator==(uint32 value)const;
bool operator==(int64 value)const;
bool operator==(uint64 value)const;
bool operator==(double keyname)const;
bool operator==(const char* keyname)const;
bool operator==(const std::string& keyname)const;
bool operator==(const dlb_key& dk)const;
bool operator>(int32 value)const;
bool operator>(uint32 value)const;
bool operator>(int64 value)const;
bool operator>(uint64 value)const;
bool operator>(double keyname)const;
bool operator>(const char* keyname)const;
bool operator>(const std::string& keyname)const;
bool operator>(const dlb_key& dk)const;
bool operator>=(int32 value)const;
bool operator>=(uint32 value)const;
bool operator>=(int64 value)const;
bool operator>=(uint64 value)const;
bool operator>=(double keyname)const;
bool operator>=(const char* keyname)const;
bool operator>=(const std::string& keyname)const;
bool operator>=(const dlb_key& dk)const;
std::string str()const;
bool is_valid()const;
bool isnum()const;
friend std::ostream& operator<<(std::ostream& os, const dlb_key& dk);
friend struct DictionaryHasher;
};
}
#endif
