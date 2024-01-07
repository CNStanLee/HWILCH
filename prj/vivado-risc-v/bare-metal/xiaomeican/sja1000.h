/*
 * sja1000.h
 *
 *  Created on: 2016-6-23
 *      Author: Administrator
 */

#ifndef SJA1000_H_
#define SJA1000_H_

#include "std_types.h"

#define     FrameNum      13//һ֡�ֽ���

uint8_t       RX_buffer[FrameNum];  //���յ�����
uint8_t       TX_buffer[FrameNum];  //���յ�����
uint8_t       Txd_data = 0;//CAN����Ҫ���͵����ݣ�Ҳ��Ҫ�������1-2λ����ʾ������
uint8_t       Rxd_data = 0;//CAN����Ҫ���յ����ݣ�Ҳ��Ҫ�������3-4λ����ʾ������

void Peli_Init(uint32_t BaseAddr); // ��ʼ��CAN����оƬ
void Peli_TXD(void);//CAN�����Ӻ���


//#define SJA_BASE_ADR     CAN_0_BASE	 //�Ĵ�����ַ�Ļ�ַ
#define SJA_BASE_ADR 		0x0			// ���¼Ĵ���������޸�Ϊƫ�Ƶ�ַ

//SJA1000�Ĵ�����ַ���壬������Peliģʽ����չ֡��ʽ
/**********************
ģʽ���ƼĴ�������λ����
************************/
#define SJA_MOD   SJA_BASE_ADR + 0x00

#define RM_BIT     0x01  //��λģʽ����λ
#define LOM_BIT    0x02  //ֻ��ģʽλ
#define STM_BIT    0x04  //�Լ�ģʽλ
#define AFM_BIT    0x08  //�����˲���ģʽλ
#define SM_BIT     0x10  //˯��ģʽλ

/**********************
����Ĵ�������λ����
************************/
#define SJA_CMR    SJA_BASE_ADR + 0x01
#define TR_BIT         0x01       //��������λ
#define AT_BIT         0x02        //��ֹ����λ
#define RRB_BIT        0x04        //�ͷŽ��ջ�����λ
#define CDO_BIT        0x08        //����������λ
#define SRR_BIT        0x10        //������������λ

/**********************
״̬�Ĵ�������λ����
************************/
#define SJA_SR     SJA_BASE_ADR + 0x02
#define RBS_BIT           0x01                    //���ջ�����״̬λ
#define DOS_BIT           0x02                     //�������״̬λ
#define TBS_BIT           0x04                      //���ͻ�����״̬λ
#define TCS_BIT           0x08                      //�������״̬λ
#define RS_BIT            0x10                    //����״̬λ
#define TS_BIT            0x20                     //����״̬λ
#define ES_BIT            0x40                   //����״̬λ
#define BS_BIT            0x80                    //����״̬λ

/**********************
�жϼĴ�������λ����
************************/
#define SJA_IR     SJA_BASE_ADR + 0x03
#define RI_BIT            0x01                     //�����ж�λ
#define TI_BIT            0x02                      //�����ж�λ
#define EI_BIT            0x04                      //���󾯸��ж�λ
#define DOI_BIT           0x08                     //��������ж�λ
#define WUI_BIT           0x10                      //�����ж�λ
#define EPI_BIT           0x20                      //���������ж�λ
#define ALI_BIT           0x40                     //�ٲö�ʧ�ж�λ
#define BEI_BIT           0x80                    //���ߴ����ж�λ

/**********************
�ж�ʹ�ܼĴ�������λ����
************************/
#define SJA_IER    SJA_BASE_ADR + 0x04
#define RIE_BIT           0x01                     //�����ж�ʹ��λ
#define TIE_BIT           0x02                      //�����ж�ʹ��λ
#define EIE_BIT           0x04                      //���󾯸��ж�ʹ��λ
#define DOIE_BIT          0x08                      //��������ж�ʹ��λ
#define WUIE_BIT          0x10                      //�����ж�ʹ��λ
#define EPIE_BIT          0x20                      //���������ж�ʹ��λ
#define ALIE_BIT          0x40                      //�ٲö�ʧ�ж�ʹ��λ
#define BEIE_BIT          0x80                     //���ߴ����ж�ʹ��λ


#define SJA_BTR0   SJA_BASE_ADR + 0x06	  //���߶�ʱ��0�Ĵ���
#define SJA_BTR1   SJA_BASE_ADR + 0x07	  //���߶�ʱ��1�Ĵ���
#define SAM_BIT           0x80               //����ģʽλ��0==���߱�����1�Σ�1== ���߱�����3��

/**********************
������ƼĴ�������λ����
************************/
#define SJA_OCR    SJA_BASE_ADR + 0x08
           /*OCMODE1 ��OCMODE0 */
#define BiPhaseMode       0x00                      //˫�����ģʽ
#define NormalMode        0x02                  //�������ģʽ
#define ClkOutMode        (0x01|0x02)       //ʱ�����ģʽ
           /*TX1 ������ܽ�����*/
