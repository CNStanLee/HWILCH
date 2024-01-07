
#include "system.h"
#include "sja1000.h"
#include <stdint.h>
#include <stdio.h>
#include "io.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"
#include "std_types.h"

#define uint unsigned int
#define uchar unsigned char

#define CAN1_BASE CAN_1_BASE
#define CAN0_BASE CAN_0_BASE

/////////// CAN 0 作为发送， CAN 1 作为接受，验证2者通信
#define CAN_TX_BASE		CAN1_BASE
#define CAN_RX_BASE 	CAN0_BASE

uint8_t DisBuff[4];

uint8_t rx_flag = 0;

void delay(uint32_t Delay)  //延时
{
	unsigned  int ty;
	for(;Delay>0;Delay--)
    {
		for(ty=0;ty<10;ty++)
		{;}
	}
}

void mDelay(uint16_t mtime)
{
	for(; mtime > 0; mtime--)
	{
		uint8_t j = 244;
		while(--j);
	}
}

void INT0_Data(void)
{//INT0按键为计数按键
    //EA = 0;
    Txd_data++; //存储计数结果，并为待发送的数据
	Peli_TXD();
    //EA = 1;
}

void Peli_RXD(void)
{
	//接收数据函数，在中断服务程序中调用

    uint8_t Status;

    Status = IORD_8DIRECT(CAN_RX_BASE, SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 接收中断
        RX_buffer[0] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR0);
        RX_buffer[1] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR1);
        RX_buffer[2] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR2);
        RX_buffer[3] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR3);
        RX_buffer[4] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR4);
        RX_buffer[5] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR5);
        RX_buffer[6] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR6);
        RX_buffer[7] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR7);
        RX_buffer[8] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR8);
        RX_buffer[9] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR9);
        RX_buffer[10] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR10);
        RX_buffer[11] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR11);
        RX_buffer[12] =  IORD_8DIRECT(CAN_RX_BASE, SJA_RBSR12);

        IOWR_8DIRECT(CAN_RX_BASE, SJA_CMR, RRB_BIT);
        Status = IORD_8DIRECT(CAN_RX_BASE, SJA_ALC);//释放仲裁随时捕捉寄存器
        Status = IORD_8DIRECT(CAN_RX_BASE, SJA_ECC);//释放错误代码捕捉寄存器
    }
    IOWR_8DIRECT(CAN_RX_BASE, SJA_IER, RIE_BIT);// .0=1--接收中断使能；

	Rxd_data = RX_buffer[5];

}

void CAN_0_isr(void* context, alt_u32 id){

	//接收数据函数，在中断服务程序中调用

    uint8_t Status;

    Status = IORD_8DIRECT(CAN0_BASE, SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 接收中断
        RX_buffer[0] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR0);
        RX_buffer[1] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR1);
        RX_buffer[2] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR2);
        RX_buffer[3] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR3);
        RX_buffer[4] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR4);
        RX_buffer[5] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR5);
        RX_buffer[6] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR6);
        RX_buffer[7] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR7);
        RX_buffer[8] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR8);
        RX_buffer[9] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR9);
        RX_buffer[10] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR10);
        RX_buffer[11] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR11);
        RX_buffer[12] =  IORD_8DIRECT(CAN0_BASE, SJA_RBSR12);

        IOWR_8DIRECT(CAN0_BASE, SJA_CMR, RRB_BIT);
        Status = IORD_8DIRECT(CAN0_BASE, SJA_ALC);//释放仲裁随时捕捉寄存器
        Status = IORD_8DIRECT(CAN0_BASE, SJA_ECC);//释放错误代码捕捉寄存器
    }
    IOWR_8DIRECT(CAN0_BASE, SJA_IER, RIE_BIT);// .0=1--接收中断使能；
    rx_flag = 1;
	Rxd_data = RX_buffer[5];
}

