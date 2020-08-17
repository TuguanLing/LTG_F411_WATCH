#include "tft.h"
#include "stdlib.h"
#include "tftfont.h"  	 
#include "delay.h"
#include "stm32f4xx.h"
#include "tftfont.h"
#include "stdlib.h"
#include "stdio.h"
#include "spi.h"
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
u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

////////////////////////////////////��ֲ�޸���///////////////////////////////////

/****************************************************************************
* ��    �ƣ�u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 Byte)
* ��    �ܣ�STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
* ��ڲ�����SPIx,Byte
* ���ڲ��������������յ�������
* ˵    ����STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//�ȴ���������	  
	SPIx->DR=Byte;	 	//����һ��byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//�ȴ�������һ��byte  
	return SPIx->DR;          	     //�����յ�������			
}
//
/****************************************************************************
* ��    �ƣ�void LCD_Writ_Bus(char data)
* ��    �ܣ�STM32_���SPI��дһ���ֽ����ݵײ㺯��
* ��ڲ�����data
* ���ڲ��������������յ�������
* ˵    ����STM32_���SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
void LCD_Writ_Bus(char data)   //��������д��
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
u8 LCD_Read_Bus(void)   //�������ݶ�����
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
* ��    �ƣ�LCD_WR_DATA8(char da)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ��ڲ�����
****************************************************************************/
void LCD_WR_DATA8(char da) //��������-8λ����
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
* ��    �ƣ�void LCD_WR_DATA16(int da)
* ��    �ܣ���Һ����дһ��16λ����
* ��ڲ�����da
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��16λ����
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
* ��    �ƣ�LCD_WR_DATA8(char da)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ��ڲ�����
****************************************************************************/
u8 LCD_Read_DATA8(void) //��������-8λ����
{	
	LCD_CS_Clr();//ѡ��LCD��
    LCD_DC_Set();//��д����״̬
	
#if LCD_HARDWARE
	return SPI_ReadWriteByte(SPI3,0xff);//����һ��byte
#else
	return LCD_Read_Bus();
#endif
	LCD_CS_Set();
}
//
/****************************************************************************
* ��    �ƣ�void LCD_WR_REG(char da)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����da
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��16λ����
****************************************************************************/
void LCD_WR_REG(char da)	 
{	
	LCD_CS_Clr();
    LCD_DC_Clr();
	
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,da);//����һ��byte
#else
	LCD_Writ_Bus(da);
#endif
	LCD_CS_Set();
}
//
/****************************************************************************
* ��    �ƣ�void LCD_WR_REG_DATA(int reg,int da)
* ��    �ܣ���Һ����ָ����ַдһ��8λ����
* ��ڲ�����reg��da
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��8λ����
****************************************************************************/
void LCD_WR_REG_DATA(int reg,int da)
{	
	LCD_CS_Clr();
    LCD_WR_REG(reg);
	
#if LCD_HARDWARE
	SPI_ReadWriteByte(SPI3,da);;//����һ��byte
#else
	LCD_WR_DATA8(da);
#endif
	LCD_CS_Set();
}
//
//
/****************************************************************************
* ��    �ƣ�void LCD_WR_REG_DATA(int reg,int da)
* ��    �ܣ���Һ����ָ����ַдһ��8λ����
* ��ڲ�����reg��da
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��8λ����
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
* ��    �ƣ�void Address_set(u16 x1,u16 y1,u16 x2,u16 y2)
* ��    �ܣ����õ�ַ
* ��ڲ�����x1,y1,x2,y2
* ���ڲ�������
* ˵    �����������ָ����ַд��һ������
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
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//������ʾ
	LCD_BLK_Set();		//��������
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	
	LCD_BLK_Clr();		//�رձ���
	LCD_WR_REG(0X28);	//�ر���ʾ
}
//
/****************************************************************************
* ��    �ƣ�void Lcd_Init(void)
* ��    �ܣ�Һ������ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ������ʼ��
****************************************************************************/
void LCD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTA~Bʱ��

#if LCD_HARDWARE
	//GPIO��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
	
	SPI3_Init();
#else
	//GPIO��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
#endif


	LCD_CS_Clr();  //��Ƭѡʹ��
	LCD_RST_Clr();
	delay_ms(20);
	LCD_RST_Set();
	delay_ms(20);
	LCD_BLK_Set();
	
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36); //
	LCD_WR_DATA8(0x00);//��������ԭ�㣬RGB˳��(0x08:BGR)
