#include	"generaldef.h"
#include	<intrins.h>

//								0	  1		2	  3		4	  5		6	  7		8	  9
uchar	code	KeyTable[] = {0xd7, 0xeb, 0xdb, 0xbb, 0xed, 0xdd, 0xbd, 0xee, 0xde, 0xbe,
							  0x77, 0x7b, 0x7d, 0x7e, 0xe7, 0xb7};
//							  +/a	-/b		x/c	  //d	c/e	  =/f		

// Dealy t*100ms
void	Delay100ms(uchar t)
{
	uchar	i, j;
	while(t)
	{
		for(i = 0; i < 138; i++)
			for(j = 0; j < 240; j++)
				;
		t--;
	}
}

// Delay t ms
void	Delay1ms(uchar t)
{
	uchar	i, j;
	while(t)
	{
		for(i = 0; i < 5; i++)
			for(j = 0; j < 65; j++)
				;
		t--;
	}
}

uchar	DelayWithKeyScan(uchar ucTime)
{
	uchar	ucKeyCode = 0;
	uchar	ucKeyPort;
	uchar	ucScanByte = 0xfe;
	while(ucTime)
	{
		KEYPORT = ucScanByte;
		ucKeyPort = KEYPORT;
		if((ucKeyPort & 0xf0) != 0xf0)		// Key Pressed
		{
			Delay1ms(5);					// Debouncing
			ucKeyPort = KEYPORT;
			if((ucKeyPort & 0xf0) != 0xf0)
			{
				ucKeyCode = ucKeyPort;
				break;
			}
			else
			{
				if(ucScanByte == 0xf7)
					ucScanByte = 0xfe;
				else
					ucScanByte = _crol_(ucScanByte, 1);
			}
		}
		else
		{
			if(ucScanByte == 0xf7)
				ucScanByte = 0xfe;
			else
				ucScanByte = _crol_(ucScanByte, 1);
		}
		Delay1ms(1);
		ucTime--;
	}
	return	ucKeyCode;
}

uchar	ScanKey(void)
{
	uchar	ucKeyCode;
	uchar	ucKeyPort;
	uchar	ucScanByte = 0xfe;
	while(1)
	{
		KEYPORT = ucScanByte;
		ucKeyPort = KEYPORT;
		if((ucKeyPort & 0xf0) != 0xf0)		// Key Pressed
		{
			Delay1ms(5);					// Debouncing
			ucKeyPort = KEYPORT;
			if((ucKeyPort & 0xf0) != 0xf0)
			{
				ucKeyCode = ucKeyPort;
				BackLight = 0;				// Turn on backlight of LCD
				BackLight = 1;					// Reset bakcklight	
				while(1)
				{
					ucKeyPort = KEYPORT;
					if((ucKeyPort & 0xf0) == 0xf0)		// Key Released
					{
						Delay1ms(5);					// Debouncing

						ucKeyPort = KEYPORT;
						if((ucKeyPort & 0xf0) == 0xf0)
							break;
					}
				}
				break;
			}
			else
			{
				if(ucScanByte == 0xf7)
					ucScanByte = 0xfe;
				else
					ucScanByte = _crol_(ucScanByte, 1);
				continue;
			}
		}
		else
		{
			if(ucScanByte == 0xf7)
				ucScanByte = 0xfe;
			else
				ucScanByte = _crol_(ucScanByte, 1);
			continue;
		}
	}
	return	ucKeyCode;
}
// Convert original key code to standard key code
uchar	ConvertKeycode(uchar ucCode)
{
	uchar	i;
	for(i = 0; i < 16; i++)
	{
		if(KeyTable[i] == ucCode)
			return i;
	}
	return 0xff;
}

// Convert compressed BCD code to bin code
uchar	Cbcd2Bin(uchar ucCbcdCode)
{
	uchar	ucBin;
	uchar	ucLowNibble;
	uchar	ucHighNibble;
	ucLowNibble = ucCbcdCode & 0x0f;
	ucHighNibble = (ucCbcdCode >> 4) & 0x0f;
	ucBin = ucLowNibble + ucHighNibble * 10;
	return ucBin;
}

// Expand compressed BCD code to two 8-bit BCD code
void	ExpandCbcd(uchar ucCbcdCode, pBcdCode pbcdOutput)
{
	pbcdOutput->ucLow = ucCbcdCode & 0x0f;
	pbcdOutput->ucHigh = (ucCbcdCode >> 4) & 0x0f;
}

// Convert BCD code to Ascii code
uchar	Bcd2Ascii(uchar ucBcdCode)
{
	uchar	uiAsciiCode;
	uiAsciiCode = ucBcdCode | 0x30;
	return uiAsciiCode;
}

// Convert binary data to displayable ASCII
void	Bin2Ascii(uchar	ucBin, pExpandAscii pAscii)
{
	pAscii->ucLow = ucBin & 0x0f;
	pAscii->ucHigh = (ucBin >> 4) & 0x0f;
	if(pAscii->ucLow < 10)
		pAscii->ucLow = pAscii->ucLow | 0x30;
	else
		pAscii->ucLow = (pAscii->ucLow - 9) | 0x60;
	if(pAscii->ucHigh < 10)
		pAscii->ucHigh = pAscii->ucHigh | 0x30;
	else
		pAscii->ucHigh = (pAscii->ucHigh - 9) | 0x60;
}

uchar	Deci2Ascii(uchar ucDeci)
{
	uchar	ucAsc;
	if(ucDeci >= 0 && ucDeci <= 9)
		ucAsc = ucDeci + 0x30;
	else if(ucDeci > 9 && ucDeci < 16)
		ucAsc = ucDeci - 9 + 0x60;
	else
		ucAsc = 0xff;
	return ucAsc;
}

uchar	Bin2Cbcd(uchar ucBin)
{
	uchar	ucCbcd;
	if(ucBin < 10)
		ucCbcd = ucBin;
	else
		ucCbcd = ((ucBin / 10) << 4) | (ucBin % 10);
	return	ucCbcd;
}

uchar	Ascii2Bin(uchar ucAscii)
{
	uchar	ucBin;
	if(ucAscii >= 0x30 && ucAscii <= 0x39)
		ucBin = ucAscii - 0x30;
	else if(ucAscii >= 41 && ucAscii <= 46)
		ucBin = ucAscii - 0x41 + 10;
	return ucBin;
}

uchar	EAscii2Cbcd(pExpandAscii pEAscii)
{
	uchar	ucCbcd;
	pEAscii->ucLow = Ascii2Bin(pEAscii->ucLow);
	pEAscii->ucHigh = Ascii2Bin(pEAscii->ucHigh);
	ucCbcd = 0x0f & pEAscii->ucLow;
	ucCbcd = ucCbcd | (pEAscii->ucHigh << 4);
	return ucCbcd;
}