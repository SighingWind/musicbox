#include	<reg52.h>
#include	"lcddefine.h"

extern	void	LcdWriteCmd(uchar ucCommand);
extern	uchar	LcdGetAddr(void);
extern	void	LcdWriteChar(uchar ucChar);
extern	uchar	LcdReadChar(void);
extern	void	LcdSetPos(uchar ucRow, uchar ucCol);

void	LcdInit()
{
	LcdWriteCmd(0x01);
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x06);
}

void	LcdSetCursor(uchar ucOnOff, uchar ucBlink)
{
	uchar	ucControl = 0x0c;
	if(ucOnOff)
		ucControl |= 0x02;
	if(ucBlink)
		ucControl |= 0x01;
	LcdWriteCmd(ucControl);
}

void	LcdClearDisplay()
{
	LcdWriteCmd(0x01);
}

// Display a character at a specified positon
uchar	LcdPutChar(uchar ucRow, uchar ucCol, uchar ucChar)
{
	if(LcdSetPosition(ucRow, ucCol))
	{
		LcdWriteChar(ucChar);
		return 1;
	}
	else
		return 0;
}

// Display a string from code memory at a specified position
uchar	LcdPutStringC(uchar ucRow, uchar ucCol, uchar code * pchString)
{
	if(LcdSetPosition(ucRow, ucCol))
	{
		while(*pchString)
		{
			LcdWriteChar(*pchString);
			pchString++;
		}
		return 1;
	}
	else
		return 0;
}

// Display a string from internal memory at a specified position
uchar	LcdPutStringD(uchar ucRow, uchar ucCol, uchar data *pchString)
{
	if(LcdSetPosition(ucRow, ucCol))
	{
		while(*pchString)
		{
			LcdWriteChar(*pchString);
			pchString++;
		}
		return 1;
	}
	else
		return 0;
}

// Display a string using general pointer
uchar	LcdPutString(uchar ucRow, uchar ucCol, uchar *pchString)
{
	if(LcdSetPosition(ucRow, ucCol))
	{
		while(*pchString)
		{
			LcdWriteChar(*pchString);
			pchString++;
		}
		return 1;
	}
	else
		return 0;
}

// Set cursor position
uchar	LcdSetPosition(uchar ucRow, uchar ucCol)
{
	if(ucRow > 4 || ucRow < 1)
	{
		LcdWriteCmd(0x80);
		LcdWriteChar('R');
		LcdWriteChar('O');
		LcdWriteChar('W');
		LcdWriteChar(' ');
		LcdWriteChar('E');
		LcdWriteChar('R');
		LcdWriteChar('R');
		return 0;
	}
	else if(ucCol > 20 || ucCol < 1)
	{
		LcdWriteCmd(0x80);
		LcdWriteChar('C');
		LcdWriteChar('O');
		LcdWriteChar('L');
		LcdWriteChar(' ');
		LcdWriteChar('E');
		LcdWriteChar('R');
		LcdWriteChar('R');
		return 0;
	}
	else
	{
		LcdSetPos(ucRow, ucCol);
		return 1;
	}
}

// Get character at specified
uchar	LcdGetChar(uchar ucRow, uchar ucCol)
{
	uchar	ch = '\0';
	if(ucRow > 4 || ucRow < 1)
		LcdPutString(1, 1, "Row Error");
	else if(ucCol > 20 || ucCol < 1)
		LcdPutString(1, 1, "Column Error");
	else
	{
		LcdSetPos(ucRow, ucCol);
		ch = LcdReadChar();
	}
	return	ch;	
}
