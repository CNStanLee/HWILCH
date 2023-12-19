#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "cled.h"

struct led_regs{
    volatile uint32_t led_group1;
    volatile uint32_t led_group2;
    volatile uint32_t led_group3;
    volatile uint32_t led_group4;
};

void led_on_test(){
    struct led_regs * regs = (struct led_regs *)0x60030000;
    regs->led_group1 = 0x55;
    regs->led_group2 = 0x55;
    regs->led_group3 = 0x55;
    regs->led_group4 = 0x55;
}