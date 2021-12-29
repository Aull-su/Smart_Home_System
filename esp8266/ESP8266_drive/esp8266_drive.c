/*!
================================================================================
@Project    : ESP8266 Configuration  
@Module Func: esp8266�ײ�����
--------------------------------------------------------------------------------
@File Name	: esp8266_device.c                                                                                           
@Copyright by su.ch                                                    
--------------------------------------------------------------------------------
Change Records                                                              
--------------------------------------------------------------------------------
	[No]       [Name]       [Date]      [Description]                         
   1         su.ch        2021/12/27   first                                     
================================================================================
*/

//!-----------------------------------------------------------------------------
//!-----------------------------> Includes  <-----------------------------------
//!-----------------------------------------------------------------------------
#include "esp8266_drive.h"
#include "SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "sta_tcpclent_test.h"
#include "esp8266_public.h"

//!-----------------------------------------------------------------------------
//!--------------------------> Global Structure   <-----------------------------
//!-----------------------------------------------------------------------------
struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };

//!=============================================================================
//!            Function Define Section(as bellows)                            
//!=============================================================================

/*!
================================================================================
@Brief
	init UART3.                                       
@Param
	uBaud	[the baud rate of UART3]                                                                    
@Return	
	None                                   
--------------------------------------------------------------------------------
*/
void USART3_Init(u32 uBaud)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                       //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                                 //�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                       //PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                            //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);                                           //��ʼ��GPIOB 11  
  
	//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                         //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                           //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                             //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                                 //����ָ���Ĳ�����ʼ��VIC�Ĵ���

  //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = uBaud;                                      //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                              //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 	               //�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);                                        //��ʼ������3

  USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);                     //�������ڽ��պ����߿����ж�
  	
	USART_Cmd(USART3, ENABLE);                                                       //ʹ�ܴ���3 	
}

/*!
================================================================================
@Brief
	UART3 handler.                                       
@Param
	void                                                                    
@Return	
	None                                   
--------------------------------------------------------------------------------
*/
void USART3_IRQHandler( void )
{	
	u8 ucCh;
	
	if(USART_GetITStatus( USART3, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( USART3 );
		
		if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
		{
			//Ԥ��1���ֽ�д������
			ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;	
		}                      
	}
	 	 
	if( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )  //����֡�������
	{
     ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
		
		 ucCh = USART_ReceiveData( USART3 );  //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
	
		 TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
  }	

}

/*!
================================================================================
@Brief
	init UART2.                                       
@Param
	uBaud	[the baud rate of UART3]                                                                    
@Return	
	None                                   
--------------------------------------------------------------------------------
*/
void USART2_Init(u32 uBaud)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                        //PA2  TXD
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                                 //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                        //PA3  RXD
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                            //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                           //��ʼ��GPIOA3  
  
	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                         //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                           //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                             //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                                 //����ָ���Ĳ�����ʼ��VIC�Ĵ���

  //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = uBaud;                                      //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                              //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 //�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);                                        //��ʼ������2

  USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);                     //�������ڽ��ܺ����߿����ж�
  	
	USART_Cmd(USART2, ENABLE);                                                       //ʹ�ܴ���2 	
}

/*!
================================================================================
@Brief
	UART2 handler.                                       
@Param
	void                                                                    
@Return	
	None                                   
--------------------------------------------------------------------------------
*/
void USART2_IRQHandler( void )
{	
	u8 ucCh;
	
	if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )  //�����ж�
	{
	  ucCh  = USART_ReceiveData( USART2 );
		
		if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
		{
			//Ԥ��1���ֽ�д������
		  ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;	
		}                      
	}
	 	 
	if( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )  //����֡�������
	{
     ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
		
		 ucCh = USART_ReceiveData( USART2 );  //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
	
		 TcpClosedFlag = strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;  //tcp���ӱ�־
  	}
}

/*!
================================================================================
@Brief
	Initial Esp8266 Configuration.                                       
@Param
	uBaud	[the baud rate of UART2]                                                                     
@Return	
	None                                   
-------------------------------------------------------------------------------
*/
void ESP8266_Init(u32 uBaud)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(ESP8266_RST_Pin_Periph_Clock|ESP8266_CH_PD_Pin_Periph_Clock, ENABLE);

	//PE6-RST
	GPIO_InitStructure.GPIO_Pin = ESP8266_RST_Pin;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(ESP8266_RST_Pin_Port, &GPIO_InitStructure);

	//PG7-EN
  GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_Pin;			   
	GPIO_Init(ESP8266_CH_PD_Pin_Port, &GPIO_InitStructure);
	
	//��ʼ��UART2
	USART2_Init(uBaud);
	
	//����������ʧ��
	ESP8266_RST_Pin_SetH;
	ESP8266_CH_PD_Pin_SetL;	
}


