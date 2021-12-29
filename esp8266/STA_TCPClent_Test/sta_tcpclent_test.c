#include "sta_tcpclent_test.h"
#include "SysTick.h"
#include "usart.h"
#include "esp8266_drive.h"


volatile u8 TcpClosedFlag = 0;


/*!
================================================================================
@Brief
	Set STA mode.                                       
@Param
	void                                                                     
@Return	
	None                                   
-------------------------------------------------------------------------------
*/
void ESP8266_STA_TCPClient_Test(void)
{
	u8 uRes;
	
	char cStr[100]={0};
	
	printf ( "\r\n正在配置ESP8266请耐心等待...\r\n" );

	//ESP8266硬件使能，EN拉高
	ESP8266_CH_PD_Pin_SetH;

	//初启动，执行AT测试指令
	ESP8266_AT_Test();
	
	//配置STA模式
	ESP8266_Net_Mode_Choose(STA);
	
	//ESP8266连接热点
	while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
	
	//多连接-Disable，只有单连接才支持透传模式
	ESP8266_Enable_MultipleId( DISABLE );
	
	//TCP/UDP连接
	while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
	
	//开启透传模式
	while(!ESP8266_UnvarnishSend());
	
	printf ( "\r\n配置ESP8266 OK！\r\n" );
	
	while ( 1 )
	{		
		//tcp透传模式发送字符串
		sprintf (cStr,"www.prechin.cn\r\n" );
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );
		delay_ms(1000);
		
		//检测是否失去连接
		if(TcpClosedFlag)
		{
			//退出透传模式
			ESP8266_ExitUnvarnishSend();
			
			do
			{
				//获取连接状态
				uRes = ESP8266_Get_LinkStatus();
			} 	
			while(!uRes);
			
			//确认失去连接后重连
			if(uRes == 4)
			{
				printf ( "\r\n请稍等，正在重连热点和服务器...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());					
		}
	}
		
}



