#include "uart.h"
#include <stdbool.h>

#define UARTSysClk 80000000
#define BAUD_RATE 57600
#define GPIO_PORTA (0x0001) // bit 1

void UART_init(){

    // Habilita UART0
    // Habilita o clock da GPIOA (porta A)
    SYSCTL_RCGCUART_R |= 0x01; //ativa uart0
    while((SYSCTL_PRUART_R & 0x01) != 0x01 ){};

    UART0_CTL_R &= ~0x01;
    // Conta do baud rate que está no slide
    // int clk_div = ((UART0_CTL_R & 0x20) == 0) ? 16 : 8;
    // float baud_rate = UARTSysClk / (clk_div * BAUD_RATE);
    // baud_rate = 86.8111;

    // Parte inteira 16 bits
    UART0_IBRD_R = 86; // baudRate = 80M / (16*57600) = 86.8111
    UART0_FBRD_R = 52; // baudRate = (int)((baudRate - (int)baudRate) * 64 + 0.5f);

    // Configura numero de bits, paridade, stop bits e fila
    // SPS | WLEN1 | WLEN0 | FEN | STP2 | EPS | PEN | BRK 
    // 0 1 1 1 0 1 1 0
    UART0_LCRH_R = 0x76;
    // UART0_LCRH_R = (1 << 4) | (1 << 3) | (1 << 1) | (0x3 << 5);

    UART0_CC_R = 0x0; // Usa o clock do sistema

    // Hablita Tx (transmissor), Rx (recepetor),
    UART0_CTL_R |= 0x301;
}

void UART_send_char(char c){
    // Aguarda enquanto TX FIFO está cheio
    // E escreve o caractere no registrador de dados
    while (UART0_FR_R & (1 << 5));
    UART0_DR_R = c;
}

void UART_send_string(const char* string){
    while (*string){
        UART_send_char(*string++);
    }
}

char UART_read_char() {
    // Aguarda até que RX FIFO tenha dados
    // Retorna o caractere recebido
    while (UART0_FR_R & (1 << 4));
    return (char)UART0_DR_R;
}

// Função para verificar se há um caractere disponível na UART
bool UART_available(){
    return !(UART0_FR_R & 0x10);
}
