

#include<iostream>
#include<string.h>
#include"dlb_types.h"
#include"dlb_key.h"

using namespace std;

namespace dlb
{

static void dlb_clean_doublestring(string& str)
{
uint32 start=str.find(".", 0);
if(start==string::npos)
{
return;
}
bool remove=true;
for(uint32 i=start+1; i<str.size(); i++)
{
if(str[i]!='0')
{
remove=false;
break;
}
}
if(remove==true)
{
str.resize(start);
}
}

dlb_key::dlb_key()
{
this->keyname="";
}

 dlb_key::dlb_key(int32 value)
{
*this=value;
}

 dlb_key::dlb_key(uint32 value)
{
*this=value;
}

 dlb_key::dlb_key(int64 value)
{
*this=value;
}

 dlb_key::dlb_key(uint64 value)
{
*this=value;
}

dlb_key::dlb_key(double value)
{
*this=value;
}

dlb_key::dlb_key(const char* value)
{
*this=value;
}

dlb_key::dlb_key(const string& value)
{
*this=value;
}

dlb_key::dlb_key(const dlb_key& value)
{
*this=value;
}

//The operator= overloads

dlb_key& dlb_key::operator=(int32 value)
{
this->keyname=to_string(value);
this->flags=dlb_key_valid|dlb_key_num;
return *this;
}

dlb_key& dlb_key::operator=(uint32 value)
{
this->keyname=to_string(value);
this->flags=dlb_key_valid|dlb_key_num;
return *this;
}

dlb_key& dlb_key::operator=(int64 value)
{
this->keyname=to_string(value);
this->flags=dlb_key_valid|dlb_key_num;
return *this;
}

dlb_key& dlb_key::operator=(uint64 value)
{
this->keyname=to_string(value);
this->flags=dlb_key_valid|dlb_key_num;
return *this;
}

dlb_key& dlb_key::operator=(double keyname)
{
this->keyname=to_string(keyname);
dlb_clean_doublestring(this->keyname);
this->flags=dlb_key_valid|dlb_key_num;
return *this;
}

dlb_key& dlb_key::operator=(const char* keyname)
{
if(keyname==NULL)
{
this->keyname="";
this->flags=0;
}
else
{
this->keyname=keyname;;
this->flags=dlb_key_valid;
}
return *this;
}

dlb_key& dlb_key::operator=(const string& keyname)
{
this->keyname=keyname;
this->flags=((keyname.size()>0) ? dlb_key_valid : 0);
return *this;
}

dlb_key& dlb_key::operator=(const dlb_key& dk)
{
this->keyname=dk.keyname;
this->flags=dk.flags;
return *this;
}

bool dlb_key::operator<(int32 value)const
{
return this->keyname<to_string(value);
}

bool dlb_key::operator<(uint32 value)const
{
return this->keyname<to_string(value);
}

bool dlb_key::operator<(int64 value)const
{
return this->keyname<to_string(value);
}

bool dlb_key::operator<(uint64 value)const
{
return this->keyname<to_string(value);
}

bool dlb_key::operator<(double keyname)const
{
string s=to_string(keyname);
dlb_clean_doublestring(s);
return this->keyname<s;
}

bool dlb_key::operator<(const char* keyname)const
{
return this->keyname<keyname;
}

bool dlb_key::operator<(const string& keyname)const
{
return this->keyname<keyname;
}

bool dlb_key::operator<(const dlb_key& dk)const
{
return this->keyname<dk.keyname;
}

bool dlb_key::operator<=(int32 value)const
{
return this->keyname<=to_string(value);
}

bool dlb_key::operator<=(uint32 value)const
{
return this->keyname<=to_string(value);
}

bool dlb_key::operator<=(int64 value)const
{
return this->keyname<=to_string(value);
}

bool dlb_key::operator<=(uint64 value)const
{
return this->keyname<=to_string(value);
}

bool dlb_key::operator<=(double keyname)const
{
string s=to_string(keyname);
dlb_clean_doublestring(s);
return this->keyname<=s;
}

bool dlb_key::operator<=(const char* keyname)const
{
return this->keyname<=keyname;
}

bool dlb_key::operator<=(const string& keyname)const
{
return this->keyname<=keyname;
}

bool dlb_key::operator<=(const dlb_key& dk)const
{
return this->keyname<=dk.keyname;
}

//The operator== overloads

bool dlb_key::operator==(int32 value)const
{
return this->keyname==to_string(value);
}

bool dlb_key::operator==(uint32 value)const
{
return this->keyname==to_string(value);
}

bool dlb_key::operator==(int64 value)const
{
return this->keyname==to_string(value);
}

bool dlb_key::operator==(uint64 value)const
{
return this->keyname==to_string(value);
}

bool dlb_key::operator==(double keyname)const
{
string s=to_string(keyname);
dlb_clean_doublestring(s);
return this->keyname==s;
}

bool dlb_key::operator==(const char* keyname)const
{
return this->keyname==keyname;
}

bool dlb_key::operator==(const string& keyname)const
{
return this->keyname==keyname;
}

bool dlb_key::operator==(const dlb_key& dk)const
{
return this->keyname==dk.keyname;
}

bool dlb_key::operator>(int32 value)const
{
return this->keyname>to_string(value);
}

bool dlb_key::operator>(uint32 value)const
{
return this->keyname>to_string(value);
}

bool dlb_key::operator>(int64 value)const
{
return this->keyname>to_string(value);
}

bool dlb_key::operator>(uint64 value)const
{
return this->keyname>to_string(value);
}

bool dlb_key::operator>(double keyname)const
{
string s=to_string(keyname);
dlb_clean_doublestring(s);
return this->keyname>s;
}

bool dlb_key::operator>(const char* keyname)const
{
return this->keyname>keyname;
}

bool dlb_key::operator>(const string& keyname)const
{
return this->keyname>keyname;
}

bool dlb_key::operator>(const dlb_key& dk)const
{
return this->keyname>dk.keyname;
}

bool dlb_key::operator>=(int32 value)const
{
return this->keyname>=to_string(value);
}

bool dlb_key::operator>=(uint32 value)const
{
return this->keyname>=to_string(value);
}

bool dlb_key::operator>=(int64 value)const
{
return this->keyname>=to_string(value);
}

bool dlb_key::operator>=(uint64 value)const
{
return this->keyname>=to_string(value);
}

bool dlb_key::operator>=(double keyname)const
{
string s=to_string(keyname);
dlb_clean_doublestring(s);
return this->keyname>=s;
}

bool dlb_key::operator>=(const char* keyname)const
{
return this->keyname>=keyname;
}

bool dlb_key::operator>=(const string& keyname)const
{
return this->keyname>=keyname;
}

bool dlb_key::operator>=(const dlb_key& dk)const
{
return this->keyname>=dk.keyname;
}

string dlb_key::str()const
{
return this->keyname;
}

bool dlb_key::is_valid()const
{
return (((this->flags&dlb_key_valid))&&(keyname.size()>0));
}

bool dlb_key::isnum()const
{
return (this->flags&dlb_key_num)>0;
}

//The operator<<

ostream& operator<<(ostream& os, const dlb_key& dk)
{
os<<dk.keyname;
return os;
}
}
