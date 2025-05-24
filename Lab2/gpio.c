// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTN  (0x1000) //bit 12

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
uint32_t flag = 1;
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTN);
	//1b.   ap?s isso verificar no PRGPIO se a porta est? pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTN) ) != ( GPIO_PORTN) ){};
	
	// 2. Limpar o AMSEL para desabilitar a anal?gica
	GPIO_PORTN_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTN_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for sa?da
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun??o alternativa	
	GPIO_PORTN_AFSEL_R = 0x00; 
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1

	SYSCTL_RCGCTIMER_R = 0x01;
	SYSCTL_PRTIMER_R = 0x01;
	TIMER2_CTL_R = 0x01;
	TIMER2_CFG_R = 0x00;
	TIMER2_TAMR_R = 0x01;
	TIMER2_TAILR_R = 59999999;
	TIMER2_TAPR_R = 0x00;
	TIMER2_ICR_R = 0x01;
	TIMER2_IMR_R = 0x01;
	NVIC_PRI5_R = 4 << 29;
	NVIC_EN0_R = 1 << 23;
	TIMER2_CTL_R = 0x01;
}	

// -------------------------------------------------------------------------------
// Função PortJ_Input
// Lê os valores de entrada do port J
// Parâmetro de entrada: Não tem
// Parâmetro de saída: o valor da leitura do port
uint32_t PortJ_Input(void)
{
	return GPIO_PORTJ_AHB_DATA_R;
}

// -------------------------------------------------------------------------------
// Função PortN_Output
// Escreve os valores no port N
// Parâmetro de entrada: Valor a ser escrito
// Parâmetro de saída: não tem
void PortN_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
    //agora vamos fazer o OR com o valor recebido na função
    temp = temp | valor;
    GPIO_PORTN_DATA_R = temp; 
}


void Invertepino(){
	if (flag == 1){
		PortN_Output(0x1);
		flag = 0;
	}else{
		PortN_Output(0x0);
		flag = 1;
	}
	
}

void Timer2A_Handler(){
	
	TIMER2_ICR_R = 0x01;
	Invertepino();
}

