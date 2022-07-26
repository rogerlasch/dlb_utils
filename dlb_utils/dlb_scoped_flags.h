

/**
*Classe auxiliar para definir flags temporárias.
*Atualmente pode ser usado com qualquer classe que possua métodos setflag e removeflag. Veja dlb_object e dlb_basic_flags.
*Esta classe é útil para definir flags que precisem ser limpas ao sair de um escopo.
*Escrito por Róger (rogerlasch@gmail.com) em Julho de 2022.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_SCOPED_FLAGS_H
#define DLB_SCOPED_FLAGS_H


#include<initializer_list>
#include<vector>

namespace dlb
{
template<class T>
struct dlb_scoped_flags
{
private:
T* hobject;
std::vector<uint32> hflags;
public:
dlb_scoped_flags(T* hobject, const std::initializer_list<uint32>& hflags)
{
this->hobject=hobject;
this->hflags=hflags;
if((this->hobject!=NULL)&&(this->hflags.size()>0))
{
for(auto& it : this->hflags)
{
this->hobject->setflag(it);
}
}
}
~dlb_scoped_flags()
{
if((this->hobject!=NULL)&&(this->hflags.size()>0))
{
for(auto& it : this->hflags)
{
this->hobject->removeflag(it);
}
}
this->hobject=NULL;
this->hflags.clear();
}
};
}
#endif
