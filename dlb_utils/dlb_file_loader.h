

/**
*Classe para carregar arquivos grandes assincronamente.
*Queria testar minha classe de estado então tive esta ideia maluca.
*Ela funciona bem, mas, sei que ela pode melhorar ainda.
*Escrito por Róger (rogerlasch@gmail.com) em Maio de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_FILE_LOADER_H
#define DLB_FILE_LOADER_H

#include<future>

namespace dlb
{
class dlb_file_loader : public dlb_state
{
private:
std::string filename;
std::string data;
std::future<uint32> asyncbuf;
public:
dlb_file_loader();
dlb_file_loader(const dlb_file_loader& dbuf)=delete;
dlb_file_loader& operator=(const dlb_file_loader& dbuf)=delete;
bool operator==(const std::string& filename)const;
bool operator==(const dlb_file_loader& dbuf)const;
std::string get_filename()const;
uint32 size()const;
char* getdata()const;
uint32 load(const std::string& filename);
uint32 load_async(const std::string& filename);
uint32 cancel();
void cleanup();
private:
uint32 readfile();
};
}
#endif
