/**
*Classe para armazenar alguns tipos que é usado por dlb_dictionary.
*Também fornece algumas funções úteis para certos casos.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em outubro de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_VALUE
#define DLB_VALUE

#include<string>
#include<vector>
#include<iostream>

namespace dlb
{

enum dlb_value_types
{
dlb_type_void=0,
dlb_type_int8,
dlb_type_uint8,
dlb_type_int16,
dlb_type_uint16,
dlb_type_int32,
dlb_type_uint32,
dlb_type_int64,
dlb_type_uint64,
dlb_type_string,
dlb_type_wstring,
dlb_type_float,
dlb_type_double,
dlb_type_bool
};


class dlb_value
{
private:
union
{
bool bool_;
int64 int64_;
uint64 uint64_;
double double_;
std::string* string_;
} storage;
uint32 type;
public:
dlb_value();
dlb_value(const dlb_value& val);
dlb_value(int8 val);
dlb_value(uint8 val);
dlb_value(int16 val);
dlb_value(uint16 val);
dlb_value(int32 val);
dlb_value(uint32 val);
dlb_value(int64 val);
dlb_value(uint64 val);
dlb_value(float val);
dlb_value(double val);
dlb_value(bool val);
dlb_value(const char* s);
dlb_value(const std::string& s);
dlb_value(const wchar_t* s);
dlb_value(const std::wstring& s);
dlb_value& operator=(const dlb_value& val);
uint32 gettype()const;
void cleanup();
~dlb_value();
std::string to_string()const;
bool get(int8& val)const;
bool get(uint8& val)const;
bool get(int16& val)const;
bool get(uint16& val)const;
bool get(int32& val)const;
bool get(uint32& val)const;
bool get(int64& val)const;
bool get(uint64& val)const;
bool get(float& val)const;
bool get(double& val)const;
bool get(bool& val)const;
bool get(std::string& val)const;
bool get(std::wstring& val)const;
};

std::ostream& operator<<(std::ostream& os, dlb::dlb_value& val);

typedef std::vector<dlb::dlb_value> dlb_value_args;

template<class ...Args>
dlb_value_args dlb_make_args(const Args&... args)
{
dlb_value_args vect={args...};
return vect;
}

}
#endif
