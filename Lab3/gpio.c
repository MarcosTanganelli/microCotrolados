// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTE  (0x10) //bit 4

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
uint32_t flag = 1;
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTE);
	// SYSCTL_RCGCADC_R = -> Ativar clock 
	// SYSCTL_PRADC_R -> Verificar se esta preparado para uso
	//1b.   ap?s isso verificar no PRGPIO se a porta est? pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTE) ) != ( GPIO_PORTE) ){};
	
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

