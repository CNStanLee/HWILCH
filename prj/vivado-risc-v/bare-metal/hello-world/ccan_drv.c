/*
 * @Author: Changhongli lic9@tcd.com
 * @Date: 2024-01-02 15:10:16
 * @LastEditors: Changhongli lic9@tcd.com
 * @LastEditTime: 2024-01-07 13:52:17
 * @FilePath: /vivado-risc-v/bare-metal/hello-world/ccan_drv.c
 * @Description: 
 * 
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "ccan_drv.h"
#include "cgpio_def.h"
#include "cgpio.h"
#include "cdelay.h"
#include "kprintf.h"

uint8_t       RX_buffer[FrameNum]; 
uint8_t       TX_buffer[FrameNum];  

void can_set_pin(uint32_t can_addr, uint8_t pin, uint8_t value){
    write_gpio_1(can_addr, CAN_CMD_BYTE, pin, value);
}

void can_set_data(uint32_t can_addr, uint8_t value){
    write_gpio_8(can_addr, CAN_PTX_BYTE, value);
}

uint8_t can_read_data(uint32_t can_addr){
    return read_gpio_8(can_addr, CAN_PRX_BYTE);
}

void can_init(uint32_t can_addr, uint32_t can_rx_addr){
    uint8_t Status;
    /* reset all pins */
    can_set_pin(can_addr, RST, FALSE);
    can_set_pin(can_addr, ALE, FALSE);
    can_set_pin(can_addr, RD, FALSE);
    can_set_pin(can_addr, WR, FALSE);
    can_set_pin(can_addr, CS, FALSE);

    /* set choose chip */
    //can_set_pin(can_addr, CS, TRUE);

    /* set reset chip */
    can_set_pin(can_addr, RST, TRUE);
    usleep(200);    // wait reset
    can_set_pin(can_addr, RST, FALSE);

    /* reset finish */
    kprintf("can bus reset finished\n");

    /* enter reset mode*/
    do
    {
        write_register(can_addr, SJA_MOD, RM_BIT);
		Status = read_register(can_addr, can_rx_addr, SJA_MOD) ;
    }
    while(!(Status & RM_BIT));
    kprintf("enter reset mode\n");

    // basic mode set
    write_register(can_addr, SJA_BTR0, 0x43);
    write_register(can_addr, SJA_BTR1, 0x2f); //16MHZ
    write_register(can_addr, SJA_CDR, CANMode_BIT|CLKOff_BIT);//Peli CAN, output clock off
    // PeliCAN mode parameters set
    write_register(can_addr, SJA_IER, RIE_BIT); // enable receive irq
    write_register(can_addr, SJA_CMR, RRB_BIT); // release rxd buffer
    kprintf("enter Pelican mode\n");
    // init iden code
    write_register(can_addr, SJA_ACR0, 0x00);
    write_register(can_addr, SJA_ACR1, 0x00);
    write_register(can_addr, SJA_ACR2, 0x00);
    write_register(can_addr, SJA_ACR3, 0x00);
    // init mask
    write_register(can_addr, SJA_AMR0, 0xff);
    write_register(can_addr, SJA_AMR1, 0xff);
    write_register(can_addr, SJA_AMR2, 0xff);
    write_register(can_addr, SJA_AMR3, 0xff);
    kprintf("code and mask set\n");
    // enter normal mode
    Status = read_register(can_addr, can_rx_addr, SJA_MOD);
    do{
        write_register(can_addr, SJA_MOD, 0x00);
        Status = read_register(can_addr, can_rx_addr, SJA_MOD);
    }while(!(Status == 0x00));
    kprintf("enter normal mode\n");
    usleep(1000);
    kprintf("init done\n");
}

void can_txd(uint32_t can_addr, uint32_t can_rx_addr){
    uint8_t Status;

    // init head
        TX_buffer[0] = 0x88;//.7=1 extend；.6=0 data; .3~0=8 data length
        TX_buffer[1] = 0x12;//this node addr
        TX_buffer[2] = 0x23;//
        TX_buffer[3] = 0x56;//
        TX_buffer[4] = 0xff;//
    // init data    
        TX_buffer[5]  = 0x11;
        TX_buffer[6]  = 0x22;
        TX_buffer[7]  = 0x33;
        TX_buffer[8]  = 0x44;//
        TX_buffer[9]  = 0x55;//
        TX_buffer[10] = 0x66;//
        TX_buffer[11] = 0x77;//
        TX_buffer[12] = 0x88;//
        kprintf("init buffer done\n");
    do{
        Status = read_register(can_addr, can_rx_addr, SJA_SR);
    }while(Status & RS_BIT); // SR.4 = 1 receiving , wait
    kprintf("SR4 OK\n");
    do{
        Status = read_register(can_addr, can_rx_addr, SJA_SR);
    }while(!(Status & TCS_BIT)); // SR.3 = 0 transfering , wait
    kprintf("SR3 OK\n");
    do{
        Status = read_register(can_addr, can_rx_addr, SJA_SR);
    }while(!(Status & TBS_BIT)); //SR.2 = 0, transfer reg is locked, wait
    kprintf("SR2 OK\n");
    // transfer data to buffer
    write_register(can_addr, SJA_TBSR2, TX_buffer[2]);
    write_register(can_addr, SJA_TBSR3, TX_buffer[3]);
    write_register(can_addr, SJA_TBSR4, TX_buffer[4]);
    write_register(can_addr, SJA_TBSR5, TX_buffer[5]);
    write_register(can_addr, SJA_TBSR1, TX_buffer[1]);
    write_register(can_addr, SJA_TBSR0, TX_buffer[0]);
    write_register(can_addr, SJA_TBSR6, TX_buffer[6]);
    write_register(can_addr, SJA_TBSR7, TX_buffer[7]);
    write_register(can_addr, SJA_TBSR8, TX_buffer[8]);
    write_register(can_addr, SJA_TBSR9, TX_buffer[9]);
    write_register(can_addr, SJA_TBSR10, TX_buffer[10]);
    write_register(can_addr, SJA_TBSR11, TX_buffer[11]);
    write_register(can_addr, SJA_TBSR12, TX_buffer[12]);

    //request to transer
    write_register(can_addr, SJA_CMR, TR_BIT | AT_BIT);
    //write_register(can_addr, SJA_CMR, 0x01);
    kprintf("tx done\n");
}


