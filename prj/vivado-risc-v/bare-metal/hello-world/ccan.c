#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "ccan.h"

#define SET_BIT(data, bit)		data = ((data) | (0x1 << (bit)))
#define CLEAR_BIT(data, bit)    data = ((data) & (~(0x1 << (bit))))

uint8_t port_free;
uint8_t       CAN_RX_buffer[FrameNum];
uint8_t       CAN_TX_buffer[FrameNum]; 


static void usleep2(unsigned us) {
    uintptr_t cycles0;
    uintptr_t cycles1;
    asm volatile ("csrr %0, 0xB00" : "=r" (cycles0));
    for (;;) {
        asm volatile ("csrr %0, 0xB00" : "=r" (cycles1));
        if (cycles1 - cycles0 >= us * 100) break;
    }
}

static void nsleep(unsigned ten_ns) {
    uintptr_t cycles0;
    uintptr_t cycles1;
    asm volatile ("csrr %0, 0xB00" : "=r" (cycles0));
    for (;;) {
        asm volatile ("csrr %0, 0xB00" : "=r" (cycles1));
        if (cycles1 - cycles0 >= ten_ns ) break;
    }
}

void data_txd1(){
    uint8_t Status;
    // head of the code
        CAN_TX_buffer[0] = 0x88;//
        CAN_TX_buffer[1] = 0x01;// addr of this node
        CAN_TX_buffer[2] = 0x02;//
        CAN_TX_buffer[3] = 0x03;//
        CAN_TX_buffer[4] = 0xff;//

    // data
        CAN_TX_buffer[5]  = 0x11;
        CAN_TX_buffer[6]  = 0x22;
        CAN_TX_buffer[7]  = 0x33;
        CAN_TX_buffer[8]  = 0x44;//
        CAN_TX_buffer[9]  = 0x55;//
        CAN_TX_buffer[10] = 0x66;//
        CAN_TX_buffer[11] = 0x77;//
        CAN_TX_buffer[12] = 0x88;//

    do
    {
        Status = read_register1(SJA_SR);
    }
    while( Status & RS_BIT);  //SR.4=1 receiving

    do
    {
    	Status = read_register1(SJA_SR);
    }
    while(!(Status & TCS_BIT)); //SR.3=0,tx request is not done

    do
    {
    	Status = read_register1(SJA_SR);
    }
    while(!(Status & TBS_BIT)); //SR.2=0 lock
    

    // write data
    write_register1(SJA_TBSR2, CAN_TX_buffer[2]);
    write_register1(SJA_TBSR3, CAN_TX_buffer[3]);
    write_register1(SJA_TBSR4, CAN_TX_buffer[4]);
    write_register1(SJA_TBSR5, CAN_TX_buffer[5]);
    write_register1(SJA_TBSR1, CAN_TX_buffer[1]);
    write_register1(SJA_TBSR0, CAN_TX_buffer[0]);
    write_register1(SJA_TBSR6, CAN_TX_buffer[6]);
    write_register1(SJA_TBSR7, CAN_TX_buffer[7]);
    write_register1(SJA_TBSR8, CAN_TX_buffer[8]);
    write_register1(SJA_TBSR9, CAN_TX_buffer[9]);
    write_register1(SJA_TBSR10, CAN_TX_buffer[10]);
    write_register1(SJA_TBSR11, CAN_TX_buffer[11]);
    write_register1(SJA_TBSR12, CAN_TX_buffer[12]);

    // request transfer
    write_register1(SJA_TBSR12, CAN_TX_buffer[12]);

}

