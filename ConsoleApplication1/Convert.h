#ifndef CONVERT_H
#define CONVERT_H
#include "arch.h"
#include "pch.h"

extern u32 string_to_int(string s);

extern string int_to_string(int i);

extern string int_to_hex(u64 dec);

extern u8 char_to_int(char c);

extern u32 hex_to_int(string hexs);

#endif // !CONVERT_H

