	.global _start_c906a
	.option norvc
_start_c906a:
	j reset_c906a
	.word 0
	.word 0 /* BL2 MSID */
	.word 0 /* BL2 version */
	.word 0
	.word 0
	.word 0
	.word 0

reset_c906a:
	la sp, __stack_c906a_end
	add sp, sp, -8

	la a0, __bss_start
	li a1, 0
	la a3, __bss_end
	sub a2, a3, a0
	call memset

	j main_c906a