void data_rxd2(){
    uint8_t Status;

    Status = read_register2(SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 接收中断
        CAN_RX_buffer[0] =   read_register2(SJA_RBSR0);
        CAN_RX_buffer[1] =   read_register2(SJA_RBSR1);
        CAN_RX_buffer[2] =   read_register2(SJA_RBSR2);
        CAN_RX_buffer[3] =   read_register2(SJA_RBSR3);
        CAN_RX_buffer[4] =   read_register2(SJA_RBSR4);
        CAN_RX_buffer[5] =   read_register2(SJA_RBSR5);
        CAN_RX_buffer[6] =   read_register2(SJA_RBSR6);
        CAN_RX_buffer[7] =   read_register2(SJA_RBSR7);
        CAN_RX_buffer[8] =   read_register2(SJA_RBSR8);
        CAN_RX_buffer[9] =   read_register2(SJA_RBSR9);
        CAN_RX_buffer[10] =  read_register2(SJA_RBSR10);
        CAN_RX_buffer[11] =  read_register2(SJA_RBSR11);
        CAN_RX_buffer[12] =  read_register2(SJA_RBSR12);

        write_register2(SJA_CMR, RRB_BIT);
        Status = read_register2(SJA_ALC);//释放仲裁随时捕捉寄存器
        Status = read_register2(SJA_ECC);//释放错误代码捕捉寄存器
    }
    write_register2(SJA_IER, RIE_BIT);// .0=1--接收中断使能；

	//Rxd_data = RX_buffer[5];
	
}

void can_init_test(){
    struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;

    regs->can1_cmd = 0x12;
    regs->can1_txd = 0x23;
    regs->can2_cmd = 0x34;
    regs->can2_txd = 0x45;

    usleep2(200);

    

    //write_register1(SJA_MOD, RM_BIT);
    //Status = read_register1(SJA_MOD);
}


void can_init(){
    struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;
    //regs->led_group1 = 0x11;
    uint8_t Status;
    //SJA1000 Initialization
    
    CLEAR_BIT(regs->can1_cmd, CAN_RST);
    CLEAR_BIT(regs->can1_cmd, CAN_ALE);
    CLEAR_BIT(regs->can1_cmd, CAN_RD);
    CLEAR_BIT(regs->can1_cmd, CAN_WR);
    CLEAR_BIT(regs->can1_cmd, CAN_CS);

    CLEAR_BIT(regs->can1_cmd, CAN_RST);
    CLEAR_BIT(regs->can1_cmd, CAN_ALE);
    CLEAR_BIT(regs->can1_cmd, CAN_RD);
    CLEAR_BIT(regs->can1_cmd, CAN_WR);
    CLEAR_BIT(regs->can1_cmd, CAN_CS);

    SET_BIT(regs->can1_cmd, CAN_RST);
    SET_BIT(regs->can2_cmd, CAN_RST);

    usleep2(200);

    CLEAR_BIT(regs->can1_cmd, CAN_RST);
    CLEAR_BIT(regs->can2_cmd, CAN_RST);

    usleep2(200);

    // Enter reset mode

    do{
        write_register1(SJA_MOD, RM_BIT);
        Status = read_register1(SJA_MOD);
    }
    while(!(Status & RM_BIT));

    do{
        write_register2(SJA_MOD, RM_BIT);
        Status = read_register2(SJA_MOD);
    }
    while(!(Status & RM_BIT));

    // set bus timing reg 0
    write_register1(SJA_BTR0, 0x00);
    write_register2(SJA_BTR0, 0x00);

    // set bus timing reg 1
    write_register1(SJA_BTR1, 0x14);
    write_register2(SJA_BTR1, 0x14);

    // 16M clock, baud 1Mbps
    write_register1(SJA_CDR, CANMode_BIT|CLKOff_BIT);   //extended mode
    write_register2(SJA_CDR, CANMode_BIT|CLKOff_BIT);

    // Enable Receive Irq
    write_register1(SJA_IER, RIE_BIT);
    write_register2(SJA_IER, RIE_BIT);

    // Release Receive buffer

    write_register1(SJA_CMR, RRB_BIT);
    write_register2(SJA_CMR, RRB_BIT);


    // init code and mask

    write_register1(SJA_ACR0, RRB_BIT);

    write_register1(SJA_ACR0, 0xa6);
    write_register1(SJA_ACR1, 0xb0);
    write_register1(SJA_ACR2, 0x12);
    write_register2(SJA_ACR3, 0x30);
    write_register1(SJA_AMR0, 0xff);
    write_register1(SJA_AMR1, 0xff);
    write_register1(SJA_AMR2, 0xff);
    write_register1(SJA_AMR3, 0xff);

    write_register2(SJA_ACR0, 0xa6);
    write_register2(SJA_ACR1, 0xb0);
    write_register2(SJA_ACR2, 0x12);
    write_register2(SJA_ACR3, 0x30);
    write_register2(SJA_AMR0, 0xff);
    write_register2(SJA_AMR1, 0xff);
    write_register2(SJA_AMR2, 0xff);
    write_register2(SJA_AMR3, 0xff);
    
    // Enter Normal mode
    do{
        write_register1(SJA_MOD, AFM_BIT);
        Status = read_register1(SJA_MOD);
    }
    while( !(Status == AFM_BIT) );

    do{
        write_register2(SJA_MOD, AFM_BIT);
        Status = read_register2(SJA_MOD);
    }
    while( !(Status == AFM_BIT) );


    // wait can bus to normal mode
    usleep2(20000);
}

