#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H

//!-----------------------------------------------------------------------------
//!------------------------------> Includes  <----------------------------------
//!-----------------------------------------------------------------------------
#include "system.h"


/*------------------------------------------------------------------------------
                              Global Define
------------------------------------------------------------------------------*/
//#define HONOR_X10MAX

#ifdef HONOR_X10MAX
#define User_ESP8266_SSID	             "HONOR_X10MAX"	      //要连接的热点的名称
#define User_ESP8266_PWD	             "syh20010406"	      //要连接的热点的密码

#define User_ESP8266_TCPServer_IP	     "192.168.43.126"	    //要连接的服务器的IP
#define User_ESP8266_TCPServer_PORT	   "8080"	              //要连接的服务器的端口

#else
//#define User_ESP8266_SSID	             "LMX202-2.4G"	      //要连接的热点的名称
//#define User_ESP8266_PWD	             "LMX20031231"	      //要连接的热点的密码

//#define User_ESP8266_TCPServer_IP	     "192.168.0.106"	    //要连接的服务器的IP
//#define User_ESP8266_TCPServer_PORT	   "8080"	              //要连接的服务器的端口

#define User_ESP8266_SSID	             "1005~2"	            //要连接的热点的名称
#define User_ESP8266_PWD	             "1234567890"	        //要连接的热点的密码

#define User_ESP8266_TCPServer_IP	     "192.168.0.106"	    //要连接的服务器的IP
#define User_ESP8266_TCPServer_PORT	   "8080"	              //要连接的服务器的端口

#endif


extern volatile uint8_t TcpClosedFlag;  //定义一个全局变量


void ESP8266_STA_TCPClient_Test(void);



#endif
