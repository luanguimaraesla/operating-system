#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/moduleparam.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0) 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luan Guimarães Lacerda");
MODULE_DESCRIPTION("To learn");
MODULE_VERSION("0.0.1");

// Module with params:
// insmod going_on.ko howmany=10 name="luan"
//
// Possible types:
// 1. bool
// 2. invbool -> The invbool type inverts the value: !value
// 3. charp   -> A char pointer value.
// 4. int
// 5. long
// 6. short
// 7. uint    -> Unsigned int
// 8. ulong
// 9. ushort
//
// All module parameters should be given a default value; insmod changes the
// value only if explicitly told to by the user
//
// Use S_IRUGO for a parameter that can be read by the world but cannot be 
// changed; S_IRUGO|S_IWUSR allows root to change the parameter

static char *name = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(name, charp, S_IRUGO);


// __initdata and __init are strange but tells kernel that the function
// or variable will be only used at the beggining of the module's loading,
// the module loader drops the initialization function after the module
// is loaded, making its memory available for other uses.

int __initdata counter = 0;

static int __init begin(void){

  // Module code must always check return values, and be sure that the
  // requested operations have actually succeeded.
  while(counter++ <= howmany){
    printk( KERN_ALERT "Loop %d: Begin to the %s\n", counter, name);
  }

  return 0;
}


// If your module is built directly into the kernel, or if your kernel is
// configured to disallow the unloading of modules, functions marked __exit
// are simply discarded. For this reason, a function marked __exit can be
// called only at module unload or system shutdown time; any other is an error
static int __exit end(void){
  printk( KERN_ALERT "End\n");
  return 0;
}

// This macro adds a special section to the module’s object code stating
// where the module’s initialization function is to be found.
// Without this definition, your initialization function is never called.
module_init(begin);

// The module_exit declaration is necessary to enable to kernel to find your
// cleanup function.
module_exit(end);

#endif

