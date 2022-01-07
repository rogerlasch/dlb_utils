

/**
*Classe de vetor para usar com dlb_sound_positioning.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em 2020 e reescrito em março de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_VECTOR_H
#define DLB_VECTOR_H

namespace dlb
{
class dlb_vector
{
public:
float x;
float y;
float z;
public:
dlb_vector(float x=0.0f, float y=0.0f, float z=0.0f);
dlb_vector(const dlb_vector& dv);
dlb_vector& operator=(const dlb_vector& dv);
bool operator==(const dlb_vector& dv)const;
//geters
float get_x()const;
float get_y()const;
float get_z()const;
void vector_get(float* out_x, float* out_y, float* out_z)const;
//seters
void set_x(float x);
void set_y(float y);
void set_z(float z);
void vector_set(float x, float y, float z);
};
}
#endif
