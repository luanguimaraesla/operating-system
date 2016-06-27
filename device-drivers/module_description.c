#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Luan Guimarães Lacerda");
MODULE_DESCRIPTION("Description of module declarations");
MODULE_VERSION("1.2.3");

static int descript_init(void){
  printk( KERN_ALERT "Module about description" );
  return 0;
}

module_init(descript_init);
