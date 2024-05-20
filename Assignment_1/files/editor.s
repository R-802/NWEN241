	.file	"editor.c"
	.text
	.globl	editor_insert_char
	.def	editor_insert_char;	.scl	2;	.type	32;	.endef
	.seh_proc	editor_insert_char
editor_insert_char:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movl	%r8d, %eax
	movl	%r9d, 40(%rbp)
	movb	%al, 32(%rbp)
	cmpl	$0, 40(%rbp)
	js	.L2
	movl	40(%rbp), %eax
	cmpl	24(%rbp), %eax
	jl	.L3
.L2:
	movl	$0, %eax
	jmp	.L4
.L3:
	movl	40(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movl	40(%rbp), %eax
	cltq
	movq	16(%rbp), %rdx
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movl	24(%rbp), %eax
	subl	40(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movslq	%eax, %rcx
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movq	%rax, %rcx
	call	memmove
	movl	40(%rbp), %eax
	cltq
	movq	16(%rbp), %rdx
	addq	%rax, %rdx
	movzbl	32(%rbp), %eax
	movb	%al, (%rdx)
	movl	$1, %eax
.L4:
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "    Index: %d\12\0"
.LC1:
	.ascii "    Destination: %c\12\0"
.LC2:
	.ascii "    Source: %c\12\0"
	.text
	.globl	editor_delete_char
	.def	editor_delete_char;	.scl	2;	.type	32;	.endef
	.seh_proc	editor_delete_char
editor_delete_char:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movl	%r8d, %eax
	movl	%r9d, 40(%rbp)
	movb	%al, 32(%rbp)
	movl	40(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L6
.L9:
	movl	-4(%rbp), %eax
	cltq
	movq	16(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	%al, 32(%rbp)
	jne	.L7
	movl	-4(%rbp), %eax
	cltq
	movq	16(%rbp), %rdx
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC0(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movq	-16(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edx
	leaq	.LC1(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movq	-16(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	24(%rbp), %eax
	subl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	movslq	%eax, %rcx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rcx, %r8
	movq	%rax, %rcx
	call	memmove
	movl	$1, %eax
	jmp	.L8
.L7:
	addl	$1, -4(%rbp)
.L6:
	movl	-4(%rbp), %eax
	cmpl	24(%rbp), %eax
	jl	.L9
	movl	$0, %eax
.L8:
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	editor_replace_str
	.def	editor_replace_str;	.scl	2;	.type	32;	.endef
	.seh_proc	editor_replace_str
editor_replace_str:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$72, %rsp
	.seh_stackalloc	72
	leaq	64(%rsp), %rbp
	.seh_setframe	%rbp, 64
	.seh_endprologue
	movq	%rcx, 32(%rbp)
	movl	%edx, 40(%rbp)
	movq	%r8, 48(%rbp)
	movq	%r9, 56(%rbp)
	cmpl	$0, 64(%rbp)
	js	.L11
	movl	64(%rbp), %eax
	cmpl	40(%rbp), %eax
	jl	.L12
.L11:
	movl	$-1, %eax
	jmp	.L13
.L12:
	movl	64(%rbp), %eax
	cltq
	movq	32(%rbp), %rdx
	addq	%rdx, %rax
	movq	48(%rbp), %rdx
	movq	%rax, %rcx
	call	strstr
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L14
	movl	$-1, %eax
	jmp	.L13
.L14:
	movl	40(%rbp), %edx
	movq	-16(%rbp), %rax
	subq	32(%rbp), %rax
	movl	%edx, %ebx
	subl	%eax, %ebx
	movq	48(%rbp), %rax
	movq	%rax, %rcx
	call	strlen
	movl	%eax, %edx
	movl	%ebx, %eax
	subl	%edx, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movslq	%eax, %rbx
	movq	48(%rbp), %rax
	movq	%rax, %rcx
	call	strlen
	movq	-16(%rbp), %rdx
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rbx, %r8
	movq	%rax, %rcx
	call	memmove
	movl	$0, -4(%rbp)
	jmp	.L15
.L16:
	movq	-16(%rbp), %rax
	subq	32(%rbp), %rax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, %r8d
	movl	-4(%rbp), %eax
	cltq
	movq	56(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %ecx
	movl	40(%rbp), %edx
	movq	32(%rbp), %rax
	movl	%r8d, %r9d
	movl	%ecx, %r8d
	movq	%rax, %rcx
	call	editor_insert_char
	addl	$1, -4(%rbp)
.L15:
	movl	-4(%rbp), %eax
	movslq	%eax, %rbx
	movq	56(%rbp), %rax
	movq	%rax, %rcx
	call	strlen
	cmpq	%rax, %rbx
	jb	.L16
	movq	-16(%rbp), %rax
	subq	32(%rbp), %rax
	movl	%eax, %ebx
	movq	56(%rbp), %rax
	movq	%rax, %rcx
	call	strlen
	addl	%ebx, %eax
	subl	$1, %eax
.L13:
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.seh_endproc
	.globl	editor_view
	.def	editor_view;	.scl	2;	.type	32;	.endef
	.seh_proc	editor_view
editor_view:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movq	%r9, 40(%rbp)
	movl	24(%rbp), %eax
	cltq
	subq	$1, %rax
	movq	%rax, -8(%rbp)
	nop
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-Builds project) 13.2.0"
	.def	memmove;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	strstr;	.scl	2;	.type	32;	.endef
	.def	strlen;	.scl	2;	.type	32;	.endef
