#ifndef TERMINAL_UART_H
#define TERMINAL_UART_H

#include "stm32f7xx_hal.h"
#include "Serial.h"

//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
void Terminal_Uart_Enviar(UART_HandleTypeDef *UARTEnviar)
{
		uint8_t * Terminal_Uart_Mensaje;
	
	int8_t Status = Serial_getString(Terminal_Uart_Mensaje);
	if( Status != SIN_CADENA)
	{
		HAL_UART_Transmit(UARTEnviar, Terminal_Uart_Mensaje, Status, 100);
	}
}

//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
void Terminal_Uart_Recibir(UART_HandleTypeDef *UARTRecibir)
{
	uint8_t * Terminal_Uart_Rx;

	if(HAL_UART_Receive(UARTRecibir, Terminal_Uart_Rx,1,100) == HAL_TIMEOUT)
	{
		return;
	}
	printf("%c",*Terminal_Uart_Rx);
	Terminal_Uart_Rx++;
	
}

//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
void Terminal_Uart_Atencion(UART_HandleTypeDef *UART)
{
	Serial_Atencion();
	Terminal_Uart_Enviar(UART);
	Terminal_Uart_Recibir(UART);
}


#endif
