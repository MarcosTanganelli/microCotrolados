
#include <stdint.h>
#include "lab3.h"

uint32_t reset;       // = 0x00;
uint32_t toggle_step; // = 0x00;

void set_toggle_step(uint32_t value){
    toggle_step = value;
}

uint32_t get_toggle_step(void){
    return toggle_step;
}

void set_reset(uint32_t value){
    reset = value;
}

uint32_t get_reset(void){
    return reset;
}
