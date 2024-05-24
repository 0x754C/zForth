ENTRY(_start_c906a)

MEMORY {
	TPU_SRAM (rwx): ORIGIN = 0x0C000000, LENGTH = 220K
}

SECTIONS {
	.text : {
		*(.text*)
		*(.rodata*)
	} > TPU_SRAM
	.data : {
		. = ALIGN(16);
		*(.data*)
		. = ALIGN(16);
	} > TPU_SRAM
	.bss (NOLOAD) : {
		. = ALIGN(16);	
		PROVIDE(__bss_start = .);
		*(.bss*)
		*(.sbss*)
		PROVIDE(__bss_end = .);
		. = ALIGN(16);
	} > TPU_SRAM

	.stack_c906a (NOLOAD) : {
		. = ALIGN(16);
		PROVIDE(__stack_c906a_start = .);
		. += 0x2000;
		PROVIDE(__stack_c906a_end = .);
		. = ALIGN(16);
	} > TPU_SRAM

	.stack_c906b (NOLOAD) : {
		. = ALIGN(16);
		PROVIDE(__stack_c906b_start = .);
		. += 0x2000;
		PROVIDE(__stack_c906b_end = .);
		. = ALIGN(16);
	} > TPU_SRAM
}
