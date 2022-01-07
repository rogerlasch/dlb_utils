

#include<fstream>
#include<mutex>
#include"dlb_debug.h"

using namespace std;

namespace dlb
{
static ofstream d_bug;
static mutex debug_mtx;


void dlb_debug(const string& str)
{
unique_lock<mutex> lck(debug_mtx);
if(!d_bug.is_open())
{
d_bug.open("dlb_debux.txt");
}
if(dlb_isdebugging())
{
d_bug<<str<<endl;
}
}
}
