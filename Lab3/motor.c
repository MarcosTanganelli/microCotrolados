#include "motor.h"

#define STEPS 2
#define TEMPO_MIN_MS 2   // Tempo mínimo entre passos (máxima velocidade)
#define TEMPO_MAX_MS 50  // Tempo máximo entre passos (velocidade mais lenta)

// Funções externas
extern void SysTick_Wait1ms(int delay);
extern void SysTick_Wait1us(int delay);
void PortH_Output(uint32_t valor);


void vel_control(){
    

    if(direction==direction_target){
        
        pwm_duty_cycle= (pwm_duty_cycle_target + pwm_duty_cycle)/STEPS+1;
        if(pwm_duty_cycle==0){
            pwm_duty_cycle=1;
            //disable motor
            // GPIO_PORTE_AHB_DATA_R &= 0xFFF0;
            GPIO_PORTF_AHB_DATA_R = 0x00;
        }
        else if(pwm_duty_cycle==100){
            pwm_duty_cycle=99;
            GPIO_PORTF_AHB_DATA_R = 0x04;
        }
        else{
            GPIO_PORTF_AHB_DATA_R = ~0x04;
        }

    }
    else{
        
        pwm_duty_cycle= ( 0 + pwm_duty_cycle)/STEPS;
        if(pwm_duty_cycle==0){
            direction=direction_target;
            pwm_duty_cycle= (pwm_duty_cycle_target + pwm_duty_cycle)/STEPS;
        }
        
    }
}

void PWM_SetDutyCycle(int vel){
    if (vel < 0) vel = 0;
    if (vel > 100) vel = 100;
    
    // Calcula o valor do PWM conforme a velocidade
    uint32_t pwm = (vel * 1023) / 100;
    
    // Configura o PWM
    PWM0_0_CMPA_R = pwm;

}

