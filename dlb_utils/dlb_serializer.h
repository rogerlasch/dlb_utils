

/**
*Classe para serialização de dados.
*Atualmente não lida direito com endian, mas logo deve ser resolvido.
*Escrito por Róger (rogerlasch@gmail.com) em 2020 e reescrito em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_SERIALIZER_H
#define DLB_SERIALIZER_H

namespace dlb
{
class dlb_serializer
{
private:
std::string stream;
bool store;
public:
dlb_serializer();
dlb_serializer(const dlb_serializer& ds);
dlb_serializer& operator=(const dlb_serializer& ds);
void set_store(bool store);
void cleanup_store();
uint32 get_store(std::string& output);
uint32 save_store(const std::string& filename);
std::string serialize_int8(int8 value);
std::string serialize_uint8(uint8 value);
std::string serialize_int16(int16 value);
std::string serialize_uint16(uint16 value);
std::string serialize_int32(int32 value);
std::string serialize_uint32(uint32 value);
std::string serialize_int64(int64 value);
std::string serialize_uint64(uint64 value);
std::string serialize_float(float value);
std::string serialize_double(double value);
std::string serialize_bool(bool value);
std::string serialize_string(const std::string& value);
std::string serialize_wstring(const std::wstring& value);
};
}
#endif
