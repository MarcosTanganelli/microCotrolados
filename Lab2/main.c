// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include "tm4c1294ncpdt.h"
#include <string.h>
#include "globals.h"

volatile uint8_t flagPiscarLed = 0;
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);

void handleTransistorLed(int signal);
void GPIOInitLed(void);
void insertMessageLCD(char message[2][16]);  // ? CERTO
void GPIOInitLCD(void);
char scanKeypad(void);
void GPIOInitTeclado(void);
void GPIOInitLedPlaca(void);
void GPIOInitMotor(void);
void TurnOnMotor(int sentido, int velocidade, int grau);


int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIOInitLed();
	GPIOInitLedPlaca();
	GPIOInitLCD();
	GPIOInitTeclado();
	GPIOInitMotor();
	handleTransistorLed(1);

	char start = 'X';
	char digito_1 = 'X';
	char digito_2 = 'X';
	char digito_3 = 'X';
	char velocidade = 'X';
	char sentido = 'X';
	char message[2][16];
	sprintf(message[0], "Press: *");
	insertMessageLCD(message);
	
	while (start == 'X'){
		start = scanKeypad();
	}
	
	while (digito_1 == 'X'){
		digito_1 = scanKeypad();
	}
	sprintf(message[0], "G:%c__ S:_", digito_1);
	sprintf(message[1], "Velo:_");
	insertMessageLCD(message);
	while (digito_2 == 'X'){
		digito_2 = scanKeypad();
	}
	sprintf(message[0], "G:%c%c_ S:_", digito_1, digito_2);
	sprintf(message[1], "Velo:_");
	insertMessageLCD(message);
	while (digito_3 == 'X'){
		digito_3 = scanKeypad();
	}
	sprintf(message[0], "G:%c%c%c S:_", digito_1, digito_2, digito_3);
	sprintf(message[1], "Velo:_");
	insertMessageLCD(message);

	
	while (sentido == 'X'){
		sentido = scanKeypad();
	}
	
	sprintf(message[0], "G:%c%c%c S:%c", digito_1, digito_2, digito_3, sentido);
	sprintf(message[1], "Velo:_");
	insertMessageLCD(message);

	while (velocidade == 'X'){
		velocidade = scanKeypad();
	}
	sprintf(message[1], "Velo:%c", velocidade);
	insertMessageLCD(message);
	

	int grau = (digito_1 - '0') * 100 + (digito_2 - '0') * 10 + (digito_3 - '0');
	int sentido_int = sentido - '0';
	int velocidade_int = velocidade - '0';

	flagPiscarLed = 1;
	TurnOnMotor(sentido_int, velocidade_int, grau);
	flagPiscarLed = 0;
  sprintf(message[0], "FIM");
  insertMessageLCD(message);
  SysTick_Wait1ms(1000);
	sprintf(message[0], "Press: *");
	insertMessageLCD(message);
	start = 'X';
  digito_1 =  'X';
	digito_2 =  'X';
	digito_3 = 'X';
  velocidade =  'X';
	sentido = 'X';
}




