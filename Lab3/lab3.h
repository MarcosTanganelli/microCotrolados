#ifndef __LAB3__H__
#define __LAB3__H__

#include <stdint.h>
#include "tm4c1294ncpdt.h"

// extern uint32_t reset;// = 0x00;
// extern uint32_t toggle_step;// = 0x00;
void set_toggle_step(uint32_t value);

uint32_t get_toggle_step(void);

void set_reset(uint32_t value);

uint32_t get_reset(void);

#endif //!__LAB3__H__