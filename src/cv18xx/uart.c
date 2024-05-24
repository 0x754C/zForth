#include "uart.h"
#include "io.h"

uint64_t uart_get_base(int n) {
  switch(n) {
  case UART0:
    return UART0_BASE;
  default:
    return 0;
  }
}

int uart_tx_ready(int n) {
  uint64_t base = uart_get_base(n);
  uint32_t val;
  val = readl(base + UART_LSR);
  return !!(val & UART_LSR_THR_EMPTY);
}

void uart_wait_tx_ready(int n) {
  while (!uart_tx_ready(n));
}

void uart_txc(int n, int c) {
  uart_wait_tx_ready(n);
  writel(c, uart_get_base(n) + UART_THR);
}

int uart_rx_ready(int n) {
  uint64_t base = uart_get_base(n);
  uint32_t val;
  val = readl(base + UART_LSR);
  return !!(val & UART_LSR_DATA_READY);
}

void uart_wait_rx_ready(int n) {
  while(!uart_rx_ready(n));
}

int uart_rxc(int n) {
  uart_wait_rx_ready(n);
  return (readl(uart_get_base(n) + UART_RBR) & 0xFF);
}

void uart_txs(int n, char *s) {
  while(*s != '\0') {
    uart_txc(n, *s);
    s++;
  }
}
