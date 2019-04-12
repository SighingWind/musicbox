#ifndef		_LCDDEFINE_H_
#define		_LCDDEFINE_H_

#include	"generaldef.h"

extern	void	LcdWriteCmd(uchar);
extern	void	LcdWriteChar(uchar);
extern	uchar	LcdReadChar(void);
extern	void	LcdSetPos(uchar, uchar);

void	LcdClearDisplay();
void	LcdInit(void);
void	LcdSetCursor(uchar ucOnOff, uchar ucBlink);
uchar	LcdSetPosition(uchar ucRow, uchar ucCol);
uchar	LcdPutChar(uchar ucRow, uchar ucCol, uchar ucChar);
uchar	LcdPutString(uchar ucRow, uchar ucCol, uchar *pchString);
uchar	LcdPutStringC(uchar ucRow, uchar ucCol, uchar code * pchString);
uchar	LcdPutStringD(uchar ucRow, uchar ucCol, uchar data * pchString);
uchar	LcdGetChar(uchar ucRow, uchar ucCol);

#endif