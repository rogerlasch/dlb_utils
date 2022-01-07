
#include<cstdlib>
#include"dlb_vector.h"

namespace dlb
{
dlb_vector::dlb_vector(float x, float y, float z)
{
this->set_x(x);
this->set_y(y);
this->set_z(z);
}

dlb_vector::dlb_vector(const dlb_vector& dv)
{
*this=dv;
}

dlb_vector& dlb_vector::operator=(const dlb_vector& dv)
{
this->set_x(dv.get_x());
this->set_y(dv.get_y());
this->set_z(dv.get_z());
return *this;
}

bool dlb_vector::operator==(const dlb_vector& dv)const
{
return this->get_x()==dv.get_x()&&this->get_y()==dv.get_y()&&this->get_z()==dv.get_z();
}

//geters
float dlb_vector::get_x()const
{
return this->x;
}

float dlb_vector::get_y()const
{
return this->y;
}

float dlb_vector::get_z()const
{
return this->z;
}

void dlb_vector::vector_get(float* out_x, float* out_y, float* out_z)const
{
if(out_x!=NULL)
{
*out_x=this->get_x();
}
if(out_y!=NULL)
{
*out_y=this->get_y();
}
if(out_z!=NULL)
{
*out_z=this->get_z();
}
}

//seters
void dlb_vector::set_x(float x)
{
this->x=x;
}

void dlb_vector::set_y(float y)
{
this->y=y;
}

void dlb_vector::set_z(float z)
{
this->z=z;
}

void dlb_vector::vector_set(float x, float y, float z)
{
this->set_x(x);
this->set_y(y);
this->set_z(z);
}
}
