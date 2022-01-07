
#define CURL_STATICLIB
#include<curl/curl.h>
#include<safe_ptr.h>
#include<Tolk.h>
#include"dlb_main.h"
#if !defined(_DLB_NO_SOUND)
#include"dlb_sound.h"
#endif
#include"dlb_http.h"
#include"dlb_event.h"
#include"dlb_worker.h"
#include"dlb_log.h"


using namespace std;

namespace dlb
{
static map<uint32, bool> table_subsystems={
{dlb_sub_bass, false},
{dlb_sub_curl, false},
{dlb_sub_speak, false},
};


uint32 dlb_init(uint32 dlb_subsystems)
{
uint32 loadeds=0;
if(dlb_subsystems==0)
{
dlb_subsystems=dlb_getconfig()->subsystems;
if(dlb_subsystems==0)
{
return 0;
}
}
dlb_config* sconfig=dlb_getconfig();
if(((dlb_subsystems&dlb_sub_bass))&&(dlb_subsystem_load(dlb_sub_bass)))
{
loadeds|=dlb_sub_bass;
}
if(((dlb_subsystems&dlb_sub_curl))&&(dlb_subsystem_load(dlb_sub_curl)))
{
loadeds|=dlb_sub_curl;
}
if(((dlb_subsystems&dlb_sub_speak))&&(dlb_subsystem_load(dlb_sub_speak)))
{
loadeds|=dlb_sub_speak;
}
return loadeds;
}

bool dlb_subsystem_load(uint32 dlb_subsystem)
{
dlb_config* sconfig=dlb_getconfig();
if(dlb_subsystem_isloaded(dlb_subsystem))
{
return false;
}
switch(dlb_subsystem)
{
case dlb_sub_bass:
{
#if !defined(_DLB_NO_SOUND)
if(dlb_sound_init_device(sconfig->sounddevice, sconfig->freq, sconfig->bass_deviceflags))
{
table_subsystems.at(dlb_subsystem)=true;
return true;
}
else
{
if(sconfig->sounddevice!=-1)
{
if(dlb_sound_init_device(-1, sconfig->freq, sconfig->bass_deviceflags))
{
table_subsystems.at(dlb_subsystem)=true;
BASS_DEVICEINFO info;
int32 i=1;
while(BASS_GetDeviceInfo(i, &info))
{
i++;
if((info.flags&BASS_DEVICE_DEFAULT))
{
sconfig->sounddevice=i;
return true;
break;
}
}
}
}
};
table_subsystems.at(dlb_subsystem)=false;
return false;
#else
return false;
#endif
}
case dlb_sub_curl:
{
int32 res=curl_global_init(CURL_GLOBAL_ALL);
if(res!=0)
{
_log("Erro ao iniciar a libcurl. Erro: {}", res);
table_subsystems.at(dlb_subsystem)=false;
return false;
}
table_subsystems.at(dlb_subsystem)=true;
return true;
}
case dlb_sub_speak:
{
#if !defined(_DLB_NO_GUI)
Tolk_Load();
if(!Tolk_IsLoaded())
{
_log("O subsistema de fala não pode ser iniciado.");
table_subsystems.at(dlb_subsystem)=false;
return false;
}
table_subsystems.at(dlb_subsystem)=true;
return true;
#else
return false;
#endif
}
}
return false;
}

bool dlb_subsystem_isloaded(uint32 subsystem)
{
if(table_subsystems.count(subsystem)==0)
{
return false;
}
return table_subsystems.at(subsystem);
}

bool dlb_subsystem_unload(uint32 dlb_subsystem)
{
if(!dlb_subsystem_isloaded(dlb_subsystem))
{
return false;
}
switch(dlb_subsystem)
{
case dlb_sub_bass:
{
#if !defined(_DLB_NO_SOUND)
BASS_DEVICEINFO info;
int32 i=1;
dlb_setconfig(dlb_config_device, -1);
while(BASS_GetDeviceInfo(i, &info))
{
i++;
if((info.flags&BASS_DEVICE_INIT))
{
BASS_SetDevice(i);
BASS_Free();
}
}
table_subsystems.at(dlb_subsystem)=false;
return true;
#else
return false;
#endif
}
case dlb_sub_curl:
{
curl_global_cleanup();
table_subsystems.at(dlb_subsystem)=false;
return true;
}
case dlb_sub_speak:
{
#if !defined(_DLB_NO_GUI)
Tolk_Unload();
table_subsystems.at(dlb_subsystem)=false;
return true;
#else
return false;
#endif
}
}
return false;
}

uint32 dlb_subsystem_get()
{
uint32 flags=0;
for(auto it=table_subsystems.begin(); it!=table_subsystems.end(); ++it)
{
if(it->second==true)
{
flags|=it->first;
}
}
return flags;
}

void dlb_mainloop()
{
//dlb_soundloop();
//dlb_curlloop();
}

void dlb_unload()
{
dlb_worker_stop_all();
dlb_event_cleanup();
for(auto it=table_subsystems.begin(); it!=table_subsystems.end(); ++it)
{
dlb_subsystem_unload(it->first);
}
}
}
