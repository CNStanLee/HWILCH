/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2024-01-01 21:20:45
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 15:37:36
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/cgpio.h
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _BOOT_CGPIO_H
#define _BOOT_CGPIO_H

extern uint32_t read_gpio_32(uint32_t addr);
extern void write_gpio_32(uint32_t addr, uint32_t value);
extern void write_gpio_8(uint32_t addr, uint32_t byte_offset, uint8_t value);
extern void write_gpio_1(uint32_t addr, uint32_t byte_offset, uint32_t bit_offset, uint8_t value);
extern uint8_t read_gpio_8(uint32_t addr, uint32_t byte_offset);
extern uint8_t read_gpio_1(uint32_t addr, uint32_t byte_offset, uint32_t bit_offset);


#endif /* _CGPIO_H */
