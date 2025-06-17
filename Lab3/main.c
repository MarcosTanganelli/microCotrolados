// LAB 3 - CONTROLE DE POSIÇÃO DO SERVOMOTOR
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "uart.h"

void PLL_Init(void);
void GPIO_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

typedef enum {
    STATE_IDLE = 0,
    STATE_MENU,
    STATE_POTENTIOMETER,
    STATE_TERMINAL
} SystemState;

SystemState program_state = STATE_IDLE;

char read_char = 0;
// bool direction = 0;
// bool direction_target = 0;
uint32_t pwm_duty_cycle = 1;
uint32_t pwm_duty_cycle_target = 1;
const int degrees_array[9] = {-90,-60, -45, -30, 0, 30, 45, 60, 90};
void handle_idle_state();
int handle_input_degree(int number);
void handle_menu_state();
void handle_potentiometer_state();
void handle_terminal_state();
void ADC_Init(void);
uint32_t ADC0_InSeq0(void);


int main(void) {
    PLL_Init();
    SysTick_Init();
    GPIO_Init();
    UART_init();
    ADC_Init();

    while (1) {
        switch (program_state) {
            case STATE_IDLE:
                handle_idle_state();
                break;
            case STATE_MENU:
                handle_menu_state();
                break;
            case STATE_POTENTIOMETER:
                handle_potentiometer_state();
                break;
            case STATE_TERMINAL:
                handle_terminal_state();
                break;
            default:
                program_state = STATE_IDLE;
                break;
        }
    }
}

void handle_idle_state() {
    UART_send_string("Motor parado, pressione * para iniciar.");
    read_char = UART_read_char();
    while(read_char != '*');
    program_state = STATE_MENU;
}

void handle_menu_state() {
    UART_send_string("\nControle por terminal (t) ou potenciômetro (p):");
    char option = 0;
    while (option != 'p' && option != 't') {
        option = UART_read_char();
    }
    program_state = (option == 'p') ? STATE_POTENTIOMETER : STATE_TERMINAL;
}

void handle_potentiometer_state() {
    UART_send_string("\nModo potenciômetro ativo. Pressione 's' para parar.");
    while (1) {
        uint32_t adc_value = ADC0_InSeq0();
        char buffer[30];
        sprintf(buffer, "ADC Value: %u", adc_value);
        UART_send_string(buffer);
        if (UART_available()) {
            read_char = UART_read_char();
            if (read_char == 's') {
                program_state = STATE_IDLE;
                pwm_duty_cycle_target = 1;
                break;
            } else if (read_char == 't') {
                program_state = STATE_TERMINAL;
                pwm_duty_cycle_target = 1;
                break;
            }
        }
        SysTick_Wait1ms(1000);
    }
}

void handle_terminal_state() {

    UART_send_string("Selecione a nova posição do motor (1-9):\n");
    do {
        read_char = UART_read_char();
    } while (read_char < '1' || read_char > '9');

    // pwm_duty_cycle_target = (read_char == '1') ? 100 : (read_char - '0') * 10;
    int degree = degrees_array[read_char - '1'];
    while (1) {
        char buffer[20];
        sprintf(buffer, "Degree motor: %d\n", degree);
        UART_send_string(buffer);

        if (UART_available()) {
            read_char = UART_read_char();

             if (read_char == 's') {
                program_state = STATE_IDLE;
                pwm_duty_cycle_target = 1;
                break;
            } else if (read_char == 'p') {
                program_state = STATE_POTENTIOMETER;
                pwm_duty_cycle_target = 1;
                break;
            }
        }
        SysTick_Wait1ms(1000);
    }
}