/*!
================================================================================
@Brief
	��ESP8266ģ�鷢��ATָ��.                                       
@Param
	cmd        [�����͵�ָ��]                               
	ack1,ack2  [�ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ]
  uTime      [�ȴ���Ӧ��ʱ��]
@Return	
	1  [success]
	0  [fail]
-------------------------------------------------------------------------------
*/
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 uTime)
{
	ESP8266_Fram_Record_Struct.InfBit .FramLength = 0;	 //���¿�ʼ�����µ����ݰ�
	
	ESP8266_USART("%s\r\n", cmd);
	if(ack1==0&&ack2==0)  //����Ҫ��������
	{
		return true;
	}
	delay_ms(uTime);  //��ʱuTimeʱ��

	ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';

	PC_USART("%s", ESP8266_Fram_Record_Struct .Data_RX_BUF);  //�˽ṹ������֡���������ATָ��ͷ��صĴ�����

	if(ack1!=0&&ack2!=0)
	{
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) || 
						 ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
	}
	else if( ack1 != 0 )
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) );
	
	else
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );		
}


/*!
================================================================================
@Brief
	����ESP8266ģ��.                                       
@Param
	void                                                                     
@Return	
	None                                   
-------------------------------------------------------------------------------
*/
void ESP8266_Rst(void)
{
	ESP8266_RST_Pin_SetL;  //low,reset
	delay_ms(500); 
	ESP8266_RST_Pin_SetH;  //��λ����֮�󣬸ߵ�ƽ��������
}


/*!
================================================================================
@Brief
	��ESP8266ģ�����AT��������                                       
@Param
	void                                     
@Return	
	None.                                  
-------------------------------------------------------------------------------
*/
void ESP8266_AT_Test(void)
{
	char count=0;
	
	//�������ţ�ȷ����������
	ESP8266_RST_Pin_SetH;
	delay_ms(1000);
	
	while(count < 10)
	{
		if(ESP8266_Send_AT_Cmd("AT","OK",NULL,500)) 
			return;
		ESP8266_Rst();
		++ count;
	}
}

/*!
================================================================================
@Brief
	ѡ��ESP8266ģ��Ĺ���ģʽ                                       
@Param
	enumMode  [STA/AP/STA_AP]                                     
@Return	
	1  [success]
  0  [fail]
--------------------------------------------------------------------------------
*/
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Send_AT_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Send_AT_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Send_AT_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
	}		
}


/*!
================================================================================
@Brief
	ESP8266ģ�������ⲿWiFi                                     
@Param
	pSSID      [WiFi�����ַ���]
  pPassWord  [WiFi�����ַ���]
@Return	
	1  [���ӳɹ�]
  0  [����ʧ��]
--------------------------------------------------------------------------------
*/
bool ESP8266_JoinAP( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 5000 );
	
}


/*!
================================================================================
@Brief
	ESP8266ģ������������                                   
@Param
	enumEnUnvarnishTx  [�����Ƿ������]
@Return	
	1  [���óɹ�]
  0  [����ʧ��]
--------------------------------------------------------------------------------
*/
bool ESP8266_Enable_MultipleId (FunctionalState enumEnUnvarnishTx )
{
	char cCmd [20];
	
	sprintf ( cCmd, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Send_AT_Cmd ( cCmd, "OK", 0, 500 );
	
}


/*!
================================================================================
@Brief
	ESP8266ģ�������ⲿ������                                   
@Param
	enumE  [����Э��]
  ip     [������IP]
  ComNum  [�������˿��ַ���]
  id      [ģ�����ӷ�������ID]
@Return	
	1  [���ӳɹ�]
  0  [����ʧ��]
--------------------------------------------------------------------------------
*/
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
	  case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Send_AT_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
}


/*!
================================================================================
@Brief
	����ESP8266ģ�����͸������                                   
@Param
	void
@Return	
	1  [���óɹ�]
  0  [����ʧ��]
--------------------------------------------------------------------------------
*/
bool ESP8266_UnvarnishSend ( void )
{
	if (!ESP8266_Send_AT_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ))
		return false;
	
	return 
	  	ESP8266_Send_AT_Cmd( "AT+CIPSEND", "OK", ">", 500 );
}


/*!
================================================================================
@Brief
	ESP8266ģ�鷢���ַ���                                   
@Param
	enumEnUnvarnishTx  [�����Ƿ���ʹ����͸��ģʽ]
  pStr               [Ҫ���͵��ַ���]
  ulStrLength        [Ҫ���͵��ַ������ֽ���]
  ucId               [�ĸ�ID���͵��ַ���]
@Return	
	1  [���ͳɹ�]
  0  [����ʧ��]
--------------------------------------------------------------------------------
*/
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if ( enumEnUnvarnishTx )
	{
		ESP8266_USART ( "%s", pStr );
		
		bRet = true;
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Send_AT_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Send_AT_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;
}


/*!
================================================================================
@Brief
	ESP8266ģ���˳�͸��ģʽ                                   
@Param
	void
@Return	
	None.
--------------------------------------------------------------------------------
*/
void ESP8266_ExitUnvarnishSend ( void )
{
	delay_ms(1000);
	ESP8266_USART( "+++" );
	delay_ms( 500 ); 	
}


/*!
================================================================================
@Brief
	ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��                                 
@Param
	void
@Return	
	0  [��ȡ״̬ʧ��]
  2  [���ip]
  3  [��������]
  4  [ʧȥ����]
--------------------------------------------------------------------------------
*/
u8 ESP8266_Get_LinkStatus ( void )
{
	if (ESP8266_Send_AT_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		
	}
	
	return 0;
}

