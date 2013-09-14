/*
This file discribe all function for GUI button
Vadym Voloktin. 2013
*/

#include "ts_button.h"

/*Initialisation of TS_BUTTON object.
* TS_BUTTON object should have some properties like size,
* color,text on button, pointers to events and so on. Lets create structure in
* ts_button,h and use it to describe this object.
*/

/* Fill botton structure by valid values. */
void GuiButtonInit(GUI_BUTTON *button, uint16_t Xpos, uint16_t Ypos, uint16_t height, uint16_t width,uint8_t *Text)
{

  button->button_Xpos   = Xpos;
  button->button_Ypos   = Ypos;
  button->button_Heigth = height;
  button->button_Width  = width;
  button->button_Border_Color = sys_Gray;
  button->button_Color  = sys_Gray_1;
  button->button_Pressed_Color  = sys_DarkGray;
  button->button_UnPressed_Color = sys_Gray_2;
  button->button_Border_Width = 2;
  button->button_Text.Text   = Text;
  button->button_Text.TextRotate =3;
  button->button_Text.Text_Color = sys_BLACK;
  button->button_Text.bg_Text_Color = button->button_Color;
};
/* Draw button on the screen. */
void GuiButtonOnDraw(GUI_BUTTON *button,uint8_t condition)
{
  uint16_t x=0,y=0,h=0,w=0,bw=0,tw=0,counter=0;

  x = button->button_Xpos;
  y = button->button_Ypos;
  h = button->button_Heigth;
  w = button->button_Width;
  bw= button->button_Border_Width;
  tw= strlen(button->button_Text.Text);
  tw = tw*8;
  /* Drawing button border. */
  while(counter < bw)
  {
    board_Draw_Rect(x+counter,y+counter,h-(counter<<1),w-(counter<<1),button->button_Border_Color);
    counter++;
  }
  /* Drawing button filled inside border. */
  board_Draw_FilledRect(x+counter,y+counter,h-(counter<<1),w-(counter<<1),button->button_Color);
  /* Draw press unpress border. */
  GuiButtonPressOnOffDraw(button,condition);
  /* Print button text to center of button.*/
  GUI_Text_Rotated(
                    x+(h/2)-8,      /* Calculating button to text midle X position. */
                    y+(w/2)-(tw/2), /* Calculating button to text midle Y position. */
                    button->button_Text.Text,
                    button->button_Text.Text_Color,
                    button->button_Text.bg_Text_Color,
                    button->button_Text.TextRotate
                  );
}

/* This function redraw Press UnPress zone of buttotn. */
void GuiButtonPressOnOffDraw(GUI_BUTTON *button,uint8_t condition)
{
  uint16_t x=0,y=0,h=0,w=0,bw=0,tw=0,counter=0;
  GUI_COLOR color;

  x = button->button_Xpos;
  y = button->button_Ypos;
  h = button->button_Heigth;
  w = button->button_Width;
  bw= button->button_Border_Width;
  x = x + bw;
  y = y + bw;
  h = h - 2 * bw;
  w = w - 2 * bw;

  if(condition == 0)
  {
    color = button->button_UnPressed_Color;
    button->button_Touch.Pressed = 0;
    button->button_Touch.Press_Hold_Counter = 0;
    button->button_Touch.JustReleased = 1;
    button->button_Touch.Changed = 1;
    button->button_Touch.Press_JitterCounter = 0;

  }
  else
  {
    color = button->button_Pressed_Color;
    button->button_Touch.Pressed = 1;
    button->button_Touch.JustPressed =1;
    button->button_Touch.Press_JitterCounter = 0;
    button->button_Touch.Changed = 1;
  }
  /* Drawing button border. */
  while(counter < bw)
  {
    board_Draw_Rect(x+counter,y+counter,h-(counter<<1),w-(counter<<1),color);
    counter++;
  }
}

void GuiButtonOnTouch(GUI_BUTTON *button,uint16_t Xpos,uint16_t Ypos)
{
  static uint8_t result_old = 0;
  uint8_t result_new;
  /* Read Press button state. */
  GuiButtonOnTouchRead(button,Xpos,Ypos);
  result_new = button->button_Touch.Pressed;
  if( result_new != result_old)
  {
    GuiButtonPressOnOffDraw(button,result_new);
    result_old = result_new;
  }

}

void GuiButtonOnTouchRead(GUI_BUTTON *button,uint16_t Xpos,uint16_t Ypos)
{
  uint8_t a,b,c,d;
  a = ( button->button_Xpos <= Xpos);
  b = ( Xpos <= (button->button_Xpos + button->button_Heigth));
  c = ( button->button_Ypos <= Ypos);
  d = (Ypos <= (button->button_Xpos + button->button_Width));

  if(a&b&c&d)
  {
    button->button_Touch.Press_JitterCounter++;
    button->button_Touch.Press_Hold_Counter++;
    button->button_Touch.UnPress_Counter = 0;
    if(button->button_Touch.Press_JitterCounter >=3)
    {
      button->button_Touch.Pressed = 1;
    }
  }
  else
  {
    button->button_Touch.Press_JitterCounter = 0;
    button->button_Touch.UnPress_Counter++;
    if(button->button_Touch.UnPress_Counter > 100)
    {
      button->button_Touch.Pressed = 0;
      button->button_Touch.UnPress_Counter  = 0;
    }
  }
}






