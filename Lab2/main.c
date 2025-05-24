// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include "tm4c1294ncpdt.h"

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);

void handleTransistor(int signal);
void GPIOInitLed(void);
void acendeLed(int numLeds);

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIOInitLed();
	handleTransistor(1);
	for(int i = 1; i > 9; i++){
		acendeLed(i);
		SysTick_Wait1ms(5000);
		if (i == 8){
			i = 1;
		}
	}
}


