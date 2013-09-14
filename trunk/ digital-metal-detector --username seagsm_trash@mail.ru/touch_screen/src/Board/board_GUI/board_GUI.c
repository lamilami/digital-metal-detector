/* Here we will put function to
  to proced all common fing for GUI like main GUI loop.
*/
#include "board_GUI.h"


GUI_BUTTON button_1,button_2;


void board_GUI_Init(void)
{
//    GuiButtonInit(&button_1, 50, 60, 90, 100,"ON");
    GuiButtonInit(&button_2, 50, 100, 90, 100,"OFF");
//    GuiButtonOnDraw(&button_1,0);
    GuiButtonOnDraw(&button_2,0);
}

void board_GUI_OnTouch(uint16_t Xpos,uint16_t Ypos)
{
  /* List of objects.*/
 // GuiButtonOnTouch(&button_1,Xpos,Ypos);
  GuiButtonOnTouch(&button_2,Xpos,Ypos);
}