
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

/////////// CAN 0 ��Ϊ���ͣ� CAN 1 ��Ϊ���ܣ���֤2��ͨ��
#define CAN_TX_BASE		CAN1_BASE
#define CAN_RX_BASE 	CAN0_BASE

uint8_t DisBuff[4];

uint8_t rx_flag = 0;

void delay(uint32_t Delay)  //��ʱ
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
{//INT0����Ϊ��������
    //EA = 0;
    Txd_data++; //�洢�����������Ϊ�����͵�����
	Peli_TXD();
    //EA = 1;
}

void Peli_RXD(void)
{
	//�������ݺ��������жϷ�������е���

    uint8_t Status;

    Status = IORD_8DIRECT(CAN_RX_BASE, SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 �����ж�
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
        Status = IORD_8DIRECT(CAN_RX_BASE, SJA_ALC);//�ͷ��ٲ���ʱ��׽�Ĵ���
        Status = IORD_8DIRECT(CAN_RX_BASE, SJA_ECC);//�ͷŴ�����벶׽�Ĵ���
    }
    IOWR_8DIRECT(CAN_RX_BASE, SJA_IER, RIE_BIT);// .0=1--�����ж�ʹ�ܣ�

	Rxd_data = RX_buffer[5];

}

void CAN_0_isr(void* context, alt_u32 id){

	//�������ݺ��������жϷ�������е���

    uint8_t Status;

    Status = IORD_8DIRECT(CAN0_BASE, SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 �����ж�
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
        Status = IORD_8DIRECT(CAN0_BASE, SJA_ALC);//�ͷ��ٲ���ʱ��׽�Ĵ���
        Status = IORD_8DIRECT(CAN0_BASE, SJA_ECC);//�ͷŴ�����벶׽�Ĵ���
    }
    IOWR_8DIRECT(CAN0_BASE, SJA_IER, RIE_BIT);// .0=1--�����ж�ʹ�ܣ�
    rx_flag = 1;
	Rxd_data = RX_buffer[5];
}

