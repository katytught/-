
#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "lcd.h"
#include "usmart.h"		
#include "hc05.h" 	 
#include "usart3.h" 	
#include "key.h" 	 
#include "string.h"	 

#define NULL 0
typedef unsigned int u32;

int main(void)
{ 
	
	u8 t;
	u8 key;
	u8 sendmask=0;
	u8 sendcnt=0;
	u8 sendbuf[20];	  
	u8 reclen=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);
	LED_Init();
	
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	usmart_dev.init(84); 		//��ʼ��USMART		

	delay_ms(1000);			//�ȴ�����ģ���ϵ��ȶ�
	
	GPIO_SetBits(GPIOD,GPIO_Pin_12);
	
 	while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��  
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_15);
		delay_ms(500);
		GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		delay_ms(500);
	}	 			
	
	GPIO_SetBits(GPIOD,GPIO_Pin_13);
	USART3_RX_STA=0;
	t=0;
	
 	while(1) 
	{
/*
		if(t==10)
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_14);
			u3_printf("%d\r\n",USART_RX_STA);		//���͵�����ģ��
			sendcnt++;
			if(sendcnt>99)
				sendcnt=0;	  
			t=0;     
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		}
*/	  
		if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  USART3_RX_BUF[reclen]=0;	 	//���������
 			USART3_RX_STA=0;
			if(USART3_RX_BUF[0]==0x31){ // if == '1'
				u3_printf("1 %s\r\n",USART3_RX_BUF);
			}else{
				u3_printf("0 %s\r\n",USART3_RX_BUF);
			}
		}	 
		if(USART_RX_STA&0X8000)			//���յ�һ��������
		{
 			reclen=USART_RX_STA&0X3FFF;	//�õ����ݳ���
		  USART_RX_BUF[reclen]=0;	 	//���������
 			USART_RX_STA=0;
			u3_printf("distance is %s\r\n",USART_RX_BUF);
			
			GPIO_SetBits(GPIOD,GPIO_Pin_15);
			delay_ms(200);
			printf("1");
			GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		}	 				
/*		
		GPIO_SetBits(GPIOD,GPIO_Pin_15);
		delay_ms(500);
		printf("hello world\n");
		GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		
	*/
		
		t++;
	}
	
	u3_printf("00000000\r\n");
}


/*
int main(void){
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	usmart_dev.init(84); 		//��ʼ��USMART
	delay_init(168);
	LED_Init();
	uart_init(115200);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_13);
	
	u8 t=0;
	u8 len=0;	
	
	if(USART_RX_STA&0x8000)
	{					   
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		delay_ms(100);
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		for(t=0;t<len;t++)
		{
			USART_SendData(USART1, 1);         //�򴮿�1��������
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		USART_RX_STA=0;
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		delay_ms(100);
	}
	
	GPIO_SetBits(GPIOD,GPIO_Pin_14);
	delay_ms(500);
	USART_SendData(USART1, 0);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	delay_ms(500);
}
*/
