#include "util_c.h"

// C-language utility method definitions.
// Setup the core system clock.
void init_clock(void) {
  // Initial clock setup.
  #ifdef VVC_F0
    // Reset the Flash 'Access Control Register', and
    // then set 1 wait-state and enable the prefetch buffer.
    // (The device header files only show 1 bit for the F0
    //  line, but the reference manual shows 3...)
    FLASH->ACR &= ~(0x00000017);
    FLASH->ACR |=  (FLASH_ACR_LATENCY |
                    FLASH_ACR_PRFTBE);
    // Configure the PLL to (HSI / 2) * 12 = 48MHz.
    // Use a PLLMUL of 0xA for *12, and keep PLLSRC at 0
    // to use (HSI / PREDIV) as the core source. HSI = 8MHz.
    RCC->CFGR  &= ~(RCC_CFGR_PLLMUL |
                    RCC_CFGR_PLLSRC);
    RCC->CFGR  |=  (RCC_CFGR_PLLSRC_HSI_DIV2 |
                    RCC_CFGR_PLLMUL12);
    // Turn the PLL on and wait for it to be ready.
    RCC->CR    |=  (RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY)) {};
    // Select the PLL as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_PLL);
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
    // The system clock is now 48MHz.
    core_clock_hz = 48000000;
  #elif VVC_L0
    // Set the Flash ACR to use 1 wait-state
    // and enable the prefetch buffer and pre-read.
    FLASH->ACR |=  (FLASH_ACR_LATENCY |
                    FLASH_ACR_PRFTEN |
                    FLASH_ACR_PRE_READ);
    // Enable the HSI oscillator, since the L0 series boots
    // to the MSI one.
    RCC->CR    |=  (RCC_CR_HSION);
    while (!(RCC->CR & RCC_CR_HSIRDY)) {};
    // Configure the PLL to use HSI16 with a PLLDIV of
    // 2 and PLLMUL of 4.
    RCC->CFGR  &= ~(RCC_CFGR_PLLDIV |
                    RCC_CFGR_PLLMUL |
                    RCC_CFGR_PLLSRC);
    RCC->CFGR  |=  (RCC_CFGR_PLLDIV2 |
                    RCC_CFGR_PLLMUL4 |
                    RCC_CFGR_PLLSRC_HSI);
    // Enable the PLL and wait for it to stabilize.
    RCC->CR    |=  (RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY)) {};
    // Select the PLL as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_PLL);
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
    // Set the global clock speed variable.
    core_clock_hz = 32000000;
  #elif VVC_F3
    // TODO
  #endif
}

/*
 * Define starting values for global variables.
 * No need to reset any buffers to 0s. They are in the
 * 'BSS' section of RAM, so the assembly startup logic has
 * already cleared them out.
 */
void init_vars(void) {
}

