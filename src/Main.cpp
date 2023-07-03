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
	bool dcd = false;
	const string projectname = result[result.size() - 2];
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

	const bool is_debug = ("HexMod" == projectname);
	const bool is_dcd1 = ("dcd1" == projectname);
	const bool is_dcd_old = ("danish_core_devices" == projectname);
	if(is_debug){
		dcd = true;
		wayToMapHex += "dist/" + device + "/production/";
		mapName = "dcd1.production.map";
		hexName = "dcd1.production.hex";
	} else if (is_dcd1 || is_dcd_old) {
		dcd = true;
		wayToMapHex += "dist/" + device + "/production/";
		mapName = projectname + ".production.map";
		hexName = projectname + ".production.hex";
	} else {
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
			//0x000000009d03e000
			if (regex_match(s, match, config_in_flash)) {
				string smatch(match[1]);
				const int addrlen = smatch.length();
				string shigh("");
				const string slow(smatch.substr(addrlen - 4, 4));
				if (dcd) {
					shigh = "1" + smatch.substr(addrlen - 7, 3);
				} else {
					shigh = smatch.substr(addrlen - 8, 4);
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

				const int now_address = h.addres;

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