void CAN_1_isr(void* context, alt_u32 id){

	//接收数据函数，在中断服务程序中调用

    uint8_t Status;

    Status = IORD_8DIRECT(CAN1_BASE, SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 接收中断
        RX_buffer[0] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR0);
        RX_buffer[1] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR1);
        RX_buffer[2] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR2);
        RX_buffer[3] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR3);
        RX_buffer[4] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR4);
        RX_buffer[5] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR5);
        RX_buffer[6] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR6);
        RX_buffer[7] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR7);
        RX_buffer[8] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR8);
        RX_buffer[9] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR9);
        RX_buffer[10] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR10);
        RX_buffer[11] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR11);
        RX_buffer[12] =  IORD_8DIRECT(CAN1_BASE, SJA_RBSR12);

        IOWR_8DIRECT(CAN1_BASE, SJA_CMR, RRB_BIT);
        Status = IORD_8DIRECT(CAN1_BASE, SJA_ALC);//释放仲裁随时捕捉寄存器
        Status = IORD_8DIRECT(CAN1_BASE, SJA_ECC);//释放错误代码捕捉寄存器
    }
    IOWR_8DIRECT(CAN1_BASE, SJA_IER, RIE_BIT);// .0=1--接收中断使能；
    rx_flag = 1;
	Rxd_data = RX_buffer[5];
}

int main(void)
{

	int rc,i;
	uint8_t StatusTx, StatusRx;

	printf("Enter main function!!\r\n");
	rc = alt_ic_isr_register(
			CAN_0_IRQ_INTERRUPT_CONTROLLER_ID,
			CAN_0_IRQ,
			(alt_isr_func)&CAN_0_isr,
			NULL,
			NULL);
	rc = alt_ic_isr_register(
			CAN_1_IRQ_INTERRUPT_CONTROLLER_ID,
			CAN_1_IRQ,
			(alt_isr_func)&CAN_1_isr,
			NULL,
			NULL);
    Peli_Init(CAN0_BASE);
    Peli_Init(CAN1_BASE);
	StatusTx = IORD_8DIRECT(CAN_TX_BASE, SJA_SR);
	StatusRx = IORD_8DIRECT(CAN_RX_BASE, SJA_SR);
	mDelay(10);
	Peli_TXD();
    while(1)
    {
    	Peli_TXD();
    	mDelay(10000);
    	if(rx_flag)
    	{
    		printf("RX_buffer\t");
    		for(i=0;i<=12;i++)
    		{
    			printf("%x ,", RX_buffer[i]);
    		}
    		printf("\n");
    		rx_flag = 0;
    	}

    }
   return 0;
}

