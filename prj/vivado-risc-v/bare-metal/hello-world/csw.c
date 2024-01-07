/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2023-12-29 10:56:19
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 15:06:06
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/csw.c
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "csw.h"


struct sw_regs{
    volatile uint8_t sw_group1;
};

uint8_t sw_read(){
    struct sw_regs * regs = (struct sw_regs *)0x60060000;
    return regs->sw_group1;
}