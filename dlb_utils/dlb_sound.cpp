
#if !defined(_DLB_NO_SOUND)
#include<stdexcept>
#include<bass.h>
#include<thread>
#include<map>
#include<chrono>
#include"dlb_types.h"
#include"dlb_sound.h"
#include"dlb_main.h"


using namespace std;
namespace dlb
{

dlb_pitch::dlb_pitch(int32 original_freq)
{
this->set(original_freq);
}

dlb_pitch::dlb_pitch(const dlb_pitch& dp)
{
*this=dp;
}

dlb_pitch& dlb_pitch::operator=(const dlb_pitch& dp)
{
this->original_freq=dp.original_freq;
this->max_freq=dp.max_freq;
this->current_freq=dp.current_freq;
return *this;
}

int32 dlb_pitch::getpitch()const
{
return ((100*current_freq)/original_freq);
}

bool dlb_pitch::setpitch(int32 new_value)
{
if((new_value<0)||(new_value>400))
{
return false;
}
current_freq=((new_value*original_freq)/100);
return true;
}

void dlb_pitch::set(int32 new_pitch)
{
this->original_freq=new_pitch;
this->max_freq=this->original_freq*4;
this->current_freq=this->original_freq;
}

//The sound class
static uint32 dlb_force_load_bass=0;
static int32 dlb_current_sound_device=-1;
uint32 dlb_sound::number_of_instances=0;
dlb_sound::dlb_sound()
{
this->original_handle=0;
this->handle=0;
this->internal_init();
}

dlb_sound::dlb_sound(const string& filename)
{
this->internal_init();
if(!stream(filename))
{
return;
}
if(!play(true))
{
return;
}
}

dlb_sound::dlb_sound(const wstring& filename)
{
this->internal_init();
if(!stream(filename))
{
return;
}
if(!play(true))
{
return;
}
}

dlb_sound::~dlb_sound()
{
close();
}

uint32 dlb_sound::gethandle()
{
return this->original_handle;
}

bool dlb_sound::stream(const string& filename)
{
close();
this->original_handle=BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_SAMPLE_FX|BASS_STREAM_PRESCAN);
if(this->original_handle==0)
{
return false;
}
this->handle=this->original_handle;
float freq=44100;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_FREQ, &freq))
{
close();
return false;
}
this->pitch.set(static_cast<int32>(freq));
return true;
}

bool dlb_sound::stream(const wstring& filename)
{
close();
this->original_handle=BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_UNICODE|BASS_SAMPLE_FX|BASS_STREAM_PRESCAN);
if(this->original_handle==0)
{
return false;
}
this->handle=this->original_handle;
float freq=44100;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_FREQ, &freq))
{
close();
return false;
}
this->pitch.set(static_cast<int32>(freq));
return true;
}

bool dlb_sound::load(const string& filename)
{
close();
this->original_handle=BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 65500, BASS_SAMPLE_FX);
if(this->original_handle==0)
{
return false;
}
this->handle=BASS_SampleGetChannel(this->original_handle, TRUE);
float freq=44100;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_FREQ, &freq))
{
close();
return false;
}
this->pitch.set(static_cast<int32>(freq));
return true;
}

bool dlb_sound::load(const wstring& filename)
{
close();
this->original_handle=BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 65500, BASS_UNICODE|BASS_SAMPLE_FX);
if(this->original_handle==0)
{
return false;
}
this->handle=BASS_SampleGetChannel(this->original_handle, TRUE);
float freq=44100;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_FREQ, &freq))
{
close();
return false;
}
this->pitch.set(static_cast<int32>(freq));
return true;
}

bool dlb_sound::load_url(const string& url)
{
close();
this->original_handle=BASS_StreamCreateURL(url.c_str(), 0, BASS_SAMPLE_FX|BASS_STREAM_PRESCAN, NULL, NULL);
if(this->original_handle==0)
{
return false;
}
this->handle=original_handle;
float freq=44100;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_FREQ, &freq))
{
close();
return false;
}
this->pitch.set(static_cast<int32>(freq));
return true;
}

bool dlb_sound::play(bool restart)
{
return BASS_ChannelPlay(handle, restart);
}

bool dlb_sound::play_wait(bool restart)
{
if(!this->play(restart))
{
return false;
}
while(this->is_playing())
{
this_thread::sleep_for(chrono::milliseconds(5));
}
return true;
}

bool dlb_sound::play_looped(bool restart)
{
if(!this->is_active())
{
return false;
}
if(BASS_ChannelFlags(this->handle, 0, 0)&BASS_SAMPLE_LOOP)
{
return this->play(restart);
}
BASS_ChannelFlags(this->handle, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
return this->play(restart);
}

bool dlb_sound::stop()
{
return BASS_ChannelStop(handle);
}

bool dlb_sound::pause()
{
return BASS_ChannelPause(handle);
}

void dlb_sound::close()
{
if(original_handle!=0)
{
fx_remove_all();
if(BASS_StreamFree(original_handle))
{
this->original_handle=0;
this->handle=0;
}
else
{
BASS_SampleFree(this->original_handle);
this->original_handle=0;
this->handle=0;
}
}
}

bool dlb_sound::is_active()
{
return this->original_handle!=0;
}

bool dlb_sound::is_playing()
{
return BASS_ChannelIsActive(this->handle)==BASS_ACTIVE_PLAYING;
}

bool dlb_sound::is_paused()
{
return BASS_ChannelIsActive(this->handle)==BASS_ACTIVE_PAUSED;
}

bool dlb_sound::is_stopped()
{
return BASS_ChannelIsActive(this->handle)==BASS_ACTIVE_STOPPED;
}

bool dlb_sound::setvol(float new_vol)
{
if((new_vol<0)||(new_vol>16))
{
return false;
}
return BASS_ChannelSetAttribute(this->handle, BASS_ATTRIB_VOL, new_vol);
}

float dlb_sound::getvol()
{
float f=-1;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_VOL, &f))
{
return -1;
}
return f;
}