// Setup the starting pin settings.
void init_pins(void) {
  // Enable the GPIOA clock (buttons on pins A2-A7,
  // user LED on pin A12).
  // Enable the GPIOB clock (I2C1 used on pins B6/B7,
  // buzzer on pin B0).
  #if defined(VVC_F0) || defined(VVC_F3)
    RCC->AHBENR   |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
    // Enable the USART1 clock.
    //RCC->APB2ENR  |= RCC_APB2ENR_USART1EN;
    RCC->APB1ENR  |= RCC_APB1ENR_USART2EN;
  #elif VVC_L0
    RCC->IOPENR   |= RCC_IOPENR_IOPAEN;
    RCC->IOPENR   |= RCC_IOPENR_IOPBEN;
    // Enable the USART2 clock.
    RCC->APB1ENR  |= RCC_APB1ENR_USART2EN;
  #endif
  // Enable the TIM2 clock.
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  #if defined(VVC_F0) || defined(VVC_F3)
    // Setup GPIO pin A1 as push-pull output with pull-up.
    // Setup GPIO pins A2, A3 as alternate function 1.
    // A2 is TX (AF output), A3 is RX (floating input).
    GPIOA->AFR[0]  &= ~(GPIO_AFRL_AFSEL2 |
                        GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0]  |=  ((1 << GPIO_AFRL_AFSEL2_Pos) |
                        (1 << GPIO_AFRL_AFSEL3_Pos));
    GPIOA->MODER   &= ~(GPIO_MODER_MODER1 |
                        GPIO_MODER_MODER2 |
                        GPIO_MODER_MODER3);
    GPIOA->MODER   |=  ((1 << GPIO_MODER_MODER1_Pos) |
                        (2 << GPIO_MODER_MODER2_Pos) |
                        (2 << GPIO_MODER_MODER3_Pos));
    GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPDR1 |
                        GPIO_PUPDR_PUPDR2 |
                        GPIO_PUPDR_PUPDR3);
    GPIOA->PUPDR   |=  ((1 << GPIO_PUPDR_PUPDR1_Pos) |
                        (1 << GPIO_PUPDR_PUPDR2_Pos) |
                        (1 << GPIO_PUPDR_PUPDR3_Pos));
    GPIOA->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEEDR3_Pos));
  #elif VVC_L0
    // Setup GPIO pin A1 as push-pull output with pull-up.
    // Setup GPIO pins A2, A3 as alternate function 1.
    // A2 is TX (AF output), A3 is RX (floating input).
    GPIOA->AFR[0]  &= ~(GPIO_AFRL_AFRL2 |
                        GPIO_AFRL_AFRL3);
    GPIOA->AFR[0]  |=  ((4 << GPIO_AFRL_AFRL2_Pos) |
                        (4 << GPIO_AFRL_AFRL3_Pos));
    GPIOA->MODER   &= ~(GPIO_MODER_MODE1 |
                        GPIO_MODER_MODE2 |
                        GPIO_MODER_MODE3);
    GPIOA->MODER   |=  ((1 << GPIO_MODER_MODE1_Pos) |
                        (2 << GPIO_MODER_MODE2_Pos) |
                        (2 << GPIO_MODER_MODE3_Pos));
    GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD1 |
                        GPIO_PUPDR_PUPD2 |
                        GPIO_PUPDR_PUPD3);
    GPIOA->PUPDR   |=  ((1 << GPIO_PUPDR_PUPD1_Pos) |
                        (1 << GPIO_PUPDR_PUPD2_Pos) |
                        (1 << GPIO_PUPDR_PUPD3_Pos));
    GPIOA->OSPEEDR |=  ((3 << GPIO_OSPEEDER_OSPEED3_Pos));
  #endif
}

/*
 * Check whether the first N bytes of a memory location
 * equal the first N bytes of a given string.
 */
uint8_t buffer_equals_string(char* buffer_start,
                             const char* string,
                             uint32_t len) {
  uint32_t i;
  for (i = 0; i < len; ++i) {
    if (buffer_start[i] != string[i]) {
      // Mismatch; strings are not equal.
      return 0;
    }
  }
  // No mismatch yet, so the first N characters matched.
  return 1;
}

/*
 * Check whether a given memory location matches
 * a given "C String", which must be null-terminated.
 */
uint8_t buffer_equals_cstring(char* buffer_start,
                              const char* string) {
  uint32_t i = 0;
  while (string[i] != '\0') {
    if (buffer_start[i] != string[i]) {
      // Mismatch; the strings are not equal.
      return 0;
    }
    ++i;
  }
  // Reached the end of the string; they must be equal.
  return 1;
}

/*
 * Assemble the command to send to the ESP8266 for
 * joining a WiFi network. Stored in 'str_buf'.
 * This assumes that the string buffer is larger than
 * 255 bytes, and that the command won't be that long.
 */
void assemble_join_wifi_str(void) {
  snprintf((char*)str_buf, 16, "%s", "AT+CWJAP_DEF=\"\0");
  uint8_t i = 14;
  uint8_t wi = 0;
  while (WIFI_SSID[wi] != '\0') {
    str_buf[i] = WIFI_SSID[wi];
    ++wi;
    ++i;
  }
  str_buf[i] = '"';
  ++i;
  str_buf[i] = ',';
  ++i;
  str_buf[i] = '"';
  ++i;
  wi = 0;
  while (WIFI_PW[wi] != '\0') {
    str_buf[i] = WIFI_PW[wi];
    ++wi;
    ++i;
  }
  str_buf[i] = '"';
  ++i;
  str_buf[i] = '\r';
  ++i;
  str_buf[i] = '\n';
  ++i;
  str_buf[i] = '\0';
}
