#ifndef DATE_H
#define DATE_H
#include "arch.h"
class Date {


	u8 str2month(string mon);

public:
	u8 day;
	u8 month;
	u8 year;

	Date(string s);

	string to_string();
};

extern ostream& operator <<(ostream& os, Date& d);
#endif