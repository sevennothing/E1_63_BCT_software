/******************************************************************
** �ļ�����sw1021.h
** Copyright (c) 2016  �ɶ�����о
** �����ˣ�caijun.Li

** �衡�������ý⸴��оƬͷ�ļ�
**         
** �ա��ڣ�2016.03.17

**
** ��������Keil C51 V5.5 �� MCC68K V4.4
**         
** ��  ����1.0
********************************************************************/

#ifndef __SW1021_H
#define __SW1021_H

#define SOFTWARE_RST_REG		0x001
	#define SRST_E1		(1 << 7)
	#define SRST_SDH_A  (1 << 6)
	#define SRST_SDH_B  (1 << 5)
	
	#define RST_CANCLE   0x00
	
#define E1_CTRL_REG		0x002
	#define E1LOSAISE		(1 << 4)   /* E1 �źŶ�ʧ���Զ������в���AIS */
	#define TCLKI				(1 << 3)	 /* ����E1��·ʱ�ӷ���  */
	#define RCLKI				(1 << 2) 	/* ����E1��·ʱ�ӷ���  */
	#define TRZSEL			(1 << 1)  /* ѡ���͹����� (HDB3 ��Ч)*/
	#define RRZSEL			(1 << 0)  /* ѡ����չ����� (HDB3 ��Ч)*/
	
#define SDH_IF_CLK_REG	0x004  /* SDH �ӿ�ʱ�ӷ���ѡ��Ĵ��� */
	#define BDCLKI			(1 << 3)	/* BDCLK ʱ�ӷ���ѡ�� */
	#define BACLKI			(1 << 2)	/* BACLK ʱ�ӷ���ѡ�� */
	#define ADCLKI			(1 << 1)	/* ADCLK ʱ�ӷ���ѡ�� */
	#define AACLKI			(1 << 0)	/* AACLK ʱ�ӷ���ѡ�� */
	
#define E1_IF_STATUS_REG		0x006
	#define E1LOF		(1 << 7)		/* E1 ͨ��֡��ʧ�澯 */
	#define E1LOMF		(1 << 6)		/* E1 ͨ��CRC��֡��ʧ�澯 */
	#define E1RDI		(1 << 5)		/* E1 ͨ��Զ�� RDI�澯 */
	#define EXTLOC		(1 << 4)		/* �ⲿʱ�Ӷ�ʧ */
	#define BDLOC		(1 << 3)		/* B ·��������ʱ�Ӷ�ʧ */
	#define BALOC		(1 << 2)		/* B ·��������ʱ�Ӷ�ʧ�澯 */
	#define ADLOC		(1 << 1)		/* A ·��������ʱ�Ӷ�ʧ */
	#define AALOC		(1 << 2)		/* A ·��������ʱ�Ӷ�ʧ�澯 */
	
#define PORT_RST_REG(PID)  (((PID) * 0x20) + 0x20 + (0x20 * ((PID)/7)))   // PID form 0 to 20
	#define RST_PORT	(1 << 7)


#define PORT_CFG_REG(PID)  (((PID) * 0x20) + 0x21 + (0x20 * ((PID)/7)))
	#define A_DOWN	(0 << 5)
	#define B_DOWN	(1 << 5)
	#define A_UP_DOWN	(2 << 5)
	#define B_UP_DOWN	 (3 << 5)
	#define A_DOWN_B_UP	(4 << 5)
	#define A_UP_B_DOWN (5 << 5)
	#define A_DOWN_AB_UP	(6 << 5)
	#define B_DOWN_AB_UP	(7 << 5)
	#define NRZSEL	(1 << 4)    /* ����ʹ��NRZ�� , Ĭ��ʹ��HDB3*/
	#define RnEN		(1 << 3)		/* ���յ�ʱ�������������  */
	#define RCLKSEL	(1 << 2)		/* ѡ������ʧ�٣�Ĭ��ʹ�ý�ӳ��ָ�ʱ�� */
	#define TAISEN   (1 << 1) 	/* ����E1��·AISʹ�� */
	#define RAISEN   (1 << 0)   /* ����E1��·AISʹ��  */ 

#define PORT_TEST_REG(PID)  (((PID) * 0x20) + 0x22 + (0x20 * ((PID)/7)))
	#define E1CRCEN		(1 << 7)
	#define E1CRCT		(1 << 6)		/* ��������»����ߵ����ݵ�CRC , Ĭ��ΪE1��·���� */
	#define	PRBSEN		(1 << 5)		/* ��α������з����� */
	#define PRBST			(1 << 4)    /* α������з��͵��ϻ����ߣ�Ĭ�Ϸ��͵�E1��· */
	#define ANANEN		(1 << 3)    /* ��α������з����� */
	#define ANANT			(1 << 2)    /* ��������»����ߵ����ݣ� Ĭ�ϼ쳵����E1��·������  */
	#define E1LBK			(1 << 1)		/* ��·���豸���� */
	#define LnLBK			(1 << 0)    /* ��·���� */
	
