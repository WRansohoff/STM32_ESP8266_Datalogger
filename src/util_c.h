#ifndef _VVC_UTIL_C_H
#define _VVC_UTIL_C_H

#include "global.h"
#include "peripherals.h"

// C-languages utility method signatures.

// Core helper methods.
void init_clock(void);
void init_vars(void);
void init_pins(void);

// General-purpose utility methods.
uint8_t buffer_equals_string(char* buffer_start,
                             const char* string,
                             uint32_t len);
uint8_t buffer_equals_cstring(char* buffer_start,
                              const char* string);

// Wifi-specific utility methods.
void assemble_join_wifi_str(void);

#endif
