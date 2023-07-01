

#include<iostream>
#include<vector>
#include<chrono>
#include"dlb_calendar.h"

using namespace std;

namespace dlb
{
#define dlb_one_day 86400
#define dlb_one_hour 3600
#define dlb_one_minute 60

static vector<string> days_names={"Domingo", "Segunda-feira", "Ter�a-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "S�bado"};
static vector<string> month_names={"Janeiro", "Fevereiro", "Mar�o", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

//Based on angel script function...
tm int64_to_tm_local(int64 stime)
{
        tm local;
#ifdef _MSC_VER
        localtime_s(&local, &stime);
#else
        local = *localtime(&stime);
#endif
        return local;
}

static tm int64_tm_to_utc(int64 stime)
{
        tm utc;
#ifdef _MSC_VER
        gmtime_s(&utc, &stime);
#else
        utc = *gmtime(&stime);
#endif
        return utc;
}

//the main class
dlb_calendar::dlb_calendar()
{
this->getutc=false;
this->reset();
}

dlb_calendar::dlb_calendar(const dlb_calendar& dc)
{
*this=dc;
}

//overloads...

dlb_calendar& dlb_calendar::operator=(const dlb_calendar& dc)
{
this->timestamp=dc.timestamp;
this->getutc=dc.getutc;
memcpy(&this->current_time, &dc.current_time, sizeof(dc.current_time));
return *this;
}

bool dlb_calendar::operator<(const dlb_calendar& dc)const
{
return this->timestamp<dc.timestamp;
}

bool dlb_calendar::operator<=(const dlb_calendar& dc)const
{
return this->timestamp<=dc.timestamp;
}

bool dlb_calendar::operator==(const dlb_calendar& dc)const
{
return this->timestamp==dc.timestamp;
}

bool dlb_calendar::operator!=(const dlb_calendar& dc)const
{
return this->timestamp!=dc.timestamp;
}

bool dlb_calendar::operator>(const dlb_calendar& dc)const
{
return this->timestamp>dc.timestamp;
}

bool dlb_calendar::operator>=(const dlb_calendar& dc)const
{
return this->timestamp>=dc.timestamp;
}

//Metods...

void dlb_calendar::reset()
{
chrono::system_clock::time_point ct=chrono::system_clock::now();
int64 stime=chrono::system_clock::to_time_t(ct);
current_time=((getutc==false) ? int64_to_tm_local(stime) : int64_tm_to_utc(stime));
timestamp=stime;
}

//Geters...
int32 dlb_calendar::getyear()const
{
return this->current_time.tm_year+1900;
}

int32 dlb_calendar::getmonth()const
{
return this->current_time.tm_mon+1;
}

int32 dlb_calendar::getday()const
{
return this->current_time.tm_mday;
}

int32 dlb_calendar::gethour()const
{
return this->current_time.tm_hour;
}

int32 dlb_calendar::getminute()const
{
return this->current_time.tm_min;
}

int32 dlb_calendar::getsecond()const
{
return this->current_time.tm_sec;
}

int32 dlb_calendar::get_weekday()const
{
return this->current_time.tm_wday;
}

string dlb_calendar::get_weekday_name()const
{
return days_names[this->current_time.tm_wday];
}

string dlb_calendar:: get_month_name()const
{
return month_names[this->current_time.tm_mon];
}

int64 dlb_calendar::get_unix()const
{
return this->timestamp;
}

//set date...
bool dlb_calendar::set(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
if(year<1970)
{
return false;
}
if((month<1)||(month>12))
{
return false;
}
if((hour<0)||(hour>23))
{
return false;
}
if((minute<0)||(minute>59))
{
return false;
}
if((second<0)||(second>59))
{
return false;
}
switch(month)
{
case 1:
case 3:
case 5:
case 7:
case 8:
case 10:
case 12:
{
if((day<1)||(day>31))
{
return false;
}
break;
}
case 4:
case 6:
case 9:
case 11:
{
if((day<1)||(day>30))
{
return false;
}
break;
}
case 2:
{
if(((year%4)==0)||((year%100)==0)||((year%400)==0))
{
if((day<1)||(day>29))
{
return false;
}
}
else
{
if((day<1)||(day>28))
{
return false;
}
}
break;
}
}
tm ts;
ts.tm_year=(year-1900);
ts.tm_mon=(month-1);
ts.tm_mday=day;
ts.tm_hour=hour;
ts.tm_min=minute;
ts.tm_sec=second;
int64 x=mktime(&ts);
if(x<0)
{
return false;
}
timestamp=x;
return true;
}

bool dlb_calendar::set_unix(int64 stime)
{
if(stime<0)
{
return false;
}
current_time=((getutc==false) ? int64_to_tm_local(stime) : int64_tm_to_utc(stime));
this->timestamp=stime;
return true;
}

void dlb_calendar::set_utc(bool getutc)
{
this->getutc=getutc;
}

//add metods

bool dlb_calendar::leap_year()const
{
int32 x=this->getyear();
return (x%4)==0&&(x%100)==0&&(x%400)==0;
}

void dlb_calendar::add_years(int32 years)
{
if(years==0)
{
return;
}
this->current_time.tm_year+=years;
if(current_time.tm_year<70)
{
current_time.tm_year=70;
}
timestamp=mktime(&current_time);
}

void dlb_calendar::add_months(int32 months)
{
if(months==0)
{
return;
}
int32 years=static_cast<int32>(ceil(months/12));
int32 months_final=months%12;
if(months_final>0)
{
if((current_time.tm_mon+months_final)>11)
{
current_time.tm_year++;
current_time.tm_mon=(current_time.tm_mon+months_final)-12;
}
else
{
current_time.tm_mon+=months_final;
}
}
else
{
if((current_time.tm_mon+months_final)<0)
{
current_time.tm_year--;
current_time.tm_mon=(12-(current_time.tm_mon+months_final));
}
else
{
current_time.tm_mon+=months_final;
}
}
current_time.tm_year+=years;
timestamp=mktime(&current_time);
}

void dlb_calendar::add_days(int32 days)
{
add_seconds((days*dlb_one_day));
}

void dlb_calendar::add_hours(int32 hours)
{
add_seconds((hours*dlb_one_hour));
}

void dlb_calendar::add_minutes(int32 minutes)
{
add_seconds((minutes*dlb_one_minute));
}

void dlb_calendar::add_seconds(int32 seconds)
{
if(seconds==0)
{
return;
}
timestamp=(timestamp+seconds);
current_time=((getutc==false) ? int64_to_tm_local(timestamp) : int64_tm_to_utc(timestamp));
}

//Diff dates

int32 dlb_calendar::diff_years(const dlb_calendar& dc)
{
int res=diff_months(dc);
return((res<=0) ? 0 : res/12);
}

int32 dlb_calendar::diff_months(const dlb_calendar& dc)
{
int res=diff_days(dc);
return ((res<=0) ? 0 : static_cast<int32>(res/30.33));
}

int32 dlb_calendar::diff_days(const dlb_calendar& dc)
{
int32 res=this->diff_hours(dc);
return ((res<=0) ? 0 : res/24);
}

int32 dlb_calendar::diff_hours(const dlb_calendar& dc)
{
int32 res=this->diff_minutes(dc);
return ((res<=0) ? 0 : res/60);
}

int32 dlb_calendar::diff_minutes(const dlb_calendar& dc)
{
int32 res=this->diff_seconds(dc);
return ((res<=0) ? 0 : res/60);
}

int32 dlb_calendar::diff_seconds(const dlb_calendar& dc)
{
return static_cast<int32>(abs(this->timestamp-dc.timestamp));
}

string dlb_calendar::to_string()const
{
string msg="%d de %s de %d %d:%d:%d";
string result="";
result.resize(128);
int32 x=snprintf(&result[0], result.size(), msg.c_str(), getday(), get_month_name().c_str(), getyear(), gethour(), getminute(), getsecond());
result.resize(x);
return result;
}

ostream& operator<<(ostream& os, const dlb_calendar& dc)
{
os<<dc.to_string();
return os;
}
}
