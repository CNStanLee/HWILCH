/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2024-01-01 21:20:38
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 15:36:58
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/cgpio.c
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "cgpio.h"


uint32_t read_gpio_32(uint32_t addr){
    return *(uint32_t*)addr;
}

void write_gpio_32(uint32_t addr, uint32_t value){
    *(uint32_t*)addr = value;
}

void write_gpio_8(uint32_t addr, uint32_t byte_offset, uint8_t value){
    uint32_t origin_value = *(uint32_t*)addr;
    origin_value &= ~(0xFFu << (byte_offset*8));
    origin_value |= ((value & 0xFFu) << (byte_offset*8));
    *(uint32_t*)addr = origin_value;
}

void write_gpio_1(uint32_t addr, uint32_t byte_offset, uint32_t bit_offset, uint8_t value){
    uint32_t originalValue = *(uint32_t*)addr;
    originalValue &= ~(1u << (byte_offset * 8 + bit_offset));
    originalValue |= ((value & 1u) << (byte_offset * 8 + bit_offset));
    *(uint32_t*)addr = originalValue;
}

uint8_t read_gpio_8(uint32_t addr, uint32_t byte_offset) {
    uint32_t originalValue = *(uint32_t*)addr;
    return (originalValue >> (byte_offset * 8)) & 0xFFu;
}

uint8_t read_gpio_1(uint32_t addr, uint32_t byte_offset, uint32_t bit_offset) {
    uint32_t originalValue = *(uint32_t*)addr;
    return (originalValue >> (byte_offset * 8 + bit_offset)) & 0x01u;
}
