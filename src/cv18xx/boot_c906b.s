	.global _start_c906b
	.option norvc
_start_c906b:
	j reset_c906b
	.word 0
	.word 0 /* BL2 MSID */
	.word 0 /* BL2 version */
	.word 0
	.word 0
	.word 0
	.word 0

reset_c906b:
	la sp, __stack_c906b_end
	add sp, sp, -8

	j main_c906b
