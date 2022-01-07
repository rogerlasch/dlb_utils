
/**
*Algumas funções de posicionamento de som em 1, 2 e 3d.
*É inspirado no sound_positioning do bgt e no sound_positioning.bgt de Sam Tupy.
*Escrito por Róger (rogerlasch@gmail.com) em março de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_SOUND_POSITIONING_H
#define DLB_SOUND_POSITIONING_H

namespace dlb
{
void dlb_position_sound_1d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float pan_step, float volume_step);
void dlb_position_sound_custom_1d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float pan_step, float volume_step, float start_pan, float start_volume);
void dlb_position_sound_2d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease);
void dlb_position_sound_custom_2d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease, float start_pan, float start_volume, int32 start_pitch);
void dlb_position_sound_3d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease);
void dlb_position_sound_custom_3d(dlb_sound& handle, const dlb_vector& listener, const dlb_vector& source, float theta, float pan_step, float volume_step, int32 pitch_decrease, float start_pan, float start_volume, int32 start_pitch);
}
#endif
