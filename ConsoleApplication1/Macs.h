#ifndef MACS_H
#define MACS_H
#include "arch.h"
#include "pch.h"
//defines for puls mac
// 8 decimal numbers
#define DECDIV(a, n) (a%n)
#define CORRECTLEFT(a) ((a==1)?(3):(0))
#define CORRECT(a) ((a==2)?(1):(CORRECTLEFT(a)))

#define CORRECT_PACK_0(a) ((CORRECT(DECDIV(a,10UL))<<(6)))
#define CORRECT_PACK_1(a) ((CORRECT(DECDIV(a,100UL)/10L))<<(4))
#define CORRECT_PACK_2(a) ((CORRECT(DECDIV(a,1000UL)/100L))<<(2))
#define CORRECT_PACK_3(a) ((CORRECT(DECDIV(a,10000UL)/1000L))<<(0))

#define TRITGEN4(a) (CORRECT_PACK_3(a)|CORRECT_PACK_2(a)|CORRECT_PACK_1(a)|CORRECT_PACK_0(a))
#define TRITGEN(b,a) (TRITGEN4(b)|(TRITGEN4(a)<<8))

#define ADRESS_PREFFIX  2222L

#define MACS_COUNT 50

//work
static u32 macs[] = {
		(TRITGEN(ADRESS_PREFFIX,1010) << 8), //0
		(TRITGEN(ADRESS_PREFFIX,1011) << 8), //1
		(TRITGEN(ADRESS_PREFFIX,1012) << 8), //2
		(TRITGEN(ADRESS_PREFFIX,1020) << 8), //3
		(TRITGEN(ADRESS_PREFFIX,1021) << 8), //4
		(TRITGEN(ADRESS_PREFFIX,1022) << 8), //5
		(TRITGEN(ADRESS_PREFFIX,1100) << 8), //6
		(TRITGEN(ADRESS_PREFFIX,1101) << 8), //7
		(TRITGEN(ADRESS_PREFFIX,1102) << 8), //8
		(TRITGEN(ADRESS_PREFFIX,1110) << 8), //9
		(TRITGEN(ADRESS_PREFFIX,1111) << 8), //10
		(TRITGEN(ADRESS_PREFFIX,1112) << 8), //11
		(TRITGEN(ADRESS_PREFFIX,1120) << 8), //12
		(TRITGEN(ADRESS_PREFFIX,1121) << 8), //13
		(TRITGEN(ADRESS_PREFFIX,1122) << 8), //14
		(TRITGEN(ADRESS_PREFFIX,1200) << 8), //15
		(TRITGEN(ADRESS_PREFFIX,1201) << 8), //16
		(TRITGEN(ADRESS_PREFFIX,1202) << 8), //17
		(TRITGEN(ADRESS_PREFFIX,1210) << 8), //18
		(TRITGEN(ADRESS_PREFFIX,1211) << 8), //19
		(TRITGEN(ADRESS_PREFFIX,1212) << 8), //20
		(TRITGEN(ADRESS_PREFFIX,1220) << 8), //21
		(TRITGEN(ADRESS_PREFFIX,1221) << 8), //22
		(TRITGEN(ADRESS_PREFFIX,1222) << 8), //23
		(TRITGEN(ADRESS_PREFFIX,2000) << 8), //24
		(TRITGEN(ADRESS_PREFFIX,2001) << 8), //25
		(TRITGEN(ADRESS_PREFFIX,2002) << 8), //26
		(TRITGEN(ADRESS_PREFFIX,2010) << 8), //27
		(TRITGEN(ADRESS_PREFFIX,2011) << 8), //28
		(TRITGEN(ADRESS_PREFFIX,2012) << 8), //29
		(TRITGEN(ADRESS_PREFFIX,2020) << 8), //30
		(TRITGEN(ADRESS_PREFFIX,2021) << 8), //31
		(TRITGEN(ADRESS_PREFFIX,2022) << 8), //32
		(TRITGEN(ADRESS_PREFFIX,2100) << 8), //33
		(TRITGEN(ADRESS_PREFFIX,2101) << 8), //34
		(TRITGEN(ADRESS_PREFFIX,2102) << 8), //35
		(TRITGEN(ADRESS_PREFFIX,2110) << 8), //36
		(TRITGEN(ADRESS_PREFFIX,2111) << 8), //37
		(TRITGEN(ADRESS_PREFFIX,2112) << 8), //38
		(TRITGEN(ADRESS_PREFFIX,2120) << 8), //40
		(TRITGEN(ADRESS_PREFFIX,2121) << 8), //41
		(TRITGEN(ADRESS_PREFFIX,2122) << 8), //42
		(TRITGEN(ADRESS_PREFFIX,2200) << 8), //43
		(TRITGEN(ADRESS_PREFFIX,2201) << 8), //44
		(TRITGEN(ADRESS_PREFFIX,2202) << 8), //45
		(TRITGEN(ADRESS_PREFFIX,2210) << 8), //46
		(TRITGEN(ADRESS_PREFFIX,2211) << 8), //47
		(TRITGEN(ADRESS_PREFFIX,2212) << 8), //48
		(TRITGEN(ADRESS_PREFFIX,2220) << 8), //49
		(TRITGEN(ADRESS_PREFFIX,2221) << 8), //50
};
//#define DETONATOR

static string names_macs[] = {
#ifdef DETONATOR
		/* detonator names */
		"1010", //0
		"1011", //1
		"1012", //2
		"1020", //3
		"1021", //4
		"1022", //5
		"1100", //6
		"1101", //7
		"1102", //8
		"1110", //9
		"1111", //10
		"1112", //11
		"1120", //12
		"1121", //13
		"1122", //14
		"1200", //15
		"1201", //16
		"1202", //17
		"1210", //18
		"1211", //19
		"1212", //20
		"1220", //21
		"1221", //22
		"1222", //23
		"2000", //24
		"2001", //25
		"2002", //26
		"2010", //27
		"2011", //28
		"2012", //29
		"2020", //30
		"2021", //31
		"2022", //32
		"2100", //33
		"2101", //34
		"2102", //35
		"2110", //36
		"2111", //37
		"2112", //38
		"2120", //40
		"2121", //41
		"2122", //42
		"2200", //43
		"2201", //44
		"2202", //45
		"2210", //46
		"2211", //47
		"2212", //48
		"2220", //49
		"2221", //50
		
#else

		/* uprt names */
		"6510", //0
		"6511", //1
		"6512", //2
		"6513", //3
		"6514", //4
		"6515", //5
		"6516", //6
		"6517", //7
		"6518", //8
		"6519", //9
		"6520", //10
		"6521", //11
		"6522", //12
		"6523", //13
		"6524", //14
		"6525", //15
		"6526", //16
		"6527", //17
		"6528", //18
		"6529", //19
		"6530", //20
		"6531", //21
		"6532", //22
		"6533", //23
		"6534", //24
		"6535", //25
		"6536", //26
		"6537", //27
		"6538", //28
		"6539", //29
		"6540", //30
		"6541", //31
		"6542", //32
		"6543", //33
		"6544", //34
		"6545", //35
		"6546", //36
		"6547", //37
		"6548", //38
		"6549", //40
		"6550", //41
		"6551", //42
		"6552", //43
		"6553", //44
		"6554", //45
		"6555", //46
		"6556", //47
		"6557", //48
		"6558", //49
		"6559", //50
#endif // DETONATOR

};

#endif // !MACS_H

