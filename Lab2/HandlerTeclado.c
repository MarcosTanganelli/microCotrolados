	#include "tm4c1294ncpdt.h"
	#include <stdint.h>

	void SysTick_Wait1ms(uint32_t delay);
	void SysTick_Wait1us(uint32_t delay);

	void GPIOInitTeclado() {
			SYSCTL_RCGCGPIO_R |= (0x0400 | 0x0800); // PL e PM
			while((SYSCTL_PRGPIO_R & (0x0400 | 0x0800)) != (0x0400 | 0x0800));

			// Configura linhas PL0-PL3 como saída
			GPIO_PORTL_DIR_R &= 0xF0;
			GPIO_PORTL_DEN_R |= 0x0F;
			GPIO_PORTL_PUR_R |= 0x0F;  // se quiser pull-up

			// Configura colunas PM4-PM7 como entrada
			GPIO_PORTM_DIR_R |= 0xF0;
			GPIO_PORTM_DEN_R |= 0xF0;
	}

	char scanKeypad(void) {
			uint32_t valor;

			// 1. Garante todas as linhas em nível alto antes de alterar direção
			GPIO_PORTM_DATA_R |= 0xF0;

			// 2. Colocar todas as linhas como entrada (alta impedância)
			GPIO_PORTM_DIR_R &= ~0xF0;
			SysTick_Wait1ms(1);

			// VARRENDO A PRIMEIRA LINHA
			GPIO_PORTM_DIR_R |= 0b00010000; 
			GPIO_PORTM_DATA_R &= 0b11101111;

			SysTick_Wait1ms(1);
			valor = GPIO_PORTL_DATA_R & 0x0F;

			switch (valor) {
					case 0b1110:
							return '1';
					case 0b1101:
							return '4';
					case 0b1011:
							return '7';
					case 0b0111:
							return '*';
			}

			// VARRENDO A SEGUNDA LINHA
			GPIO_PORTM_DIR_R &= ~0b00010000;
			GPIO_PORTM_DIR_R |=  0b00100000;
			
			GPIO_PORTM_DATA_R &= ~0b00100000;  // força PM5 = 0
			
			SysTick_Wait1ms(1);
			valor = GPIO_PORTL_DATA_R & 0x0F;

			
			 switch (valor) {
					case 0b1110:
							return '2';
					case 0b1101:
							return '5';
					case 0b1011:
							return '8';
					case 0b0111:
							return '0';
			}
			
			// VARRENDO A TERCEIRA LINHA
			GPIO_PORTM_DIR_R &= ~0b00100000;
			GPIO_PORTM_DIR_R |=  0b01000000;
			
			GPIO_PORTM_DATA_R &= ~0b01000000;
			
			SysTick_Wait1ms(1);
			valor = GPIO_PORTL_DATA_R & 0x0F;

			switch (valor) {
					case 0b1110:
							return '3';
					case 0b1101:
							return '6';
					case 0b1011:
							return '9';
					case 0b0111:
							return '#';
			}
			
			// VARRENDO A QUARTA LINHA
			GPIO_PORTM_DIR_R &= ~0b01000000;
			GPIO_PORTM_DIR_R |= 0b10000000;
			
			GPIO_PORTM_DATA_R &= ~0b10000000;

			
			SysTick_Wait1ms(1);
			valor = GPIO_PORTL_DATA_R & 0x0F;

			switch (valor) {
					case 0b1110:
							return 'A';
					case 0b1101:
							return 'B';
					case 0b1011:
							return 'C';
					case 0b0111:
							return 'D';  // retorna nada
			}
			
			return 0;
}	
	
