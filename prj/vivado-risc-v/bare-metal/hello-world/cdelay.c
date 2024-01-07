/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2024-01-02 15:46:46
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 15:51:11
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/cdelay.c
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "cdelay.h"

void usleep(unsigned us) {
    uintptr_t cycles0;
    uintptr_t cycles1;
    asm volatile ("csrr %0, 0xB00" : "=r" (cycles0));
    for (;;) {
        asm volatile ("csrr %0, 0xB00" : "=r" (cycles1));
        if (cycles1 - cycles0 >= us * 100) break;
    }
}

void ten_nsleep(unsigned ten_ns) {
    uintptr_t cycles0;
    uintptr_t cycles1;
    asm volatile ("csrr %0, 0xB00" : "=r" (cycles0));
    for (;;) {
        asm volatile ("csrr %0, 0xB00" : "=r" (cycles1));
        if (cycles1 - cycles0 >= ten_ns) break;
    }
}