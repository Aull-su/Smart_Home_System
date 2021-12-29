/*!
================================================================================
@Project    : ESP8266 Configuration  
@Module Func: esp8266底层驱动
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                                 //复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                       //PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                            //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);                                           //初始化GPIOB 11  
  
	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                         //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                           //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                             //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                                 //根据指定的参数初始化VIC寄存器

  //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = uBaud;                                      //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                              //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 	               //收发模式
  USART_Init(USART3, &USART_InitStructure);                                        //初始化串口3

  USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);                     //开启串口接收和总线空闲中断
  	
	USART_Cmd(USART3, ENABLE);                                                       //使能串口3 	
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
			//预留1个字节写结束符
			ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;	
		}                      
	}
	 	 
	if( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )  //数据帧接收完毕
	{
     ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
		
		 ucCh = USART_ReceiveData( USART3 );  //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
	
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                                 //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                        //PA3  RXD
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                            //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                           //初始化GPIOA3  
  
	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                         //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                           //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                             //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                                 //根据指定的参数初始化VIC寄存器

  //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = uBaud;                                      //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                              //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 //收发模式
  USART_Init(USART2, &USART_InitStructure);                                        //初始化串口2

  USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);                     //开启串口接受和总线空闲中断
  	
	USART_Cmd(USART2, ENABLE);                                                       //使能串口2 	
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
	
	if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )  //接收中断
	{
	  ucCh  = USART_ReceiveData( USART2 );
		
		if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
		{
			//预留1个字节写结束符
		  ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;	
		}                      
	}
	 	 
	if( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )  //数据帧接收完毕
	{
     ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
		
		 ucCh = USART_ReceiveData( USART2 );  //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
	
		 TcpClosedFlag = strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;  //tcp连接标志
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
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能时钟
	RCC_APB2PeriphClockCmd(ESP8266_RST_Pin_Periph_Clock|ESP8266_CH_PD_Pin_Periph_Clock, ENABLE);

	//PE6-RST
	GPIO_InitStructure.GPIO_Pin = ESP8266_RST_Pin;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(ESP8266_RST_Pin_Port, &GPIO_InitStructure);

	//PG7-EN
  GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_Pin;			   
	GPIO_Init(ESP8266_CH_PD_Pin_Port, &GPIO_InitStructure);
	
	//初始化UART2
	USART2_Init(uBaud);
	
	//正常工作、失能
	ESP8266_RST_Pin_SetH;
	ESP8266_CH_PD_Pin_SetL;	
}


/*!
================================================================================
@Brief
	对ESP8266模块发送AT指令.                                       
@Param
	cmd        [待发送的指令]                               
	ack1,ack2  [期待的响应，为NULL表不需响应，两者为或逻辑关系]
  uTime      [等待响应的时间]
@Return	
	1  [success]
	0  [fail]
-------------------------------------------------------------------------------
*/
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 uTime)
{
	ESP8266_Fram_Record_Struct.InfBit .FramLength = 0;	 //重新开始接收新的数据包
	
	ESP8266_USART("%s\r\n", cmd);
	if(ack1==0&&ack2==0)  //不需要接收数据
	{
		return true;
	}
	delay_ms(uTime);  //延时uTime时间

	ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';

	PC_USART("%s", ESP8266_Fram_Record_Struct .Data_RX_BUF);  //此结构体数据帧包含输入的AT指令和返回的处理结果

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
	重启ESP8266模块.                                       
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
	ESP8266_RST_Pin_SetH;  //复位启动之后，高电平正常工作
}


/*!
================================================================================
@Brief
	对ESP8266模块进行AT测试启动                                       
@Param
	void                                     
@Return	
	None.                                  
-------------------------------------------------------------------------------
*/
void ESP8266_AT_Test(void)
{
	char count=0;
	
	//拉高引脚，确保正常工作
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
	选择ESP8266模块的工作模式                                       
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
	ESP8266模块连接外部WiFi                                     
@Param
	pSSID      [WiFi名称字符串]
  pPassWord  [WiFi密码字符串]
@Return	
	1  [连接成功]
  0  [连接失败]
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
	ESP8266模块启动多连接                                   
@Param
	enumEnUnvarnishTx  [配置是否多连接]
@Return	
	1  [配置成功]
  0  [配置失败]
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
	ESP8266模块连接外部服务器                                   
@Param
	enumE  [网络协议]
  ip     [服务器IP]
  ComNum  [服务器端口字符串]
  id      [模块连接服务器的ID]
@Return	
	1  [连接成功]
  0  [连接失败]
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
	配置ESP8266模块进入透传发送                                   
@Param
	void
@Return	
	1  [配置成功]
  0  [配置失败]
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
	ESP8266模块发送字符串                                   
@Param
	enumEnUnvarnishTx  [声明是否已使能了透传模式]
  pStr               [要发送的字符串]
  ulStrLength        [要发送的字符串的字节数]
  ucId               [哪个ID发送的字符串]
@Return	
	1  [发送成功]
  0  [发送失败]
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
	ESP8266模块退出透传模式                                   
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
	ESP8266 的连接状态，较适合单端口时使用                                 
@Param
	void
@Return	
	0  [获取状态失败]
  2  [获得ip]
  3  [建立连接]
  4  [失去连接]
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

