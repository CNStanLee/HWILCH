/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2023-12-19 18:15:43
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 15:05:33
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/cled.c
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "cled.h"

struct led_regs{
    volatile uint8_t led_group1;
};

void led_on_test(uint8_t led_val){
    struct led_regs * regs = (struct led_regs *)0x60050000;
    //regs->led_group1 = 0x11;
    regs->led_group1 = led_val;
}