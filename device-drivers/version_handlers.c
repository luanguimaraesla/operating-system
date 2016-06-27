// Usage of the definitions found in linux/version.h.
// This header file is not automatically included by linux/module.h
// Author: Luan Guimarães Lacerda

#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
MODULE_LICENSE("GPL");


  // UTS_RELEASE:
  // This macro expands to a string describing the version
  // of this kernel tree. For example, "2.6.10"

  // LINUX_VERSION_CODE
  // This macro expands to the binary representation of the kernel version 

  // KERNEL_VERSIO(a,b,c)
  // This is the macro used to build an integer version code 
  // from the individual numbers that build up a version number
  // Used to compare with info above


#if LINUX_VERSION_CODE <= KERNEL_VERSION(3,16,5)

static int print_info(void){
  printk( KERN_ALERT "This kernel version is down to 3.16.5" );
  return 0;
}

#else

static int print_info(void){
  printk( KERN_ALERT "This kernel version is up to 3.16.5" );
  return 0;
}

#endif

module_init(print_info);
