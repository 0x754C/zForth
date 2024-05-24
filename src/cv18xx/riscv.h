#ifndef _RISCV_H_
#define _RISCV_H_

#define RISCV_FENCE(p, s) \
  __asm__ __volatile__ ("fence " #p "," #s : : : "memory")

#define mb() RISCV_FENCE(rw, rw)
#define rmb() RISCV_FENCE(ir, ir)
#define wmb() RISCV_FENCE(ow, ow)

#endif
