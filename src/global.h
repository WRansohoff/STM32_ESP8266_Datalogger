#ifndef _VVC_GLOBAL_H
#define _VVC_GLOBAL_H

#include <stdio.h>

// Core includes.
#ifdef VVC_F0
  #include "stm32f0xx.h"
#elif VVC_L0
  #include "stm32l0xx.h"
#elif VVC_F3
  #include "stm32f3xx.h"
#endif

// Project includes.
#include "wifi_config.h"

// Define GPIOB pin mappings for software '4-wire' SPI interface.
#define PA_ESP_CHPD (1)

// Peripheral definitions.
#if defined(VVC_F0) || defined(VVC_F3)
  //#define VVC_UARTx     (USART1)
  #define VVC_UARTx     (USART2)
#elif VVC_L0
  #define VVC_UARTx     (USART2)
#endif

// Assembly methods.
extern void delay_cycles(unsigned int d);
extern void delay_us(unsigned int d);
extern void delay_ms(unsigned int d);
extern void delay_s(unsigned int d);
extern void pulse_out_pin(volatile void* gpiox_odr,
                          unsigned int pulse_pinmask,
                          unsigned int pulse_halfw,
                          unsigned int num_pulses);

// ----------------------
// Global variables and defines.
volatile uint32_t core_clock_hz;
#define MAX_RX_LEN      (1024)
volatile char     str_buf[MAX_RX_LEN];

#endif
