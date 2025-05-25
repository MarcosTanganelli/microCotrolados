// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include "tm4c1294ncpdt.h"
#include <string.h>

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);

void handleTransistorLed(int signal);
void GPIOInitLed(void);
void acendeLed(int numLeds);
void insertMessageLCD(char message[2][16]);  // ? CERTO
void GPIOInitLCD(void);
char scanKeypad(void);
void GPIOInitTeclado(void);
void GPIOInitLedPlaca(void);
void GPIOInitMotor(void);

void wishedMessage(int grauValue, int sentido, int velocidade, int fim){
	char message[2][16];
	// Inicialização message 
	for(int row = 0; row < 2; row++) {
		for(int col = 0; col < 16; col++) {
			message[row][col] = ' ';
		}
	}
	if (fim == 1){
		strcpy(message[0], "FIM");
		
	}
	else{
		sprintf(message[0], "G:%03d S:%s", grauValue, sentido == 0 ? "H" : "AH");
		// Monta segunda linha: V:MP ou V:P
		sprintf(message[1], "V:%s", velocidade == 0 ? "P" : "MP");
	
	}
	insertMessageLCD(message);
}


		//sprintf(message[0], "Grau:%03d Sent:%s", grauValue, sentido == 0 ? "H" : "AH");
		// Monta segunda linha: V:MP ou V:P
		//sprintf(message[1], "Velo:%s----------", velocidade == 0 ? "P" : "MP");



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
	int i = 1;
	while(1){
			//char tecla = scanKeypad();
        //if (tecla != 0) {
					// Processa imediatamente
				//	wishedMessage(tecla - '0', 1, 15, 0);
					// Espera até tecla ser solta, para evitar múltiplas leituras rápidas
        //}
			//acendeLed(i);
			//SysTick_Wait1ms(2000);
			//i++;
			//if(i > 8){
			//		i = 1;
			//}
	}
}