void can_rxd(uint32_t can_addr, uint32_t can_rx_addr){
    uint8_t Status;
    uint8_t Rxd_data;

    Status = read_register(can_addr, can_rx_addr, SJA_IR);

    if(Status & RI_BIT){
        kprintf("DETECTED RXD MSG\n");
        RX_buffer[0] =   read_register(can_addr, can_rx_addr, SJA_RBSR0);
        RX_buffer[1] =   read_register(can_addr, can_rx_addr, SJA_RBSR1);
        RX_buffer[2] =   read_register(can_addr, can_rx_addr, SJA_RBSR2);
        RX_buffer[3] =   read_register(can_addr, can_rx_addr, SJA_RBSR3);
        RX_buffer[4] =   read_register(can_addr, can_rx_addr, SJA_RBSR4);
        RX_buffer[5] =   read_register(can_addr, can_rx_addr, SJA_RBSR5);
        RX_buffer[6] =   read_register(can_addr, can_rx_addr, SJA_RBSR6);
        RX_buffer[7] =   read_register(can_addr, can_rx_addr, SJA_RBSR7);
        RX_buffer[8] =   read_register(can_addr, can_rx_addr, SJA_RBSR8);
        RX_buffer[9] =   read_register(can_addr, can_rx_addr, SJA_RBSR9);
        RX_buffer[10] =  read_register(can_addr, can_rx_addr, SJA_RBSR10);
        RX_buffer[11] =  read_register(can_addr, can_rx_addr, SJA_RBSR11);
        RX_buffer[12] =  read_register(can_addr, can_rx_addr, SJA_RBSR12);

        write_register(can_addr, SJA_CMR, RRB_BIT); // release receive register
        Status = read_register(can_addr, can_rx_addr, SJA_ALC); // release judge catch
        Status = read_register(can_addr, can_rx_addr, SJA_ECC); // release error catch
    }

    write_register(can_addr, SJA_IER, RIE_BIT);// enable rx irq


    Rxd_data = RX_buffer[7];
    kprintf("rx done\n");
    kprintf("rxd data = %d\n", Rxd_data);

}

void write_register(uint32_t can_addr, uint8_t reg_addr, uint8_t reg_data){
    /* set choose chip */
    can_set_pin(can_addr, CS, TRUE);

    /* set ALE, prepare to set addr */
    can_set_pin(can_addr, ALE, TRUE);
    /* transfer addr value*/
    can_set_data(can_addr, reg_addr);
    /* clear ALE */
    can_set_pin(can_addr, ALE, FALSE);
    /* wait 90ns */
    ten_nsleep(9);
    /* set reg data*/
    can_set_data(can_addr, reg_data);
    /* prepare to trans data*/
    can_set_pin(can_addr, WR, TRUE);
    /* wait write */
    ten_nsleep(16);
    /* release chip*/

    can_set_pin(can_addr, WR, FALSE);
    can_set_pin(can_addr, CS, FALSE);
}

uint8_t read_register(uint32_t can_addr,uint32_t can_rxd_addr, uint8_t reg_addr){
    uint8_t reg_data;

    /* set choose chip*/
    can_set_pin(can_addr, CS, TRUE);

    /* set ALE, prepare to set addr */
    can_set_pin(can_addr, ALE, TRUE);
    /* transfer addr value*/
    can_set_data(can_addr, reg_addr);
    /* clear ALE */
    can_set_pin(can_addr, ALE, FALSE);
    /* wait 90ns */
    ten_nsleep(9);
    /* prepare to read data*/
    can_set_pin(can_addr, RD, TRUE);
    /* wait read */
    ten_nsleep(15);
    /* read data */
    reg_data = can_read_data(can_rxd_addr);
    /* clear RD*/
    can_set_pin(can_addr, RD, FALSE);
    /* clear CS*/
    can_set_pin(can_addr, CS, FALSE);

    return reg_data;
}