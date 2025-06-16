#ifndef __UART__H__
#define __UART__H__

#include <stdint.h>
#include <stdbool.h>

#include "tm4c1294ncpdt.h"

void UART_init();
void UART_send_char(char c);
void UART_send_string(const char *string);
char UART_read_char();
bool UART_available();

#endif //!__UART__H__