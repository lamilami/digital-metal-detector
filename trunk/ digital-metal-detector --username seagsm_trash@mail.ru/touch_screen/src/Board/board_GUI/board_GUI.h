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










#endif