void CAN_1_isr(void* context, alt_u32 id){

	//�������ݺ��������жϷ�������е���

    uint8_t Status;

    Status = IORD_8DIRECT(CAN1_BASE, SJA_IR);
    if(Status & RI_BIT)
    {//IR.0 = 1 �����ж�
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
        Status = IORD_8DIRECT(CAN1_BASE, SJA_ALC);//�ͷ��ٲ���ʱ��׽�Ĵ���
        Status = IORD_8DIRECT(CAN1_BASE, SJA_ECC);//�ͷŴ�����벶׽�Ĵ���
    }
    IOWR_8DIRECT(CAN1_BASE, SJA_IER, RIE_BIT);// .0=1--�����ж�ʹ�ܣ�
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
	//SJA1000 �ĳ�ʼ��
    uint8_t Status;
    do
    {//.0=1---reset MODRe,���븴λģʽ���Ա�������Ӧ�ļĴ���
     //��ֹδ���븴λģʽ���ظ�д��
        IOWR_8DIRECT(BaseAddr, SJA_MOD, RM_BIT);
		Status = IORD_8DIRECT(BaseAddr, SJA_MOD) ;
    }
    while(!(Status & RM_BIT));

    IOWR_8DIRECT(BaseAddr, SJA_BTR0, 0x00);		// tscl = 2tclk, tsjw = tscl, sam = single, tseg2 = 2tscl, tseg1 = 5tscl (tsyncseg = 1tscl)
    IOWR_8DIRECT(BaseAddr, SJA_BTR1, 0x14);//16M���񣬲�����1Mbps

    IOWR_8DIRECT(BaseAddr, SJA_CDR, CANMode_BIT|CLKOff_BIT);// CDR.3=1--ʱ�ӹر�, .7=0---basic CAN, .7=1---Peli CAN

    // ���¾���PeliCANģʽ����
    IOWR_8DIRECT(BaseAddr, SJA_IER, RIE_BIT);// .0=1--�����ж�ʹ�ܣ�  .1=0--�رշ����ж�ʹ��
//    IOWR_8DIRECT(SJA_OCR, 0, NormalMode|Tx0PullDn|OCPOL1_BIT|Tx1PullUp);// ����������ƼĴ���		�߼����漰 by xujy
    IOWR_8DIRECT(BaseAddr, SJA_CMR, RRB_BIT);//�ͷŽ��ջ�����

    IOWR_8DIRECT(BaseAddr, SJA_ACR0, 0xa6);
    IOWR_8DIRECT(BaseAddr, SJA_ACR1, 0xb0);
    IOWR_8DIRECT(BaseAddr, SJA_ACR2, 0x12);
    IOWR_8DIRECT(BaseAddr, SJA_ACR3, 0x30);//��ʼ����ʾ��

    IOWR_8DIRECT(BaseAddr, SJA_AMR0, 0xff);
    IOWR_8DIRECT(BaseAddr, SJA_AMR1, 0xff);
    IOWR_8DIRECT(BaseAddr, SJA_AMR2, 0xff);
    IOWR_8DIRECT(BaseAddr, SJA_AMR3, 0xff);//��ʼ������

    Status  = IORD_8DIRECT(BaseAddr, SJA_MOD);		// ����Ƿ�����RESETģʽ
    do
    {
//    	IOWR_8DIRECT(SJA_MOD, 0, STM_BIT);	//�����Խ���ģʽ
    	IOWR_8DIRECT(BaseAddr, SJA_MOD, AFM_BIT);	// ����ģʽ
	Status  = IORD_8DIRECT(BaseAddr, SJA_MOD);
     }
    while( !(Status == AFM_BIT) );
//    while( !(Status & STM_BIT) );
    mDelay(20000);		// �ȴ��㹻ʱ����CAN�ص�����ģʽ
    printf("INIT_DONE!!\r\n");
}//SJA1000 �ĳ�ʼ��

void Peli_TXD( void )
{
    uint8_t Status;
    //��ʼ����ʾ��ͷ��Ϣ
        TX_buffer[0] = 0x88;//.7=1��չ֡��.6=0����֡; .3~0=8���ݳ���
        TX_buffer[1] = 0x01;//���ڵ��ַ
        TX_buffer[2] = 0x02;//
        TX_buffer[3] = 0x03;//
        TX_buffer[4] = 0xff;//

    //��ʼ���������ݵ�Ԫ
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
    while( Status & RS_BIT);  //SR.4=1 ���ڽ��գ��ȴ�

    do
    {
    	Status = IORD_8DIRECT(CAN_TX_BASE, SJA_SR);
    }
    while(!(Status & TCS_BIT)); //SR.3=0,��������δ�����꣬�ȴ�

    do
    {
    	Status = IORD_8DIRECT(CAN_TX_BASE, SJA_SR);
    }
    while(!(Status & TBS_BIT)); //SR.2=0,���ͻ������������ȴ�

    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR0, TX_buffer[0]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR1, TX_buffer[1]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR2, TX_buffer[2]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR3, TX_buffer[3]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR4, TX_buffer[4]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR5, TX_buffer[5]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR6, TX_buffer[6]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR7, TX_buffer[7]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR8, TX_buffer[8]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR9, TX_buffer[9]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR10, TX_buffer[10]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR11, TX_buffer[11]);
    IOWR_8DIRECT(CAN_TX_BASE, SJA_TBSR12, TX_buffer[12]);

//    IOWR_8DIRECT(SJA_CMR, 0, SRR_BIT);//��λ�Է��ͽ�������
    IOWR_8DIRECT(CAN_TX_BASE, SJA_CMR, TR_BIT | AT_BIT);//��λ��������
    printf("TX DONE!!\r\n");
}
