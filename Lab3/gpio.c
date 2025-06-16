#include <stdint.h>

#include "tm4c1294ncpdt.h"
#include "lab3.h"

#define GPIO_PORTA (0x0001) // bit 1
#define GPIO_PORTE (0x0010) // bit 5
#define GPIO_PORTF (0x0020) // bit 6
#define GPIO_PORTJ (0x0100) // bit 8
#define GPIO_PORTK (0x0200) // bit 9
#define GPIO_PORTL (0x0400) // bit 10
#define GPIO_PORTM (0x0800) // bit 11
#define GPIO_PORTN (0x1000) // bit 12
#define GPIO_PORTP (0x2000) // bit 13
#define GPIO_PORTQ (0x4000) // bit 14
#define GPIO_PORTH (0x0080) // bit 7

// -------------------------------------------------------------------------------
// Funçãoo GPIO_Init
void GPIO_Init(void)
{
	const uint32_t or_portas = (GPIO_PORTA |
								GPIO_PORTE |
								GPIO_PORTF |
								GPIO_PORTJ |
								GPIO_PORTK |
								GPIO_PORTL |
								GPIO_PORTM |
								GPIO_PORTN |
								GPIO_PORTP |
								GPIO_PORTQ |
								GPIO_PORTH);
	// 1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	// SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN);
	SYSCTL_RCGCGPIO_R = or_portas;

	// 1b. Após isso verificar no PRGPIO se a porta est? pronta para uso.
	// while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTN) ) != (GPIO_PORTJ | GPIO_PORTN) ){};
	while ((SYSCTL_PRGPIO_R & or_portas) != or_portas){};

	// 2. Limpar o AMSEL para desabilitar a anal?gica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTE_AHB_AMSEL_R = 0x10 | 0x03;
	GPIO_PORTF_AHB_AMSEL_R = 0x0C;
	GPIO_PORTH_AHB_AMSEL_R = 0x00;
	GPIO_PORTK_AMSEL_R = 0x00;
	GPIO_PORTL_AMSEL_R = 0x00;
	GPIO_PORTM_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTP_AMSEL_R = 0x00;
	GPIO_PORTQ_AMSEL_R = 0x00;
	GPIO_PORTF_AHB_AMSEL_R = 0x00;

	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTA_AHB_PCTL_R |= 0x00000011; // ativa fun��o alternativa para poder usar uart
	GPIO_PORTH_AHB_PCTL_R = 0x00;
	GPIO_PORTK_PCTL_R = 0x00;
	GPIO_PORTL_PCTL_R = 0x00;
	GPIO_PORTM_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTP_PCTL_R = 0x00;
	GPIO_PORTQ_PCTL_R = 0x00;
	GPIO_PORTF_AHB_PCTL_R = 0x00;
	GPIO_PORTE_AHB_PCTL_R = 0x00;

	// 4. DIR para 1 se for entrada, 0 se for saída
	//	GPIO_PORTJ_AHB_DIR_R = 0x00;
	// GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1

	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTA_AHB_DIR_R = 0xF0 | 0x03;
	GPIO_PORTE_AHB_DIR_R = 0x00;
	GPIO_PORTH_AHB_DIR_R = 0x00;
	GPIO_PORTK_DIR_R = 0xFF; // LCD
	GPIO_PORTL_DIR_R = 0x00;
	GPIO_PORTM_DIR_R = 0xF7;	 // 2_11110111
	GPIO_PORTN_DIR_R = 0x03;	 // 2_0011
	GPIO_PORTP_DIR_R = 0x20;	 // 2_0010 0000
	GPIO_PORTQ_DIR_R = 0x0F;	 // 2_00001111
	GPIO_PORTF_AHB_DIR_R = 0x00; // 2_00000000

	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa

	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTA_AHB_AFSEL_R = 0x03; //
	GPIO_PORTE_AHB_AFSEL_R = 0x10; //
	GPIO_PORTH_AHB_AFSEL_R = 0x00; //
	GPIO_PORTK_AFSEL_R = 0x00;	   //
	GPIO_PORTL_AFSEL_R = 0x00;
	GPIO_PORTM_AFSEL_R = 0x00;	   //
	GPIO_PORTN_AFSEL_R = 0x00;	   //
	GPIO_PORTP_AFSEL_R = 0x00;	   //
	GPIO_PORTQ_AFSEL_R = 0x00;	   //
	GPIO_PORTF_AHB_AFSEL_R = 0x00; //

	// 6. Setar os bits de DEN para habilitar I/O digital

	GPIO_PORTJ_AHB_DEN_R = 0x03;
	GPIO_PORTA_AHB_DEN_R = 0xF0 | 0x03; // 2_11110000
	GPIO_PORTE_AHB_DEN_R |= (0x10 | 0X0F);
	// GPIO_PORTE_AHB_DEN_R |= 0x03;
	GPIO_PORTH_AHB_DEN_R = 0x0F; // 2_11110000
	GPIO_PORTK_DEN_R = 0xFF;	 // LCD
	GPIO_PORTL_DEN_R = 0x0F;
	GPIO_PORTM_DEN_R = 0xF7;	 // 2_11110111
	GPIO_PORTN_DEN_R = 0x03;	 // 2_0011
	GPIO_PORTP_DEN_R = 0x20;	 // 2_0010 0000
	GPIO_PORTQ_DEN_R = 0x0F;	 // 2_00001111
	GPIO_PORTF_AHB_DEN_R = 0x0C; // 2_00001111

	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03; // Bit0 e bit1
	GPIO_PORTL_DEN_R = 0x0F;
	// 8. SETANDO GPIOIM DA PORTA J
	GPIO_PORTJ_AHB_IM_R = 0x00;
	// 9. SETANDO GPIOIS DA PORTA J
	GPIO_PORTJ_AHB_IS_R = 0x00;
	// 10. SETANDO GPIOIBE DA PORTA J
	GPIO_PORTJ_AHB_IBE_R = 0x00;
	// 11. SETANDO GPIOIEV DA PORTA J
	GPIO_PORTJ_AHB_IEV_R = 0x03;
	// 12. SETANDO GPIOICR DA PORTA J
	GPIO_PORTJ_AHB_ICR_R = 0x03;
	// 13. SETANDO GPIOIM DA PORTA J
	GPIO_PORTJ_AHB_IM_R = 0x03;
	// 14. SETANDO NVIC EN1 DA PORTA J
	NVIC_EN1_R = (NVIC_EN1_R | 0x00080000);
	// 15. SETANDO NVIC PRI DA PORTA J
	NVIC_PRI12_R = (NVIC_PRI12_R | (0x05 << 29));
}