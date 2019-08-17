
.set MAGIC, 0x1badb002
.set FLAGS, (1 << 0 | 1 << 1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

# Text section
.section .text

# Kernel entry point definition
.extern kernelMain

# Loader entry point
.global main

# Set stack pointer and
# Call C++ kernel entry point
main:
	mov $kernel_stack, %esp
	push %eax
	push %ebx
	call kernelMain

# Safety infinite loop
stop: 
	nop
	jmp stop

# Kernel stack pointer definition
.section .bss
.space 10 * 1024 * 1024
kernel_stack:
