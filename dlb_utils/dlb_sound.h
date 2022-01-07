
/**
*Classe de som que usa o bass para a reprodução.
*Suporta pitch, pan e efeitos.
*Algumas coisas ainda precisam melhorar por aqui.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em março de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_SOUND_H
#define DLB_SOUND_H

#include<string>
#include<unordered_map>
#include<bass.h>
#include<bass_fx.h>

namespace dlb
{
enum dlb_sound_device
{
dlb_sound_all_device=-2,
dlb_sound_default_device=-1
};

enum dlb_sound_effects
{
 fx_rotate=65536,
 fx_echo,
 fx_flanger,
 fx_volume,
 fx_peakeq,
 fx_reverb,
 fx_lpf,
 fx_mix,
 fx_damp,
 fx_autowah,
 fx_echo2,
 fx_phaser,
 fx_echo3,
 fx_chorus,
 fx_apf,
 fx_compressor,
 fx_distortion,
 fx_compressor2,
 fx_volume_env,
 fx_bqf,
 fx_echo4,
 fx_pitchshift,
 fx_freeverb
};

//Não tem necessidade de se usar isto diretamente.
class dlb_pitch
{
private:
int32 original_freq;
int32 current_freq;
int32 max_freq;
public:
dlb_pitch(int32 original_freq=44100);
dlb_pitch(const dlb_pitch& dp);
dlb_pitch& operator=(const dlb_pitch& dp);
int32 getpitch()const;
bool setpitch(int32 new_pitch);
void set(int32 new_freq);
friend class dlb_sound;
};

class dlb_sound
{
private:
static uint32 number_of_instances;
uint32 original_handle;
uint32 handle;
dlb_pitch pitch;
std::unordered_map<uint32, HFX> effects;
public:
dlb_sound();
dlb_sound(const std::string& filename);
dlb_sound(const std::wstring& filename);
dlb_sound(const dlb_sound& ds)=delete;
dlb_sound& operator=(const dlb_sound& ds)=delete;
~dlb_sound();
uint32 gethandle();
bool stream(const std::string& filename);
bool stream(const std::wstring& filename);
bool load(const std::string& filename);
bool load(const std::wstring& filename);
bool load_url(const std::string& url);
bool play(bool restart=false);
bool play_wait(bool restart=false);
bool play_looped(bool restart=false);
bool stop();
bool pause();
void close();
bool is_active();
bool is_playing();
bool is_paused();
bool is_stopped();
bool setvol(float new_vol);
float getvol();
bool setpan(float new_pan);
float getpan();
bool setposition(double new_position);
int32 getpitch()const;
bool setpitch(int32 new_pitch);
double getposition();
double getlength();
bool fad_out(float min_vol, uint32 time);
bool fad_in(float max_vol, uint32 time);
bool is_fadding();
void wait_for_fade();
bool fx_set(uint32 fx_type);
bool fx_exists(uint32 fx_type);
bool fx_remove(uint32 fx_type);
uint32 fx_remove_all();
uint32 fx_count();
private:
void internal_init();
};

bool dlb_sound_init_device(int32 device_number, int32 freq, uint32 flags=0);
int32 dlb_sound_get_device();
bool dlb_sound_set_device(int32 device_number);
bool dlb_sound_device_is_initialised(int32 device_number);
bool dlb_sound_device_free(int32 device_number);
}
#endif
