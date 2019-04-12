#ifndef		_GENERALDEF_H_
#define		_GENERALDEF_H_

#include	<reg52.h>

#define		KEYPORT		P1
#define		TRUE		1
#define		FALSE		0
#define		GRAPHIC		1
#define		TEXT		0

#define		LOW(X) (uchar)(X)
#define		HIGH(X) (uchar)((X)>>8)

sfr		WDTRST = 0xa6;

typedef	unsigned char	uchar;
typedef unsigned int	uint;
typedef	unsigned char	BOOL;
typedef	unsigned int	wchar;

typedef struct
{
	uchar	ucPitch;
	uchar	ucDuration;
}Note, *pNote;

typedef	struct
{
	uchar	ucYear;
	uchar	ucMonth;
	uchar	ucDay;
	uchar	ucWeekday;
	uchar	ucHour;
	uchar	ucMinute;
	uchar	ucSecond;
	uchar	ucCentiSecond;
}SystemTime, *pSystemTime;

typedef	struct
{
	uchar	ucHigh;
	uchar	ucLow;
}BcdCode, *pBcdCode;

// Used for display data in memory
typedef	struct
{
	uchar	ucHigh;
	uchar	ucLow;
}ExpandAscii, *pExpandAscii;


sbit	BuzzPort = P2^0;
sbit	BackLight = P2^4;

void	Delay100ms(uchar t);
void	Delay1ms(uchar t);
uchar	DelayWithKeyScan(uchar ucTime);
uchar	ScanKey(void);
uchar	ConvertKeycode(uchar ucCode);
uchar	Cbcd2Bin(uchar ucCbcdCode);
void	ExpandCbcd(uchar ucCbcdCode, pBcdCode pbcdOutput);
uchar	Bcd2Ascii(uchar ucBcdCode);
void	Bin2Ascii(uchar	ucBin, pExpandAscii pAscii);
uchar	Bin2Cbcd(uchar ucBin);
uchar	Deci2Ascii(uchar ucDeci);
uchar	Ascii2Bin(uchar ucAscii);
uchar	EAscii2Cbcd(pExpandAscii pEAscii);
#endif