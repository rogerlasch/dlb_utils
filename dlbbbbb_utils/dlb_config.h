
/**
*Classe de configuração usado internamente na dlb. Ainda não é tão útil como gostaria, mas espero concluir em breve.
*Escrito por Róger (rogerlasch@gmail.com) em Julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_CONFIG_H
#define dlb_CONFIG_H



namespace dlb
{
enum dlb_config_subsystems
{
dlb_sub_bass=(1<<0),
dlb_sub_curl=(1<<1),
dlb_sub_speak=(1<<2),
dlb_sub_all=(dlb_sub_bass|dlb_sub_curl|dlb_sub_speak)
};

enum dlb_config_options
{
dlb_config_gvol,
dlb_config_device,//define o número do dispositivo de saída
dlb_config_freq, //Frequência do dispositivo
dlb_config_bass_deviceflags,//As flags para o dispositivo bass
dlb_config_debug,
dlb_config_error,
};

class dlb_config
{
public:
int32 sounddevice;
int32 freq;
uint32 bass_deviceflags;
int32 gvol;
uint32 subsystems;
uint32 error_code;
bool debug;
dlb_config();
dlb_config(const dlb_config& conf);
dlb_config& operator=(const dlb_config& conf);
void reset();
std::string to_string();
std::string serialize();
bool load(const std::string& str);
};

bool dlb_setconfig(uint32 config_option, int32 value);
int32 dlb_getconfig(uint32 config_option);
dlb_config* dlb_getconfig();
bool dlb_isdebugging();
void dlb_seterror(uint32 error_code);
uint32 dlb_getlasterror();
bool dlb_config_import(const std::string& str, bool from_file=false);
std::string dlb_config_export(const std::string& str, bool to_file=false);
}
#endif
