/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2023-12-19 18:15:43
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-07 13:51:07
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/main.c
 * @Description: 
 * 
 */
#include <stdint.h>
#include <stdlib.h>

#include "common.h"

#include "kprintf.h"
#include "cled.h"
#include "csw.h"
#include "cgpio.h"
#include "cgpio_def.h"
#include "cdelay.h"
#include "ccan_drv.h"






int main(void) {
    //struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;
    uint8_t Status;
    kprintf("\n");
    //can_init_test();




    usleep(1000);

    //write_gpio_8(CAN_GPIO_BASE_ADDR3, 0, 0x11);
    //write_gpio_8(CAN_GPIO_BASE_ADDR3, 8, 0x22);
    //write_gpio_1(CAN_GPIO_BASE_ADDR3, 0, 0, 0);
    write_gpio_8(CAN1_CMD, CAN_PTX_BYTE, 0x41);
    write_gpio_1(CAN1_CMD, 0, RST, 0x00);

    can_set_pin(CAN1_CMD, ALE, 0x01);
    //usleep(1000000);
    //kprintf(read_gpio_32(CAN_GPIO_BASE_ADDR));
    //  regs->can1_cmd = 0x11;
    //  regs->can1_txd = 0x23;
    //  regs->can2_cmd = 0x34;
    //  regs->can2_txd = 0x78;

    //regs3->can1_cmd2 = 0x12345678;

    usleep(1000000);
    can_init(CAN1_CMD, CAN1_RXD);
    kprintf("can1 init finished\n");
    can_init(CAN2_CMD, CAN2_RXD);
    kprintf("can2 init finished\n");
    Status = read_register(CAN1_CMD, CAN1_RXD, SJA_SR);
    kprintf("%d\n", Status);
    usleep(1000000);
    can_txd(CAN1_CMD, CAN1_RXD);
    usleep(1000000);
    //can_txd(CAN1_CMD, CAN1_RXD);
    
    //write_register(CAN1_CMD, SJA_BTR1, 0x14);
    //usleep(1000000);



    for (;;) {
        //kprintf("Hello World Test2!\n");
        usleep(10);
        led_on_test(sw_read());

        //test write and read register of can controller

        usleep(1000000);
        //write_register2(SJA_ACR3, 0x30);
        //Status = read_register1(SJA_MOD);
        kprintf("1s\n");
        can_rxd(CAN2_CMD, CAN2_RXD);
        
        //can_rxd(CAN2_CMD, CAN2_RXD);

        //0x60050000
        //Status = read_register(CAN1_CMD, CAN1_RXD, SJA_SR);
        //kprintf("%d\n", Status);



    }

    return 0;
}
