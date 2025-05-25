#include "tm4c1294ncpdt.h"
#include <stdint.h>

void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

void GPIOInitTeclado(){
	SYSCTL_RCGCGPIO_R |= (0x0400 | 0x0800); // PL e PM
	while((SYSCTL_PRGPIO_R & (0x0400 | 0x0800)) != (0x0400 | 0x0800));

	// Configura linhas PL0-PL3 como saída
	GPIO_PORTL_DIR_R |= 0x0F;
	GPIO_PORTL_DEN_R |= 0x0F;

	// Configura colunas PM4-PM7 como entrada
	GPIO_PORTM_DIR_R &= ~0xF0;
	GPIO_PORTM_DEN_R |= 0xF0;
	GPIO_PORTM_PUR_R |= 0xF0;  // se quiser pull-up
}

char scanKeypad(void) {
    const char keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int row = 0; row < 4; row++) {
        // 1. Garante todas as linhas em nível alto antes de alterar direção
        GPIO_PORTL_DATA_R |= 0x0F;

        // 2. Colocar todas as linhas como entrada (alta impedância)
        GPIO_PORTL_DIR_R &= ~0x0F;
        SysTick_Wait1ms(100);

        // 3. Colocar a linha atual em nível baixo
        GPIO_PORTL_DATA_R &= ~(1 << row);

        // 4. Colocar a linha atual como saída
        GPIO_PORTL_DIR_R |= (1 << row);
        SysTick_Wait1us(100);

        // 5. Ler colunas PM4-PM7
        uint8_t col_read = (GPIO_PORTM_DATA_R & 0xF0) >> 4;

        for (int col = 0; col < 4; col++) {
            if (!(col_read & (1 << col))) {  // Detectou tecla pressionada
                // Espera liberação
                while (!(GPIO_PORTM_DATA_R & (1 << (col + 4))));
                SysTick_Wait1ms(1);
                return keys[row][col];
            }
        }

        // 6. Colocar a linha de volta para entrada (alta impedância)
        GPIO_PORTL_DIR_R &= ~(1 << row);
        SysTick_Wait1us(100);
    }

    return 0;  // Nenhuma tecla detectada
}

