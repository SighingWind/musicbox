#include	<reg52.h>
#include	<intrins.h>
#include	"generaldef.h"
#include	"musicnotes.h"
#include	"lcddefine.h"

uchar	Duration;
uchar	Singing;
uchar	Timer1High;
uchar	Timer1Low;
uchar	Tune;
uchar	CounterT0;
uchar	KeyCode;
uchar	ScanByte = 0xfe;
uchar	KeyPressed = 0;
Note	StopNote[2] = {{0, 0}, {0, 0}};

void	InitBeep(float fFrequency);
void	SetBeepFreq(float fFrequency);
void	SingOneNote(pNote pCurNote);
void	SingASong(pNote pSong);
void	T1ISR(void);

void	main()
{
	uchar	ucKeyCode;
	uchar	ucAsc;
	
	Delay100ms(5);

	// initial Timer
	TMOD = 0x11;
	
	// Set Timer0 high priority
	IP = 0x02;
	
	// enable interrupt
	IE = 0x8a;
	
	InitBeep(440.0);
	LcdInit();
	
	CounterT0 = 0;
	TH0 = 0x3c;
	TL0 = 0xb0;
	TR0 = 1;

	while(1)
	{
//		ucKeyCode = ScanKey();
		ucKeyCode = ConvertKeycode(KeyCode);
		ucAsc = Deci2Ascii(ucKeyCode);
		if(ucKeyCode < 10)
		{
			LcdClearDisplay();
			LcdPutChar(3, 1, ucAsc);
			LcdWriteChar(':');
			LcdWriteChar(' ');
			LcdPutStringC(3, 4, SongName[ucKeyCode]);
			switch(ucKeyCode)
			{
				case 0:
					SingASong(HappyBirth);
					break;
				case 1:
					SingASong(LittleStar);
					break;
				case 2:
					SingASong(Badinerie);
					break;
				case 3:
					SingASong(Liangzhu);
					break;
				case 4:
					SingASong(Bethoven9);
					break;
				case 5:
					SingASong(SeasonsRun);
					break;
				case 6:
					SingASong(CradleSong);
					break;
				case 7:
					SingASong(Katyusha);
					break;
				case 8:
					SingASong(Onion);
					break;
				case 9:
					SingASong(Anonymous);
					break;
			}
		}
	}
}

void	SingASong(pNote pSong)
{
	uint	uiBeat;
	float	fBeat;
	// Set velosity according to the first element of the song
	fBeat = (37500.0 / pSong->ucDuration )*100.0;
	uiBeat = (uint)fBeat;
	uiBeat = ~uiBeat + 1;
	Timer1High = uiBeat >> 8 ;
	Timer1Low = uiBeat;
	Tune = pSong->ucPitch;
	pSong++;
	Singing = 0;
	
	TH1 = Timer1High;
	TL1 = Timer1Low;
	
	while(pSong->ucDuration)
	{
		if(!Singing)
		{
			if(KeyPressed)
			{
				pSong = &StopNote;
				KeyPressed = 0;			// reset
			}
			SingOneNote(pSong);
			pSong++;
		}
	}
}
	

void	SingOneNote(pNote pCurNote)
{
	Singing = 1;
	Duration = pCurNote->ucDuration;
	if(pCurNote->ucPitch == 0)
	{
		TR2 = 0;
		TR1 = 1;
	}
	else
	{
		SetBeepFreq(Freq[pCurNote->ucPitch + Tune]);
		TR2 = 1;
		TR1 = 1;
	}
}
	
void	T1ISR(void)	interrupt 3 using 1
{
	TH1 = Timer1High;
	TL1 = Timer1Low;
	if(Duration !=0)
	{
		Duration--;
	}
	else
	{
		TR2 = 0;
		TR1 = 0;
		Singing = 0;
	}
}

void	T0ISR(void)	interrupt 1 using 2
{
	uchar	ucKeyPort;
	uchar	ucScanByte = 0xfe;
	ET0 = 0;		// turn off T0 intterrupt
	TH0 = 0x3c;
	TL0 = 0xb0;
	CounterT0++;
	// Check Key
	P3 = ScanByte;
	ucKeyPort = P3;
	if((ucKeyPort && 0xf0) != 0xf0)
	{
		Delay1ms(10);					// Debouncing
		ucKeyPort = P3;
		if((ucKeyPort & 0xf0) != 0xf0)
		{
			KeyPressed = 1;
			KeyCode = ucKeyPort;
		}
		while(1)
		{
			ucKeyPort = P3;
			if((ucKeyPort & 0xf0) == 0xf0)		// Key Released
			{
				Delay1ms(10);					// Debouncing
				ucKeyPort = P3;
				if((ucKeyPort & 0xf0) == 0xf0)
				break;
			}
		}
	}
	if(ScanByte == 0xf7)
		ScanByte = 0xfe;
	else
		ScanByte = _crol_(ScanByte, 1);
	if(CounterT0 == 20)
	{
		CounterT0 = 0;
		//Update Time and Temperature
	}
	ET0 = 1;		// Turn on T0 Interrupt
}

void	InitBeep(float fFrequency)
{
	uint	uiLoadValue;
	uiLoadValue = ~(uint)(3e6 / fFrequency) + 1;
	C_T2 = 0;
	T2MOD = 0x02;
	RCAP2L = uiLoadValue;
	RCAP2H = uiLoadValue >> 8;
}

void	SetBeepFreq(float fFrequency)
{
	uint	uiLoadValue;
	uiLoadValue = ~(uint)(3e6 / fFrequency) + 1;
	RCAP2L = uiLoadValue;
	RCAP2H = uiLoadValue >> 8;
}