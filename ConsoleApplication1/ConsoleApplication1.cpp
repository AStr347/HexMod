// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <regex>
#include <fstream>


#include "Convert.h"
#include "HexRow.h"
#include "Macs.h"
#include "Date.h"


using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

//split string
vector<string> split(string s, char delim = ' ') {
	stringstream ss(s);
	int i = 0;
	string tmp;
	vector<string> v;

	while (std::getline(ss, tmp, delim)) {
		v.push_back(tmp);
	}

	return v;
}

//string replacer(string s, char bef, char aft) {
//	string result(s);
//	for (auto& i : result) {
//		if (i == bef) {
//			i = aft;
//		}
//	}
//	return result;
//}
//
//void tolow(string& s) {
//	std::locale loc;
//	for (auto& i : s) {
//		i = std::tolower(i, loc);
//	}
//}



int main(int argc, char* argv[])
{
	vector<string> result = split(argv[0], '\\');
	bool dcd = false;
	string projectname = result[result.size() - 2];
	string wayToMapHex("./");
	string mapName("");
	string hexName("");

	string device(argv[1]);
	string endname = device;
	string date = Date(__DATE__).to_string();
	if (argc > 2) {
		endname = argv[2];
	}

	if (argc > 3) {
		date = argv[3];
	}

	if (projectname == "dcd1" || projectname == "danish_core_devices") {
		dcd = true;
		wayToMapHex += "dist/" + device + "/production/";
		mapName = projectname + ".production.map";
		hexName = projectname + ".production.hex";
	}
	else {
		wayToMapHex += "Release/";
		mapName = "output.map";
		hexName = device + ".hex";
	}

	cout << "ProjName :\t" << projectname << endl;
	cout << "Path to hex/map file :\t"<< wayToMapHex << endl;
	cout << "Mapfile name :\t"<< mapName << endl;
	cout << "Hexfile name :\t" << hexName << endl;
	cout << "Chosen device :\t" << device << endl;
	cout << "Date :\t" << date << endl << endl << endl;

	//open map File
	std::ifstream mapFile = ifstream(wayToMapHex + mapName);
	if (mapFile.is_open() == true) {
		//regular expression to search config_in_flash address
		std::regex config_in_flash("\\s+(\\w+)\\s+config_in_flash");

		//string for read file
		string s;

		int high = 0;
		int low = 0;

		while (getline(mapFile, s)) {
			std::smatch match;
			if (regex_match(s, match, config_in_flash)) {
				string smatch(match[1]);
				string shigh("");
				string slow(smatch.substr(6, 4));
				if (dcd) {
					shigh = "1" + smatch.substr(3, 3);
				} else {
					shigh = smatch.substr(2, 4);
				}
				
				cout << "config address = "<< shigh << '\t' << slow << endl;
				high = hex_to_int(shigh);
				low = hex_to_int(slow);
				break;
			}
		}
		mapFile.close();

		//vector for store all rows of hex file
		vector<HexRow> lines;

		//index of config_in_flash HexRow
		int indexneedHexRow = 0;
		int mac_offset = 0;

		//open hex file
		ifstream hexFile = ifstream(wayToMapHex + hexName);

		if (hexFile.is_open() == true) {
			int now_high = 0;

			while (getline(hexFile, s)) {
				HexRow h(s);

				int now_address = h.addres;

				if (h.type == 4) {
					now_high = h.addres;
				} else if (h.type == 0 && h.size == 0x10){//only full data strings
					if (now_high == high) {
						int offset = low - now_address;
						if (offset >= 0 && offset < 12) {//in string only 16 byte (12 offset + 4 for mac) 
							cout << "offset = " << offset << endl;
							mac_offset = offset;
							indexneedHexRow = lines.size();
						}
					}
				}
				lines.push_back(h);
			}
			hexFile.close();

			HexRow* needHexRow = (HexRow*) &(lines[indexneedHexRow]);


			for (int i = 0; i < MACS_COUNT; i++) {
				//modify hex row
				needHexRow->SetBytes32(macs[i], mac_offset);
				needHexRow->DataToContent();
				needHexRow->GenerateCheckSumm();

				string finalName(names_macs[i] + "_" + endname + "_" + date + ".hex");
				cout << finalName << std::endl;
				/*
				cout << "MacConf = " << *((u32*)(needHexRow->data)) << std::endl;
				cout << "TRITGEN = " << macs[i] << std::endl;
				cout << "HexRow  = " << needHexRow->to_string() << std::endl;
				*/

				//create new hex file
				ofstream newHex(wayToMapHex + finalName);
				//write all lines
				for (size_t i = 0; i < lines.size(); i++) {
					newHex << lines[i];
				}
				newHex.close();
			}
		}
		else {
			cout << "hexfile not found";
		}
	}
	else {
		cout << "mapfile not found";
	}
	return 0;
}