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

//constructor from hex string 
// format 
// :020000041d03da 
// or
// :10e000000055f300000400040401000101100101a7
HexRow::HexRow(string row) {
	size = hex_to_int(row.substr(1, 2));//ok
	type = hex_to_int(row.substr(7, 2));//ok
	data = nullptr;

	content = "";

	if (type != 4)
		addres = hex_to_int(row.substr(3, 4));
	else
		addres = hex_to_int(row.substr(9, 4));

	content = row.substr(9, row.length() - 2 - 9);
	if (type != 1) {
		chs = hex_to_int(row.substr(row.length() - 2, 2));
		data = DataStringToBytes(content);
	} else {
		chs = hex_to_int(row.substr(row.length() - 2, 2));
	}
}

//adds zeros forward to the desired length
string HexRow::add_zeros(string s, u8 need_len) {
	int len = s.length();
	string result = s;
	if (len < need_len)
		result = string(need_len - len, '0') + s;
	return result;
}


//translates HexRow into a string
string HexRow::to_string() {
	stringstream ss;
	ss << *this;
	string s = "";
	ss >> s;
	return s;
}


//Hex string to butes(u8) array
u8* HexRow::DataStringToBytes(string data)
{
	int size = data.length() / 2;
	u8* result = new uint8_t[size];



	for (int i = 0; i < size; i++)
	{
		result[i] = hex_to_int(data.substr(2 * i, 2));
	}
	return result;
}

//return and set CheckSumm
int HexRow::GenerateCheckSumm()
{
	string temp = (this->to_string()).substr(1);

	u32 size = temp.length() / 2 - 1;
	u8* tempdata = DataStringToBytes(temp.substr(0, size * 2));

	u8 chm = 0;

	for (u8 i = 0; i < size; i++)
	{
		chm += tempdata[i];
	}
	chm = 256 - chm;
	chs = chm;
	return chm;
}

//not used
bool HexRow::EditDataString(u32 _address, u8* _data, u32 _size)
{
	bool result = false;
	u32 loc_size = _size;

	if (addres == 4096)
	{

	}

	if ((addres <= _address) && (addres + size > _address))
	{
		int index = _address - addres;

		if (_address + loc_size > addres + size)
		{

		}

		for (u8 i = 0; i < loc_size; i++)
		{
			data[i + index] = _data[i];
		}
		result = true;
	}

	return result;
}

//not used
bool HexRow::EditDataString(u32 address, u32 data, u32 size)
{
	u64 c1 = ((u64)(data) & (0xFF));
	u64 c0 = (((u64)(data) >> 8 * 1) & (0xFF));
	u64 c2 = (((u64)(data) >> 8 * 2) & (0xFF));
	u64 f0 = ((1 << 7) & (0xFF));
	string temp = int_to_hex((f0 << (8) | (c2 << (8 * 7) | c1 << (5 * 8) | c0 << (3 * 8)) & (0xFF00FF00FF00FF00UL)));

	while (temp.length() < size * 2)
	{
		temp = temp.insert(0, "0");
	}
	if (temp.length() % 2 != 0)
	{
		temp = temp.insert(0, "0");
	}


	u8* tempData = DataStringToBytes(temp);

	bool result = EditDataString(address, tempData, temp.length() / 2);
	return result;
}

//set values from data into content (hex string)
void HexRow::DataToContent() {
	string s = "";
	for (u8 i = 0; i < content.length() / 2; i++) {
		s += add_zeros(int_to_hex(data[i]), 2);
	}
	content = s;
}


void HexRow::SetBytes32(u32 data, u8 offset) {
	if ((offset % 4 == 0) && (offset <= 12)) {
		*((u32*)(this->data + offset)) = data;
	}
	std::cout << *this << std::endl;
}

//uotput operator for HexRow
ostream& operator << (ostream& os, HexRow hex) {

	os << ":" << hex.add_zeros(int_to_hex(hex.size), 2);

	if (hex.type == 4) {
		os << "0000";
		os << hex.add_zeros(int_to_hex(hex.type), 2);
		os << hex.add_zeros(int_to_hex(hex.addres), 4);
		os << hex.add_zeros(int_to_hex(hex.chs), 2);
	}
	else {
		os << hex.add_zeros(int_to_hex(hex.addres), 4);;
		os << hex.add_zeros(int_to_hex(hex.type), 2);
		os << hex.content;
		os << hex.add_zeros(int_to_hex(hex.chs), 2);
	}
	os << std::endl;
	return os;
}