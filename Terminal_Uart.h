#ifndef TERMINAL_UART_H
#define TERMINAL_UART_H

#include "stm32f7xx_hal.h"
#include "Serial.h"

void Terminal_Uart_Atencion(UART_HandleTypeDef *UART, uint8_t * Terminal_Uart_Mensaje, uint16_t Tamanio)
{
	
	//Para Recibir Mensaje desde Uart hasta Consola
	if(HAL_UART_Receive(UART, Terminal_Uart_Mensaje, Tamanio, 100) == HAL_OK)
	{
		Serial_ImprimirString(Terminal_Uart_Mensaje);
	}
	
	//Para enviar Mensaje desde Consola a Uart 
	Serial_Atencion();
	if(Serial_getString(Terminal_Uart_Mensaje) != SIN_CADENA)
	{
		HAL_UART_Transmit(UART, Terminal_Uart_Mensaje, Tamanio, 100);
	}

}

#endif