#define PORT_E1_ALARM_INTERRPUT_MASK_REG(PID)  (((PID) * 0x20) + 0x24 + (0x20 * ((PID)/7)))
	#define TFFAERRE		(1 << 5)   /* ������ ���� FIFO_A �������澯�ж� */
	#define TFFBERRE		(1 << 4)   /* ������ ���� FIFO_B �������澯�ж� */
	#define RFFERRE		  (1 << 3)   /* ������ ���� FIFO   �������澯�ж� */
	#define E1AISE   		(1 << 2)   /* ������ E1ӳ������AIS�������ж� */
	#define E1LOCE  		(1 << 1)   /* ������ E1ӳ��ʱ�Ӷ�ʧ�������ж� */
	#define E1LOSE		  (1 << 0)   /* ������ E1ӳ�����ݶ�ʧ�������ж�*/

#define PORT_E1_ALARM_REG(PID)  (((PID) * 0x20) + 0x25 + (0x20 * ((PID)/7)))
	#define TFFAERR   (1 << 5)	/* ���з���FIFO_A �������澯 */
	#define TFFBERR   (1 << 4)  /* ���з���FIFO_B �������澯 */
	#define RFFERR   (1 << 3)		/* ���з���FIFO �������澯 */
	#define E1AIS   (1 << 2)		/*  E1 ӳ������AIS */ 
	#define E1LOC   (1 << 1)		/* E1 ӳ��ʱ�Ӷ�ʧ */
	#define E1LOS   (1 << 0)		/* E1 ӳ�����ݶ�ʧ */

#define HDB3_CV_REG(PID)  		 (((PID) * 0x20) + 0x26 + (0x20 * ((PID)/7)))


#define  A_BUS_BASE    0x400
#define  B_BUS_BASE    0x800

#define  SDH_CTRL_REG(SUB) (SUB)
	#define IOP		(1 << 7)          /* ������żУ��Ϊ��У�飬 Ĭ��ΪżУ��*/
	#define IC1J1V1EN		(1 << 6)    /* �������� ADC1J1V1 �μ���żУ�����  */
	#define IPLEN				(1 << 5)    /* �������� ADSPE �μ���żУ����� */
	#define OOP					(1 << 4)    /* �����żУ�����Ϊ��У�飬 Ĭ��ΪżУ�� */
	#define LC1J1V1EN   (1 << 3)    /* �����żУ�� ��Ҫ C1J1V1 �μ�  */
	#define LPLEN				(1 << 2) 		/* �����У�� ��Ҫ SPE �μ�  */
	#define UP_DELAY_2		(1 << 1)    /* ��ʱ2��19Mʱ�ӣ� Ĭ����ʱһ��19M ʱ�� */
	#define RH4SEL			 ( 1 << 0)  /* �� H4 ���и�֡��λ  */
	
#define  BUS_CTRL_REG(SUB) ((SUB) + 0x01)     /* SUB : A_BUS_BASE or B_BUS_BASE*/
	#define UPBUS_TIMER   (0x2 << 6)
	#define DOWNBUS_TIMER   (0x3 << 6)
	#define AHZE (1 << 5)
	#define ADDI (1 << 4)   /* �����������ָʾ�ź�ADD��ָʾ��Ч */
	#define TH4EN  (1 << 3)
	//TODO: V5 ����ֵ
	
/* ����J2 */
#define J2_RX_DATA_REG(SUB)		((SUB) + 6)
#define J2_RX_CTRL_REG(SUB)		((SUB) + 7)
	#define RW_SEL		(1 << 7)	/* д����J2*/
	//RJ2SLOT
#define J2_RX_J2TYPE_CTRL_REG(SUB)	((SUB) + 8)
	#define EXPECT_J2  ( 3<< 6)
	//J2RNUM		(0 ~15)
#define J2_RAM_STATUS_REG(SUB)	((SUB)+9)
	#define READY    1			/* �ڲ�RAM������� */
	
/* ���� J2 */
#define J2_TX_DATA_REG(SUB)	((SUB)+0x0A)
#define J2_TX_SLOT_REG(SUB)	((SUB)+0x0B)
	//#define RW_SEL		(1 << 7)	/* д����J2*/
	//TJ2SLOT
