#include "HexRow.h"
#include "Convert.h"
//default constructor
HexRow::HexRow() {
	size = 0;
	type = 0;
	content = "";
	addres = 0;
	chs = 0;
	data = nullptr;
}

// constructor from hex string 
// format 
// :020000041d03da 
// or
// :10e000000055f300000400040401000101100101a7
HexRow::HexRow(const string row) {
	size = hex_to_int(row.substr(1, 2));//ok
	type = hex_to_int(row.substr(7, 2));//ok
	data = nullptr;

	if (type != 4) {
		addres = hex_to_int(row.substr(3, 4));
	} else {
		addres = hex_to_int(row.substr(9, 4));
	}

	content = row.substr(9, row.length() - 2 - 9);
	if (type != 1) {
		chs = hex_to_int(row.substr(row.length() - 2, 2));
		data = DataStringToBytes(content);
	} else {
		chs = hex_to_int(row.substr(row.length() - 2, 2));
	}
}

//translates HexRow into a string
string HexRow::to_string() {
	stringstream ss;
	ss << *this;
	string s = "";
	ss >> s;
	return s;
}

//return and set CheckSumm
int HexRow::GenerateCheckSumm()
{
	const string self = this->to_string();
	const string temp = self.substr(1);

	const u32 size = temp.length() / 2 - 1;
	const string data = temp.substr(0, size * 2);
	const u8* tempdata = DataStringToBytes(data);

	u8 chm = 0;

	for (u8 i = 0; i < size; i++) {
		chm += tempdata[i];
	}
	chm = 256 - chm;
	this->chs = chm;
	return chm;
}

//not used
bool HexRow::EditDataString(const u32 address, const u8* src, const u32 size)
{
	bool result = false;
	const u32 loc_size = size;
	const u32 addr = this->addres;

	if ((addr <= address) && (addr + size > address)) {
		const u8 offset = address - addr;
		for (u8 i = 0; i < loc_size; i++) {
			this->data[i + offset] = src[i];
		}
		result = true;
	}

	return result;
}

//not used
bool HexRow::EditDataString(const u32 address, const u32 data, const u32 size)
{
	const u64 c1 = ((u64)(data) & (0xFF));
	const u64 c0 = (((u64)(data) >> 8 * 1) & (0xFF));
	const u64 c2 = (((u64)(data) >> 8 * 2) & (0xFF));
	const u64 f0 = ((1 << 7) & (0xFF));
	string temp = int_to_hex((f0 << (8) | (c2 << (8 * 7) | c1 << (5 * 8) | c0 << (3 * 8)) & (0xFF00FF00FF00FF00UL)));

	while (temp.length() < size * 2) {
		temp = temp.insert(0, "0");
	}

	if (temp.length() % 2 != 0) {
		temp = temp.insert(0, "0");
	}

	u8* tempData = DataStringToBytes(temp);
	const bool result = this->EditDataString(address, tempData, temp.length() / 2);
	return result;
}

//set values from data into content (hex string)
void HexRow::DataToContent(void) {
	string s = "";
	const size_t len = this->content.length() / 2;
	for (u8 i = 0; i < len; i++) {
		const u8 d = data[i];
		const string hex = int_to_hex(d);
		s += add_zeros(hex, 2);
	}
	this->content = s;
}


void HexRow::SetBytes32(const u32 data, const u8 offset) {
	if ((offset % 4 == 0) && (offset <= 12)) {
		*((u32*)(this->data + offset)) = data;
	}
	this->DataToContent();
	this->GenerateCheckSumm();
	std::cout << *this << std::endl;
}

//adds zeros forward to the desired length
string add_zeros(const string s, const u8 need_len) {
	const int len = s.length();
	string result = s;
	if (len < need_len) {
		result = string(need_len - len, '0') + s;
	}
	return result;
}

//Hex string to butes(u8) array
u8* DataStringToBytes(const string data)
{
	const int size = data.length() / 2;
	u8* result = new uint8_t[size];

	for (int i = 0; i < size; i++)
	{
		const int pos = 2 * i;
		const string sub = data.substr(pos, 2);
		result[i] = hex_to_int(sub);
	}
	return result;
}

//uotput operator for HexRow
ostream& operator << (ostream& os, const HexRow hex) {

	const string size = add_zeros(int_to_hex(hex.size), 2);
	const string type = add_zeros(int_to_hex(hex.type), 2);
	const string addres = add_zeros(int_to_hex(hex.addres), 4);
	const string chs = add_zeros(int_to_hex(hex.chs), 2);

	os << ":" << size;

	if (hex.type == 4) {
		os << "0000" << type << addres << chs;
	} else {
		os << addres << type << hex.content << chs;
	}
	os << std::endl;
	return os;
}