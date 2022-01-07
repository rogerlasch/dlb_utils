
#pragma warning(disable: 4244)
#include<filesystem>
#include<fstream>
#include"dlb_packfile.h"
#include"dlb_string_manager.h"

using namespace std;

namespace dlb
{

static int dlb_packfile_extractfile_callback(void* data, int argc, char** values, char** colnames);
static int dlb_packfile_extract_tomemory_callback(void* data, int argc, char** values, char** colnames);
static int dlb_packfile_exists_callback(void* data, int argc, char** values, char** colnames);
static int dlb_packfile_list_callback(void* data, int argc, char** values, char** colnames);

dlb_filebuf::dlb_filebuf()
{
this->cleanup();
}

dlb_filebuf::dlb_filebuf(const dlb_filebuf& df)
{
*this=df;
}

dlb_filebuf& dlb_filebuf::operator=(const dlb_filebuf& df)
{
this->cleanup();
this->name=df.name;
this->data=df.data;
return *this;
}

void dlb_filebuf::cleanup()
{
name.resize(0);
data.resize(0);
}

char* dlb_filebuf::get_ptr()const
{
return ((data.size()>0) ? (char*)data.c_str() : NULL);
}

//The dlb_packfile class

dlb_packfile::dlb_packfile()
{
filename="";
handle=NULL;
files.clear();
active=false;
}

dlb_packfile::~dlb_packfile()
{
close();
}

string dlb_packfile::getname()const
{
return this->filename;
}

bool dlb_packfile::is_active()const
{
return active;
}

bool dlb_packfile::open(const std::string& filename, bool create_new)
{
if(is_active())
{
return false;
}
if((create_new==false)&&(!filesystem::exists(filename)))
{
return false;
}
int32 res=sqlite3_open(filename.c_str(), &handle);
if(res!=SQLITE_OK)
{
handle=NULL;
return false;
}
this->filename=filename;
files.clear();
active=true;
verife_internal();
return true;
}

bool dlb_packfile::close()
{
if(!is_active())
{
return false;
}
active=false;
filename="";
sqlite3_close(handle);
handle=NULL;
files.clear();
return true;
}

bool dlb_packfile::add_file(const std::string& filename, const std::string& internal_name)
{
if((!is_active())||(!verife_internal()))
{
return false;
}
if((filename.size()==0)||(internal_name.size()==0)||(!filesystem::exists(filename))||(this->file_exists(internal_name)))
{
return false;
}
string filedata="";
if(!prepare_file(filename, filedata))
{
return false;
}
string sql="";
sql.resize((filedata.size()+1024));
string sql_original="insert into %s(internal_name, filedata, filesize) values(\'%s\', \'%s\', %u);";
int32 x=snprintf(&sql[0], sql.size(), sql_original.c_str(), dlb_packfile_tablename, internal_name.c_str(), filedata.c_str(), filedata.size());
filedata.clear();
sql.resize(x);
return dlb_sqlite_exec(handle, sql, NULL, NULL)==SQLITE_OK;
}

bool dlb_packfile::extract_file(const std::string& internal_name, const std::string& outfile)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0)||(filename.size()==0))
{
return false;
}
if(!this->file_exists(internal_name))
{
return false;
}
ofstream ofn(outfile, ios::binary);
string sql="";
sql.resize(1024);
string str="select internal_name, filedata, filesize from %s where internal_name=\'%s\';";
int32 x=snprintf(&sql[0], sql.size(), str.c_str(), dlb_packfile_tablename, internal_name.c_str());
sql.resize(x);
int32 res=dlb_sqlite_exec(handle, sql, dlb_packfile_extractfile_callback, &ofn);
ofn.close();
return res==SQLITE_OK;
}

bool dlb_packfile::extract_to_memory(const std::string& internal_name, std::string& buffer)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0))
{
return false;
}
buffer.clear();
//Caso já tenha cido carregado...
if(files.count(internal_name)>0)
{
buffer=files.at(internal_name)->data;
return true;
}
if(!this->load_file(internal_name))
{
return false;
}
buffer=files.at(internal_name)->data;
return true;
}


bool dlb_packfile::get_file_ptr(const std::string& internal_name, char** ptr, dwparam* size)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0))
{
return false;
}
if((ptr==NULL)||(*ptr!=NULL))
{
return false;
}
if(files.count(internal_name)==0)
{
if(!load_file(internal_name))
{
return false;
}
}
*ptr=files.at(internal_name)->get_ptr();
if(size!=NULL)
{
*size=files.at(internal_name)->data.size();
}
return true;
}

