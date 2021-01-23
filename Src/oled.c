#include "oled.h"
#include "gpio.h"
#include "spi.h"
#include "oledfont.h"
uint8_t OLED_Write_CMD(uint8_t CMD)//д������
{
	uint8_t RXDATA = 0;
	
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,0);//CS��Ƭѡ�͵�ƽѡ��
	HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,0);//DC�ŵ͵�ƽд����
	HAL_SPI_TransmitReceive(&hspi2,&CMD,&RXDATA,1,10);
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,1);//CS��Ƭѡ�ߵ�ƽʧ��
	
	return RXDATA;
}
uint8_t OLED_Write_DATA(uint8_t DATA)//д������
{
	uint8_t RXDATA = 0;
	
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,0);//CS��Ƭѡ�͵�ƽѡ��
	HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,1);//DC�Ÿߵ�ƽд����
	HAL_SPI_TransmitReceive(&hspi2,&DATA,&RXDATA,1,10);
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,1);//CS��Ƭѡ�ߵ�ƽʧ��
	
	return RXDATA;
}

void OLED_Set_Pos(unsigned char x, unsigned char y)//ҳѰַ
{ 
	OLED_Write_CMD(0xb0+y);
	OLED_Write_CMD(((x&0xf0)>>4)|0x10);
	OLED_Write_CMD((x&0x0f)|0x01); 
}  

void OLED_RES_Set()//RES��1
{
	HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,1);
}

void OLED_RES_Clr()//RES��0
{
	HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,0);
}

void OLED_Clear(void)//����
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_CMD (0xb0+i);    //����ҳ��ַ��0~7��
		OLED_Write_CMD (0x00);      //������ʾλ�á��е͵�ַ
		OLED_Write_CMD (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_DATA(0); 
	} //������ʾ
}

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)//��ʾ�ַ�
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_Write_DATA(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_Write_DATA(F8X16[c*16+i+8]);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_Write_DATA(F6x8[c][i]);
				
			}
}

void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)//��ʾ�ַ���
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t n)//��ʾ����
{      		
	uint8_t t;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_Write_DATA(Hzk[2*n][t]);
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_Write_DATA(Hzk[2*n+1][t]);
      }					
}

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])//��ʾBMPͼƬ
{
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_Write_DATA(BMP[j++]);	    	
	    }
	}
} 



void OLED_Init()//OLED��ʼ��
{
	OLED_RES_Set();
	HAL_Delay(100);
	OLED_RES_Clr();
	HAL_Delay(100);
	OLED_RES_Set();
	
	
	OLED_Write_CMD(0xAE);//--turn off oled panel
	OLED_Write_CMD(0x00);//---set low column address
	OLED_Write_CMD(0x10);//---set high column address
	OLED_Write_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write_CMD(0x81);//--set contrast control register
	OLED_Write_CMD(0xCF); // Set SEG Output Current Brightness
	OLED_Write_CMD(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_Write_CMD(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_Write_CMD(0xA6);//--set normal display
	OLED_Write_CMD(0xA8);//--set multiplex ratio(1 to 64)
	OLED_Write_CMD(0x3f);//--1/64 duty
	OLED_Write_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write_CMD(0x00);//-not offset
	OLED_Write_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_Write_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write_CMD(0xD9);//--set pre-charge period
	OLED_Write_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write_CMD(0xDA);//--set com pins hardware configuration
	OLED_Write_CMD(0x12);
	OLED_Write_CMD(0xDB);//--set vcomh
	OLED_Write_CMD(0x40);//Set VCOM Deselect Level
	OLED_Write_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write_CMD(0x02);//
	OLED_Write_CMD(0x8D);//--set Charge Pump enable/disable
	OLED_Write_CMD(0x14);//--set(0x10) disable
	OLED_Write_CMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_Write_CMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_Write_CMD(0xAF);//--turn on oled panel
	
	OLED_Write_CMD(0xAF);//--turn on oled panel
	OLED_Clear();
	OLED_Set_Pos(0,0); 
}

