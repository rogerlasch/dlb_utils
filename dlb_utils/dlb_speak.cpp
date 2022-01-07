
#include<Tolk.h>
#include<format>
#include<fmt/core.h>
#include"dlb_main.h"
#include"dlb_string_manager.h"
#include"dlb_wstring_manager.h"
#include"dlb_Speak.h"

using namespace std;

namespace dlb
{
void dlb_Speak_init()
{
if(Tolk_IsLoaded())
{
return;
}
Tolk_Load();
}

void dlb_Speak_unload()
{
if(Tolk_IsLoaded())
{
return;
}
Tolk_Unload();
}

bool dlb_speak(const string& str, bool interrupt)
{
if(!dlb_subsystem_isloaded(dlb_sub_speak))
{
if(!dlb_subsystem_load(dlb_sub_speak))
{
return false;
}
}
return Tolk_Speak(dlb_string_to_wstring(str).c_str(), interrupt);
}

bool dlb_speak_w(const wstring& str, bool interrupt)
{
if(!dlb_subsystem_isloaded(dlb_sub_speak))
{
if(!dlb_subsystem_load(dlb_sub_speak))
{
return false;
}
}
return Tolk_Speak(str.c_str(), interrupt);
}

bool dlb_Speak_stop()
{
return Tolk_Silence();
}
}
