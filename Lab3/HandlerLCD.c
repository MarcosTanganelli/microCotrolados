#include "tm4c1294ncpdt.h"
#include <stdint.h>

void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);


void pulseEnable(void) {
    GPIO_PORTM_DATA_R &= ~0x04;
}

void sendCommandLCD(int command){
	GPIO_PORTK_DATA_R = command;
	
	//Set RS=0 (PM0), R/W=0 (PM1)
	GPIO_PORTM_DATA_R = 0b100;
	SysTick_Wait1us(40);
	
	pulseEnable();
}

void sendData(int data){
	GPIO_PORTK_DATA_R = data;
	uint32_t reg = GPIO_PORTM_DATA_R;
	reg &= ~0x07;   // limpa PM0, PM1, PM2
	reg |= 0x05;    // seta PM0 e PM2
	GPIO_PORTM_DATA_R = reg;
	SysTick_Wait1us(40);
	pulseEnable();
	SysTick_Wait1us(2000);
}



void LCDInit(){
	// Inicialização do LCD
	sendCommandLCD(0x38);
	SysTick_Wait1us(2000);
	
	// ; Display ON, cursor ON
	sendCommandLCD(0x06);
	SysTick_Wait1us(2000);
	
	// ; Display ON, cursor ON
	sendCommandLCD(0x0E);
	SysTick_Wait1us(2000);
	
		
	// Clear display
	sendCommandLCD(0x01);
	SysTick_Wait1us(4000);
}


void GPIOInitLCD(void)
{
	uint32_t GPIO_PORTK = 0x0200;   // Porta K ? bit 9
	uint32_t GPIO_PORTM = 0x0800;   // Porta M ? bit 11
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R |= ( GPIO_PORTK | GPIO_PORTM );
	
	// Configurar os pinos como saída
	GPIO_PORTM_DIR_R = 0x07;// PM0, PM1, PM2
	
	GPIO_PORTK_DIR_R = 0xFF; // PK0–PK7
	
	// Habilitar função digital
	GPIO_PORTM_DEN_R = 0x07;
	GPIO_PORTK_DEN_R=  0xFF;
	LCDInit();
}



void insertMessageLCD(char message[2][16]){
    for(int row = 0; row < 2; row++) {

        if (row == 0) {
            // Opcional: garantir que está na primeira linha
            sendCommandLCD(0x80);
        } else if (row == 1) {
            // Move para a segunda linha
            sendCommandLCD(0xC0);
        }

        for(int col = 0; col < 16; col++) {
            sendData(message[row][col]);
        }
    }
}
	
