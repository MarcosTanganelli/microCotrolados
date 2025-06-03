#include "tm4c1294ncpdt.h"
#include <stdint.h>

void handleTransistorLed(int signal){
	if(signal == 1){
		// liga transistor
		GPIO_PORTP_DATA_R = 0x20;  // 0010 0000 acende o 5 pino
	}
	if(signal == 0){
		GPIO_PORTP_DATA_R &= ~0x20;;  // 0010 0000 Apaga o 5 pino
		// desliga trasistor
	}
	return;
}

void GPIOInitLed(void)
{
	uint32_t GPIO_PORTA = 0x01;
	uint32_t GPIO_PORTP = 0x2000;
	uint32_t GPIO_PORTQ = 0x4000;
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (GPIO_PORTA | GPIO_PORTP| GPIO_PORTQ);
	
	// Aguarda até que todas as portas estejam prontas
	while((SYSCTL_PRGPIO_R & (GPIO_PORTA | GPIO_PORTP | GPIO_PORTQ)) != (GPIO_PORTA | GPIO_PORTP | GPIO_PORTQ)) {};
		// ======================
	  // Configuração do Transistor 
		// ======================
		// Desbloqueia PORTP
		// Desbloqueia a PORTP
		GPIO_PORTP_LOCK_R = 0x4C4F434B;

		// Habilita o pino PP5
		GPIO_PORTP_CR_R = 0x20;  // Bit 5

		// Desativa função analógica
		GPIO_PORTP_AMSEL_R = 0;

		// Desativa função alternativa
		GPIO_PORTP_AFSEL_R = 0;

		// Habilita função digital no pino PP5
		GPIO_PORTP_DEN_R = 0x20;  // Bit 5

		// Define direção como saída para PP5
		GPIO_PORTP_DIR_R = 0x20;  // Bit 5


		// ======================
	  // Configuração da PORTA
		// ======================
		// Desbloqueia a porta com a "chave m?gica"
		GPIO_PORTA_AHB_LOCK_R =0x4C4F434B; // Valor fixo necess?rio para desbloqueio
		// Habilita os pinos PA4?PA7 para altera??es
		GPIO_PORTA_AHB_CR_R = 0xF; // 1111 0000 ? PA4?PA7
		// Desativa fun??o anal?gica (todos os bits = 0)
		GPIO_PORTA_AHB_AMSEL_R = 0;
		// Desativa fun??o alternativa (usaremos apenas fun??o GPIO)
		GPIO_PORTA_AHB_AFSEL_R = 0;
		//Habilita fun??o digital nos pinos PA4?PA7
		GPIO_PORTA_AHB_DEN_R =  0xF0;
		// Define dire??o como sa?da (DIR = 1) para PA4?PA7
		GPIO_PORTA_AHB_DIR_R = 0xF0;

	
		// ======================
		// Configuração da PORTQ
		// ======================

		// Desbloqueia a porta com a "chave m?gica"
		GPIO_PORTQ_LOCK_R =0x4C4F434B; //Valor fixo necess?rio para desbloqueio
		// Habilita os pinos PA0?PA3 para altera??es
		GPIO_PORTQ_CR_R = 0x0F;  // 0000 1111 ? PQ0?PQ3
		// Desativa fun??o anal?gica (todos os bits = 0)
		GPIO_PORTQ_AMSEL_R = 0;
		// Desativa fun??o alternativa (usaremos apenas fun??o GPIO)
	  GPIO_PORTQ_AFSEL_R = 0;
		//Habilita fun??o digital nos pinos PQ0-PQ3
		GPIO_PORTQ_DEN_R = 0x0F;
		//Define dire??o como sa?da (DIR = 1) para PQ0-PQ3
		GPIO_PORTQ_DIR_R = 0x0F;

	}

void acendeLedEsquerda(int numLeds){

	switch(numLeds){
		case 0:
      GPIO_PORTA_AHB_DATA_R = 0b00000000;   
			GPIO_PORTQ_DATA_R = 0b00000000;		
			break;
		case 1:
      GPIO_PORTA_AHB_DATA_R = 0b10000000;   
			GPIO_PORTQ_DATA_R = 0b00000000;		
			break;
		case 2:
			GPIO_PORTA_AHB_DATA_R = 0b11000000;  
			GPIO_PORTQ_DATA_R = 0b00000000;	
			break;
		case 3:
			GPIO_PORTA_AHB_DATA_R = 0b11100000;  
			GPIO_PORTQ_DATA_R = 0b00000000;	
			break;
		case 4:
			GPIO_PORTA_AHB_DATA_R = 0b11110000;
			GPIO_PORTQ_DATA_R = 0b00000000;			
			break;
		case 5:
			GPIO_PORTA_AHB_DATA_R = 0b11110000;
			GPIO_PORTQ_DATA_R = 0b00001000; 
 		
			break;
		case 6:
			GPIO_PORTA_AHB_DATA_R = 0b11110000;
			GPIO_PORTQ_DATA_R = 0b00001100;  
			break;
		case 7:
			GPIO_PORTA_AHB_DATA_R = 0b11110000;
			GPIO_PORTQ_DATA_R = 0b00001110;   
			break;
		case 8:
			GPIO_PORTA_AHB_DATA_R = 0b11110000;
			GPIO_PORTQ_DATA_R = 0b00001111;  
			break;
		default:
			break;
	}
}

void acendeLedDireita(int numLeds){
	switch(numLeds){
		case 0:
			GPIO_PORTA_AHB_DATA_R = 0b00000000;
			GPIO_PORTQ_DATA_R = 0b00000000;
			break;
		case 1:
			GPIO_PORTQ_DATA_R = 0b00000001;
			GPIO_PORTA_AHB_DATA_R = 0b00000000;
			break;
		case 2:
			GPIO_PORTQ_DATA_R = 0b00000011;
			GPIO_PORTA_AHB_DATA_R = 0b00000000;
			break;
		case 3:
			GPIO_PORTQ_DATA_R = 0b00000111;
			GPIO_PORTA_AHB_DATA_R = 0b00000000;
			break;
		case 4:
			GPIO_PORTQ_DATA_R = 0b00001111;
			GPIO_PORTA_AHB_DATA_R = 0b00000000;
			break;
		case 5:
			GPIO_PORTQ_DATA_R = 0b00001111;
			GPIO_PORTA_AHB_DATA_R = 0b00010000;
			break;
		case 6:
			GPIO_PORTQ_DATA_R = 0b00001111;
			GPIO_PORTA_AHB_DATA_R = 0b00110000;
			break;
		case 7:
			GPIO_PORTQ_DATA_R = 0b00001111;
			GPIO_PORTA_AHB_DATA_R = 0b01110000;
			break;
		case 8:
			GPIO_PORTQ_DATA_R = 0b00001111;
			GPIO_PORTA_AHB_DATA_R = 0b11110000;
			break;
		default:
			break;
	}
}