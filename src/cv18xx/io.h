#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>
#include <riscv.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

# define __iomem	__attribute__((noderef, address_space(2)))

#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))
#define __arch_getq(a)			(*(volatile unsigned long long *)(a))

#define __arch_putb(v, a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v, a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v, a)		(*(volatile unsigned int *)(a) = (v))
#define __arch_putq(v, a)		(*(volatile unsigned long long *)(a) = (v))

#define __raw_writeb(v, a)		__arch_putb(v, a)
#define __raw_writew(v, a)		__arch_putw(v, a)
#define __raw_writel(v, a)		__arch_putl(v, a)
#define __raw_writeq(v, a)		__arch_putq(v, a)

#define __raw_readb(a)			__arch_getb(a)
#define __raw_readw(a)			__arch_getw(a)
#define __raw_readl(a)			__arch_getl(a)
#define __raw_readq(a)			__arch_getq(a)

/* adding for cadence_qspi_apb.c */
#define memcpy_fromio(a, c, l)		memcpy((a), (c), (l))
#define memcpy_toio(c, a, l)		memcpy((c), (a), (l))


#define dmb()		mb()
#define __iormb()	rmb()
#define __iowmb()	wmb()

static inline u8 readb(const volatile void __iomem *addr)
{
	u8	val;

	val = __arch_getb(addr);
	__iormb();
	return val;
}

static inline u16 readw(const volatile void __iomem *addr)
{
	u16	val;

	val = __arch_getw(addr);
	__iormb();
	return val;
}

static inline u32 readl(const volatile void __iomem *addr)
{
	u32	val;

	val = __arch_getl(addr);
	__iormb();
	return val;
}

static inline u64 readq(const volatile void __iomem *addr)
{
	u64	val;

	val = __arch_getq(addr);
	__iormb();
	return val;
}

static inline void writeb(u8 val, volatile void __iomem *addr)
{
	__iowmb();
	__arch_putb(val, addr);
}

static inline void writew(u16 val, volatile void __iomem *addr)
{
	__iowmb();
	__arch_putw(val, addr);
}

static inline void writel(u32 val, volatile void __iomem *addr)
{
	__iowmb();
	__arch_putl(val, addr);
}

static inline void writeq(u64 val, volatile void __iomem *addr)
{
	__iowmb();
	__arch_putq(val, addr);
}

#endif
