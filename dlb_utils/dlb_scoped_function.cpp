

#include"dlb_types.h"
#include"dlb_format.h"
#include"dlb_log.h"
#include"dlb_timer.h"
#include"dlb_scoped_function.h"

using namespace std;

namespace dlb
{
dlb_scoped_function::dlb_scoped_function(const string& func_name)
{
this->func_name=func_name;
start_time=dlb_gettimestamp();
_log("{} iniciou em {}", func_name, start_time);
}

dlb_scoped_function::~dlb_scoped_function()
{
int64 end=dlb_gettimestamp();
_log("{} finalizou em {}.", func_name, dlb_difftime(start_time, end));
}

void dlb_scoped_function::write(const string& str, dlb_format_args&& args)
{
string vf=dlb_vformat(str, args);
_log("{} {}", vf, func_name);
}
}
