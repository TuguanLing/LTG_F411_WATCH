#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tft.h"
#include "key.h"  
#include "string.h"	
#include "math.h"	
#include "timer.h"

#include "lvgl.h"
#include "lv_port_disp.h"

#include "lv_img_test.h"
	
void Main_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(180);  //初始化延时函数（100超频180）
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化  
	TIM3_Int_Init(999,71);		//定时器初始化（1ms中断），用于给lvgl提供1ms的心跳节拍
	
	lv_init();//lvgl系统初始化
	lv_port_disp_init();	//lvgl显示接口初始化,放在lv_init()的后面
}

int main(void)
{        
	Main_Init();//初始化函数
	
	lv_img_test_start();	//运行例程
	
	while(1)
	{
		lv_task_handler();
		key_handler();//按键处理
	}	
}

