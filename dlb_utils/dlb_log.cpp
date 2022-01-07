
#pragma warning(disable: 4244)
#include<string>
#include<fstream>
#include<mutex>
#include<iostream>
#include<format>
#include<fmt/core.h>
#include"dlb_types.h"
#include"dlb_string.h"
#include"dlb_config.h"


using namespace std;

namespace dlb
{
static bool DLB_stdout=false;
static ofstream dlb_flog;
static mutex log_mtx;

void dlb_log_open(const string& filename)
{
unique_lock<mutex> lck(log_mtx);
if(dlb_flog.is_open())
{
return;
}
dlb_flog.open(filename);
}

void dlb_log_open(const wstring& filename)
{
dlb_log_open(dlb_wstring_to_string(filename));
}

void dlb_log_close()
{
unique_lock<mutex> lck(log_mtx);
dlb_flog.close();
}

void dlb_log_set_stdout(bool dlb_stdout)
{
DLB_stdout=dlb_stdout;
}

//c++ 20
void dlb_log_write(const string& str)
{
if(!dlb_flog.is_open())
{
dlb_log_open("default log file.txt");
}
unique_lock<mutex> lck(log_mtx);
dlb_flog<<str<<endl;
if(DLB_stdout)
{
cout<<str<<endl;
}
}

void dlb_log_write_w(const wstring& str)
{
if(!dlb_flog.is_open())
{
dlb_log_open("default log file.txt");
}
unique_lock<mutex> lck(log_mtx);
string final=_f8(str);
dlb_flog<<final<<endl;
if(DLB_stdout)
{
cout<<final<<endl;
}
}
}
