#include "Convert.h"

u32 string_to_int(const string s) {
	return stoi(s);
}

string int_to_string(const int i) {
	return  std::to_string(i);
}


string int_to_hex(const u64 dec) {
	stringstream ss("");
	ss << std::uppercase << std::hex << dec;
	string s;
	ss >> s;
	return s;
}

u8 char_to_int(const char c) {
	int result;
	if (c < 0x41)
		result =  c - 0x30;
	else if (c < 70)
		result =  c - 55;
	else
		result = c - 87;
	return result;
}

u32 hex_to_int(const string hexs) {
	u32 result = 0;
	stringstream ss(hexs);
	ss >> std::hex >> result;
	return result;
}