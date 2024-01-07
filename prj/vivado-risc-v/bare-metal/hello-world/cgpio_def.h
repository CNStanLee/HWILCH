/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2024-01-02 14:19:24
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-02 17:01:56
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/cgpio_def.h
 * @Description: RISC-V CHIP 1 AXI ADDR DEF
 * 
 */


#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _BOOT_CGPIO_DEF_H
#define _BOOT_CGPIO_DEF_H

#define GPIO_0_ADDR 0x60050000
#define GPIO_1_ADDR 0x60060000
#define GPIO_2_ADDR 0x60070000 // CAN 1 CMD
#define GPIO_3_ADDR 0x60080000 // CAN 1 RXD
#define GPIO_4_ADDR 0x60090000 // CAN 2 CMD
#define GPIO_5_ADDR 0x600A0000 // CAN 2 RXD

/* LED ADDR DEF */

#define LED_ARRD GPIO_0_ADDR

// BIT DEF

#define LED0 0x00
#define LED1 0x01
#define LED2 0x02
#define LED3 0x03
#define LED4 0x04
#define LED5 0x05
#define LED6 0x06
#define LED7 0x07

/* SW ADDR DEF */

#define SW_ARRD GPIO_1_ADDR

// BIT DEF

#define SW0 0x00
#define SW1 0x01
#define SW2 0x02
#define SW3 0x03
#define SW4 0x04
#define SW5 0x05
#define SW6 0x06
#define SW7 0x07

/* CAN ADDR DEF */

#define CAN1_CMD GPIO_2_ADDR
#define CAN1_RXD GPIO_3_ADDR

#define CAN2_CMD GPIO_4_ADDR
#define CAN2_RXD GPIO_5_ADDR

// CMD byte offset
#define CAN_CMD_BYTE 0x00
#define CAN_PTX_BYTE 0x01

// Bit offset
#define RST 0x00
#define ALE 0x01
#define RD  0x02
#define WR  0x03
#define CS  0x04

// RXD byte offset
#define CAN_PRX_BYTE 0x00
#define CAN_RXD_BYTE 0x01

// Bit Offset
#define BOO  0x00
#define IRQO 0x01
#define CLKO 0x02


#endif /* _CGPIO_DEF_H */