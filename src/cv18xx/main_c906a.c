#include "riscv.h"
#include "uart.h"
#include "zforth.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

enum {
  ZF_SYSCALL_REG_RD32 = 1024,
  ZF_SYSCALL_REG_WR32,
  ZF_SYSCALL_HEX32,
};

void emit(int c) {
  uart_txc(UART0, c);
}

void emits(char *s) {
  uart_txs(UART0, s);
}

int key(void) {
  return uart_rxc(UART0);
}

zf_input_state zf_host_sys(zf_syscall_id id, const char *input)
{
  (void)input;
  char buf[128];
  zf_cell t1, t2;
  switch((int)id) {
  case ZF_SYSCALL_EMIT:
    emit(zf_pop());
    break;
  case ZF_SYSCALL_PRINT:
    itoa(zf_pop(), buf, 10);
    emits(buf);
    break;
  case ZF_SYSCALL_REG_RD32:
    zf_push(readl(zf_pop()));
    break;
  case ZF_SYSCALL_REG_WR32:
    t1 = zf_pop();
    t2 = zf_pop();
    writel(t2, t1);
    break;
  case ZF_SYSCALL_HEX32:
    snprintf(buf, 127, "%08lX", zf_pop());
    emits(buf);
    break;
  }
  return 0;
}

zf_cell zf_host_parse_num(const char *buf)
{
  char *end;
  zf_cell v = strtol(buf, &end, 0);
  if(*end != '\0') {
    zf_abort(ZF_ABORT_NOT_A_WORD);
  }
  return v;
}

#define DEL 127

char ansi_term_delchar[] = { 0x1B, '[', '1', 'D', ' ',
			     0x1B, '[', '1', 'D', '\0' };

// simple line edit
int getaline(char *buf, int maxlen) {
  int i = 0;
  while (i < (maxlen - 1)) {
    buf[i] = key();
    if ((buf[i] == '\n') || (buf[i] == '\r')) {
      emits("\n\r");
      break;
    } else if ((buf[i] == '\b') || (buf[i] == DEL)) {
      if (i > 0) {
	emits(ansi_term_delchar);
	i--;
      }
    } else {
      emit(buf[i]);
      i++;
    }
  }
  buf[i] = '\0';
  return i;
}

char *startup[] = {
  ": emit 0 sys ;",
  ": . 1 sys ;",
  ": cr 10 emit 13 emit ;",
  ": space 32 emit ;",
  ": reg32@ 1024 sys ;",
  ": reg32! 1025 sys ;",
  ": hex32 1026 sys ;",
  ": reg32? reg32@ hex32 ;",
  ": over 1 pick ;",
  ": reg32setbit 1 swap << over reg32@ | swap reg32! ;",
  ": reg32invbit 1 swap << over reg32@ ^ swap reg32! ;",
  ": reg32clrbit 1 swap << 0xFFFFFFFF ^ over reg32@ & swap reg32! ;",
  ": reg32getbit 1 swap << swap reg32@ & ;",
  ": pinmux-base 0x03001000 ;",
  ": pinmux-sd0-pwr-en pinmux-base 0x38 + ;",
  ": pinmux-gpio-mode 0x03 ;",
  ": reset-base 0x03003000 ;",
  NULL,
};

int main_c906a(void) {
  mb();
  emit(0x55);
  emit(0x55);
  emit(0x55);

  zf_init(1);
  zf_bootstrap();
  emits("\n\rzForth on cv18xx\n\r");

  int i;
  for (i = 0; startup[i] != NULL; i++) {
    zf_eval(startup[i]);
  }

  zf_result r;
  char line[128];
  char *msg;

  while(1) {
    if (getaline(line, 127) > 0) {
      r = zf_eval(line);
      msg = NULL;
      switch(r) {
      case ZF_OK: break;
      case ZF_ABORT_INTERNAL_ERROR: msg = " internal error"; break;
      case ZF_ABORT_OUTSIDE_MEM: msg = " outside memory"; break;
      case ZF_ABORT_DSTACK_OVERRUN: msg = " dstack overrun"; break;
      case ZF_ABORT_DSTACK_UNDERRUN: msg = " dstack underrun"; break;
      case ZF_ABORT_RSTACK_OVERRUN: msg = " rstack overrun"; break;
      case ZF_ABORT_RSTACK_UNDERRUN: msg = " rstack underrun"; break;
      case ZF_ABORT_NOT_A_WORD: msg = " not a word"; break;
      case ZF_ABORT_COMPILE_ONLY_WORD: msg = " compile-only word"; break;
      case ZF_ABORT_INVALID_SIZE: msg = " invalid size"; break;
      case ZF_ABORT_DIVISION_BY_ZERO: msg = " division by zero"; break;
      default: msg = " unknown error";
      }
      if (msg != NULL) {
	emits(line);
	emits(msg);
	emits("\n\r");
      }
    }
  }
}
