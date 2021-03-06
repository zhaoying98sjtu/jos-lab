// System call stubs.

#include <inc/syscall.h>
#include <inc/lib.h>

static inline int32_t
syscall(int num, int check, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
{
	int32_t ret;

	// Generic system call: pass system call number in AX,
	// up to five parameters in DX, CX, BX, DI, SI.
	// Interrupt kernel with T_SYSCALL.
	//
	// The "volatile" tells the assembler not to optimize
	// this instruction away just because we don't use the
	// return value.
	//
	// The last clause tells the assembler that this can
	// potentially change the condition codes and arbitrary
	// memory locations.
	// leal after_sysenter_label, %%esi
	asm volatile( 
			"pushl %%ecx\n\t"
			"pushl %%edx\n\t"
			"pushl %%ebx\n\t"
			"pushl %%esp\n\t"
			"pushl %%ebp\n\t"
			"pushl %%esi\n\t"
			"pushl %%edi\n\t"
			"leal after_sysenter_label%=, %%esi\n\t"
			"movl %%esp, %%ebp\n\t"
			"sysenter\n\t"
			"after_sysenter_label%=: \n\t"
			"popl %%edi\n\t"
			"popl %%esi\n\t"
			"popl %%ebp\n\t"
			"popl %%esp\n\t"
			"popl %%ebx\n\t"
			"popl %%edx\n\t"
			"popl %%ecx\n\t"
		     : "=a" (ret)
		     : "i" (T_SYSCALL),
		       "a" (num),
		       "d" (a1),
		       "c" (a2),
		       "b" (a3),
		       "D" (a4),
		       "S" (a5)
		     : "cc", "memory");

	if(check && ret > 0)
		panic("syscall %d returned %d (> 0)", num, ret);

	return ret;
}

void
sys_cputs(const char *s, size_t len)
{
	syscall(SYS_cputs, 0, (uint32_t)s, len, 0, 0, 0);
}

int
sys_cgetc(void)
{
	return syscall(SYS_cgetc, 0, 0, 0, 0, 0, 0);
}

int
sys_env_destroy(envid_t envid)
{
	return syscall(SYS_env_destroy, 1, envid, 0, 0, 0, 0);
}

envid_t
sys_getenvid(void)
{
	 return syscall(SYS_getenvid, 0, 0, 0, 0, 0, 0);
}

int
sys_map_kernel_page(void* kpage, void* va)
{
	return syscall(SYS_map_kernel_page, 0, (uint32_t)kpage, (uint32_t)va, 0, 0, 0);
}

int
sys_sbrk(uint32_t inc)
{
	return syscall(SYS_sbrk, 0, (uint32_t)inc, (uint32_t)0, 0, 0, 0);
}
