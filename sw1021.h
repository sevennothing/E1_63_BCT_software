/******************************************************************
** 文件名：sw1021.h
** Copyright (c) 2016  成都朗锐芯
** 创建人：caijun.Li

** 描　述：复用解复用芯片头文件
**         
** 日　期：2016.03.17

**
** 编译器：Keil C51 V5.5 或 MCC68K V4.4
**         
** 版  本：1.0
********************************************************************/

#ifndef __SW1021_H
#define __SW1021_H

#define SOFTWARE_RST_REG		0x001
	#define SRST_E1		(1 << 7)
	#define SRST_SDH_A  (1 << 6)
	#define SRST_SDH_B  (1 << 5)
	
	#define RST_CANCLE   0x00
	
#define E1_CTRL_REG		0x002
	#define E1LOSAISE		(1 << 4)   /* E1 信号丢失后自动向上行插入AIS */
	#define TCLKI				(1 << 3)	 /* 发送E1线路时钟反相  */
	#define RCLKI				(1 << 2) 	/* 接收E1线路时钟反相  */
	#define TRZSEL			(1 << 1)  /* 选择发送归零码 (HDB3 有效)*/
	#define RRZSEL			(1 << 0)  /* 选择接收归零码 (HDB3 有效)*/
	
#define SDH_IF_CLK_REG	0x004  /* SDH 接口时钟反相选择寄存器 */
	#define BDCLKI			(1 << 3)	/* BDCLK 时钟反相选择 */
	#define BACLKI			(1 << 2)	/* BACLK 时钟反相选择 */
	#define ADCLKI			(1 << 1)	/* ADCLK 时钟反相选择 */
	#define AACLKI			(1 << 0)	/* AACLK 时钟反相选择 */
	
#define E1_IF_STATUS_REG		0x006
	#define E1LOF		(1 << 7)		/* E1 通道帧丢失告警 */
	#define E1LOMF		(1 << 6)		/* E1 通道CRC复帧丢失告警 */
	#define E1RDI		(1 << 5)		/* E1 通道远端 RDI告警 */
	#define EXTLOC		(1 << 4)		/* 外部时钟丢失 */
	#define BDLOC		(1 << 3)		/* B 路下行总线时钟丢失 */
	#define BALOC		(1 << 2)		/* B 路上行总线时钟丢失告警 */
	#define ADLOC		(1 << 1)		/* A 路下行总线时钟丢失 */
	#define AALOC		(1 << 2)		/* A 路上行总线时钟丢失告警 */
	
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
	#define NRZSEL	(1 << 4)    /* 数据使用NRZ码 , 默认使用HDB3*/
	#define RnEN		(1 << 3)		/* 接收的时钟数据正常输出  */
	#define RCLKSEL	(1 << 2)		/* 选择再生失踪，默认使用解映射恢复时钟 */
	#define TAISEN   (1 << 1) 	/* 发送E1线路AIS使能 */
	#define RAISEN   (1 << 0)   /* 接收E1线路AIS使能  */ 

#define PORT_TEST_REG(PID)  (((PID) * 0x20) + 0x22 + (0x20 * ((PID)/7)))
	#define E1CRCEN		(1 << 7)
	#define E1CRCT		(1 << 6)		/* 检测来自下话总线的数据的CRC , 默认为E1线路数据 */
	#define	PRBSEN		(1 << 5)		/* 打开伪随机序列发生器 */
	#define PRBST			(1 << 4)    /* 伪随机序列发送到上话总线，默认发送到E1线路 */
	#define ANANEN		(1 << 3)    /* 打开伪随机序列分析器 */
	#define ANANT			(1 << 2)    /* 检测来自下话总线的数据， 默认检车来自E1线路的数据  */
	#define E1LBK			(1 << 1)		/* 线路侧设备环回 */
	#define LnLBK			(1 << 0)    /* 线路环回 */
	
#define PORT_E1_ALARM_INTERRPUT_MASK_REG(PID)  (((PID) * 0x20) + 0x24 + (0x20 * ((PID)/7)))
	#define TFFAERRE		(1 << 5)   /* 不屏蔽 上行 FIFO_A 溢出错误告警中断 */
	#define TFFBERRE		(1 << 4)   /* 不屏蔽 上行 FIFO_B 溢出错误告警中断 */
	#define RFFERRE		  (1 << 3)   /* 不屏蔽 下行 FIFO   溢出错误告警中断 */
	#define E1AISE   		(1 << 2)   /* 不屏蔽 E1映射数据AIS产生的中断 */
	#define E1LOCE  		(1 << 1)   /* 不屏蔽 E1映射时钟丢失产生的中断 */
	#define E1LOSE		  (1 << 0)   /* 不屏蔽 E1映射数据丢失残生的中断*/

#define PORT_E1_ALARM_REG(PID)  (((PID) * 0x20) + 0x25 + (0x20 * ((PID)/7)))
	#define TFFAERR   (1 << 5)	/* 上行方向FIFO_A 溢出错误告警 */
	#define TFFBERR   (1 << 4)  /* 上行方向FIFO_B 溢出错误告警 */
	#define RFFERR   (1 << 3)		/* 下行方向FIFO 溢出错误告警 */
	#define E1AIS   (1 << 2)		/*  E1 映射数据AIS */ 
	#define E1LOC   (1 << 1)		/* E1 映射时钟丢失 */
	#define E1LOS   (1 << 0)		/* E1 映射数据丢失 */

#define HDB3_CV_REG(PID)  		 (((PID) * 0x20) + 0x26 + (0x20 * ((PID)/7)))


#define  A_BUS_BASE    0x400
#define  B_BUS_BASE    0x800

