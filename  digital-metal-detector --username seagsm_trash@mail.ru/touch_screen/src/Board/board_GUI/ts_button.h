#ifndef __TS_BUTTON_H
#define __TS_BUTTON_H

#include "stm32f10x.h"
#include "board_GUI_primitives.h"

typedef struct
{
	uint16_t	button_Xpos;
	uint16_t	button_Ypos;
	uint16_t	button_Heigth;
	uint16_t	button_Width;
  GUI_COLOR button_Color;
  GUI_COLOR button_Pressed_Color;
  GUI_COLOR button_UnPressed_Color;
  uint8_t   button_Border_Width;
  uint16_t  button_Border_Color;
  GUI_TEXT  button_Text;
  GUI_TOUCH button_Touch;
	void (*fOnTouch)(void);
	void (*fOnUntouch)(void);
} GUI_BUTTON;

extern uint16_t touch_Xpos, touch_Ypos;


void GuiButtonInit(GUI_BUTTON *button, uint16_t Xpos, uint16_t Ypos, uint16_t width, uint16_t height,uint8_t *Text);
void GuiButtonOnDraw(GUI_BUTTON *button,uint8_t condition);
void GuiButtonPressOnOffDraw(GUI_BUTTON *button,uint8_t condition);
void GuiButtonOnTouchRead(GUI_BUTTON *button,uint16_t Xpos,uint16_t Ypos);
void GuiButtonOnTouch(GUI_BUTTON *button,uint16_t Xpos,uint16_t Ypos);

#endif