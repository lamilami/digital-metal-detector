
/*Here we are describing a basic function for driwing. */

#include "board_GUI_primitives.h"


/* This function draw line in paralel X ordinate */
void board_Draw_X_Line(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t color)
{
  uint16_t counter = 0;
  LCD_SetPoint(Xpos,Ypos,color);
  heigth --;/* We already put one pixel. */
  /*
    We should set register one time (LCD_WriteRAM_Prepare()) and can write to it using
    autoincrement of address.
  */
  LCD_WriteRAM_Prepare();
  while(counter < heigth)
  {
    /* For line X work autoincrementing, so we dod not need address variable. */
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
//**************************************************************************************
// from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
// https://github.com/AndreyFursov/AFGUI/blob/master/GUICore/src/guiPrimitives.c
//In summary, when we compare the sum of N odd numbers to this algorithm we have:
// ddF_y = -2 * radius       is connected to last member of sum of N odd numbers.
//                           This member has index equal to value of radius (integral).
//                           Since odd number is 2*n + 1 there is 1 handled elsewhere
//                           or it should be -2*radius - 1
// ddF_x = 0                 should be 1. Because difference between two consecutive odd numbers is 2.
//                           If so f += ddF_y + 1 is f+= ddF_y. Saving one operation.
// f = - radius + 1          Initial error equal to half of "bigger" step.
//                           In case of saving one addition it should be either -radius or -radius + 2.
// In any case there should be addition of 1 driven out of outer loop.
// So.
// f += ddF_y                Adding odd numbers from Nth to 1st.
// f += ddF_x                Adding odd numbers from 1st to Nth. 1 is missing because it can be moved outside of loop.
void board_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t radius,uint16_t color)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	LCD_SetPoint(Xpos, Ypos + radius,color);
	LCD_SetPoint(Xpos, Ypos - radius,color);
	LCD_SetPoint(Xpos + radius, Ypos,color);
	LCD_SetPoint(Xpos - radius, Ypos,color);

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		LCD_SetPoint(Xpos + x, Ypos + y,color);
		LCD_SetPoint(Xpos - x, Ypos + y,color);
		LCD_SetPoint(Xpos + x, Ypos - y,color);
		LCD_SetPoint(Xpos - x, Ypos - y,color);
		LCD_SetPoint(Xpos + y, Ypos + x,color);
		LCD_SetPoint(Xpos - y, Ypos + x,color);
		LCD_SetPoint(Xpos + y, Ypos - x,color);
		LCD_SetPoint(Xpos - y, Ypos - x,color);
	}
}
/* https://github.com/AndreyFursov/AFGUI/blob/master/GUICore/src/guiPrimitives.c */
void board_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

  /* Draw centrall horizontal line*/
	board_Draw_Y_Line(Xpos + x, Ypos - y, 2*y, color);

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

    /* Draw horizontal line from center. */
    board_Draw_Y_Line(Xpos + x, Ypos - y, 2*y, color);
		board_Draw_Y_Line(Xpos - x, Ypos - y, 2*y, color);
		/* draw horizontal line fron up and down. */
    board_Draw_Y_Line(Xpos + y, Ypos - x, 2*x, color);
		board_Draw_Y_Line(Xpos - y, Ypos - x, 2*x, color);
	}
	board_DrawCircle(Xpos, Ypos, radius,color);
}