void Peli_Init(uint32_t BaseAddr)
{
	//SJA1000 的初始化
    uint8_t Status;
    do
    {//.0=1---reset MODRe,进入复位模式，以便设置相应的寄存器
     //防止未进入复位模式，重复写入
        IOWR_8DIRECT(BaseAddr, SJA_MOD, RM_BIT);
		Status = IORD_8DIRECT(BaseAddr, SJA_MOD) ;
    }
    while(!(Status & RM_BIT));

    IOWR_8DIRECT(BaseAddr, SJA_BTR0, 0x00);		// tscl = 2tclk, tsjw = tscl, sam = single, tseg2 = 2tscl, tseg1 = 5tscl (tsyncseg = 1tscl)
    IOWR_8DIRECT(BaseAddr, SJA_BTR1, 0x14);//16M晶振，波特率1Mbps

    IOWR_8DIRECT(BaseAddr, SJA_CDR, CANMode_BIT|CLKOff_BIT);// CDR.3=1--时钟关闭, .7=0---basic CAN, .7=1---Peli CAN

    // 以下均在PeliCAN模式进行
    IOWR_8DIRECT(BaseAddr, SJA_IER, RIE_BIT);// .0=1--接收中断使能；  .1=0--关闭发送中断使能
//    IOWR_8DIRECT(SJA_OCR, 0, NormalMode|Tx0PullDn|OCPOL1_BIT|Tx1PullUp);// 配置输出控制寄存器		逻辑不涉及 by xujy
    IOWR_8DIRECT(BaseAddr, SJA_CMR, RRB_BIT);//释放接收缓冲器

    IOWR_8DIRECT(BaseAddr, SJA_ACR0, 0xa6);
    IOWR_8DIRECT(BaseAddr, SJA_ACR1, 0xb0);
    IOWR_8DIRECT(BaseAddr, SJA_ACR2, 0x12);
    IOWR_8DIRECT(BaseAddr, SJA_ACR3, 0x30);//初始化标示码

    IOWR_8DIRECT(BaseAddr, SJA_AMR0, 0xff);
    IOWR_8DIRECT(BaseAddr, SJA_AMR1, 0xff);
    IOWR_8DIRECT(BaseAddr, SJA_AMR2, 0xff);
    IOWR_8DIRECT(BaseAddr, SJA_AMR3, 0xff);//初始化掩码

    Status  = IORD_8DIRECT(BaseAddr, SJA_MOD);		// 检查是否仍在RESET模式
    do
    {
//    	IOWR_8DIRECT(SJA_MOD, 0, STM_BIT);	//进入自接收模式
    	IOWR_8DIRECT(BaseAddr, SJA_MOD, AFM_BIT);	// 正常模式
	Status  = IORD_8DIRECT(BaseAddr, SJA_MOD);
     }
    while( !(Status == AFM_BIT) );
//    while( !(Status & STM_BIT) );
    mDelay(20000);		// 等待足够时间让CAN回到正常模式
    printf("INIT_DONE!!\r\n");
}//SJA1000 的初始化

void Peli_TXD( void )
{
    uint8_t Status;
    //初始化标示码头信息
        TX_buffer[0] = 0x88;//.7=1扩展帧；.6=0数据帧; .3~0=8数据长度
        TX_buffer[1] = 0x01;//本节点地址
        TX_buffer[2] = 0x02;//
        TX_buffer[3] = 0x03;//
        TX_buffer[4] = 0xff;//

    //初始化发送数据单元
        TX_buffer[5]  = Txd_data;
        TX_buffer[6]  = 0x22;
        TX_buffer[7]  = 0x33;
        TX_buffer[8]  = 0x44;//
        TX_buffer[9]  = 0x55;//
        TX_buffer[10] = 0x66;//
        TX_buffer[11] = 0x77;//
        TX_buffer[12] = 0x88;//

    do
    {
        Status = IORD_8DIRECT(CAN_TX_BASE, SJA_SR);
    }
    while( Status & RS_BIT);  //SR.4=1 正在接收，等待

    do
    {
    	Status = IORD_8DIRECT(CAN_TX_BASE, SJA_SR);
    }
    while(!(Status & TCS_BIT)); //SR.3=0,发送请求未处理完，等待

    do
    {
    	Status = IORD_8DIRECT(CAN_TX_BASE, SJA_SR);
    }
    while(!(Status & TBS_BIT)); //SR.2=0,发送缓冲器被锁。等待

    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR2, TX_buffer[2]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR3, TX_buffer[3]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR4, TX_buffer[4]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR5, TX_buffer[5]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR1, TX_buffer[1]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR0, TX_buffer[0]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR6, TX_buffer[6]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR7, TX_buffer[7]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR8, TX_buffer[8]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR9, TX_buffer[9]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR10, TX_buffer[10]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR11, TX_buffer[11]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR12, TX_buffer[12]);

    //IOWR_8DIRECT(SJA_CMR, 0, SRR_BIT);//置位自发送接收请求
    IOWR_8DIRECT(CAN_TX_BASE, SJA_CMR, TR_BIT | AT_BIT);//置位发送请求
    printf("TX DONE!!\r\n");
}
