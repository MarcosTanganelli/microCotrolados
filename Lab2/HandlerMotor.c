#include "tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>


void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void acendeLedEsquerda(int numLeds);
void acendeLedDireita(int numLeds);
void insertMessageLCD(char message[2][16]);
void GPIOInitMotor(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;  // Ativa clock da porta H
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R7) == 0) {};

    GPIO_PORTH_AHB_DIR_R |= 0x0F;   // PH0 - PH3 como saída
    GPIO_PORTH_AHB_DEN_R |= 0x0F;   // Habilita função digital
}

const uint8_t seq_passo[4] = {0x0E, 0x0D, 0x0B, 0x07};
const uint8_t half_seq_passo[8] = {0x0E, 0x0C, 0x0D, 0x09, 0x0B, 0x03, 0x07, 0x0E};


void TurnOnMotor(int sentido, int velocidade, int grau) {
	// velocidade -> indica o meio passo ou passo completo
	// velocidade = 1 -> passo || velocidade = 0 -> meio passo
	// grau -> indica quantos graus tem que girar
	// sentido -> indica horario ou anti-horario
	// sentido = 1 -> horario || sentido = 0 -> anti-horario
	// retorna um valor ao acabar, para setar a flag do led
	int passos = grau * 0.35;
	int grauAnteriorLCD = -15;
	int grauAtual = 0;
	char message[2][16];

	//int totalLeds = grau / 45;
    if(velocidade) {  // Passo completo
			for(int j = 0; j < passos; j++){
        for(int i = 0; i < 4; i++) {
					grauAtual = (i + j) / 1.4;
					if ( sentido > 0) {
						acendeLedDireita(grauAtual/45);
						GPIO_PORTH_AHB_DATA_R = seq_passo[i];
					}else{
						acendeLedEsquerda(grauAtual/45);
						GPIO_PORTH_AHB_DATA_R = seq_passo[3-i];
					}
					
					if (grauAtual - grauAnteriorLCD >= 15) {
						grauAnteriorLCD = grauAtual;
						sprintf(message[0], "Sent:%s Vel:%s", sentido ? "H" : "AH", velocidade ? "1" : "0");
						sprintf(message[1], "Grau:%03d", grauAtual);
						insertMessageLCD(message);
					}
					
					SysTick_Wait1ms(5);
        }
			}
    } else {  // Meio passo
			for(int j = 0; j < passos / 2; j++){
        for(int i = 0; i < 8; i++) {
					grauAtual = (i + j) / 1.4;
					if ( sentido > 0){
						acendeLedDireita(grauAtual/45);
						GPIO_PORTH_AHB_DATA_R = half_seq_passo[i];
					}else{
						acendeLedEsquerda(grauAtual/45);
						GPIO_PORTH_AHB_DATA_R = half_seq_passo[7-i];
					}
					
					if (grauAtual - grauAnteriorLCD >= 15) {
						grauAnteriorLCD = grauAtual;
						sprintf(message[0], "Sent:%s Vel:%s", sentido ? "H" : "AH", velocidade ? "1" : "0");
						sprintf(message[1], "Grau:%03d", grauAtual);
						insertMessageLCD(message);
					}
					SysTick_Wait1ms(5);
        }
    }
	}
}