#define J2_TX_WHICH_REG(SUB)	((SUB)+0x0C)
	//J2TNUM
#define J2_TX_RAM_STATUS_REG(SUB)	((SUB)+0x0D)
	#define NOBUSY	(1 << 7)  /* �ڲ�RAM������� */
	
	
	
#define RXTU12_SLOT_REG(SUB,PID)  (((PID) * 0x20) + 0x20 + (0x20 * ((PID)/7)) + (SUB))
	#define TRDI_EN		(1 << 7)  /* ����RDI��������ͨ�� */
#define TXTU12_SLOT_REG(SUB,PID)  (((PID) * 0x20) + 0x21 + (0x20 * ((PID)/7)) + (SUB))
#define TX_CTRL_REG(SUB,PID)  (((PID) * 0x20) + 0x22 + (0x20 * ((PID)/7)) + (SUB))
	#define AIS_EN   (1 << 1)
	#define TUNEQ		 (1 << 0)

#define TX_V5_REG(SUB,PID)  (((PID) * 0x20) + 0x23 + (0x20 * ((PID)/7)) + (SUB))
	#define REI_CNF	(1 << 5)
	#define CRFI		(1 << 4)
	//#define PLAB_CNF
	#define RDI_CNF		(1 << 0)
	//#define TEST_BIP  

#define TX_K4_REG(SUB,PID)  (((PID) * 0x20) + 0x24 + (0x20 * ((PID)/7)) + (SUB))	


#define INTERRUPT_ENABLE_REG(SUB,PID)  (((PID) * 0x20) + 0x26 + (0x20 * ((PID)/7)) + (SUB))
	#define LOPE	(1 << 7)  
	#define AISE	(1 << 6)
	#define JUSTE	 (1 << 5)		/* ָ�����ʱ�����ж� */
	#define RDIE   (1 << 4)
	#define RFIE   (1 << 3)
	#define J2_RTIME   (1 << 2)
	#define J2_RTIUE   (1 << 1)
	#define PLME   (1 << 0)
	
//#define INTERRUPT_ENABLE_REG2(SUB,PID)  EX_INTERRUPT_ENABLE_REG_##PID(SUB) + 1
#define INTERRUPT_ENABLE_REG2(SUB,PID)  (((PID) * 0x20) + 0x27 + (0x20 * ((PID)/7)) + (SUB))
	#define NDFE   (1 << 1)
	#define UNEQE   (1 << 0)
	
#define TU12_J2_STATUS_REG(SUB,PID)  (((PID) * 0x20) + 0x29 + (0x20 * ((PID)/7)) + (SUB)) /* �������棬 ʵʱ��� */
	#define LOPV   (1 << 7)			/* ָ�붪ʧ */
	#define AISV   (1 << 6)			/* оƬ����AIS ״̬ */
	#define RDIV   (1 << 5)			/* */
	#define RFIV   (1 << 4)			/* */
	#define RTIMV   (1 << 3)			/* ���յ�J2 ��ƥ�� */
	#define RTIUV   (1 << 2)			/* ���յ�J2 ���ȶ� */
	#define UNEQV   (1 << 1)			/* */
	#define PLMV   (1 << 0)			/* �������澯 SLM ? */
	
	
#define TU12_J2_STATUS_REG2(SUB,PID)  (((PID) * 0x20) + 0x2A + (0x20 * ((PID)/7)) + (SUB))   /* �����棬������� */
	#define LOPI   (1 << 7)			/* ָ�붪ʧ */ 
	#define AISI   (1 << 6)			/* оƬ����AIS ״̬ */
	#define RDII   (1 << 5)			/* */
	#define RFII   (1 << 4)			/* */
	#define RTIMI   (1 << 3)			/* ���յ�J2 ��ƥ�� */
	#define RTIUI   (1 << 2)			/* ���յ�J2 ���ȶ� */
	#define UNEQI   (1 << 1)			/* */
	#define PLMI   (1 << 0)			/* �������澯 SLM ? */

#define RX_K4_REG(SUB,PID)  (((PID) * 0x20) + 0x2C + (0x20 * ((PID)/7)) + (SUB))	


#define BIPERR_COUNTER_REG(SUB,PID)  (((PID) * 0x20) + 0x2E + (0x20 * ((PID)/7)) + (SUB)) /* 16 λ�Ĵ����� ��Ҫ������������ֲ�  */

#define REI_COUNTER_REG(SUB,PID)  (((PID) * 0x20) + 0x30 + (0x20 * ((PID)/7)) + (SUB))   /* 16 λ�Ĵ����� ��Ҫ������������ֲ�  */


#endif