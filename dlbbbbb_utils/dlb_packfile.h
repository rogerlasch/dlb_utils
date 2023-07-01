
/**
*Classe de pacote de arquivos.
*Isto normalmente é usado em jogos para empacotar fases, sons, etc...
*Ele é baseado no pack_file do bgt. Aqui, uso um banco de dados em sqlite para guardar as coisas.
*Provavelmente existem métodos melhores para fazer isto, mas não consegui achar nada parecido. E usar arquivos zips não me parecia muito atraente.
*Escrito por Róger (rogerlasch@gmail.com) em agosto de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_PACKFILE_H
#define DLB_PACKFILE_H

#include<string>
#include<map>
#include<vector>
#include<memory>
#if !defined(DLB_SQL_H)
#include"dlb_sql.h"
#endif

namespace dlb
{
#define dlb_packfile_tablename "dlb_packfile_sounds"
#define dlb_packfile_max_filesize (1024*1024)*1024

class dlb_filebuf
{
public:
std::string name;
std::string data;
dlb_filebuf();
dlb_filebuf(const dlb_filebuf& df);
dlb_filebuf& operator=(const dlb_filebuf& df);
void cleanup();
char* get_ptr()const;
};

class dlb_packfile
{
private:
std::string filename;
sqlite3* handle;
std::map<std::string, std::shared_ptr<dlb_filebuf>> files;
bool active;
public:
dlb_packfile();
~dlb_packfile();
std::string getname()const;
bool is_active()const;
bool open(const std::string& filename, bool create_new=false);
bool close();
bool add_file(const std::string& filename, const std::string& internal_name);
bool extract_file(const std::string& internal_name, const std::string& outfile);
bool extract_to_memory(const std::string& internal_name, std::string& buffer);
bool get_file_ptr(const std::string& internal_name, char** ptr, dwparam* size);
bool load_file(const std::string& internal_name);
bool unload_file(const std::string& internal_name);
void unload_all();
bool file_exists(const std::string& internal_name);
bool file_delete(const std::string& internal_name);
uint32 list_files(std::vector<std::string>& list_of_files);
private:
bool verife_internal();
bool prepare_file(const std::string& filename, std::string& data);
};
}
#endif
