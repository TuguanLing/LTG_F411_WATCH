#ifndef __TFT_H
#define __TFT_H			  	 
#include "sys.h"
#include "stdlib.h"	   
    						  
//-----------------OLED�˿ڶ���----------------
//=========================================��Դ����================================================//
//   Һ����ģ��            STM32��Ƭ��
//      VCC        ��       DC5V/3.3V      //��Դ
//      GND        ��          GND         //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//   Һ����ģ��            STM32��Ƭ��    
//      SDA        ��          PB5        //SPI���������ź�
//=======================================Һ���������߽���==========================================//
//   Һ����ģ�� 					 STM32��Ƭ�� 
//      LED        ��          PB7         //��������źţ��������Ҫ���ƣ���5V��3.3V
//      SCK        ��          PB3        //SPI����ʱ���ź�
//    A0(RS/DC)    ��          PB4        //����/��������ź�
//      RST        ��          PB6        //��λ�����ź�
//      CS         ��          PA15        //Ƭѡ�����ź�
//=================================================================================================//

//��Ҫ����
#define LCD_HARDWARE 1		//1ʹ��Ӳ��PSI��0ʹ�����SPI

//LCD��Ҫ����
#define LCD_W 240
#define LCD_H 240
#define LCD_DIR 3

//LCD��ҪIO����
#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_3)//CLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define LCD_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)//DIN
#define LCD_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define LCD_SDIN PBin(5)

#define LCD_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//RES
#define LCD_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define LCD_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)//DC
#define LCD_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)
 		     
#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define LCD_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//CS
#define LCD_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define LCD_CMD  0	//д����
#define LCD_DATA 1	//д����

extern  u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

//��ֲ�õ��ĺ���
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 Byte);//Ӳ��SPIx��дһ���ֽڣ�8λ��
void LCD_Writ_Bus(char data);//���SPI��дһ���ֽڣ�8λ��

void LCD_WR_DATA8(char da); //��������-8λ����
void LCD_WR_DATA16(int da); //��������-16λ����
u8 LCD_Read_DATA8(void); //��ȡ����-8λ����
void LCD_WR_REG(char da);//���͵�ַ-8λ����
void LCD_WR_REG_DATA(int reg,int da);//����ָ����ַ-8λ����

void LCD_SetCursor(u16 Xpos, u16 Ypos);	//��������
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);//���ô���					   						   																			 

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);//���ٻ���
void LCD_DrawPoint_big(u16 x,u16 y);//��һ�����
u16  LCD_ReadPoint(u16 x,u16 y); //����

void LCD_DisplayOn(void); //����ʾ
void LCD_DisplayOff(void); //����ʾ

void LCD_Init(void); //��ʼ��
void LCD_Clear(u16 Color);//����

void LCD_Fill_2(u16 xsta,u16 ysta,u16 width,u16 height,u16 color);
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//�����ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);//���ָ����ɫ

//GUI
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);//��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);//����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);//������  

//��ʾ�ַ�
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);//��ʾ�ַ�
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);//��ʾ�ַ���
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);//��ʾ���֣���λ�����㣩
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//��ʾ���֣���λ���㣩


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define SBLUE 			 0x251F //����ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
//
	 



