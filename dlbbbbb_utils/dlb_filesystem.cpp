
#pragma warning(disable: 4101)
#include<fstream>
#include"dlb_debug.h"
#include"dlb_types.h"
#include"dlb_filesystem.h"

using namespace std;

namespace dlb
{
bool dlb_file_exists(const filesystem::path& ph)
{
bool success=true;
if(dlb_is_directory(ph))
{
return false;
}
try
{
success=filesystem::exists(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

bool dlb_file_delete(const filesystem::path& ph)
{
bool success=true;
if(dlb_is_directory(ph))
{
return false;
}
try
{
success=filesystem::remove(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

bool dlb_file_copy(const filesystem::path& source, const filesystem::path& destination, bool replace_on_exists)
{
bool success=true;
if(replace_on_exists)
{
//Poem o código em inspeção para capturar qualquer exceção que o sistema operacional lance.
//Se uma exceção não for tratada o comportamento é indefinido.
try
{
success=filesystem::copy_file(source, destination,
filesystem::copy_options::overwrite_existing);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
}
else
{
//Poem o código em inspeção para capturar qualquer exceção que o sistema operacional lance.
//Se uma exceção não for tratada o comportamento é indefinido.
try
{
success=filesystem::copy_file(source, destination);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
}
return success;
}

bool dlb_is_file(const filesystem::path& ph)
{
bool success=true;
try
{
success=filesystem::is_regular_file(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

bool dlb_directory_exists(const filesystem::path& ph)
{
if(dlb_is_file(ph))
{
return false;
}
bool success=true;
try
{
success=filesystem::exists(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

bool dlb_directory_create(const filesystem::path& ph)
{
bool success=true;
try
{
success=filesystem::create_directories(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

bool dlb_directory_delete(const filesystem::path& ph)
{
if(dlb_is_file(ph))
{
return false;
}
bool success=true;
try
{
success=filesystem::remove_all(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

bool dlb_is_directory(const filesystem::path& ph)
{
bool success=true;
try
{
success=filesystem::is_directory(ph);
} catch(const exception& e) {
success=false;
_dl_except(__FUNCTION__, e.what());
}
return success;
}

uint64 dlb_file_write(const string& filename, const string& data, bool overwrite)
{
if((dlb_file_exists(filename))&&(overwrite==false)||(data.size()==0))
{
return 0;
}
ofstream ofn(filename, ios::binary);
if(!ofn.is_open())
{
return 0;
}
ofn.write(&data[0], data.size());
uint64 size=ofn.tellp();
ofn.close();
return size;
}

uint64 dlb_file_read(const string& filename, string& data)
{
if((filename.size()==0)||(!dlb_file_exists(filename)))
{
return 0;
}
ifstream ifs(filename, ios::binary);
if(!ifs.is_open())
{
return 0;
}
ifs.seekg(0, ifs.end);
uint64 size=ifs.tellg();
ifs.seekg(0, ifs.beg);
data.resize(static_cast<size_t>(size));
ifs.read(&data[0], size);
data.resize(static_cast<size_t>(ifs.gcount()));
size=ifs.gcount();
ifs.close();
return size;
}
}
