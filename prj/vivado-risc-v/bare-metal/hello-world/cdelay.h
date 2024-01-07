/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2024-01-02 15:46:53
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 15:49:55
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/cdelay.h
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _BOOT_CDELAY_H
#define _BOOT_CDELAY_H


void usleep(unsigned us);
void ten_nsleep(unsigned ten_ns);

#endif