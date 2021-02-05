	.file	"ex1_4.c"
	.text
	.globl	total_mark
	.type	total_mark, @function
total_mark:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movsd	8(%rax), %xmm1
	movq	-40(%rbp), %rax
	movsd	16(%rax), %xmm0
	addsd	%xmm0, %xmm1
	movq	-40(%rbp), %rax
	movsd	24(%rax), %xmm0
	addsd	%xmm1, %xmm0
	movsd	.LC0(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movapd	%xmm0, %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movq	-40(%rbp), %rax
	movsd	32(%rax), %xmm1
	movsd	.LC2(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movq	-40(%rbp), %rax
	movsd	40(%rax), %xmm2
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	movsd	-24(%rbp), %xmm0
	addsd	-16(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	total_mark, .-total_mark
	.section	.rodata
	.align 8
.LC9:
	.string	"John Doe's Final Mark: %lf / 100\n"
	.align 8
.LC10:
	.string	"Jane Doe's Final Mark: %lf / 100\n"
	.align 8
.LC11:
	.string	"\nNote: The Jane and John Doe marks simply represent pointer vs direct refrence to the assignment of the individual grade values"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$12345678, -112(%rbp)
	movsd	.LC4(%rip), %xmm0
	movsd	%xmm0, -104(%rbp)
	movsd	.LC5(%rip), %xmm0
	movsd	%xmm0, -96(%rbp)
	movsd	.LC6(%rip), %xmm0
	movsd	%xmm0, -88(%rbp)
	movsd	.LC7(%rip), %xmm0
	movsd	%xmm0, -80(%rbp)
	movsd	.LC8(%rip), %xmm0
	movsd	%xmm0, -72(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, -120(%rbp)
	movq	-120(%rbp), %rax
	movq	$12345678, (%rax)
	movq	-120(%rbp), %rax
	movsd	.LC4(%rip), %xmm0
	movsd	%xmm0, 8(%rax)
	movq	-120(%rbp), %rax
	movsd	.LC5(%rip), %xmm0
	movsd	%xmm0, 16(%rax)
	movq	-120(%rbp), %rax
	movsd	.LC6(%rip), %xmm0
	movsd	%xmm0, 24(%rax)
	movq	-120(%rbp), %rax
	movsd	.LC7(%rip), %xmm0
	movsd	%xmm0, 32(%rax)
	movq	-120(%rbp), %rax
	movsd	.LC8(%rip), %xmm0
	movsd	%xmm0, 40(%rax)
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	total_mark
	leaq	.LC9(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-120(%rbp), %rax
	movq	%rax, %rdi
	call	total_mark
	leaq	.LC10(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	leaq	.LC11(%rip), %rdi
	call	puts@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1074266112
	.align 8
.LC1:
	.long	2576980378
	.long	1070176665
	.align 8
.LC2:
	.long	858993459
	.long	1070805811
	.align 8
.LC3:
	.long	0
	.long	1071644672
	.align 8
.LC4:
	.long	3435973837
	.long	1079413964
	.align 8
.LC5:
	.long	0
	.long	1079091200
	.align 8
.LC6:
	.long	2576980378
	.long	1079253401
	.align 8
.LC7:
	.long	858993459
	.long	1079284531
	.align 8
.LC8:
	.long	3435973837
	.long	1079233740
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
