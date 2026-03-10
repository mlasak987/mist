#ifndef _KERNEL_SHELL
#define _KERNEL_SHELL

#ifndef __OS_VERSION_
#define __OS_VERSION_ "NULL"
#endif

#ifndef __KERNEL_ARCH_
#define __KERNEL_ARCH_ "NULL"
#endif

#define __KERNEL_SHELL_VERSION_ "0.1.1"

void kernel_shell_init();
void kernel_shell_loop();

#endif