bool dlb_sound::setpan(float new_pan)
{
if((new_pan<-1)||(new_pan>1))
{
return false;
}
return BASS_ChannelSetAttribute(this->handle, BASS_ATTRIB_PAN, new_pan);
}

float dlb_sound::getpan()
{
float f=-1;
if(!BASS_ChannelGetAttribute(this->handle, BASS_ATTRIB_PAN, &f))
{
return -1;
}
return f;
}

int32 dlb_sound::getpitch()const
{
return this->pitch.getpitch();
}

bool dlb_sound::setpitch(int32 new_pitch)
{
if(!this->pitch.setpitch(new_pitch))
{
return false;
}
return BASS_ChannelSlideAttribute(this->handle, BASS_ATTRIB_FREQ,
static_cast<float>(this->pitch.current_freq), 75);
}

bool dlb_sound::setposition(double new_position)
{
return BASS_ChannelSetPosition(this->handle, BASS_ChannelSeconds2Bytes(this->handle, new_position), BASS_POS_BYTE);
}

double dlb_sound::getposition()
{
return BASS_ChannelBytes2Seconds(this->handle, BASS_ChannelGetPosition(this->handle, BASS_POS_BYTE));
}

double dlb_sound::getlength()
{
return BASS_ChannelBytes2Seconds(this->handle, BASS_ChannelGetLength(this->handle, BASS_POS_BYTE));
}

bool dlb_sound::fad_out(float min_vol, uint32 time)
{
if((min_vol<0)||(min_vol>=this->getvol())||(time<=0))
{
return 0;
}
return BASS_ChannelSlideAttribute(this->handle, BASS_ATTRIB_VOL, min_vol, time);
}

bool dlb_sound::fad_in(float max_vol, uint32 time)
{
if((max_vol>16)||(max_vol==0)||(time==0))
{
return false;
}
return BASS_ChannelSlideAttribute(this->handle, BASS_ATTRIB_VOL, max_vol, time);
}

bool dlb_sound::is_fadding()
{
return BASS_ChannelIsSliding(this->handle, BASS_ATTRIB_VOL);
}

void dlb_sound::wait_for_fade()
{
while(this->is_fadding())
{
this_thread::sleep_for(chrono::milliseconds(5));
}
}
//fx effects

bool dlb_sound::fx_set(uint32 fx_type)
{
if(fx_exists(fx_type))
{
return false;
}
HFX fxhandle=BASS_ChannelSetFX(this->handle, fx_type, 1);
if(fxhandle==0)
{
return false;
}
std::pair<uint32, HFX> ps(fx_type, fxhandle);
effects.insert(ps);
return true;
}

bool dlb_sound::fx_exists(uint32 fx_type)
{
return effects.count(fx_type)>0;
}

bool dlb_sound::fx_remove(uint32 fx_type)
{
if(!fx_exists(fx_type))
{
return false;
}
HFX fxhandle=effects.at(fx_type);
BASS_ChannelRemoveFX(this->handle, fxhandle);
effects.erase(fx_type);
return true;
}

uint32 dlb_sound::fx_remove_all()
{
if(effects.size()==0)
{
return 0;
}
uint32 size=effects.size();
for(auto it=effects.begin(); it!=effects.end(); ++it)
{
BASS_ChannelRemoveFX(this->handle, it->second);
}
effects.clear();
return size;
}

uint32 dlb_sound::fx_count()
{
return effects.size();
}

void dlb_sound::internal_init()
{
if(!dlb_subsystem_isloaded(dlb_sub_bass))
{
if(!dlb_subsystem_load(dlb_sub_bass))
{
throw runtime_error("Unable to invoc BASS_Init. Error_code is: "+to_string(BASS_ErrorGetCode()));
}
}
}

//Functions

bool dlb_sound_init_device(int32 device_number, int32 freq, uint32 flags)
{
dlb_config* sconfig=dlb_getconfig();
if(BASS_Init(device_number, freq, flags, NULL, 0))
{
if((device_number!=-1)&&(sconfig->sounddevice==-1))
{
sconfig->sounddevice=device_number;
}
return true;
}
return false;
}

int32 dlb_sound_get_device()
{
return dlb_getconfig(dlb_config_device);
}

bool dlb_sound_set_device(int32 device_number)
{
if(BASS_SetDevice(device_number))
{
dlb_setconfig(dlb_config_device, device_number);
return true;
}
return false;
}

bool dlb_sound_device_is_initialised(int32 device_number)
{
BASS_DEVICEINFO info;
if(BASS_GetDeviceInfo(device_number, &info))
{
return (info.flags&BASS_DEVICE_INIT)!=0;
}
return false;
}

bool dlb_sound_device_free(int32 device_number)
{
if(!BASS_SetDevice(device_number))
{
return false;
}
if(BASS_Free())
{
if(dlb_getconfig(dlb_config_device)==device_number)
{
bool found=false;
BASS_DEVICEINFO info;
int i=1;
while(BASS_GetDeviceInfo(i, &info))
{
i++;
if((info.flags&BASS_DEVICE_INIT))
{
found=true;
dlb_getconfig()->sounddevice=i;
return true;
}
}
}
dlb_getconfig()->sounddevice=-1;
return true;
}
return false;
}

}
#endif
