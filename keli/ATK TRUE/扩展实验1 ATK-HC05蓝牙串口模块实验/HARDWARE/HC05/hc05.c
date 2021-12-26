#include "delay.h" 			 
#include "usart.h" 			 
#include "usart3.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//ATK-HC05À¶ÑÀÄ£¿éÇı¶¯´úÂë	   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ĞŞ¸ÄÈÕÆÚ:2014/10/26
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2014-2024
//All rights reserved									  
//********************************************************************************
//ÎŞĞŞ¸ÄĞÅÏ¢							  
////////////////////////////////////////////////////////////////////////////////// 	   


//³õÊ¼»¯ATK-HC05Ä£¿é
// ³õÊ¼»¯KEY LED²»ÓÃ¹Ü
//·µ»ØÖµ:0,³É¹¦;1,Ê§°Ü.
u8 HC05_Init(void)
{
	u8 retry=10,t;	  		 
	u8 temp=1;
	
	GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOC, ENABLE);//Ê¹ÄÜGPIOC,GPIOFÊ±ÖÓ
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //LED¶ÔÓ¦Òı½Å
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//ÆÕÍ¨ÊäÈëÄ£Ê½
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  //GPIO_Init(GPIOC, &GPIO_InitStructure);//³õÊ¼»¯GPIOC0
 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//KEY¶ÔÓ¦Òı½Å
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  //GPIO_Init(GPIOF, &GPIO_InitStructure); //¸ù¾İÉè¶¨²ÎÊı³õÊ¼»¯PF6

	GPIO_SetBits(GPIOF,GPIO_Pin_6);
 	
	usart3_init(9600);	//³õÊ¼»¯´®¿Ú3Îª:9600,²¨ÌØÂÊ.
	usart6_init(9600);	// ³õÊ¼»¯´®¿Ú6Îª9600,²¨ÌØÂÊ
	
/*
	while(retry--)
	{
		HC05_KEY=1;					//KEYÖÃ¸ß,½øÈëATÄ£Ê½
		delay_ms(10);
		u2_printf("AT\r\n");		//·¢ËÍAT²âÊÔÖ¸Áî
		HC05_KEY=0;					//KEYÀ­µÍ,ÍË³öATÄ£Ê½
		for(t=0;t<10;t++) 			//×î³¤µÈ´ı50ms,À´½ÓÊÕHC05Ä£¿éµÄ»ØÓ¦
		{
			if(USART3_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART3_RX_STA&0X8000)	//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
		{
			temp=USART3_RX_STA&0X7FFF;	//µÃµ½Êı¾İ³¤¶È
			USART3_RX_STA=0;			 
			if(temp==4&&USART3_RX_BUF[0]=='O'&&USART3_RX_BUF[1]=='K')
			{
				temp=0;//½ÓÊÕµ½OKÏìÓ¦
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//¼ì²âÊ§°
*/
	return 0;	 
}	 
//»ñÈ¡ATK-HC05Ä£¿éµÄ½ÇÉ«
//·µ»ØÖµ:0,´Ó»ú;1,Ö÷»ú;0XFF,»ñÈ¡Ê§°Ü.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEYÖÃ¸ß,½øÈëATÄ£Ê½
		delay_ms(10);
		u3_printf("AT+ROLE?\r\n");	//²éÑ¯½ÇÉ«
		for(t=0;t<20;t++) 			//×î³¤µÈ´ı200ms,À´½ÓÊÕHC05Ä£¿éµÄ»ØÓ¦
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)break;
		}		
		HC05_KEY=0;					//KEYÀ­µÍ,ÍË³öATÄ£Ê½
		if(USART3_RX_STA&0X8000)	//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
		{
			temp=USART3_RX_STA&0X7FFF;	//µÃµ½Êı¾İ³¤¶È
			USART3_RX_STA=0;			 
			if(temp==13&&USART3_RX_BUF[0]=='+')//½ÓÊÕµ½ÕıÈ·µÄÓ¦´ğÁË
			{
				temp=USART3_RX_BUF[6]-'0';//µÃµ½Ö÷´ÓÄ£Ê½Öµ
				break;
			}
		}		
	}
	if(retry==0)temp=0XFF;//²éÑ¯Ê§°Ü.
	return temp;
} 							   
//ATK-HC05ÉèÖÃÃüÁî
//´Ëº¯ÊıÓÃÓÚÉèÖÃATK-HC05,ÊÊÓÃÓÚ½ö·µ»ØOKÓ¦´ğµÄATÖ¸Áî
//atstr:ATÖ¸Áî´®.±ÈÈç:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"µÈ×Ö·û´®
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦;ÆäËû,ÉèÖÃÊ§°Ü.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEYÖÃ¸ß,½øÈëATÄ£Ê½
		delay_ms(10);
		u3_printf("%s\r\n",atstr);	//·¢ËÍAT×Ö·û´®
		HC05_KEY=0;					//KEYÀ­µÍ,ÍË³öATÄ£Ê½
		for(t=0;t<20;t++) 			//×î³¤µÈ´ı100ms,À´½ÓÊÕHC05Ä£¿éµÄ»ØÓ¦
		{
			if(USART3_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART3_RX_STA&0X8000)	//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
		{
			temp=USART3_RX_STA&0X7FFF;	//µÃµ½Êı¾İ³¤¶È
			USART3_RX_STA=0;			 
			if(temp==4&&USART3_RX_BUF[0]=='O')//½ÓÊÕµ½ÕıÈ·µÄÓ¦´ğÁË
			{			
				temp=0;
				break;			 
			}
		}		
	}
	if(retry==0)temp=0XFF;//ÉèÖÃÊ§°Ü.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//Í¨¹ı¸Ãº¯Êı,¿ÉÒÔÀûÓÃUSMART,µ÷ÊÔ½ÓÔÚ´®¿Ú3ÉÏµÄATK-HC05Ä£¿é
//str:ÃüÁî´®.(ÕâÀï×¢Òâ²»ÔÙĞèÒªÔÙÊäÈë»Ø³µ·û)
void HC05_CFG_CMD(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;						//KEYÖÃ¸ß,½øÈëATÄ£Ê½
	delay_ms(10);
	u3_printf("%s\r\n",(char*)str); //·¢ËÍÖ¸Áî
	for(t=0;t<50;t++) 				//×î³¤µÈ´ı500ms,À´½ÓÊÕHC05Ä£¿éµÄ»ØÓ¦
	{
		if(USART3_RX_STA&0X8000)break;
		delay_ms(10);
	}									    
	HC05_KEY=0;						//KEYÀ­µÍ,ÍË³öATÄ£Ê½
	if(USART3_RX_STA&0X8000)		//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
	{
		temp=USART3_RX_STA&0X7FFF;	//µÃµ½Êı¾İ³¤¶È
		USART3_RX_STA=0;
		USART3_RX_BUF[temp]=0;		//¼Ó½áÊø·û		 
		printf("\r\n%s",USART3_RX_BUF);//·¢ËÍ»ØÓ¦Êı¾İµ½´®¿Ú1
	} 				 
}














