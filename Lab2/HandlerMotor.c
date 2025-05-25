#include "tm4c1294ncpdt.h"
#include <stdint.h>

void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

void GPIOInitMotor(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;  // Ativa clock da porta H
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R7) == 0) {};

    GPIO_PORTH_AHB_DIR_R |= 0x0F;   // PH0 - PH3 como saída
    GPIO_PORTH_AHB_DEN_R |= 0x0F;   // Habilita função digital
}


void MotorStepFullPasso(int step) {
    switch(step) {
        case 0: GPIO_PORTH_AHB_DATA_R = 0x01; break;  // PH0
        case 1: GPIO_PORTH_AHB_DATA_R = 0x02; break;  // PH1
        case 2: GPIO_PORTH_AHB_DATA_R = 0x04; break;  // PH2
        case 3: GPIO_PORTH_AHB_DATA_R = 0x08; break;  // PH3
    }
}

void MotorStepHalfPasso(int step) {
    const uint8_t half_seq[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};
    GPIO_PORTH_AHB_DATA_R = half_seq[step];
}

void TurnOnMotor(int sentido, int velocidade) {
    if(velocidade) {  // Passo completo
        for(int i = 0; i < 4; i++) {
            MotorStepFullPasso((sentido > 0) ? i : 3 - i);
            SysTick_Wait1ms(10);
        }
    } else {  // Meio passo
        for(int i = 0; i < 8; i++) {
            MotorStepHalfPasso((sentido > 0) ? i : 7 - i);
            SysTick_Wait1ms(10);
        }
    }
}