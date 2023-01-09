#ifndef CONVERT_H
#define CONVERT_H
#include "arch.h"

extern u32 string_to_int(const string s);

extern string int_to_string(const int i);

extern string int_to_hex(const u64 dec);

extern u8 char_to_int(const char c);

extern u32 hex_to_int(const string hexs);

#endif // !CONVERT_H

