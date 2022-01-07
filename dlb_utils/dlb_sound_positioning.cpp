
#if !defined(_DLB_NO_SOUND)
#include<cmath>
#include"dlb_types.h"
#include"dlb_vector.h"
#include"dlb_sound.h"
#include"dlb_sound_positioning.h"

using namespace std;
namespace dlb
{
void dlb_position_sound_1d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float pan_step, float volume_step)
{
dlb_position_sound_custom_1d(handle, listener, source, pan_step, volume_step, 0.0f, 1.0f);
}

void dlb_position_sound_custom_1d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float pan_step, float volume_step, float start_pan, float start_volume)
{
float delta=0.0f;
float final_pan=start_pan;
float final_volume=start_volume;
// First, we calculate the delta between the listener and the source.
if(source.x<listener.x)
{
delta=listener.x-source.x;
final_pan-=(delta*pan_step);
final_volume-=(delta*volume_step);
}
else if(source.x>listener.x)
{
delta=source.x-listener.x;
final_pan+=(delta*pan_step);
final_volume-=(delta*volume_step);
}
// Then we check if the calculated values are out of range, and fix them if that's the case.
if(final_pan<-1)
{
final_pan=-1;
}
else if(final_pan>1)
{
final_pan=1;
}
if(final_volume<0)
{
final_volume=0;
}
else if(final_volume>1)
{
final_volume=1;
}
// Now we set the properties on the sound, provided that they are not already correct.
if(handle.getpan()!=final_pan)
{
handle.setpan(final_pan);
}
if(handle.getvol()!=final_volume)
{
handle.setvol(final_volume);
}
}

void dlb_position_sound_2d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease)
{
dlb_position_sound_custom_2d(handle, listener, source, theta, pan_step, volume_step, pitch_decrease, 0.0f, 1.0f, 100);
}

void dlb_position_sound_custom_2d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease, float start_pan, float start_volume, int32 start_pitch)
{
dlb_vector delta;
dlb_vector source2;
float final_pan=start_pan;
float final_volume=start_volume;
int32 final_pitch=start_pitch;
// First, we calculate the x and y based on the theta the listener is facing.
if(theta > 0.0f)
{
source2.x=(cos(theta) * (source.x-listener.x)) - (sin(theta) * (source.y-listener.y)) + listener.x;
source2.y=(sin(theta) * (source.x-listener.x)) + (cos(theta) * (source.y-listener.y)) + listener.y;
}
else
{
source2=source;
}
// Next, we calculate the delta between the listener and the source.
if(source2.x<listener.x)
{
delta.x=listener.x-source2.x;
final_pan-=(delta.x*pan_step);
final_volume-=(delta.x*volume_step);
}
else if(source2.x>listener.x)
{
delta.x=source2.x-listener.x;
final_pan+=(delta.x*pan_step);
final_volume-=(delta.x*volume_step);
}
if(source2.y<listener.y)
{
final_pitch-=abs(pitch_decrease);
delta.y=listener.y-source2.y;
final_volume-=(delta.y*volume_step);
}
else if(source2.y>listener.y)
{
delta.y=source2.y-listener.y;
final_volume-=(delta.y*volume_step);
}
// Then we check if the calculated values are out of range, and fix them if that's the case.
if(final_pan<-1)
{
final_pan=-1;
}
else if(final_pan>1)
{
final_pan=1;
}
if(final_volume<0)
{
final_volume=0;
}
else if(final_volume>1)
{
final_volume=1;
}
if(final_pitch<=0)
{
final_pitch=1;
}
else if(final_pitch>400)
{
final_pitch=400;
}
// We don't check for the highest possible pitch as it is hard to determine.
// Now we set the properties on the sound, provided that they are not already correct.
if(handle.getpan()!=final_pan)
{
handle.setpan(final_pan);
}
if(handle.getvol()!=final_volume)
{
handle.setvol(final_volume);
}
if(handle.getpitch()!=final_pitch)
{
handle.setpitch(final_pitch);
}
}

void dlb_position_sound_3d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease)
{
dlb_position_sound_custom_3d(handle, listener, source, theta, pan_step, volume_step, pitch_decrease, 0.0f, 1.0f, 100);
}

void dlb_position_sound_custom_3d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease, float start_pan, float start_volume, int32 start_pitch)
{
dlb_vector delta;
dlb_vector source2;
float final_pan=start_pan;
float final_volume=start_volume;
int32 final_pitch=start_pitch;
// First, we calculate the x and y based on the theta the listener is facing.
if(theta > 0.0f)
{
source2.x=(cos(theta) * (source.x-listener.x)) - (sin(theta) * (source.y-listener.y)) + listener.x;
source2.y=(sin(theta) * (source.x-listener.x)) + (cos(theta) * (source.y-listener.y)) + listener.y;
}
else
{
source2=source;
}
// Next, we calculate the delta between the listener and the source.
if(source2.x<listener.x)
{
delta.x=listener.x-source2.x;
final_pan-=(delta.x*pan_step);
final_volume-=(delta.x*volume_step);
}
else if(source2.x>listener.x)
{
delta.x=source2.x-listener.x;
final_pan+=(delta.x*pan_step);
final_volume-=(delta.x*volume_step);
}
if(source2.y<listener.y)
{
final_pitch-=abs(pitch_decrease);
delta.y=listener.y-source2.y;
final_volume-=(delta.y*volume_step);
}
else if(source2.y>listener.y)
{
delta.y=source2.y-listener.y;
final_volume-=(delta.y*volume_step);
}
if(source2.z<listener.z)
{
final_pitch-=abs(pitch_decrease);
delta.z=listener.z-source2.z;
final_volume-=(delta.z*volume_step);
}
else if(source2.z>listener.z)
{
delta.z=source2.z-listener.z;
final_volume-=(delta.z*volume_step);
}
// Then we check if the calculated values are out of range, and fix them if that's the case.
if(final_pan<-1)
{
final_pan=-1;
}
else if(final_pan>1)
{
final_pan=1;
}
if(final_volume<0)
{
final_volume=0;
}
else if(final_volume>1)
{
final_volume=1;
}
if(final_pitch<=0)
{
final_pitch=1;
}
else if(final_pitch>400)
{
final_pitch=400;
}
// We don't check for the highest possible pitch as it is hard to determine.
// Now we set the properties on the sound, provided that they are not already correct.
if(handle.getpan()!=final_pan)
{
handle.setpan(final_pan);
}
if(handle.getvol()!=final_volume)
{
handle.setvol(final_volume);
}
if(handle.getpitch()!=final_pitch)
{
handle.setpitch(final_pitch);
}
}
}
#endif