#define  SDH_CTRL_REG(SUB) (SUB)
	#define IOP		(1 << 7)          /* 输入奇偶校验为奇校验， 默认为偶校验*/
	#define IC1J1V1EN		(1 << 6)    /* 输入下行 ADC1J1V1 参加奇偶校验控制  */
	#define IPLEN				(1 << 5)    /* 输入下行 ADSPE 参加奇偶校验控制 */
	#define OOP					(1 << 4)    /* 输出奇偶校验控制为奇校验， 默认为偶校验 */
	#define LC1J1V1EN   (1 << 3)    /* 输出奇偶校验 需要 C1J1V1 参加  */
	#define LPLEN				(1 << 2) 		/* 输出就校验 需要 SPE 参加  */
	#define UP_DELAY_2		(1 << 1)    /* 延时2个19M时钟， 默认延时一个19M 时钟 */
	#define RH4SEL			 ( 1 << 0)  /* 用 H4 进行复帧定位  */
	
#define  BUS_CTRL_REG(SUB) ((SUB) + 0x01)     /* SUB : A_BUS_BASE or B_BUS_BASE*/
	#define UPBUS_TIMER   (0x2 << 6)
	#define DOWNBUS_TIMER   (0x3 << 6)
	#define AHZE (1 << 5)
	#define ADDI (1 << 4)   /* 上行总线输出指示信号ADD高指示有效 */
	#define TH4EN  (1 << 3)
	//TODO: V5 期望值
	
/* 期望J2 */
#define J2_RX_DATA_REG(SUB)		((SUB) + 6)
#define J2_RX_CTRL_REG(SUB)		((SUB) + 7)
	#define RW_SEL		(1 << 7)	/* 写期望J2*/
	//RJ2SLOT
#define J2_RX_J2TYPE_CTRL_REG(SUB)	((SUB) + 8)
	#define EXPECT_J2  ( 3<< 6)
	//J2RNUM		(0 ~15)
#define J2_RAM_STATUS_REG(SUB)	((SUB)+9)
	#define READY    1			/* 内部RAM操作完成 */
	
/* 发送 J2 */
#define J2_TX_DATA_REG(SUB)	((SUB)+0x0A)
#define J2_TX_SLOT_REG(SUB)	((SUB)+0x0B)
	//#define RW_SEL		(1 << 7)	/* 写发送J2*/
	//TJ2SLOT
#define J2_TX_WHICH_REG(SUB)	((SUB)+0x0C)
	//J2TNUM
#define J2_TX_RAM_STATUS_REG(SUB)	((SUB)+0x0D)
	#define NOBUSY	(1 << 7)  /* 内部RAM操作完成 */
	
	
	
#define RXTU12_SLOT_REG(SUB,PID)  (((PID) * 0x20) + 0x20 + (0x20 * ((PID)/7)) + (SUB))
	#define TRDI_EN		(1 << 7)  /* 屏蔽RDI插入上行通道 */
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
	#define JUSTE	 (1 << 5)		/* 指针调整时产生中断 */
	#define RDIE   (1 << 4)
	#define RFIE   (1 << 3)
	#define J2_RTIME   (1 << 2)
	#define J2_RTIUE   (1 << 1)
	#define PLME   (1 << 0)
	
//#define INTERRUPT_ENABLE_REG2(SUB,PID)  EX_INTERRUPT_ENABLE_REG_##PID(SUB) + 1
#define INTERRUPT_ENABLE_REG2(SUB,PID)  (((PID) * 0x20) + 0x27 + (0x20 * ((PID)/7)) + (SUB))
	#define NDFE   (1 << 1)
	#define UNEQE   (1 << 0)
	
#define TU12_J2_STATUS_REG(SUB,PID)  (((PID) * 0x20) + 0x29 + (0x20 * ((PID)/7)) + (SUB)) /* 不带锁存， 实时结果 */
	#define LOPV   (1 << 7)			/* 指针丢失 */
	#define AISV   (1 << 6)			/* 芯片进入AIS 状态 */
	#define RDIV   (1 << 5)			/* */
	#define RFIV   (1 << 4)			/* */
	#define RTIMV   (1 << 3)			/* 接收的J2 不匹配 */
	#define RTIUV   (1 << 2)			/* 接收的J2 不稳定 */
	#define UNEQV   (1 << 1)			/* */
	#define PLMV   (1 << 0)			/* 标记适配告警 SLM ? */
	
	
#define TU12_J2_STATUS_REG2(SUB,PID)  (((PID) * 0x20) + 0x2A + (0x20 * ((PID)/7)) + (SUB))   /* 带锁存，读后清除 */
	#define LOPI   (1 << 7)			/* 指针丢失 */ 
	#define AISI   (1 << 6)			/* 芯片进入AIS 状态 */
	#define RDII   (1 << 5)			/* */
	#define RFII   (1 << 4)			/* */
	#define RTIMI   (1 << 3)			/* 接收的J2 不匹配 */
	#define RTIUI   (1 << 2)			/* 接收的J2 不稳定 */
	#define UNEQI   (1 << 1)			/* */
	#define PLMI   (1 << 0)			/* 标记适配告警 SLM ? */

#define RX_K4_REG(SUB,PID)  (((PID) * 0x20) + 0x2C + (0x20 * ((PID)/7)) + (SUB))	


#define BIPERR_COUNTER_REG(SUB,PID)  (((PID) * 0x20) + 0x2E + (0x20 * ((PID)/7)) + (SUB)) /* 16 位寄存器， 需要特殊操作，见手册  */

#define REI_COUNTER_REG(SUB,PID)  (((PID) * 0x20) + 0x30 + (0x20 * ((PID)/7)) + (SUB))   /* 16 位寄存器， 需要特殊操作，见手册  */


#endif