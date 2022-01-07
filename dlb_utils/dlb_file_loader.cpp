
#include<fstream>
#include"dlb_state.h"
#include"dlb_timer.h"
#include"dlb_file_loader.h"

using namespace std;

namespace dlb
{
dlb_file_loader::dlb_file_loader() : dlb_state()
{
this->filename="";
this->data="";
}

bool dlb_file_loader::operator==(const string& filename)const
{
return this->filename==filename;
}

bool dlb_file_loader::operator==(const dlb_file_loader& dbuf)const
{
return this->filename==dbuf.filename;
}

string dlb_file_loader::get_filename()const
{
return this->filename;
}

uint32 dlb_file_loader::size()const
{
return this->data.size();
}

char* dlb_file_loader::getdata()const
{
return (char*)this->data.c_str();
}

uint32 dlb_file_loader::load(const string& filename)
{
switch(getstate())
{
case dlb_state_hidle:
{
setstate(dlb_state_verifying_params);
if(filename.size()==0)
{
seterror(dlb_error_illparam);
setstate(dlb_state_error);
return 0;
}
this->filename=filename;
return readfile();
}
case dlb_state_error:
{
this->cleanup();
return load(filename);
}
default:
{
seterror(dlb_error_invalid_state);
return 0;
}
}
}

uint32 dlb_file_loader::load_async(const string& filename)
{
switch(getstate())
{
case dlb_state_hidle:
{
setstate(dlb_state_verifying_params);
if(filename.size()==0)
{
seterror(dlb_error_illparam);
setstate(dlb_state_error);
return getstate();
}
this->filename=filename;
asyncbuf=async(&dlb_file_loader::readfile, this);
//Aguarda até o estado mudar para retornar
dlb_timer tmr;
while(getstate()==dlb_state_verifying_params)
{
tmr.wait(2);
if(geterror()==dlb_error_invalid_state)
{
return getstate();
}
}
return getstate();
}
case dlb_state_error:
{
this->cleanup();
return load_async(filename);
}
default:
{
seterror(dlb_error_invalid_state);
return dlb_state_error;
}
}
}

void dlb_file_loader::cleanup()
{
dlb_state::cancel();
dlb_state::reset();
this->filename="";
this->data="";
}

uint32 dlb_file_loader::readfile()
{
seterror(dlb_error_ok);
if(getstate()!=dlb_state_verifying_params)
{
seterror(dlb_error_invalid_state);
return 0;
}
setstate(dlb_state_verifying_file);
//Depois por um dlb_file_exists
ifstream ifs(filename, ios::binary);
if(!ifs.is_open())
{
seterror(dlb_error_open_file);
setstate(dlb_state_error);
return 0;
}
ifs.seekg(0, ifs.end);
int64 x=ifs.tellg();
ifs.seekg(0, ifs.beg);
if(x<=0)
{
seterror(dlb_error_file_empty);
setstate(dlb_state_error);
return 0;
}
data.resize(static_cast<uint32>(x));
setstate(dlb_state_reading_file);
uint32 current_size=0;
while(!ifs.eof())
{
ifs.read(&data[current_size], 1);
current_size++;
if(is_cancel())
{
break;
}
}
ifs.close();
if(data.size()>current_size)
{
data.resize(current_size);
}
if(is_cancel())
{
setstate(dlb_state_canceled);
}
else
{
setstate(dlb_state_finished);
}
return data.size();
}
}
