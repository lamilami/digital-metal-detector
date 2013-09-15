/* Here we will describe functions for
   process all common things for GUI like main GUI loop.
*/
#include "board_GUI.h"


GUI_BUTTON button_1,button_2,button_3,button_4;


void board_GUI_Init(void)
{
    GuiButtonInit(&button_1, 10, 10, 30, 73,"ON");
    GuiButtonInit(&button_2, 10, 85, 30, 73,"OFF");
    GuiButtonInit(&button_3, 10, 160, 30, 73,"LightOn");
    GuiButtonInit(&button_4, 10, 235, 30, 73,"LightOff");
    GuiButtonOnDraw(&button_1,0);
    GuiButtonOnDraw(&button_2,0);
    GuiButtonOnDraw(&button_3,0);
    GuiButtonOnDraw(&button_4,0);
}

void board_GUI_OnTouch(uint16_t Xpos,uint16_t Ypos)
{
  /* List of objects.*/
  GuiButtonOnTouch(&button_1,Xpos,Ypos);
  GuiButtonOnTouch(&button_2,Xpos,Ypos);
  GuiButtonOnTouch(&button_3,Xpos,Ypos);
  GuiButtonOnTouch(&button_4,Xpos,Ypos);
}