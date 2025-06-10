#include <stdint.h>
#include "tm4c1294ncpdt.h"
#define GPIO_PORTE  (0x10) //bit 4


void ADC_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTE);
	// SYSCTL_RCGCADC_R = -> Ativar clock 
	// SYSCTL_PRADC_R -> Verificar se esta preparado para uso
	//1b.   ap?s isso verificar no PRGPIO se a porta est? pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTE) ) != ( GPIO_PORTE) ){};
	
	GPIO_PORTE_AHB_AMSEL_R |= 0x10;
		
	GPIO_PORTE_AHB_PCTL_R = 0x00000000;
		
	GPIO_PORTE_AHB_DIR_R = 0x00;
	GPIO_PORTE_AHB_AFSEL_R |= 0x10;
	GPIO_PORTE_AHB_DEN_R &= 0x10;
	SYSCTL_RCGCADC_R |= 0x01;
	while((SYSCTL_PRADC_R & (0x01)) != (0x01) ){};
		
	ADC0_PC_R = 0x01;
		
	ADC0_SSPRI_R = 0x0123;
		
	ADC0_ACTSS_R &= ~0x0008;
	ADC0_EMUX_R &= ~0xF000;
	ADC0_SSMUX3_R = 9;
	ADC0_SSCTL3_R = 0x0006;
	ADC0_IM_R &= ~0x0008;	
		
	ADC0_ACTSS_R |= 0x0008;		
}

uint32_t ADC0_InSeq0(void)
{
	uint32_t resultado;
	ADC0_PSSI_R = 0x0008;
	
	while((ADC0_RIS_R & 0x08) == 0){};
	resultado = ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008; //ACK
	return resultado;
}


