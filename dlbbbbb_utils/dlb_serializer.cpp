

#include<fstream>
#include<endian.h>
#include"dlb_types.h"
#include"dlb_value.h"
#include"dlb_wstring_manager.h"
#include"dlb_serializer.h"

using namespace std;

namespace dlb
{
dlb_serializer::dlb_serializer()
{
this->stream.clear();
this->store=false;
}

dlb_serializer::dlb_serializer(const dlb_serializer& ds)
{
*this=ds;
}

dlb_serializer& dlb_serializer::operator=(const dlb_serializer& ds)
{
this->stream=ds.stream;
this->store=ds.store;
return *this;
}

void dlb_serializer::set_store(bool store)
{
this->store=store;
if(store==false)
{
this->stream.clear();
}
}

void dlb_serializer::cleanup_store()
{
this->stream.clear();
}

uint32 dlb_serializer::get_store(std::string& output)
{
output.clear();
output.append(this->stream);
return this->stream.size();
}

uint32 dlb_serializer::save_store(const string& filename)
{
if((this->store==false)||(filename.size()==0)||(this->stream.size()==0))
{
return 0;
}
ofstream ofn(filename, ios::binary);
if(!ofn.is_open())
{
return 0;
}
ofn.write(&this->stream[0], this->stream.size());
ofn.close();
return this->stream.size();
}

string dlb_serializer::serialize_int8(int8 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_int8);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_uint8(uint8 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_uint8);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_int16(int16 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_int16);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_uint16(uint16 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_uint16);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_int32(int32 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_int32);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_uint32(uint32 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_uint32);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_int64(int64 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_int64);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_uint64(uint64 value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_uint64);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_float(float value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_float);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_double(double value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_double);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
if(IsBigEndian())
{
value=to_little_endian(value);
}
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_bool(bool value)
{
string output="";
uint8 dlb_type=static_cast<uint8>(dlb_type_bool);
uint32 size_type=sizeof(value);
uint32 final_size=(sizeof(dlb_type)+size_type);
output.resize(final_size);
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &value, sizeof(value));
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_string(const string& value)
{
string output="";
if(value.size()==0)
{
return "";
}
uint8 dlb_type=static_cast<uint8>(dlb_type_string);
uint32 size_str=value.size();
uint32 final_size=(sizeof(dlb_type)+sizeof(size_str))+value.size();
output.resize(final_size);
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &size_str, sizeof(size_str));
memcpy(&output[sizeof(dlb_type)+sizeof(size_str)], &value[0], value.size());
if(this->store==true)
{
this->stream.append(output);
}
return output;
}

string dlb_serializer::serialize_wstring(const wstring& value)
{
string output="";
if(value.size()==0)
{
return "";
}
string str=dlb_wstring_to_string(value);
uint8 dlb_type=static_cast<uint8>(dlb_type_wstring);
uint32 size_str=str.size();
uint32 final_size=(sizeof(dlb_type)+sizeof(size_str))+str.size();
output.resize(final_size);
memcpy(&output[0], &dlb_type, sizeof(dlb_type));
memcpy(&output[sizeof(dlb_type)], &size_str, sizeof(size_str));
memcpy(&output[sizeof(dlb_type)+sizeof(size_str)], &str[0], str.size());
if(this->store==true)
{
this->stream.append(output);
}
return output;
}
}
