#include "esp8266_public.h"
#include <stdarg.h>



static char *itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */


/*!
================================================================================
@Brief
	使用UART发送AT指令具体实现                        
@Param
	USARTx  [UART1/UART2/UART3...]                               
	* Data  [the string of command]                                       
@Return	
  None.                                
-------------------------------------------------------------------------------
*/
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *pStr;
	int d; 
	char cBuf[16];
	const char *pBuf;

	va_list ap;
	va_start(ap, Data);

	while( * Data != 0 )  //判断是否到达字符串结束符
	{			                          
		if ( * Data == 0x5c )  //反斜杠'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //换行符
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{
			switch ( *++Data )
			{				
				case 's':										  //字符串
				pStr = va_arg(ap, const char *);			
				for ( ; *pStr; pStr++) 
				{
					USART_SendData(USARTx,*pStr);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				break;

				case 'd':			                //十进制
				d = va_arg(ap, int);
				itoa(d, cBuf, 10);
				for (pBuf = cBuf; *pBuf; pBuf++) 
				{
					USART_SendData(USARTx,*pBuf);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;	
				break;
				
				default:
				Data++;
				break;
				
			}		 
		}
		
		else USART_SendData(USARTx, *Data++);
		
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
		
	}
}