//////////////////////////////////////////////////////////
	//0000 0000(0x00) ���� ˳ʱ��12
	//0011 0000(0x70) ���� ˳ʱ��3
	//0000 0000(0x00) ���� ˳ʱ��6
	//1000 0100(0x00) ���� ˳ʱ��9
	//���� ���ң��µ���
	
	//D7��1���µ��ϣ�0���ϵ��£�Page Address Order(MY)��
	//D6��1���ҵ���0�����ң�Column Address Order(MX)��
	//D5��1���н�����0��������Page/Column Order (MV)��
	//D4��1���ϵ��£�0���µ��ϣ�Line Address Order (ML)��
	//D3��1BGR��0RGB��RGB/ BGR Order (RGB)��
	//D2��1�����ң�0���ҵ���Display Data Latch Data Orderˮƽ˳��
	//D1:
	//D0:
	LCD_WR_REG(0x36); // ��Ļ��ʾ��������
	switch(LCD_DIR)
	{
		case 0:LCD_WR_DATA8(0x00);break;
		case 1:LCD_WR_DATA8(0x70);break;
		case 2:LCD_WR_DATA8(0x90);break;
		case 3:LCD_WR_DATA8(0xA0);break;
	}	
//////////////////////////////////////////////////////////
	LCD_WR_REG(0x3A); //65k ģʽ
	LCD_WR_DATA8(0x05);


	//֡��������
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x35);  

	//�����趨
	LCD_WR_REG(0xBB);//VCOM
	LCD_WR_DATA8(0x19);

	LCD_WR_REG(0xC0);//���ʿ���
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

	//٤���趨
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

	LCD_WR_REG(0x21); //����

	LCD_WR_REG(0x11); 
	//Delay (120); 

	LCD_WR_REG(0x29); //������ʾ
	
	LCD_Clear(BACK_COLOR);
 
} 
//











//��������
//Color:Ҫ���������ɫ
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
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Set_Window(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA16(color);//���ù��λ�� 	    
	} 					  	    
}
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill_2(u16 xsta,u16 ysta,u16 width,u16 height,u16 color)
{ 
//	u16 xend,yend;
//	u16 i,j;
//	width=xend-xsta+1; 			//�õ����Ŀ��
//	height=yend-ysta+1;			//�߶�
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(xsta,ysta+i);   	//���ù��λ�� 
//		for(j=0;j<width;j++)
//		{
//			LCD_WR_DATA16(color);//���ù��λ�� 
//		}
//	}	

	u16 i,j; 
	LCD_Set_Window(xsta,ysta,xsta+width,ysta+height);      //���ù��λ�� 
	for(i=ysta;i<=(ysta+height);i++)
	{													   	 	
		for(j=xsta;j<=(xsta+width);j++)LCD_WR_DATA16(color);//���ù��λ�� 	    
	} 		
}




















//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		for(j=0;j<width;j++)
			LCD_WR_DATA16(color[i*width+j]);//д������ 
	}		  
}

////��ָ�����������ָ����ɫ��			 
////(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
////color:Ҫ������ɫ
//void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
//{  
////	u16 height,width;
////	u16 i,j;
////	width=ex-sx+1; 			//�õ����Ŀ��
////	height=ey-sy+1;			//�߶�
//// 	for(i=0;i<height;i++)
////	{
//// 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
////		for(j=0;j<width;j++)
////		{
////			SPI_ReadWriteByte(SPI3,color[i*width+j]);//д������ 
////		}
////	}
////	
//	u16 height,width;
//	u16 i,j;
//	width=ex-sx+1; 			//�õ����Ŀ��
//	height=ey-sy+1;			//�߶�
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
//		for(j=0;j<width;j++)
//			LCD_WR_DATA16(color[i*width+j]);//д������ 
//	}		  
//} 


//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WR_DATA16(POINT_COLOR); 	    
}
//����
//color:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//��������
	LCD_WR_REG(0x2E);//����ָ��
	LCD_Read_DATA8();//��dummy
	LCD_Read_DATA8();//��dummy��һ����16λ��
	return ((LCD_Read_DATA8()&0x007c)<<9|(LCD_Read_DATA8()<<4)|(LCD_Read_DATA8()>>2));
}
//u16 LCD_ReadPoint(u16 x,u16 y)
//{
//	u16 color;
//	if(x>=LCD_W||y>=LCD_H)
//	{
//		return 0;	//�����˷�Χ,ֱ�ӷ���	
//	}
//	LCD_SetCursor(x,y);//���ù��λ�� 
//	color = Lcd_ReadData_16Bit();
//	return color;
//}
//���ٻ���
//x,y:����
//color:��ɫ
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
//��һ�����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}   
//








//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_H)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_W)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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









//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  











