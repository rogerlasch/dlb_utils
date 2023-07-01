

#include"dlb_types.h"
#include"dlb_serializer.h"
#include"dlb_deserializer.h"
#include"dlb_dictionary.h"

using namespace std;

namespace dlb
{
dlb_dictionary::dlb_dictionary()
{
this->delete_all();
}

dlb_dictionary::dlb_dictionary(const dlb_dictionary& dt)
{
*this=dt;
}

dlb_dictionary& dlb_dictionary::operator=(const dlb_dictionary& dt)
{
if(!this->is_empty())
{
this->delete_all();
}
scoped_lock sc(this->mtx, dt.mtx);
this->items=dt.items;
return *this;
}

uint32 dlb_dictionary::get_size()const
{
shared_lock lck(this->mtx);
return this->items.size();
}

uint32 dlb_dictionary::get_type(const dlb_key& dk)const
{
if(!this->exists(dk))
{
return dlb_type_void;
}
shared_lock lck(this->mtx);
return this->items.at(dk).gettype();
}

bool dlb_dictionary::is_empty()const
{
shared_lock lck(this->mtx);
return items.size()==0;
}

bool dlb_dictionary::exists(const dlb_key& dk)const
{
shared_lock lck(this->mtx);
return this->items.count(dk)>0;
}

bool dlb_dictionary::delete_key(const dlb_key& dk)
{
if(!this->exists(dk))
{
return false;
}
unique_lock<shared_mutex> lck(this->mtx);
this->items.erase(dk);
return true;
}

void dlb_dictionary::delete_all()
{
if(this->get_size()>0)
{
unique_lock<shared_mutex> lck(this->mtx);
this->items.clear();
}
}

uint32 dlb_dictionary::get_keys(vector<dlb_key>& keys)const
{
keys.clear();
if(this->get_size()==0)
{
return 0;
}
shared_lock lck(this->mtx);
for(auto it=this->items.begin(); it!=this->items.end(); ++it)
{
keys.push_back(it->first);
}
return keys.size();
}

uint32 dlb_dictionary::get_keys(list<dlb_key>& keys)const
{
keys.clear();
if(this->get_size()==0)
{
return 0;
}
shared_lock lck(this->mtx);
for(auto it=this->items.begin(); it!=this->items.end(); ++it)
{
keys.push_back(it->first);
}
return keys.size();
}

//set and get

void dlb_dictionary::set(const dlb_key& key_name, int8 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, uint8 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, int16 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, uint16 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, int32 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, uint32 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, int64 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, uint64 value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, float value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, double value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, bool value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, const char* value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, const string& value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, const wchar_t* value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, const wstring& value)
{
this->set(key_name, dlb_value(value));
}

void dlb_dictionary::set(const dlb_key& key_name, const dlb_value& val)
{
if(exists(key_name))
{
this->delete_key(key_name);
}
unique_lock<shared_mutex> lck(this->mtx);
items.insert(make_pair(key_name, val));
}

//get

bool dlb_dictionary::get(const dlb_key& key_name, int8& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, uint8& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, int16& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, uint16& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, int32& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, uint32& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, int64& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, uint64& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, float& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, double& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, bool& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, string& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

bool dlb_dictionary::get(const dlb_key& key_name, wstring& value)const
{
shared_lock lck(this->mtx);
return((items.count(key_name)==false) ? false : items.at(key_name).get(value));
}

//Functions

uint32 dlb_serialize_dictionary(const dlb_dictionary& dt, const string& filename, string* output)
{
if(dt.is_empty())
{
return 0;
}
dlb_serializer sr;
sr.set_store(true);
list<dlb_key> keys;
dt.get_keys(keys);
uint32 serializeds=0;
for(auto it=keys.begin(); it!=keys.end(); ++it)
{
uint32 dlb_type=dt.get_type(*it);
switch(dlb_type)
{
case dlb_type_int8:
{
int8 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_int8(value);
serializeds++;
break;
}
case dlb_type_uint8:
{
uint8 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_uint8(value);
serializeds++;
break;
}
case dlb_type_int16:
{
int16 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_int16(value);
serializeds++;
break;
}
case dlb_type_uint16:
{
uint16 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_uint16(value);
serializeds++;
break;
}
case dlb_type_int32:
{
int32 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_int32(value);
serializeds++;
break;
}
case dlb_type_uint32:
{
uint32 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_uint32(value);
serializeds++;
break;
}
case dlb_type_int64:
{
int64 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_int64(value);
serializeds++;
break;
}
case dlb_type_uint64:
{
uint64 value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_uint64(value);
serializeds++;
break;
}
case dlb_type_float:
{
float value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_float(value);
serializeds++;
break;
}
case dlb_type_double:
{
double value=0;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_double(value);
serializeds++;
break;
}
case dlb_type_bool:
{
bool value=false;
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_bool(value);
serializeds++;
break;
}
case dlb_type_string:
{
string value="";
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_string(value);
serializeds++;
break;
}
case dlb_type_wstring:
{
wstring value=L"";
dt.get(*it, value);
sr.serialize_string(it->str());
sr.serialize_wstring(value);
serializeds++;
break;
}
default:
{
continue;
}
}
}
if(serializeds==0)
{
return 0;
}
if(filename.size()>0)
{
if(!sr.save_store(filename))
{
return 0;
}
}
if(output!=NULL)
{
sr.get_store(*output);
}
return serializeds;
}

uint32 dlb_deserialize_dictionary(dlb_dictionary& dt, const std::string& indata, bool fromfile)
{
if(indata.size()==0)
{
return 0;
}
dt.delete_all();
dlb_deserializer sr;
if(fromfile==true)
{
if(!sr.deserialize_from_file(indata))
{
return 0;
}
}
else
{
if(!sr.deserialize_from_string(indata))
{
return 0;
}
}
while(sr.is_working())
{
string key_name="";
uint32 dlb_type=sr.next();
if(dlb_type!=dlb_type_string)
{
break;
}
if(!sr.get(&key_name, 0))
{
continue;
}
dlb_type=sr.next();
switch(dlb_type)
{
case dlb_type_int8:
{
int8 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_uint8:
{
uint8 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_int16:
{
int16 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_uint16:
{
uint16 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_int32:
{
int32 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_uint32:
{
uint32 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_int64:
{
int64 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_uint64:
{
uint64 value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_float:
{
float value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_double:
{
double value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_bool:
{
bool value;
if(!sr.get(&value, sizeof(value)))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_string:
{
string value;
if(!sr.get(&value, 0))
{
continue;
}
dt.set(key_name, value);
break;
}
case dlb_type_wstring:
{
wstring value;
if(!sr.get(&value, 0))
{
continue;
}
dt.set(key_name, value);
break;
}
default:
{
break;
}
}
}
sr.cleanup();
return dt.get_size();
}
}
