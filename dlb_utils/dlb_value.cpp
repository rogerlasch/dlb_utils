

#include"dlb_types.h"
#include"dlb_string_manager.h"
#include"dlb_wstring_manager.h"
#include"dlb_value.h"

using namespace std;

namespace dlb
{

dlb_value::dlb_value()
{
type=dlb_type_void;
memset(&storage, 0, sizeof(storage));
}

dlb_value::dlb_value(const dlb_value& val)
{
*this=val;
}

dlb_value::dlb_value(int8 val)
{
type=dlb_type_int8;
storage.int64_=val;
}

dlb_value::dlb_value(uint8 val)
{
type=dlb_type_uint8;
storage.uint64_=val;
}

dlb_value::dlb_value(int16 val)
{
type=dlb_type_int16;
storage.int64_=val;
}

dlb_value::dlb_value(uint16 val)
{
type=dlb_type_uint16;
storage.uint64_=val;
}

dlb_value::dlb_value(int32 val)
{
type=dlb_type_int32;
storage.int64_=val;
}

dlb_value::dlb_value(uint32 val)
{
type=dlb_type_uint32;
storage.uint64_=val;
}

dlb_value::dlb_value(int64 val)
{
type=dlb_type_int64;
storage.int64_=val;
}

dlb_value::dlb_value(uint64 val)
{
type=dlb_type_uint64;
storage.uint64_=val;
}

dlb_value::dlb_value(float val)
{
type=dlb_type_float;
storage.double_=val;
}

dlb_value::dlb_value(double val)
{
type=dlb_type_double;
storage.double_=val;
}

dlb_value::dlb_value(bool val)
{
type=dlb_type_bool;
storage.bool_=val;
}

dlb_value::dlb_value(const char* s)
{
type=dlb_type_string;
storage.string_=new string(((s==NULL) ? "" : s));
}

dlb_value::dlb_value(const string& s)
{
type=dlb_type_string;
storage.string_=new string(s);
}

dlb_value::dlb_value(const wchar_t* s)
{
type=dlb_type_wstring;
storage.string_=new string(((s==NULL) ? "" : dlb_wstring_to_string(s)));
}

dlb_value::dlb_value(const wstring& s)
{
type=dlb_type_wstring;
storage.string_=new string(dlb_wstring_to_string(s));
}

dlb_value& dlb_value::operator=(const dlb_value& val)
{
this->cleanup();
this->type=val.type;
switch(type)
{
case dlb_type_string:
case dlb_type_wstring:
{
storage.string_=new string(*val.storage.string_);
break;
}
default:
{
memcpy(&storage, &val.storage, sizeof(val.storage));
break;
}
}
return *this;
}

dlb_value::~dlb_value()
{
switch(type)
{
case dlb_type_string:
case dlb_type_wstring:
{
delete storage.string_;
memset(&storage, 0, sizeof(storage));
break;
}
default:
{
memset(&storage, 0, sizeof(storage));
}
}
}

uint32 dlb_value::gettype()const
{
return type;
}

void dlb_value::cleanup()
{
switch(type)
{
case dlb_type_string:
case dlb_type_wstring:
{
delete storage.string_;
memset(&storage, 0, sizeof(storage));
break;
}
default:
{
memset(&storage, 0, sizeof(storage));
}
}
type=dlb_type_void;
}

string dlb_value::to_string()const
{
switch(type)
{
case dlb_type_string:
case dlb_type_wstring:
{
return *storage.string_;
}
case dlb_type_bool:
{
return ((storage.bool_==true) ? "true" : "false");
}
case dlb_type_int8:
case dlb_type_int16:
case dlb_type_int32:
case dlb_type_int64:
{
return std::to_string(storage.int64_);
}
case dlb_type_uint8:
case dlb_type_uint16:
case dlb_type_uint32:
case dlb_type_uint64:
{
return std::to_string(storage.uint64_);
}
case dlb_type_float:
case dlb_type_double:
{
return std::to_string(storage.double_);
}
default:
{
return "void";
}
}
}

bool dlb_value::get( int8& value)const
{
if(type!=dlb_type_int8)
{
return false;
}
value=static_cast<int8>(storage.int64_);
return true;
}

bool dlb_value::get( uint8& value)const
{
if(type!=dlb_type_uint8)
{
return false;
}
value=static_cast<uint8>(storage.uint64_);
return true;
}

bool dlb_value::get( int16& value)const
{
if(type!=dlb_type_int16)
{
return false;
}
value=static_cast<int16>(storage.int64_);
return true;
}

bool dlb_value::get( uint16& value)const
{
if(type!=dlb_type_uint16)
{
return false;
}
value=static_cast<uint16>(storage.uint64_);
return true;
}

bool dlb_value::get( int32& value)const
{
if(type!=dlb_type_int32)
{
return false;
}
value=static_cast<int32>(storage.int64_);
return true;
}

bool dlb_value::get( uint32& value)const
{
if(type!=dlb_type_uint32)
{
return false;
}
value=static_cast<uint32>(storage.uint64_);
return true;
}

bool dlb_value::get( int64& value)const
{
if(type!=dlb_type_int64)
{
return false;
}
value=storage.int64_;
return true;
}

bool dlb_value::get( uint64& value)const
{
if(type!=dlb_type_uint64)
{
return false;
}
value=storage.uint64_;
return true;
}

bool dlb_value::get( float& value)const
{
if(type!=dlb_type_float)
{
return false;
}
value=static_cast<float>(storage.double_);
return true;
}

bool dlb_value::get( double& value)const
{
if(type!=dlb_type_double)
{
return false;
}
value=storage.double_;
return true;
}

bool dlb_value::get( bool& value)const
{
if(type!=dlb_type_bool)
{
return false;
}
value=storage.bool_;
return true;
}

bool dlb_value::get( string& value)const
{
if(type!=dlb_type_string)
{
return false;
}
value=*storage.string_;
return true;
}

bool dlb_value::get( wstring& value)const
{
if(type!=dlb_type_wstring)
{
return false;
}
value=dlb_string_to_wstring(*storage.string_);
return true;
}

ostream& operator<<(ostream& os, dlb_value& val)
{
os<<val.to_string();
return os;
}
}
