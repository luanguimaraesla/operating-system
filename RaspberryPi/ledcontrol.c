#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>
#include <mach/platform.h> 
#include <linux/timer.h>
#include <linux/err.h>

MODULE_LICENSE("GPL");


struct GpioRegisters{
    uint32_t GPFSEL[6];
    uint32_t Reserved1;
    uint32_t GPSET[2];
    uint32_t Reserved2;
    uint32_t GPCLR[2];
};

struct GpioRegisters *s_pGpioRegisters;
static struct timer_list s_BlinkTimer;
static int s_BlinkPeriod = 1000;
static const int LedGpioPin = 18;

static void SetGPIOFunction(int GPIO, int functionCode){
    int registerIndex = GPIO / 10;
    int bit = (GPIO % 10) * 3;

    unsigned oldValue = s_pGpioRegisters-> GPFSEL[registerIndex];
    unsigned mask = 0b111 << bit;
    printk("Changing function of GPIO%d from %x to %x\n",
           GPIO,
           (oldValue >> bit) & 0b111,
           functionCode);

    s_pGpioRegisters-> GPFSEL[registerIndex] =
        (oldValue & ~mask) | ((functionCode << bit) & mask);
}

static void SetGPIOOutputValue(int GPIO, bool outputValue){
    if (outputValue)
        s_pGpioRegisters-> GPSET[GPIO / 32] = (1 << (GPIO % 32));
    else
        s_pGpioRegisters-> GPCLR[GPIO / 32] = (1 << (GPIO % 32));
}

static void BlinkTimerHandler(unsigned long unused)
{
    static bool on = false;
    on = !on;
    SetGPIOOutputValue( LedGpioPin, on);
    mod_timer(&s_BlinkTimer,
              jiffies + msecs_to_jiffies( s_BlinkPeriod));
}

static int __init begin(void){
  int result;

  s_pGpioRegisters = (struct GpioRegisters *)__io_address(GPIO_BASE);

  SetGPIOFunction( LedGpioPin, 0b001); //Output

  setup_timer(&s_BlinkTimer, BlinkTimerHandler, 0);
  result = mod_timer( &s_BlinkTimer, jiffies + msecs_to_jiffies( s_BlinkPeriod));
  BUG_ON(result < 0);
  return 0;
}

static int __exit end(void){
  SetGPIOFunction( LedGpioPin, 0); //Configure the pin as input
  del_timer(&s_BlinkTimer);
  return 0;
}

module_init(begin);
module_exit(end);