bool dlb_packfile::load_file(const string& internal_name)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0))
{
return false;
}
if(files.count(internal_name)>0)
{
return true;
}
shared_ptr<dlb_filebuf> sfile=make_shared<dlb_filebuf>();
string sql="";
sql.resize(1024);
string str="select internal_name, filedata, filesize from %s where internal_name=\'%s\';";
int32 x=snprintf(&sql[0], sql.size(), str.c_str(), dlb_packfile_tablename, internal_name.c_str());
sql.resize(x);
int32 res=dlb_sqlite_exec(handle, sql, dlb_packfile_extract_tomemory_callback, &sfile);
if(res==SQLITE_OK)
{
files.insert(make_pair(internal_name, sfile));
}
return res==SQLITE_OK;
}

bool dlb_packfile::unload_file(const std::string& internal_name)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0))
{
return false;
}
if(files.count(internal_name)==0)
{
return false;
}
files.erase(internal_name);
return true;
}

bool dlb_packfile::file_exists(const std::string& internal_name)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0))
{
return false;
}
if(files.count(internal_name)>0)
{
return true;
}
string str="select count(internal_name) from %s where internal_name=\'%s\';";
string sql="";
sql.resize(512);
snprintf((char*)sql.c_str(), sql.size(), str.c_str(), dlb_packfile_tablename, internal_name.c_str());
int32 x=0;
int32 res=dlb_sqlite_exec(handle, sql, dlb_packfile_exists_callback, &x);
return x>0;
}

bool dlb_packfile::file_delete(const std::string& internal_name)
{
if((!is_active())||(!verife_internal())||(internal_name.size()==0))
{
return false;
}
string str="delete from %s where internal_name=\'%s\';";
string sql="";
sql.resize(512);
snprintf((char*)sql.c_str(), sql.size(), str.c_str(), dlb_packfile_tablename, internal_name.c_str());
int32 res=dlb_sqlite_exec(handle, sql, NULL, NULL);
return res==SQLITE_OK;
}

uint32 dlb_packfile::list_files(std::vector<std::string>& list_of_files)
{
if((!is_active())||(!verife_internal()))
{
return false;
}
string str="select internal_name from %s;";
string sql="";
sql.resize(512);
snprintf((char*)sql.c_str(), sql.size(), str.c_str(), dlb_packfile_tablename);
list_of_files.clear();
int32 res=dlb_sqlite_exec(handle, sql, dlb_packfile_list_callback, &list_of_files);
return list_of_files.size();
}

bool dlb_packfile::verife_internal()
{
if(handle==NULL)
{
active=false;
return false;
}
if(!dlb_sqlite_table_exists(handle, dlb_packfile_tablename))
{
string str="create table %s(internal_name varchar(512) not null, filedata text not null, filesize unsigned integer not null, primary key(internal_name));";
string sql="";
sql.resize(512);
snprintf(&sql[0], sql.size(), str.c_str(), dlb_packfile_tablename);
return dlb_sqlite_exec(handle, sql, NULL, NULL)==SQLITE_OK;
}
return true;
}

bool dlb_packfile::prepare_file(const string& filename, string& data)
{
if(!is_active())
{
return false;
}
data.clear();
ifstream ifs(filename, ios::binary);
if(!ifs.is_open())
{
return false;
}
ifs.seekg(0, ifs.end);
dlparam size=ifs.tellg();
ifs.seekg(0, ifs.beg);
if(size>dlb_packfile_max_filesize)
{
ifs.close();
return false;
}
data.reserve((size+(size/4))+4);
while(!ifs.eof())
{
string str="";
str.resize(3);
ifs.read(&str[0], str.size());
str.resize(ifs.gcount());
if(str.size()>0)
{
data.append(dlb_string_base64_encode(str));
}
}
ifs.close();
return true;
}


//callbacks

static int dlb_packfile_extractfile_callback(void* data, int argc, char** values, char** colnames)
{
int32 size=myatoi(values[2]);
string str=dlb_string_base64_decode(values[1]);
(*(ofstream*)data).write(str.c_str(), str.size());
return 0;
}

static int dlb_packfile_extract_tomemory_callback(void* data, int argc, char** values, char** colnames)
{
int32 size=myatoi(values[2]);
(*(shared_ptr<dlb_filebuf>*)data)->data=dlb_string_base64_decode(values[1]);
return 0;
}

static int dlb_packfile_exists_callback(void* data, int argc, char** values, char** colnames)
{
(*(int32*)data)=myatoi(values[0]);
return 0;
}

static int dlb_packfile_list_callback(void* data, int argc, char** values, char** colnames)
{
(*(vector<string>*)data).push_back(values[0]);
return 0;
}
}
