

/**
*Classe para deserializar dados em formato binário.
*Atualmente ainda não lida muito bem com big endian e litle endian, mas comecei de resolver isto aos poucos.
*Note que deve usar a classe dlb_serializer para poder deserializar os dados com esta classe.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em Junho de 2021
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_DESERIALIZER_H
#define DLB_DESERIALIZER_H

#include<fstream>
#include<string>
#include<sstream>
namespace dlb
{

class dlb_deserializer
{
private:
std::stringstream stream;
std::ifstream ifs;
uint32 dlb_type;
uint32 size;
void* value;
bool finished;
bool restored;
bool working;
bool fromfile;
public:
dlb_deserializer();
dlb_deserializer(const dlb_deserializer& ds)=delete;
dlb_deserializer& operator=(const dlb_deserializer& ds)=delete;
~dlb_deserializer();
void cleanup();
bool is_restored();
bool is_finished();
bool is_working();
bool deserialize_from_file(const std::string& filename);
bool deserialize_from_string(const std::string& str);
uint32 next();
bool get(void* output, uint32 typesize);
private:
uint32 read(char* outbuf, uint32 bytes);
};
}
#endif
