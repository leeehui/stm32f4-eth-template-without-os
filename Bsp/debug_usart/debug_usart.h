#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32_includes.h"

extern UART_HandleTypeDef huart1;

#define DEBUG_USART huart1



int fputc(int ch, FILE *f);



enum {serial_debug = 1}; 

enum {debug_info = 1}; 
enum {debug_error = 1};
enum {debug_warn = 1};
enum {debug_rgb = 0};


//Note : under EADK, __FUNCTION__ can not work !!!
#if 1

#define debug(filter, msg, ...)\
do\
{\
	if (debug_##filter)\
	{\
		if (serial_debug)\
		{\
			printf("Debug > %s(%d) " msg "\n",\
			__func__, __LINE__, ##__VA_ARGS__);\
		}\
	}\
}while(0) 


#endif


#endif /* __USART1_H */
