
/**
*Algumas funções para lidar com o sqlite.
*Não que ele não seja fácil de usar, mas é para evitar repetição de código desnecessário.
*Escrito por Róger (rogerlasch@gmail.com) em maio de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_SQL_H
#define DLB_SQL_H

#include<list>
#include<sqlite3.h>
#include"dlb_types.h"

namespace dlb
{
bool dlb_sqlite_table_exists(sqlite3* db, const std::string& table_name);
int32 dlb_sqlite_table_list(sqlite3* db, std::list<std::string>& tables);
int32 dlb_sqlite_table_create(sqlite3* db, const std::string& sql);
int32 dlb_sqlite_exec(sqlite3* db, const std::string& sql, sqlite3_callback zcallback, void* zdata);
}
#endif
