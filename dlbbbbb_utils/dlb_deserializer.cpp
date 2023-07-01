

#include<fstream>
#include"dlb_types.h"
#include"dlb_value.h"
#include"dlb_string_manager.h"
#include"dlb_deserializer.h"

using namespace std;

namespace dlb
{
dlb_deserializer::dlb_deserializer()
{
this->size=0;
this->value=NULL;
this->cleanup();
}

dlb_deserializer::~dlb_deserializer()
{
this->cleanup();
}

void dlb_deserializer::cleanup()
{
this->dlb_type=0;
this->stream.clear();
this->ifs.close();
this->restored=false;
this->finished=false;
this->working=false;
this->fromfile=false;
if(this->size>0)
{
delete [] this->value;
}
this->size=0;
this->value=NULL;
}

bool dlb_deserializer::is_restored()
{
return this->restored;
}

bool dlb_deserializer::is_finished()
{
return this->finished;
}

bool dlb_deserializer::is_working()
{
return this->working;
}

bool dlb_deserializer::deserialize_from_file(const string& filename)
{
if(this->is_working())
{
return false;
}
ifs.open(filename, ios::binary);
if(!ifs.is_open())
{
return false;
}
this->working=true;
this->fromfile=true;
return true;
}

bool dlb_deserializer::deserialize_from_string(const string& str)
{
if(this->is_working())
{
return false;
}
if(str.size()<2)
{
return false;
}
this->stream.clear();
this->stream.str(str);
this->working=true;
this->fromfile=false;
return true;
}

uint32 dlb_deserializer::next()
{
if((!this->is_working())||(this->is_restored()))
{
return dlb_type_void;
}
uint8 type=static_cast<uint8>(dlb_type_void);
string str="";
str.resize(sizeof(uint8));
if(this->read(&str[0], sizeof(uint8))==0)
{
this->working=false;
return dlb_type_void;
}
memcpy(&type, &str[0], str.size());
switch(type)
{
case dlb_type_int8:
{
this->dlb_type=dlb_type_int8;
this->size=sizeof(int8);
this->value=new int8();
break;
}
case dlb_type_uint8:
{
this->dlb_type=dlb_type_uint8;
this->size=sizeof(uint8);
this->value=new uint8();
break;
}
case dlb_type_int16:
{
this->dlb_type=dlb_type_int16;
this->size=sizeof(int16);
this->value=new int16();
break;
}
case dlb_type_uint16:
{
this->dlb_type=dlb_type_uint16;
this->size=sizeof(uint16);
this->value=new uint16();
break;
}
case dlb_type_int32:
{
this->dlb_type=dlb_type_int32;
this->size=sizeof(int32);
this->value=new int32();
break;
}
case dlb_type_uint32:
{
this->dlb_type=dlb_type_uint32;
this->size=sizeof(uint32);
this->value=new uint32();
break;
}
case dlb_type_int64:
{
this->dlb_type=dlb_type_int64;
this->size=sizeof(int64);
this->value=new int64();
break;
}
case dlb_type_uint64:
{
this->dlb_type=dlb_type_uint64;
this->size=sizeof(uint64);
this->value=new uint64();
break;
}
case dlb_type_float:
{
this->dlb_type=dlb_type_float;
this->size=sizeof(float);
this->value=new float();
break;
}
case dlb_type_double:
{
this->dlb_type=dlb_type_double;
this->size=sizeof(double);
this->value=new double();
break;
}
case dlb_type_bool:
{
this->dlb_type=dlb_type_bool;
this->size=sizeof(bool);
this->value=new bool();
break;
}
case dlb_type_string:
{
this->dlb_type=dlb_type_string;
uint32 size_str=0;
str.resize(sizeof(uint32));
if(this->read(&str[0], str.size())==0)
{
this->working=false;
return dlb_type_void;
}
memcpy(&size_str, &str[0], str.size());
if(size_str==0)
{
this->working=false;
return dlb_type_void;
}
this->value=new char[size_str];
this->size=size_str;
break;
}
case dlb_type_wstring:
{
this->dlb_type=dlb_type_wstring;
uint32 size_str=0;
str.resize(sizeof(uint32));
if(this->read(&str[0], str.size())==0)
{
this->working=false;
return dlb_type_void;
}
memcpy(&size_str, &str[0], str.size());
if(size_str==0)
{
this->working=false;
return dlb_type_void;
}
this->value=new char[size_str];
this->size=size_str;
break;
}
default:
{
this->working=false;
return dlb_type_void;
}
}
str.resize(this->size);
if(this->read(&str[0], str.size())==0)
{
this->working=false;
return dlb_type_void;
}
memcpy(this->value, &str[0], str.size());
this->restored=true;
return this->dlb_type;
}

bool dlb_deserializer::get(void* output, uint32 typesize)
{
if((!this->is_working())||(!this->is_restored()))
{
return false;
}
switch(this->dlb_type)
{
case dlb_type_void:
{
return false;
}
case dlb_type_string:
{
(*(string*)output).resize(this->size);
memcpy((char*)(*(string*)output).c_str(), this->value, this->size);
this->restored=false;
this->size=0;
delete [] this->value;
this->value=NULL;
return true;
}
case dlb_type_wstring:
{
(*(wstring*)output)=dlb_string_to_wstring((char*)this->value);
this->restored=false;
this->size=0;
delete [] this->value;
this->value=NULL;
return true;
}
default:
{
memcpy(output, this->value, this->size);
this->size=0;
delete [] this->value;
this->value=NULL;
this->restored=false;
return true;
}
}
}

uint32 dlb_deserializer::read(char* outbuf, uint32 bytes)
{
if(!this->is_working())
{
return 0;
}
if(this->fromfile==false)
{
this->stream.read(outbuf, bytes);
if(this->stream.gcount()==0)
{
if(this->stream.eof())
{
this->finished=true;
return 0;
}
return 0;
}
return bytes;
}
this->ifs.read(outbuf, bytes);
if(this->ifs.gcount()==0)
{
if(this->ifs.eof())
{
this->finished=true;
return 0;
}
return 0;
}
return bytes;
}

}
