


#ifndef DLB_STRING_H
#define DLB_STRING_H

#include"dlb_string_manager.h"
#include"dlb_wstring_manager.h"

namespace dlb
{
#define _f8(str) dlb_string_utf8_encode(str)
#define _f16(str) dlb_string_utf8_decode(str)

}

#endif
