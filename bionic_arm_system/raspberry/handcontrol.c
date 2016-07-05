#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include <asm/io.h>
#include <mach/platform.h> 
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/gfp.h>

MODULE_LICENSE("GPL");

#define CLOSE_FINGER 0
#define OPEN_FINGER 1

// Struct defining GPIO registers
struct gpio_registers{
    uint32_t GPFSEL[6];
    uint32_t Reserved1;
    uint32_t GPSET[2];
    uint32_t Reserved2;
    uint32_t GPCLR[2];
};

struct gpio_registers *p_gpio_registers; // GPIO registers controller

// Define finger's pins
static const int finger_1_pin = 27;       
static const int finger_2_pin = 18;       
static const int finger_3_pin = 17;       
static const int finger_4_pin = 22;       

// Configure GPIO outputs
static void set_gpio_function(int GPIO, int function_code){
    int register_index = GPIO / 10;
    int bit = (GPIO % 10) * 3;

    unsigned old_value = p_gpio_registers->GPFSEL[register_index];
    unsigned mask = 0b111 << bit;
    printk("Changing function of GPIO%d from %x to %x\n",
           GPIO,
           (old_value >> bit) & 0b111,
           function_code);

    p_gpio_registers-> GPFSEL[register_index] =
        (old_value & ~mask) | ((function_code << bit) & mask);
}

// Write GPIO output
static void set_gpio_output_value(int GPIO, bool output_value){
    if (output_value)
        p_gpio_registers->GPSET[GPIO / 32] = (1 << (GPIO % 32));
    else
        p_gpio_registers->GPCLR[GPIO / 32] = (1 << (GPIO % 32));
}

char * kstrtok(const char *src){
    char *dest = (char *) kmalloc (sizeof(char) * 15, GFP_KERNEL);
    int i=0;
    while(src[i] != '\0' && src[i] != '\n')
      dest[i] = src[i];

    dest[i] = '\0';
    return dest;
}

// Function to set the hand action
static ssize_t set_hand_action_callback(struct device* dev,
                                        struct device_attribute* attr,
                                        const char* cmd,
                                        size_t count){

    char *cmd_cpy = kstrtok(cmd);
    

    if (strcmp((const char *)cmd_cpy, "help") == 0)
        printk(KERN_INFO "You can only 'close' and 'open' the hand.\n");
    else if (strcmp((const char *)cmd_cpy, "close") == 0){
        printk(KERN_INFO "Closing the hand.\n");
        set_gpio_output_value(finger_1_pin, CLOSE_FINGER);
        set_gpio_output_value(finger_2_pin, CLOSE_FINGER);
        set_gpio_output_value(finger_3_pin, CLOSE_FINGER);
        set_gpio_output_value(finger_4_pin, CLOSE_FINGER);
    }else if (strcmp((const char *)cmd_cpy, "open") == 0){
        printk(KERN_INFO "Openning the hand.\n");
        set_gpio_output_value(finger_1_pin, OPEN_FINGER);
        set_gpio_output_value(finger_2_pin, OPEN_FINGER);
        set_gpio_output_value(finger_3_pin, OPEN_FINGER);
        set_gpio_output_value(finger_4_pin, OPEN_FINGER);
    }else{
        printk(KERN_INFO "You typed %s, but can only 'close' and 'open' the hand.\n",
               cmd_cpy);
        return -EINVAL;
    }

    kfree(cmd_cpy);
    return count;
}

static DEVICE_ATTR(action, (S_IWUSR | S_IWGRP), NULL, set_hand_action_callback);

static struct class *p_device_class;
static struct device *p_device_object;

static int __init begin(void){
  int result;

  p_gpio_registers = (struct gpio_registers *)__io_address(GPIO_BASE);
  set_gpio_function( finger_1_pin, 0b001); //Output
  set_gpio_function( finger_2_pin, 0b001); //Output
  set_gpio_function( finger_3_pin, 0b001); //Output
  set_gpio_function( finger_4_pin, 0b001); //Output

  p_device_class = class_create(THIS_MODULE, "bionic_hand");
  BUG_ON(IS_ERR(p_device_class));

  p_device_object = device_create(p_device_class, NULL, 0, NULL, "hand");
  BUG_ON(IS_ERR(p_device_object));

  result = device_create_file(p_device_object, &dev_attr_action);
  BUG_ON(result < 0);

  return 0;
}

static int __exit end(void){
  set_gpio_function( finger_1_pin, 0); //input
  set_gpio_function( finger_2_pin, 0); //input
  set_gpio_function( finger_3_pin, 0); //input
  set_gpio_function( finger_4_pin, 0); //input
  device_remove_file(p_device_object, &dev_attr_action);
  device_destroy(p_device_class, 0);
  class_destroy(p_device_class);
  return 0;
}

module_init(begin);
module_exit(end);
