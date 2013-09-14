#ifndef __BOARD_GUI_PRIMITIVES_H
#define __BOARD_GUI_PRIMITIVES_H

#include "stm32f10x.h"
#include "GLCD.h"


/*
  ASSEMBLE_RGB(R ,G, B): in according with structure of color map of LCD
  we have 5bit for Red, 6bit for Green and 5 bit for Blue.
  So if we want use 255x255x255 color map , we should take
  first 5 bits for Red from full 0xFF color,
  first 6 bits for Green from full 0xFF color, and
  first 5 bits for Red from full 0xFF color and mix it
  to one 16 bits value.
Example: uint16_t colot = ASSEMBLE_RGB(128 ,128, 128);//Gray
*/
#define ASSEMBLE_RGB(R ,G, B)    ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))

/*
  We should define system colors to avoid mess in them.
  If we need more collors we should add them here.
*/
typedef enum
{
  sys_RED       = 0xF800,
  sys_GREEN     = 0x07E0,
  sys_BLUE      = 0x001F,
  sys_BLACK     = 0x0000,
  sys_LightGray = 0xF79E,
  sys_Gray_1    = 0xCE59,
  sys_Gray_2    = 0xD69A,
  sys_Gray      = 0xAD55,
  sys_DarkGray  = 0x8410
} GUI_COLOR ;

typedef struct
{
  /* Start position of text. */
	uint16_t Text_Xpos;
	uint16_t Text_Ypos;
  /* Text colour. */
 	GUI_COLOR Text_Color;
  /* BackGround colour. */
 	GUI_COLOR bg_Text_Color;
  /* Text dirrection. */
	uint8_t	 TextRotate;
  /* Pointer to text string. */
	uint8_t *Text;
} GUI_TEXT;






#if 0
typedef struct
{
  /* Pressed area events */
	uint8_t Pressed             ;	// on pressed
	uint8_t Changed				      ;	// on changed
	uint8_t Hold					      ;	// on hold  (long time pressed)
	uint8_t Short					      ;	// on short (shortly pressed)
	uint8_t JustPressed			    ;	// on just pressed
	uint8_t JustReleased			  ;	// on just released
  /* Pressed area properties */
	uint8_t Press_JitterCounter	;	// Jitter time counter ( to avoid jitter error pressing )
	uint8_t Press_Hold_Counter  ;	// Hold time counter( we can change functionality depend on time)
} GUI_TOUCH;
#endif


#if 1
typedef struct
{
  /* Pressed area events */
	unsigned Pressed              : 1;	// on pressed
	unsigned Changed				      : 1;	// on changed
	unsigned Hold					        : 1;	// on hold  (long time pressed)
	unsigned Short					      : 1;	// on short (shortly pressed)
	unsigned JustPressed			    : 1;	// on just pressed
	unsigned JustReleased			    : 1;	// on just released
  /* Pressed area properties */
	unsigned  Press_JitterCounter	: 2;	// Jitter time counter ( to avoid jitter error pressing )
	unsigned  Press_Hold_Counter  : 8;	// Hold time counter( we can change functionality depend on time)
  unsigned  UnPress_Counter     : 8;	// Hold time counter( we can change functionality depend on time)
	unsigned 						          : 8;	// fitting to 32 bits (we have 32 bits CPU)
} GUI_TOUCH;
#endif
void board_Draw_X_Line(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t color);
void board_Draw_Y_Line(uint16_t Xpos,uint16_t Ypos, uint16_t width, uint16_t color);
void board_Draw_Rect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color);
void board_Draw_FilledRect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color);






#endif
