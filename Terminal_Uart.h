#ifndef TERMINAL_UART_H
#define TERMINAL_UART_H

#include "stm32f7xx_hal.h"
#include "Serial.h"

void Terminal_Uart_Inicializar(void);
void Terminal_Uart_EnviarComando(UART_HandleTypeDef * UARTEnviar);
uint8_t Terminal_Uart_Recibir(UART_HandleTypeDef * UARTRecibir);
uint8_t Terminal_Uart_GetCharRx(void);
void Terminal_Uart_Atencion(UART_HandleTypeDef UART);

extern eSerial gsSerial;

typedef enum
{
	UartBusy,
	UartIdle, 
	UartWaiting
	
}eEstadoUart;


typedef union
{
	uint8_t all;
	
	struct
	{
		uint8_t Enviando		 				: 1;
		uint8_t Recibiendo	 				: 1;
		uint8_t ComandoPendiente  	: 1;
		uint8_t Bit3								:	1;
		uint8_t Bit4								:	1;
		uint8_t Bit5								:	1;
		uint8_t Bit6								:	1;
		uint8_t Bit7								:	1;
	};
}eFlags;



typedef struct
{
	eFlags Flag;
	uint8_t CharRx;
	uint8_t BufferComando[100];
}sTerminalUart;

sTerminalUart gsTerminalUart;

//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
void Terminal_Uart_Inicializar(void)
{
	Serial_Iniciar();
	gsTerminalUart.Flag.all = 0x00;
	gsSerial.InitBuzzer(GPIOI, GPIO_PIN_3);
}



//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
void Terminal_Uart_EnviarComando(UART_HandleTypeDef * UARTEnviar)
{
	int8_t Status;
	if(gsTerminalUart.Flag.Recibiendo == true)return;
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	Status = gsSerial.getString(gsTerminalUart.BufferComando);
	if(Status  == SIN_CADENA) return;
	if(Status  == SerialBusy) 
	{
		gsTerminalUart.Flag.Enviando = true;
		return;
	}
	HAL_UART_Transmit(UARTEnviar, gsTerminalUart.BufferComando,Status,50);
	gsTerminalUart.Flag.Enviando = false;
}

//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
uint8_t Terminal_Uart_Recibir(UART_HandleTypeDef * UARTRecibir)
{
	uint8_t CaracterRecibido = 0x00;
	if(gsTerminalUart.Flag.Enviando == true) return(UartWaiting);
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	HAL_UART_Receive(UARTRecibir, &CaracterRecibido,1,100);
	gsTerminalUart.CharRx = CaracterRecibido;
	if(CaracterRecibido == 0x00) 
	{
		gsTerminalUart.Flag.ComandoPendiente = false;
		return(UartIdle);
	}
	if(CaracterRecibido != '\n')
	{ 
		gsTerminalUart.Flag.Recibiendo = true;
		printf("%c", CaracterRecibido);
		return(UartBusy);
	}
	gsTerminalUart.Flag.Recibiendo = false;
	gsTerminalUart.Flag.ComandoPendiente = true;
	printf("\n");
	return(UartBusy);
}

uint8_t Terminal_Uart_GetCharRx(void)
{
	return gsTerminalUart.CharRx;
}

//---------------------------------------------------
//
//
//
//
//---------------------------------------------------
void Terminal_Uart_Atencion(UART_HandleTypeDef UART)
{
	Terminal_Uart_EnviarComando(&UART);
	Terminal_Uart_Recibir(&UART);
	if(gsTerminalUart.Flag.all & 0x03) HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
}

#endif
