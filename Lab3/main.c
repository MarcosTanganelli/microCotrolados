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
void ADC_Init(void);
uint32_t ADC0_InSeq0(void);

int main(void)
{
	uint32_t conv;
	PLL_Init();
	SysTick_Init();
	ADC_Init();
	
	while(1){
		conv = ADC0_InSeq0();
		SysTick_Wait1ms(500);
	}
}




