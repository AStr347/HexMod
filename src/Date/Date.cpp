#include "Date.h"
#include "Convert.h"
#include <map>

u8 Date::str2month(string mon) {
	static std::map<string, u8> translator = { {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr",4}, {"May",5}, {"Jun",6}, {"Jul",7}, {"Aug",8}, {"Sep",9}, {"Okt",10}, {"Nov",11}, {"Dec",12}, };
	return translator[mon];
}


Date::Date(string s) {
	u8 len = (u8)s.length();
	year = string_to_int(s.substr(len - 2, 2));
	day = string_to_int(s.substr(len - 7, 2));

	string mon(s.substr(0, 3));
	month = str2month(mon);
}

string Date::to_string() {
	stringstream ss;
	ss << *this;
	string s = "";
	ss >> s;
	return s;
}

ostream& operator <<(ostream& os, Date& d) {
	string zero("");
	if (d.day < 10) {
		zero = "0";
	}
	os << zero << (u32)d.day;
	
	zero = "";
	if (d.month < 10) {
		zero = "0";
	}
	os << zero << (u32)d.month;
	
	zero = "";
	if (d.year < 10) {
		zero = "0";
	}
	os << zero << (u32)d.year;
	return os;
}