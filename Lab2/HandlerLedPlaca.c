#include "tm4c1294ncpdt.h"
#include <stdint.h>

void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

void GPIOInitLedPlaca(){
	
		// 1. Ativar o clock da GPIO N
	SYSCTL_RCGCGPIO_R |= (1 << 12);  // Ativa clock da PORTN
	while ((SYSCTL_PRGPIO_R & (1 << 12)) == 0) {};  // Aguarda

	// 2. Configurar a GPIO N0 como saída
	GPIO_PORTN_DIR_R |= 0x01; 
	GPIO_PORTN_DEN_R |= 0x01;
	GPIO_PORTN_AMSEL_R = 0;
	GPIO_PORTN_AFSEL_R = 0;
	GPIO_PORTN_PCTL_R = 0;

	// 3. Ativar clock do TIMER2
	SYSCTL_RCGCTIMER_R |= (1 << 2);
	while ((SYSCTL_PRTIMER_R & (1 << 2)) == 0) {};

	// 4. Configurar TIMER2A
	TIMER2_CTL_R = 0x00;  // Desliga Timer durante configuração
	TIMER2_CFG_R = 0x00;  // Modo de 32 bits
	TIMER2_TAMR_R = 0x02; // Modo periódico
	TIMER2_TAILR_R = 6000000; // Ajuste: para 10ms com 60MHz ? para 100ms = 6000000
	TIMER2_ICR_R = 0x01;  // Limpa flag de timeout
	TIMER2_IMR_R = 0x01;  // Habilita interrupção de timeout

	// 5. Configurar NVIC
	NVIC_PRI5_R = (NVIC_PRI5_R & 0xFFFF1FFF) | (3 << 13); // Prioridade
	NVIC_EN0_R |= (1 << 23);  // IRQ 23 corresponde ao TIMER2A

	// 6. Habilitar o timer
	TIMER2_CTL_R |= 0x01;

}


void TIMER2A_Handler(void){
    TIMER2_ICR_R = 0x01;  // Limpa flag
    GPIO_PORTN_DATA_R ^= 0x01;  // Inverte o LED
}