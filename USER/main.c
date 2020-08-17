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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(180);  //��ʼ����ʱ������100��Ƶ180��
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED 
	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ��  
	TIM3_Int_Init(999,71);		//��ʱ����ʼ����1ms�жϣ������ڸ�lvgl�ṩ1ms����������
	
	lv_init();//lvglϵͳ��ʼ��
	lv_port_disp_init();	//lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
}

int main(void)
{        
	Main_Init();//��ʼ������
	
	lv_img_test_start();	//��������
	
	while(1)
	{
		lv_task_handler();
		key_handler();//��������
	}	
}

