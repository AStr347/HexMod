// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <regex>
#include <fstream>

#include "Convert.h"
#include "HexRow.h"
#include "Date.h"
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

#include "Macs.h"

//split string
vector<string> split(const string s, const char delim = ' ') {
	stringstream ss(s);
	string tmp;
	vector<string> v;

	while (std::getline(ss, tmp, delim)) {
		v.push_back(tmp);
	}

	return v;
}



int main(const int argc, const char* argv[])
{
	vector<string> result = split(argv[0], '\\');
	b8 dcd = false;
	const string projectname = result[result.size() - 2];
	const string wayToMapHex("./dist/stc/");
	const string mapName("firmware.map");
	const string hexName("firmware.hex");

	const string endname(argv[1]);
	string date = Date(__DATE__).to_string();
	if (argc > 2) {
		date = argv[2];
	}

	cout << "ProjName :\t" << projectname << endl;
	cout << "Path to hex/map file :\t"<< wayToMapHex << endl;
	cout << "Mapfile name :\t"<< mapName << endl;
	cout << "Hexfile name :\t" << hexName << endl;
	cout << "Chosen device :\t" << endname << endl;
	cout << "Date :\t" << date << endl << endl << endl;

	//open map File
	std::ifstream mapFile = ifstream(wayToMapHex + mapName);
	if (false == mapFile.is_open() == true) {
		cout << "mapfile not found";
		return -__LINE__;
	}
	//regular expression to search config_in_flash address
	std::regex config_in_flash("(\\w+)\\s+_id");

	//string for read file
	string s;

	u32 hex_addr = 0;

	while (getline(mapFile, s)) {
		std::smatch match;
		if (regex_search(s, match, config_in_flash)) {
			string smatch(match[1]);
			cout << "config address = "<< smatch << endl;
			hex_addr = hex_to_int(smatch);
			break;
		}
	}
	mapFile.close();

	if(0 == hex_addr){
		cout << "config address not found" << endl;
		return -__LINE__;
	}

	//vector for store all rows of hex file
	vector<HexRow> lines;

	//open hex file
	ifstream hexFile = ifstream(wayToMapHex + hexName);

	if (false == hexFile.is_open()) {
		cout << "hexfile not found";
		return -__LINE__;
	}
	u32 now_high = 0;
	//index of config_in_flash HexRow
	u32 indexneedHexRow = 0xffffffff;
	u32 mac_offset = 0;

	while (getline(hexFile, s)) {
		HexRow h(s);

		if (HEXRT_LINE_ADDR == h.type) {
			now_high = h.addres;
		} else if (HEXRT_DATA == h.type){
			const u32 now_address = (now_high << 16) | h.addres;
			const b8 is_inside = ((now_address + 0) <= hex_addr) && (hex_addr < (now_address + h.size));
			if(is_inside){
				mac_offset = hex_addr - now_address;
				indexneedHexRow = lines.size();
				cout << "index: " << indexneedHexRow << " offset: " << mac_offset << endl;
				cout << h << endl;
			}
		}
		lines.push_back(h);
	}
	hexFile.close();
	if(0xffffffff == indexneedHexRow){
		cout << "id line not found";
		return -__LINE__;
	}

	HexRow* needHexRow = (HexRow*) &(lines[indexneedHexRow]);

	for (u8 i = 0; i < MACS_COUNT; i++) {
		const string mac_name(names_macs[i]);
		const string finalName(mac_name + "_" + endname + "_" + date + ".hex");
		cout << finalName << std::endl;
		
		//modify hex row
		needHexRow->SetBytes32(macs[i], mac_offset);
		needHexRow->GenerateCheckSumm();

		//create new hex file
		ofstream newHex(wayToMapHex + finalName);
		//write all lines
		for (size_t i = 0; i < lines.size(); i++) {
			newHex << lines[i];
		}
		newHex.close();
	}
	return 0;
}