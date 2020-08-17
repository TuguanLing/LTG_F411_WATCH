#include "tft.h"
#include "stdlib.h"
#include "tftfont.h"  	 
#include "delay.h"
#include "stm32f4xx.h"
#include "tftfont.h"
#include "stdlib.h"
#include "stdio.h"
#include "spi.h"
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
u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

////////////////////////////////////移植修改区///////////////////////////////////

/****************************************************************************
* 名    称：u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 Byte)
* 功    能：STM32_硬件SPI读写一个字节数据底层函数
* 入口参数：SPIx,Byte
* 出口参数：返回总线收到的数据
* 说    明：STM32_硬件SPI读写一个字节数据底层函数
****************************************************************************/
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPIx->DR=Byte;	 	//发送一个byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte  
	return SPIx->DR;          	     //返回收到的数据			
}
//
/****************************************************************************
* 名    称：void LCD_Writ_Bus(char data)
* 功    能：STM32_软件SPI读写一个字节数据底层函数
* 入口参数：data
* 出口参数：返回总线收到的数据
* 说    明：STM32_软件SPI读写一个字节数据底层函数
****************************************************************************/
void LCD_Writ_Bus(char data)   //串行数据写入
{	
	u8 i;			  
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(data&0x80)
		   LCD_SDIN_Set();
		else 
		   LCD_SDIN_Clr();
		LCD_SCLK_Set();
		data<<=1;   
	}			
}
u8 LCD_Read_Bus(void)   //串行数据读操作
{	
	u8 i;
	u8 data;
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(LCD_SDIN==1)
		   data=1;
		else 
		   data=0;
		LCD_SCLK_Set();
		data<<=1;   
	}	
	return data;
}
//////////////////////////////////////////////////////////////////////////////



/****************************************************************************
* 名    称：LCD_WR_DATA8(char da)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void LCD_WR_DATA8(char da) //发送数据-8位参数
{	
	LCD_CS_Clr();
    LCD_DC_Set();
	 
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,da);  
#else
	LCD_Writ_Bus(da);
#endif
	LCD_CS_Set();
} 
//
/****************************************************************************
* 名    称：void LCD_WR_DATA16(int da)
* 功    能：向液晶屏写一个16位数据
* 入口参数：da
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void LCD_WR_DATA16(int da)
{
	LCD_CS_Clr();
    LCD_DC_Set();
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,da>>8);
	SPI_ReadWriteByte(SPI3,da); 
#else
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
#endif
	LCD_CS_Set();
}
/****************************************************************************
* 名    称：LCD_WR_DATA8(char da)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
u8 LCD_Read_DATA8(void) //发送数据-8位参数
{	
	LCD_CS_Clr();//选中LCD屏
    LCD_DC_Set();//读写数据状态
	
#if LCD_HARDWARE
	return SPI_ReadWriteByte(SPI3,0xff);//发送一个byte
#else
	return LCD_Read_Bus();
#endif
	LCD_CS_Set();
}
//
/****************************************************************************
* 名    称：void LCD_WR_REG(char da)
* 功    能：向液晶屏写一个8位数据
* 入口参数：da
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void LCD_WR_REG(char da)	 
{	
	LCD_CS_Clr();
    LCD_DC_Clr();
	
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,da);//发送一个byte
#else
	LCD_Writ_Bus(da);
#endif
	LCD_CS_Set();
}
//
/****************************************************************************
* 名    称：void LCD_WR_REG_DATA(int reg,int da)
* 功    能：向液晶屏指定地址写一个8位数据
* 入口参数：reg，da
* 出口参数：无
* 说    明：向控制器指定地址写入一个8位数据
****************************************************************************/
void LCD_WR_REG_DATA(int reg,int da)
{	
	LCD_CS_Clr();
    LCD_WR_REG(reg);
	
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,da);;//发送一个byte
#else
	LCD_WR_DATA8(da);
#endif
	LCD_CS_Set();
}
//
//
/****************************************************************************
* 名    称：void LCD_WR_REG_DATA(int reg,int da)
* 功    能：向液晶屏指定地址写一个8位数据
* 入口参数：reg，da
* 出口参数：无
* 说    明：向控制器指定地址写入一个8位数据
****************************************************************************/
void LCD_SetCursor(u16 x, u16 y)
{	  	    			
	if(LCD_DIR==3)
	{
		x=x+80;
	}	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(x>>8);
	LCD_WR_DATA8(x);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(y>>8);
	LCD_WR_DATA8(y);

	LCD_WR_REG(0x2C);
} 
//
/****************************************************************************
* 名    称：void Address_set(u16 x1,u16 y1,u16 x2,u16 y2)
* 功    能：设置地址
* 入口参数：x1,y1,x2,y2
* 出口参数：无
* 说    明：向控制器指定地址写入一个坐标
****************************************************************************/
void LCD_Set_Window(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(LCD_DIR==3)
	{
		x1=x1+80;
		x2=x2+80;
	}	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(x1>>8);
	LCD_WR_DATA8(x1);
	LCD_WR_DATA8(x2>>8);
	LCD_WR_DATA8(x2);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(y1>>8);
	LCD_WR_DATA8(y1);
	LCD_WR_DATA8(y2>>8);
	LCD_WR_DATA8(y2);

	LCD_WR_REG(0x2C);					 						 
}
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//开启显示
	LCD_BLK_Set();		//开启背光
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	
	LCD_BLK_Clr();		//关闭背光
	LCD_WR_REG(0X28);	//关闭显示
}
//
/****************************************************************************
* 名    称：void Lcd_Init(void)
* 功    能：液晶屏初始化
* 入口参数：无
* 出口参数：无
* 说    明：液晶屏初始化
****************************************************************************/
void LCD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA~B时钟

