#ifndef _VVC_INTERRUPTS_C_H
#define _VVC_INTERRUPTS_C_H

#include "global.h"

#include "peripherals.h"
#include "util_c.h"

// C-language hardware interrupt method signatures.
// Handlers common to all supported lines of chip.
void TIM2_IRQ_handler(void);

#endif
