#ifndef __TFT_H
#define __TFT_H			  	 
#include "sys.h"
#include "stdlib.h"	   
    						  
//-----------------OLED端口定义----------------
//=========================================电源接线================================================//
//   液晶屏模块            STM32单片机
//      VCC        接       DC5V/3.3V      //电源
//      GND        接          GND         //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//   液晶屏模块            STM32单片机    
//      SDA        接          PB5        //SPI总线数据信号
//=======================================液晶屏控制线接线==========================================//
//   液晶屏模块 					 STM32单片机 
//      LED        接          PB7         //背光控制信号，如果不需要控制，接5V或3.3V
//      SCK        接          PB3        //SPI总线时钟信号
//    A0(RS/DC)    接          PB4        //数据/命令控制信号
//      RST        接          PB6        //复位控制信号
//      CS         接          PA15        //片选控制信号
//=================================================================================================//

//重要设置
#define LCD_HARDWARE 1		//1使用硬件PSI，0使用软件SPI

//LCD重要参数
#define LCD_W 240
#define LCD_H 240
#define LCD_DIR 3

//LCD重要IO定义
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

#define LCD_CMD  0	//写命令
#define LCD_DATA 1	//写数据

extern  u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

//移植用到的函数
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 Byte);//硬件SPIx读写一个字节（8位）
void LCD_Writ_Bus(char data);//软件SPI读写一个字节（8位）

void LCD_WR_DATA8(char da); //发送数据-8位参数
void LCD_WR_DATA16(int da); //发送数据-16位参数
u8 LCD_Read_DATA8(void); //读取数据-8位参数
void LCD_WR_REG(char da);//发送地址-8位参数
void LCD_WR_REG_DATA(int reg,int da);//发送指定地址-8位数据

void LCD_SetCursor(u16 Xpos, u16 Ypos);	//设置坐标
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);//设置窗口					   						   																			 

void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);//快速画点
void LCD_DrawPoint_big(u16 x,u16 y);//画一个大点
u16  LCD_ReadPoint(u16 x,u16 y); //读点

void LCD_DisplayOn(void); //开显示
void LCD_DisplayOff(void); //关显示

void LCD_Init(void); //初始化
void LCD_Clear(u16 Color);//清屏

void LCD_Fill_2(u16 xsta,u16 ysta,u16 width,u16 height,u16 color);
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//填充颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);//填充指定颜色

//GUI
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);//画矩形  

//显示字符
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);//显示字符
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);//显示字符串
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);//显示数字（高位不填零）
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//显示数字（高位填零）


//画笔颜色
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define SBLUE 			 0x251F //淡蓝色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


					  		 
#endif  
//
	 



