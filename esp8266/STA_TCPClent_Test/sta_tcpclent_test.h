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
#define User_ESP8266_SSID	             "HONOR_X10MAX"	      //Ҫ���ӵ��ȵ������
#define User_ESP8266_PWD	             "syh20010406"	      //Ҫ���ӵ��ȵ������

#define User_ESP8266_TCPServer_IP	     "192.168.43.126"	    //Ҫ���ӵķ�������IP
#define User_ESP8266_TCPServer_PORT	   "8080"	              //Ҫ���ӵķ������Ķ˿�

#else
//#define User_ESP8266_SSID	             "LMX202-2.4G"	      //Ҫ���ӵ��ȵ������
//#define User_ESP8266_PWD	             "LMX20031231"	      //Ҫ���ӵ��ȵ������

//#define User_ESP8266_TCPServer_IP	     "192.168.0.106"	    //Ҫ���ӵķ�������IP
//#define User_ESP8266_TCPServer_PORT	   "8080"	              //Ҫ���ӵķ������Ķ˿�

#define User_ESP8266_SSID	             "1005~2"	            //Ҫ���ӵ��ȵ������
#define User_ESP8266_PWD	             "1234567890"	        //Ҫ���ӵ��ȵ������

#define User_ESP8266_TCPServer_IP	     "192.168.0.106"	    //Ҫ���ӵķ�������IP
#define User_ESP8266_TCPServer_PORT	   "8080"	              //Ҫ���ӵķ������Ķ˿�

#endif


extern volatile uint8_t TcpClosedFlag;  //����һ��ȫ�ֱ���


void ESP8266_STA_TCPClient_Test(void);



#endif