#if LCD_HARDWARE
	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
	
	SPI3_Init();
#else
	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
#endif


	LCD_CS_Clr();  //打开片选使能
	LCD_RST_Clr();
	delay_ms(20);
	LCD_RST_Set();
	delay_ms(20);
	LCD_BLK_Set();
	
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36); //
	LCD_WR_DATA8(0x00);//设置坐标原点，RGB顺序(0x08:BGR)
//////////////////////////////////////////////////////////
	//0000 0000(0x00) 方向 顺时针12
	//0011 0000(0x70) 方向 顺时针3
	//0000 0000(0x00) 方向 顺时针6
	//1000 0100(0x00) 方向 顺时针9
	//横屏 左到右，下到上
	
	//D7：1从下到上，0从上到下（Page Address Order(MY)）
	//D6：1从右到左，0从左到右（Column Address Order(MX)）
	//D5：1行列交换，0不交换（Page/Column Order (MV)）
	//D4：1从上到下，0从下到上（Line Address Order (ML)）
	//D3：1BGR，0RGB（RGB/ BGR Order (RGB)）
	//D2：1从左到右，0从右到左（Display Data Latch Data Order水平顺序）
	//D1:
	//D0:
	LCD_WR_REG(0x36); // 屏幕显示方向设置
	switch(LCD_DIR)
	{
		case 0:LCD_WR_DATA8(0x00);break;
		case 1:LCD_WR_DATA8(0x70);break;
		case 2:LCD_WR_DATA8(0x90);break;
		case 3:LCD_WR_DATA8(0xA0);break;
	}	
//////////////////////////////////////////////////////////
	LCD_WR_REG(0x3A); //65k 模式
	LCD_WR_DATA8(0x05);


	//帧速率设置
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x35);  

	//功率设定
	LCD_WR_REG(0xBB);//VCOM
	LCD_WR_DATA8(0x19);

	LCD_WR_REG(0xC0);//功率控制
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x12);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	//伽马设定
	LCD_WR_REG(0xE0);//Set Gamma
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);//Set Gamma
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0x21); //反显

	LCD_WR_REG(0x11); 
	//Delay (120); 

	LCD_WR_REG(0x29); //开启显示
	
	LCD_Clear(BACK_COLOR);
 
} 
//











//清屏函数
//Color:要清屏的填充色
void LCD_Clear(u16 Color)
{
	u16 i,j;

	LCD_Set_Window(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{
			LCD_WR_DATA16(Color);	 			 
		}
	}
}
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Set_Window(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA16(color);//设置光标位置 	    
	} 					  	    
}
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill_2(u16 xsta,u16 ysta,u16 width,u16 height,u16 color)
{ 
//	u16 xend,yend;
//	u16 i,j;
//	width=xend-xsta+1; 			//得到填充的宽度
//	height=yend-ysta+1;			//高度
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(xsta,ysta+i);   	//设置光标位置 
//		for(j=0;j<width;j++)
//		{
//			LCD_WR_DATA16(color);//设置光标位置 
//		}
//	}	

	u16 i,j; 
	LCD_Set_Window(xsta,ysta,xsta+width,ysta+height);      //设置光标位置 
	for(i=ysta;i<=(ysta+height);i++)
	{													   	 	
		for(j=xsta;j<=(xsta+width);j++)LCD_WR_DATA16(color);//设置光标位置 	    
	} 		
}




















//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		for(j=0;j<width;j++)
			LCD_WR_DATA16(color[i*width+j]);//写入数据 
	}		  
}

////在指定区域内填充指定颜色块			 
////(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
////color:要填充的颜色
//void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
//{  
////	u16 height,width;
////	u16 i,j;
////	width=ex-sx+1; 			//得到填充的宽度
////	height=ey-sy+1;			//高度
//// 	for(i=0;i<height;i++)
////	{
//// 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
////		for(j=0;j<width;j++)
////		{
////			SPI_ReadWriteByte(SPI3,color[i*width+j]);//写入数据 
////		}
////	}
////	
//	u16 height,width;
//	u16 i,j;
//	width=ex-sx+1; 			//得到填充的宽度
//	height=ey-sy+1;			//高度
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
//		for(j=0;j<width;j++)
//			LCD_WR_DATA16(color[i*width+j]);//写入数据 
//	}		  
//} 


//画点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_DATA16(POINT_COLOR); 	    
}
//读点
//color:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置坐标
	LCD_WR_REG(0x2E);//读点指令
	LCD_Read_DATA8();//读dummy
	LCD_Read_DATA8();//读dummy（一共读16位）
	return ((LCD_Read_DATA8()&0x007c)<<9|(LCD_Read_DATA8()<<4)|(LCD_Read_DATA8()>>2));
}
//u16 LCD_ReadPoint(u16 x,u16 y)
//{
//	u16 color;
//	if(x>=LCD_W||y>=LCD_H)
//	{
//		return 0;	//超过了范围,直接返回	
//	}
//	LCD_SetCursor(x,y);//设置光标位置 
//	color = Lcd_ReadData_16Bit();
//	return color;
//}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{ 
	if(LCD_DIR==3)
	{
		x=x+80;
	}	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(x>>8);
	LCD_WR_DATA8(x);
	
	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(y>>8);
	LCD_WR_DATA8(y);

	LCD_WR_REG(0x2C);
	
	LCD_CS_Clr();
    LCD_DC_Set();
	
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,color>>8);
	SPI_ReadWriteByte(SPI3,color);
#else
	LCD_Writ_Bus(color>>8);
    LCD_Writ_Bus(color);
#endif
	LCD_CS_Set();	
} 
//画一个大点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}   
//








//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_H)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_W)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//









//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  











