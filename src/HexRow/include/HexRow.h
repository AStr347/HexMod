#ifndef HEXROW_H
#define HEXROW_H
#include "arch.h"

enum hexrow_types_t {
	HEXRT_DATA = 0x00,// data record
	HEXRT_EOF = 0x01,// end of file
	HEXRT_SEG_ADDR = 0x02,// segment addr
	HEXRT_SEG_ADDR_START = 0x03,// segment addr of start
	HEXRT_LINE_ADDR = 0x04,// lineral addr
	HEXRT_LINE_ADDR_START = 0x05,// lineral addr start
};

class HexRow {
public:
	u32 size;
	u32 addres;
	hexrow_types_t type;
	u8* data;
	u8 chs;
	string content;

	//default constructor
	HexRow();
	//constructor from hex string 
	//(format :020000041d03da or
	//:10e000000055f300000400040401000101100101a7)
	HexRow(string row);

	//translates HexRow into a string
	string to_string();

	//return and set CheckSumm
	int GenerateCheckSumm();

	//for some reason it was not needed
	bool EditDataString(const u32 _address, const u8* _data, const u32 _size);
	bool EditDataString(const u32 address, const u32 data, const u32 size);

	//set values from data into content (hex string)
	void DataToContent();

	//set 32 bit with offset
	void SetBytes32(u32 data, u8 offset);

};

//adds zeros forward to the desired length
extern
string add_zeros(const string s, const u8 need_len);

//Hex string to butes(u8) array
extern
u8* DataStringToBytes(string data);

//uotput operator for HexRow
extern
ostream& operator << (ostream&, HexRow hex);

#endif