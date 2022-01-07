
#include<string>
#include<list>
#include"dlb_types.h"
#include"dlb_log.h"
#include"dlb_sql.h"

using namespace std;

namespace dlb
{
static int sqlite_table_exists_callback(void* data, int argc, char** values, char** colnames)
{
(*(uint32*)data)=atoi(values[0]);
return 0;
}

static int sqlite_table_list_callback(void* data, int argc, char** values, char** colnames)
{
(*(list<string>*)data).push_back(values[0]);
return 0;
}

bool dlb_sqlite_table_exists(sqlite3* db, const string& table_name)
{
if(db==NULL)
{
return false;
}
string msg="select count(name) from sqlite_master where name=\'%s\' and type=\'table\';";
string sql="";
sql.resize(256);
int32 x=snprintf(&sql[0], sql.size(), msg.c_str(), table_name.c_str());
sql.resize(x);
int32 result=0;
if(dlb_sqlite_exec(db, sql.c_str(), sqlite_table_exists_callback, &result)!=SQLITE_OK)
{
return false;
}
return result>0;
}

int32 dlb_sqlite_table_list(sqlite3* db, list<string>& tables)
{
string sql="select name from sqlite_master where type=\'table\';";
tables.clear();
return dlb_sqlite_exec(db, sql, sqlite_table_list_callback, &tables);
}

int32 dlb_sqlite_table_create(sqlite3* db, const string& sql)
{
if(db==NULL)
{
return SQLITE_ERROR;
}
return dlb_sqlite_exec(db, sql, NULL, NULL);
}

/**Executa uma operação no banco de dados.
*Se algum erro ocorrer, A descrição do erro é logado, e a memória é liberada automaticamente.
*Basicamente, é a mesma coisa que chamar sqlite3_exec, mas com o log do erro imbutido.
*Retorna true em sucesso ou false em falha.
*/

int32 dlb_sqlite_exec(sqlite3* db, const std::string& sql, sqlite3_callback zcallback, void* zdata)
{
if((db==NULL)||(sql.size()==0))
{
return SQLITE_ERROR;
}
char* msgerror=NULL;
int32 res=sqlite3_exec(db, sql.c_str(), zcallback, zdata, &msgerror);
if(res!=SQLITE_OK)
{
_log("Sql error: {}\ninstruction: {}", msgerror, sql);
sqlite3_free(msgerror);
return res;
}
if(msgerror!=NULL)
{
sqlite3_free(msgerror);
}
return SQLITE_OK;
}
}
