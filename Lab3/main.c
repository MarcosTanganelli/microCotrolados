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
bool direction = 0;
bool direction_target = 0;
uint32_t pwm_duty_cycle = 1;
uint32_t pwm_duty_cycle_target = 1;

void handle_idle_state();
void handle_menu_state();
void handle_potentiometer_state();
void handle_terminal_state();

int main(void) {
    PLL_Init();
    SysTick_Init();
    GPIO_Init();
    UART_init();

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
    UART_send_string("\nMotor parado, pressione * para iniciar.\r\n");
    read_char = UART_read_char();
    while(read_char != '*');
    program_state = STATE_MENU;
}

void handle_menu_state() {
    UART_send_string("\nControle por terminal (t) ou potenciômetro (p):\n");
    char option = 0;
    while (option != 'p' && option != 't') {
        option = UART_read_char();
    }
    program_state = (option == 'p') ? STATE_POTENTIOMETER : STATE_TERMINAL;
}

void handle_potentiometer_state() {
    UART_send_string("\nModo potenciômetro ativo. Pressione 's' para parar.\n");
    while (program_state == STATE_POTENTIOMETER) {
        // TODO: Lógica de controle com ADC
        read_char = UART_read_char();
        if (UART_available() && read_char == 's') {
            program_state = STATE_IDLE;
        }
        SysTick_Wait1ms(1000);
    }
}

void handle_terminal_state() {
    UART_send_string("\nSentido horário (h) ou anti-horário (a)?\n");
    do {
        read_char = UART_read_char();
    } while (read_char != 'h' && read_char != 'a');

    direction_target = (read_char == 'h') ? 0 : 1;
    direction = direction_target;

    UART_send_string("\nSelecione a nova posição do motor (0-9):\n");
    do {
        read_char = UART_read_char();
    } while (read_char < '0' || read_char > '9');

    pwm_duty_cycle_target = (read_char == '0') ? 100 : (read_char - '0') * 10;

    while (1) {
        char buffer[80];
        sprintf(buffer, "Velocidade: %u%%, Direção: %s\r\n", pwm_duty_cycle, direction == 0 ? "Horário" : "Anti-horário");
        UART_send_string(buffer);

        if (UART_available()) {
            read_char = UART_read_char();
            if (read_char == 'h' || read_char == 'a') {
                direction_target = (read_char == 'h') ? 0 : 1;
                direction = direction_target;
            }
            if (read_char >= '0' && read_char <= '9') {
                pwm_duty_cycle_target = (read_char == '0') ? 100 : (read_char - '0') * 10;
            }
            if (read_char == 's') {
                program_state = STATE_IDLE;
                pwm_duty_cycle_target = 1;
                break;
            }
        }
        SysTick_Wait1ms(1000);
    }
}