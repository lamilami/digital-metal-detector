
/*Here we are discribing a basic function for driwing. */

#include "board_GUI_primitives.h"


/* This function draw line in paralel X ordinate */
void board_Draw_X_Line(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t color)
{
  uint16_t counter = 0;
  LCD_SetPoint(Xpos,Ypos,color);
  heigth --;/* We already put one pixel. */
  while(counter < heigth)
  { /* For line X work autoincrementing, so we dod not need address variable. */
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(color);
    counter++;
  }
}
/* This function draw line in paralel Y ordinate */
void board_Draw_Y_Line(uint16_t Xpos,uint16_t Ypos, uint16_t width, uint16_t color)
{
  uint16_t counter = 0;
  while(counter < width)
  {
    LCD_SetPoint(Xpos,Ypos+counter,color);
    counter++;
  }
}

/* This function draw empty rectangle. */
void board_Draw_Rect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color)
{
  board_Draw_X_Line(Xpos,Ypos,heigth,color);/* Draw left up line.*/
  board_Draw_Y_Line(Xpos + (heigth-1),Ypos, width,color);/* Draw up line from left to right. */
  board_Draw_X_Line(Xpos,Ypos+(width-1),heigth,color);/* Draw right up line.*/
  board_Draw_Y_Line(Xpos,Ypos, width,color);/* Draw down line from left to right. */
}

/* This function draw filled rectangle. */
void board_Draw_FilledRect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color)
{
  uint16_t counter = 0;
  while(counter < width)
  {
    /* Draw line from down to up and move to left for drawing next line.*/
    board_Draw_X_Line(Xpos,Ypos+counter,heigth,color);
    counter++;
  }
}