#define OCPOL1_BIT        0x20                  //������Կ���λ
#define Tx1Float          0x00                      //����Ϊ����
#define Tx1PullDn         0x40                  //����Ϊ����
#define Tx1PullUp         0x80                 //����Ϊ����
#define Tx1PshPull        (0x40|0x80)       //����Ϊ����
            /*TX0 ������ܽ�����*/
#define OCPOL0_BIT        0x04                      //������Կ���λ
#define Tx0Float          0x00                      //����Ϊ����
#define Tx0PullDn         0x08                      //����Ϊ����
#define Tx0PullUp         0x10                      //����Ϊ����
#define Tx0PshPull        (0x10|0x08)                    //����Ϊ����

#define SJA_TEST   SJA_BASE_ADR + 0x09         //���ԼĴ���

/********************************
 * #define SJA_10   SJA_BASE_ADR + 0x0a   �Ĵ������ܱ���
 ********************************/

/**********************
�����Ĵ�������λ����
************************/
#define SJA_ALC      SJA_BASE_ADR + 0x0b		//�ٲö�ʧ��׽�Ĵ���
#define SJA_ECC      SJA_BASE_ADR + 0x0c		//����׽�Ĵ���
#define SJA_EWLR     SJA_BASE_ADR + 0x0d		//���󱨾����ƼĴ���
#define SJA_RXERR    SJA_BASE_ADR + 0x0e		//RX ����������Ĵ���
#define SJA_TXERR    SJA_BASE_ADR + 0x0f		//TX ����������Ĵ���

/**********************
�����˲����Ĵ�������λ����
************************/
#define SJA_ACR0     SJA_BASE_ADR + 0x10		//���մ���0�Ĵ���
#define SJA_ACR1     SJA_BASE_ADR + 0x11		//���մ���1�Ĵ���
#define SJA_ACR2     SJA_BASE_ADR + 0x12		//���մ���2�Ĵ���
#define SJA_ACR3     SJA_BASE_ADR + 0x13		//���մ���3�Ĵ���

#define SJA_AMR0     SJA_BASE_ADR + 0x14		//��������0�Ĵ���
#define SJA_AMR1     SJA_BASE_ADR + 0x15		//��������1�Ĵ���
#define SJA_AMR2     SJA_BASE_ADR + 0x16		//��������2�Ĵ���
#define SJA_AMR3     SJA_BASE_ADR + 0x17		//��������3�Ĵ���

/**********************
TX��������ַ����
************************/
#define SJA_TBSR0    SJA_BASE_ADR + 0x10
#define SJA_TBSR1    SJA_BASE_ADR + 0x11
#define SJA_TBSR2    SJA_BASE_ADR + 0x12
#define SJA_TBSR3    SJA_BASE_ADR + 0x13
#define SJA_TBSR4    SJA_BASE_ADR + 0x14
#define SJA_TBSR5    SJA_BASE_ADR + 0x15
#define SJA_TBSR6    SJA_BASE_ADR + 0x16
#define SJA_TBSR7    SJA_BASE_ADR + 0x17
#define SJA_TBSR8    SJA_BASE_ADR + 0x18
#define SJA_TBSR9    SJA_BASE_ADR + 0x19
#define SJA_TBSR10   SJA_BASE_ADR + 0x1a
#define SJA_TBSR11   SJA_BASE_ADR + 0x1b
#define SJA_TBSR12   SJA_BASE_ADR + 0x1c

/**********************
RX��������ַ����
************************/
#define SJA_RBSR0    SJA_BASE_ADR + 0x10
#define SJA_RBSR1    SJA_BASE_ADR + 0x11
#define SJA_RBSR2    SJA_BASE_ADR + 0x12
#define SJA_RBSR3    SJA_BASE_ADR + 0x13
#define SJA_RBSR4    SJA_BASE_ADR + 0x14
#define SJA_RBSR5    SJA_BASE_ADR + 0x15
#define SJA_RBSR6    SJA_BASE_ADR + 0x16
#define SJA_RBSR7    SJA_BASE_ADR + 0x17
#define SJA_RBSR8    SJA_BASE_ADR + 0x18
#define SJA_RBSR9    SJA_BASE_ADR + 0x19
#define SJA_RBSR10   SJA_BASE_ADR + 0x1a
#define SJA_RBSR11   SJA_BASE_ADR + 0x1b
#define SJA_RBSR12   SJA_BASE_ADR + 0x1c

#define SJA_RMC      SJA_BASE_ADR + 0x1d           //RX ��Ϣ������ �Ĵ���
#define SJA_RBSA     SJA_BASE_ADR + 0x1e            //RX ��������ʼ��ַ �Ĵ���

/**********************
ʱ�ӷ�Ƶ�Ĵ�����ַ����
************************/
#define SJA_CDR       SJA_BASE_ADR + 0x1f		 //ʱ�ӷ�Ƶ �Ĵ���
#define CLKOff_BIT        0x08                     //ʱ�ӹر�λ��ʱ������ܽſ���λ
#define RXINTEN_BIT       0x20                      //���ڽ����жϵĹܽ�TX1
#define CBP_BIT           0x40                      //CAN �Ƚ�����·����λ
#define CANMode_BIT       0x80                     //CAN ģʽ����λ

#endif /* SJA1000_H_ */