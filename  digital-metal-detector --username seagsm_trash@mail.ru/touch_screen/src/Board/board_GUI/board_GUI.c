/* Here we will describe functions for
   process all common things for GUI like main GUI loop.
*/
#include "board_GUI.h"


GUI_BUTTON button_1,button_2,button_3,button_4,button_5;


void board_GUI_Init(void)
{
    LCD_Clear(BG_SYSTEM_SCREEN_COLOR);
    /* Initialization of screen components. */
    GuiButtonInit(&button_1, 10, 10, 30, 73,"ON",button1_OnTouch,0);
    GuiButtonInit(&button_2, 10, 85, 30, 73,"OFF",button2_OnTouch,0);
    GuiButtonInit(&button_3, 10, 160, 30, 73,"LightOn",0,0);
    GuiButtonInit(&button_4, 10, 235, 30, 73,"LightOff",button4_OnTouch,0);
    /* Drawing of screen components. */
    board_GUI_ScreenOnDraw();
}

void button4_OnTouch(void)
{
    GuiButtonInit(&button_5, 50, 10, 30, 73,"ON_1",0,0);
    GuiButtonOnDraw(&button_5,0);
    board_DrawCircle(100, 100, 20,sys_Green);
    board_DrawFullCircle(100, 140, 20, sys_Red);
}

void button1_OnTouch(void)
{
    board_DrawFullCircle(120, 50, 10, sys_Red);
}
void button2_OnTouch(void)
{
    board_DrawFullCircle(120, 50, 10, sys_Green);
}
/**/

void board_GUI_OnTouch(uint16_t Xpos,uint16_t Ypos)
{
  /* List of objects.*/
    GuiButtonOnTouch(&button_1,Xpos,Ypos);
    GuiButtonOnTouch(&button_2,Xpos,Ypos);
    GuiButtonOnTouch(&button_3,Xpos,Ypos);
    GuiButtonOnTouch(&button_4,Xpos,Ypos);
}
void board_GUI_ScreenOnDraw(void)
{
    GuiButtonOnDraw(&button_1,0);
    GuiButtonOnDraw(&button_2,0);
    GuiButtonOnDraw(&button_3,0);
    GuiButtonOnDraw(&button_4,0);
}


