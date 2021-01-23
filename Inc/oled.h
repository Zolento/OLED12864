#ifndef OLED_H
#define OLED_H
#include "main.h"
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  
uint8_t OLED_Write_CMD(uint8_t CMD);
uint8_t OLED_Write_DATA(uint8_t DATA);
void OLED_Init(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Clear(void);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t n);
void OLED_RST_Set();
void OLED_RST_Clr();
#endif