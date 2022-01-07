
#define PICOJSON_USE_INT64
#include<sstream>
#include<picojson.h>
#include<bass.h>
#include"dlb_state.h"
#include"dlb_types.h"
#include"dlb_filesystem.h"
#include"dlb_config.h"

using namespace std;

namespace dlb
{
static dlb_config dconf;

dlb_config::dlb_config()
{
this->reset();
}

dlb_config::dlb_config(const dlb_config& conf)
{
*this=conf;
}

dlb_config& dlb_config::operator=(const dlb_config& conf)
{
this->freq=conf.freq;
this->gvol=conf.gvol;
this->subsystems=conf.subsystems;
this->error_code=conf.error_code;
this->debug=conf.debug;
return *this;
}

void dlb_config::reset()
{
this->sounddevice=-1;
this->freq=44100;
this->bass_deviceflags=0;
this->gvol=10000;
this->error_code=dlb_error_ok;
this->subsystems=0;
this->debug=false;
}

string dlb_config::to_string()
{
stringstream str;
str<<"Device number: "<<sounddevice<<endl;
str<<"Audio frequence: "<<freq<<endl;
str<<"bass_flags: "<<bass_deviceflags<<endl;
str<<"Gvol: "<<gvol<<endl;
str<<"Debug: "<<debug<<endl;
str<<"Subsystem flags: "<<subsystems<<endl;
str<<"subsystems: ";
if(subsystems==0)
{
str<<"none"<<endl;
}
else
{
if((subsystems&dlb_sub_bass))
{
str<<" bass";
}
if((subsystems&dlb_sub_curl))
{
str<<" libcurl";
}
if((subsystems&dlb_sub_speak))
{
str<<" speak";
}
}
str<<endl;
return str.str();
}

string dlb_config::serialize()
{
picojson::object ob;
ob["sounddevice"]=picojson::value(static_cast<int64>(sounddevice));
ob["frequence"]=picojson::value(static_cast<int64>(freq));
ob["bassflags"]=picojson::value(static_cast<int64>(bass_deviceflags));
ob["gvol"]=picojson::value(static_cast<int64>(gvol));
ob["debug"]=picojson::value(debug);
ob["bass"]=picojson::value(((subsystems&dlb_sub_bass) ? true : false));
ob["bass"]=picojson::value(((subsystems&dlb_sub_curl) ? true : false));
ob["speak"]=picojson::value(((subsystems&dlb_sub_speak) ? true : false));
return picojson::value(ob).serialize();
}

bool dlb_config::load(const string& str)
{
if(str.size()==0)
{
return false;
}
picojson::value v;
stringstream ss(str);
ss>>v;
picojson::object vv=v.get<picojson::object>();
if(vv.count("sounddevice")>0)
{
sounddevice=static_cast<int32>(vv.at("sounddevice").get<int64>());
}
if(vv.count("frequence")>0)
{
freq=static_cast<int32>(vv.at("frequence").get<int64>());
}
if(vv.count("bassflags")>0)
{
bass_deviceflags=static_cast<uint32>(vv.at("bassflags").get<int64>());
}
if(vv.count("gvol")>0)
{
gvol=static_cast<int32>(vv.at("gvol").get<int64>());
}
if(vv.count("debug")>0)
{
debug=vv.at("debug").get<bool>();
}
if((vv.count("bass")>0)&&(vv.at("bass").get<bool>()==true))
{
subsystems|=dlb_sub_bass;
}
if((vv.count("curl")>0)&&(vv.at("curl").get<bool>()==true))
{
subsystems|=dlb_sub_curl;
}
if((vv.count("speak")>0)&&(vv.at("speak").get<bool>()==true))
{
subsystems|=dlb_sub_speak;
}
return true;
}

//functions

/**
*Função que define o comportamento da biblioteca.
*A função irá aplicar as modificações onde puderem ser modificadas.
*Por enquanto as maiores configurações são feitas no sistema de som.
**/

bool dlb_setconfig(uint32 config_option, int32 value)
{
switch(config_option)
{
case dlb_config_gvol:
{
if((value<0)||(value>10000))
{
return false;
}
dconf.gvol=value;
BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, value);
BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, value);
BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, value);
return true;
}
case dlb_config_device://define o número do dispositivo de saída
{
if(BASS_SetDevice(value))
{
dconf.sounddevice=value;
return true;
}
return false;
}
case dlb_config_freq: //Frequência do dispositivo
{
if(value<1)
{
return false;
}
dconf.freq=value;
return true;
}
case dlb_config_bass_deviceflags://As flags para o dispositivo bass
{
dconf.bass_deviceflags=value;
return true;
}
case dlb_config_debug:
{
dconf.debug=((value>0) ? true : false);
break;
}
case dlb_config_error:
{
return true;
}
}
return false;
}

int32 dlb_getconfig(uint32 config_option)
{
switch(config_option)
{
case dlb_config_gvol:
{
return dconf.gvol;
}
case dlb_config_device://define o número do dispositivo de saída
{
return dconf.sounddevice;
}
case dlb_config_freq: //Frequência do dispositivo
{
return dconf.freq;
}
case dlb_config_bass_deviceflags://As flags para o dispositivo bass
{
return static_cast<int32>(dconf.bass_deviceflags);
}
case dlb_config_error:
{
return 0;
}
}
return 0;
}

/**
*Função que recupera um ponteiro para o objeto interno de configuração.
*Cuidado extra deve ser tomado ao manipular diretamente este objeto.
*Muitas opções tem passos extras que precisam ser seguidos quando uma opção é alterada, e manipulando diretamente o objeto, o risco de esquecer algo é grande.
*Por exemplo, se você alterar o número de dispositivo de som diretamente, a biblioteca não irá mudar automaticamente na biblioteca de som.
*Então, para alterar as coisas com segurança, use as funções dlb_setconfig e dlb_getconfig.
**/
dlb_config* dlb_getconfig()
{
return &dconf;
}

bool dlb_isdebugging()
{
return dconf.debug;
}

void dlb_seterror(uint32 error_code)
{
dconf.error_code=error_code;
}

uint32 dlb_getlasterror()
{
return dconf.error_code;
}

/**
*Importa as configurações de um arquivo ou de uma string.
*Isto é útil chamar no início do seu programa para definir as configurações da biblioteca como por exemplo, que módulos carregar, que arquivo de pacote de sons usar, etc...
**/
bool dlb_config_import(const string& str, bool from_file)
{
if(str.size()==0)
{
return false;
}
if(from_file==false)
{
return dconf.load(str);
}
string data="";
if(dlb_file_read(str, data)<=0)
{
return false;
}
return dconf.load(data);
}

/**
*Exporta uma string com as preferências atuais da biblioteca.
*Isto pode ser usado mais tarde para inicializar a biblioteca novamente.
*Por exemplo:
*Chame dlb_config_import no início do seu aplicativo e em seguida dlb_init, a biblioteca será inicializada lendo as configurações do objeto.
**/
string dlb_config_export(const string& str, bool to_file)
{
if((to_file==true)&&(str.size()==0))
{
return "";
}
if(to_file==false)
{
return dconf.serialize();
}
string final=dconf.serialize();
if(dlb_file_write(str, final)==0)
{
return "";
}
return final;
}
}
