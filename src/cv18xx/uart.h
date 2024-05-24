#ifndef _UART_H_
#define _UART_H_

#define UART0 0
#define UART0_BASE 0x04140000


enum {
  UART_RBR = 0x00,
  UART_THR = 0x00,
  UART_DLL = 0x00,
  UART_DLH = 0x04,
  UART_IER = 0x04,
  UART_FCR = 0x08,
  UART_IIR = 0x08,
  UART_LCR = 0x0C,
  UART_MCR = 0x10,
  UART_LSR = 0x14,
  UART_MSR = 0x18,
};

enum {
  UART_LSR_THR_EMPTY = (1 << 5),
  UART_LSR_DATA_READY = (1 << 0),
};

#include <stdint.h>

uint64_t uart_get_base(int n);
int uart_tx_ready(int n);
void uart_wait_tx_ready(int n);
void uart_txc(int n, int c);
int uart_rx_ready(int n);
void uart_wait_rx_ready(int n);
int uart_rxc(int n);

void uart_txs(int n, char *s);

#endif
