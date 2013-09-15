#ifndef __BOARD_GUI_H
#define __BOARD_GUI_H

#include "stm32f10x.h"
#include "GLCD.h"
#include "board_GUI_primitives.h"
#include "ts_button.h"


extern GUI_BUTTON button_1;
extern GUI_BUTTON button_2;
extern GUI_BUTTON button_3;
extern GUI_BUTTON button_4;

void board_GUI_Init(void);
void board_GUI_ScreenOnDraw(void);
void board_GUI_OnTouch(uint16_t Xpos,uint16_t Ypos);
void button1_OnTouch(void);
void button2_OnTouch(void);
void button4_OnTouch(void);








#endif