void write_register1(uint8_t reg_addr, uint8_t reg_data){
    struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;
    
    SET_BIT(regs->can1_cmd, CAN_CS);

    // set reg add
    SET_BIT(regs->can1_cmd, CAN_ALE);
    regs->can1_txd = reg_addr;
    CLEAR_BIT(regs->can1_cmd, CAN_ALE);
    nsleep(9); // sleep 90 ns
    // set reg data
    regs->can1_txd = reg_data;
    SET_BIT(regs->can1_cmd, CAN_WR);
    nsleep(15);
    CLEAR_BIT(regs->can1_cmd, CAN_WR);
    // clear cs
    CLEAR_BIT(regs->can1_cmd, CAN_CS);
}

void write_register2(uint8_t reg_addr, uint8_t reg_data){
    struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;
    
    SET_BIT(regs->can2_cmd, CAN_CS);

    // set reg add
    SET_BIT(regs->can2_cmd, CAN_ALE);
    regs->can2_txd = reg_addr;
    CLEAR_BIT(regs->can2_cmd, CAN_ALE);
    nsleep(9); // sleep 90 ns
    // set reg data
    regs->can2_txd = reg_data;
    SET_BIT(regs->can2_cmd, CAN_WR);
    nsleep(15);
    CLEAR_BIT(regs->can2_cmd, CAN_WR);
    // clear cs
    CLEAR_BIT(regs->can2_cmd, CAN_CS);
}

uint8_t read_register1(uint8_t reg_addr){
    uint8_t reg_data;
    struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;
    struct can_regs2 * regs2 = (struct can_regs2 *)CAN_GPIO_BASE_ADDR2;
    
    SET_BIT(regs->can1_cmd, CAN_CS);

    // set reg add
    SET_BIT(regs->can1_cmd, CAN_ALE);
    regs->can1_txd = reg_addr;
    CLEAR_BIT(regs->can1_cmd, CAN_ALE);
    nsleep(9); // sleep 90 ns
    // read reg data
    SET_BIT(regs->can1_cmd, CAN_RD);
    nsleep(15);
    reg_data = regs2->can1_rxd;
    CLEAR_BIT(regs->can1_cmd, CAN_RD);
    // clear cs
    CLEAR_BIT(regs->can1_cmd, CAN_CS);
    return reg_data;
}

uint8_t read_register2(uint8_t reg_addr){
    uint8_t reg_data;
    struct can_regs * regs = (struct can_regs *)CAN_GPIO_BASE_ADDR;
    struct can_regs2 * regs2 = (struct can_regs2 *)CAN_GPIO_BASE_ADDR2;
    
    SET_BIT(regs->can2_cmd, CAN_CS);

    // set reg add
    SET_BIT(regs->can2_cmd, CAN_ALE);
    regs->can2_txd = reg_addr;
    CLEAR_BIT(regs->can2_cmd, CAN_ALE);
    nsleep(9); // sleep 90 ns
    // read reg data
    SET_BIT(regs->can2_cmd, CAN_RD);
    nsleep(15);
    reg_data = regs2->can2_rxd;
    CLEAR_BIT(regs->can2_cmd, CAN_RD);
    // clear cs
    CLEAR_BIT(regs->can2_cmd, CAN_CS);
    return reg_data;
}