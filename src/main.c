#include "main.h"

/**
 * Main program.
 */
int main(void) {
  // Setup the system clock frequency.
  init_clock();
  // Define starting values for global variables.
  init_vars();
  // Setup pin definitions.
  init_pins();

  // Enable the NVIC interrupt for TIM2.
  // (Timer peripheral initialized and used elsewhere)
  NVIC_SetPriority(TIM2_IRQn, 0x03);
  NVIC_EnableIRQ(TIM2_IRQn);

  // Initialize the USART peripheral in async mode.
  // (Default baud rate is 115200, but I slowed this ESP8266 down)
  //init_uart(VVC_UARTx, 115200);
  init_uart(VVC_UARTx, 9600);

  // Turn the ESP8266 module off, to start.
  //GPIOA->ODR &= ~(1 << PA_ESP_CHPD);
  // ...Then turn it back on, for testing.
  GPIOA->ODR |=  (1 << PA_ESP_CHPD);

  // Wait a bit to let the chip start up.
  delay_ms(100);

  // Set the baud rate to 9600 if necessary.
  /*
  // Enable the USART transmit/receive pins.
  VVC_UARTx->CR1 |=  (USART_CR1_TE |
                      USART_CR1_RE);
  while (!(VVC_UARTx->ISR & USART_ISR_TEACK)) {};
  while (!(VVC_UARTx->ISR & USART_ISR_REACK)) {};
  tx_string(VVC_UARTx, "AT+CIOBAUD=9600\r\n\0");
  //rx_str(VVC_UARTx, 100000);
  // Disable the USART transmit/receive pins.
  VVC_UARTx->CR1 &= ~(USART_CR1_TE |
                      USART_CR1_RE);
  */
  // Set to 'client only' mode if necessary.
  /*
  VVC_UARTx->CR1 |=  (USART_CR1_TE);
  while (!(VVC_UARTx->ISR & USART_ISR_TEACK)) {};
  tx_string(VVC_UARTx, "AT+CWMODE_DEF=1\r\n\0");
  VVC_UARTx->CR1 &= ~(USART_CR1_TE);
  */
  // Set default wifi configuration.
  assemble_join_wifi_str();
  VVC_UARTx->CR1 |=  (USART_CR1_TE |
                      USART_CR1_RE);
  while (!(VVC_UARTx->ISR & USART_ISR_TEACK)) {};
  while (!(VVC_UARTx->ISR & USART_ISR_REACK)) {};
  tx_string(VVC_UARTx, (const char*)str_buf);
  rx_str(VVC_UARTx, 100000);
  VVC_UARTx->CR1 &= ~(USART_CR1_TE |
                      USART_CR1_RE);
  // Set to auto-connect if necessary.
  /*
  VVC_UARTx->CR1 |=  (USART_CR1_TE);
  while (!(VVC_UARTx->ISR & USART_ISR_TEACK)) {};
  tx_string(VVC_UARTx, "AT+CWAUTOCONN=1\r\n\0");
  VVC_UARTx->CR1 &= ~(USART_CR1_TE);
  */

  while (1) {
    delay_ms(10000);
    VVC_UARTx->CR1 |=  (USART_CR1_TE |
                        USART_CR1_RE);
    while (!(VVC_UARTx->ISR & USART_ISR_TEACK)) {};
    while (!(VVC_UARTx->ISR & USART_ISR_REACK)) {};
    tx_string(VVC_UARTx, "AT+CWJAP?\r\n\0");
    // (Check 'str_buf' in a debugger)
    rx_str(VVC_UARTx, 100000);
    // Disable the USART transmit/receive pins.
    VVC_UARTx->CR1 &= ~(USART_CR1_TE |
                        USART_CR1_RE);
  }
  return 0;
}
