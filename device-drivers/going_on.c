#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0) 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luan Guimarães Lacerda");
MODULE_DESCRIPTION("To learn");
MODULE_VERSION("0.0.1");

// __initdata and __init are strange but tells kernel that the function
// or variable will be only used at the beggining of the module's loading,
// the module loader drops the initialization function after the module
// is loaded, making its memory available for other uses.

int __initdata example = 0;

static int __init begin(void){
  printk( KERN_ALERT "Begin %d\n", example);
  return 0;
}



module_init(begin);

#endif

