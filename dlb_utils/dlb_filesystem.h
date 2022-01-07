

/**
*Funções de gerenciamento de arquivos e pastas.
*Nenhuma das funções lançam exceções. Caso queira capturar exceções, use a biblioteca nativa std::filesystem.
*Escrito por Róger (rogerlasch@gmail.com) em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_FILESYSTEM_H
#define DLB_FILESYSTEM_H

#include<filesystem>
namespace dlb
{

bool dlb_file_exists(const std::filesystem::path& ph);
bool dlb_file_delete(const std::filesystem::path& ph);
bool dlb_file_copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool replace_on_exists);
bool dlb_is_file(const std::filesystem::path& ph);
bool dlb_directory_exists(const std::filesystem::path& ph);
bool dlb_directory_create(const std::filesystem::path& ph);
bool dlb_directory_delete(const std::filesystem::path& ph);
bool dlb_is_directory(const std::filesystem::path& ph);

uint64 dlb_file_write(const std::string& filename, const std::string& data, bool overwrite=true);
uint64 dlb_file_read(const std::string& filename, std::string& data);

}
#endif
