/******************************************************************
** 文件名：commvar.h
** Copyright (c) 2002 烽火通信传输产品部BCT软件组
** 创建人：翁益晖
** 描　述：公共库：NewLib91.LIB for Mcs51和
**          NewLib1.LIB for Mc68302的头文件
**         适用于Keil C51 和 Mcc68K 编译环境
** 日　期：2002.6.28
** 修改人：yihui
** 日　期：2004.6.29
** 描　述：更改告警和性能的结构
**
**
** 编译器：Keil C51 V5.5 或 MCC68K V4.4
** 版  本：1.0
********************************************************************/
/*Add by wyh 2004.11.16*/

/*do not need to type the word "unsigned" anymore*/
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef char BOOL;

#define TRUE 	1
#define FALSE 	0

#ifdef _MICROTEC
	#include "reg302.h"
	#define BUFLEN       0x6000        /*透明帧缓冲区16帧*/
	#define FEEDDOG      PADAT ^=0x0040
	/*Debug相关定义*/
	#define TXDISABLE       PADAT|=0x0400
	#define TXENABLE        PADAT&=0xfbff
	
	/*灯的定义*/
	#define  YELLOW_ON      PADAT&=0xdfff
	#define  YELLOW_OFF     PADAT|=0x2000
	#define  RED_ON         PADAT&=0xbfff
	#define  RED_OFF        PADAT|=0x4000
	#define  GREEN_ON       PADAT&=0x7fff
	#define  GREEN_OFF      PADAT|=0x8000
	#define  NOTGREEN       PADAT^=0x8000; /*反转绿灯*/
	
	/*IO 相关定义*/
	/*IO0~IO3为PA.2~PA.5;IO4为PA.7;IO5~IO8为PB.8~PA.11
	  必须自己操作，不支持位操作*/
	  
	/*
	 * This section defines the external address accessing method for MC68302 chips, assume CS3 point to 0x300000
	 * CS4 point to 0x400000. if you change it, you must change this defines too.
	 * Just use CS3_ACCESS(address),CS4_ACCESS(address) in programs; no need to add the *2 to MC68302 programs
	 */
	#define MOTO_CS3	0x300000
	#define MOTO_CS4	0x400000


	#define CS3_ACCESS(x) *(ushort *)(MOTO_CS3 + sizeof(ushort) * (x))
	#define CS4_ACCESS(x) *(ushort *)(MOTO_CS4 + sizeof(ushort) * (x))

	  
#endif

#ifdef __C51__
	#include "w77e532.h"
	#include <absacc.h>
	#define BUFLEN       0x2000        /*透明帧缓冲区6帧*/
	#define FEEDDOG {TA=0xaa;TA=0x55;RWT=1;}
	/*Debug相关定义*/
	extern bit LSCE,HOLD;
	extern unsigned char bdata outport; 
	#define TXENABLE	{LSCE=1;XBYTE[0x9100]=outport;}
	#define TXDISABLE	{LSCE=0;XBYTE[0x9100]=outport;}
	#define HOLDON 		{HOLD=0;XBYTE[0x9100]=outport;}
	#define HOLDOFF 	{HOLD=1;XBYTE[0x9100]=outport;}
	#define WROUTPORT 	XBYTE[0x9100]=outport
	/*灯的定义*/
	#define ON				0
	#define OFF				1
	
	extern bit ACT;
	extern bit RED;
	extern bit YELLOW;

	/*IO 相关定义*/
	extern unsigned char bdata hio;
	#define WRHIO {XBYTE[0x9000]=hio;}
	#define RDHIO {hio=XBYTE[0x9200];}

	sbit IO0=P1^0;
	sbit IO1=P1^1;
	sbit IO2=P1^2;
	sbit IO3=P1^3;
	sbit IO4=P1^4;
	sbit IO5=P1^5;
	sbit IO6=P1^6;
	sbit IO7=P1^7;
	
	unsigned char bdata hio;
	sbit IO8=hio^0;
	sbit IO9=hio^1;
	sbit IO10=hio^2;
	sbit IO11=hio^3;
	sbit IO12=hio^4;
	sbit IO13=hio^5;
	sbit IO14=hio^6;
	sbit IO15=hio^7;
	
#endif

/*From Hdlc.h*/
#define ADDRESS         0 
#define CONTROL         1  
#define INFOSTART       2

#define MARK1           0    
#define MARK2           1   
#define MARK3           2 
#define PROTO_VER       3
#define CMD_VER         4  
#define CMD_TYPE        5 
#define CMD_CODE        6 
#define CMD_AUX         7 
#define BCT_TYPE        8
#define BCT_ADDR        9  
#define CHARACTER1      10
#define CHARACTER2      11   
#define DATALEN_H       12 
#define DATALEN_L       13
#define DATA_START      14

#define COMM_CATA       0x00    	/*Common catalog*/
#define CTRL_CATA		0x20			/*control catalog*/
#define AGGR_CATA       0x03    	/*Aggresive catalog*/

#define SET_BDINF       	0x01    	/*Belonging to Common catalog*/
#define SET_TIME        	0x02
#define SET_ACT         	0x03
#define SET_MODE        	0x04
#define SET_ALMMASK     	0x05
#define SET_PMMASK		0X07
#define SET_CONF        	0x21
#define SET_OCONF        	0x22
#define SET_LED         	0x31
#define SET_RESET       	0x32
#define SET_EEPROM	 	0x0c
#define ALL_SWITCH		0x51
#define ALL_TIME        	0x52
#define SET_INIHDLC     	0x53
#define ALL_CLEAR		0x54

#define RTRV_BDINF      	0x41    
#define RTRV_PM         	0x42
#define RTRV_ALM        	0x43
#define RTRV_CONF       	0x44
#define RTRV_MODE       	0x46
#define RTRV_ALL_STATE  	0x47
#define RTRV_EEPROM		0x49
#define RTRV_B_SOFT		0x4a
#define RTRV_OCONF		0x4b

#define SET_LASER       	0x31    /*Belonging to Aggresive catalog*/
#define SET_ONE_LASER  		0x35

#define SEND_CUT		0x00    /*Belonging to Control catalog*/
#define SET_DZ			0x01
#define SEND_AIS		0x02
#define SEND_FERF		0x03
#define SET_ELOOP		0x04
#define SET_OLOOP 		0x05
#define SET_TEST 		0x06
#define NO_THIS_CMD		0x20
#define CMD_ERROR		0x80
#define CMD_RIGHT		0x00

extern unsigned char g_ucHdlcBuf[];
/*From Timer.h*/
extern unsigned char g_SecondFlag,g_QuarterFlag;
struct SYSTIME
{
	unsigned char ucYear[2];
	unsigned char ucMonth;
	unsigned char ucDate;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
};
extern struct SYSTIME g_stuSysClock;        /*System time*/
extern void Delay(unsigned char uc10ms);


/*From Debug.c*/
extern void Debug(void);

/*Form At24C02.c*/
#ifdef _MICROTEC
extern unsigned char Write24C02(unsigned char *pucDataBuf, unsigned char ucStartAddr,unsigned char ucLen);
extern unsigned char Read24C02(unsigned char *pucDataBuf, unsigned char ucStartAddr,unsigned char ucLen);
#endif

#ifdef __C51__
extern unsigned char Write24C02(unsigned char xdata *pucDataBuf, unsigned char ucStartAddr,unsigned char ucLen);
extern unsigned char Read24C02(unsigned char xdata *pucDataBuf, unsigned char ucStartAddr,unsigned char ucLen);
#endif

/*From Comm.c*/
extern void ProcHisData(void);
extern void ProcAlmData(void);
#define MAXLEN        (BUFLEN-32)
extern unsigned char g_AllClrFlag;
extern unsigned short g_usRxDataLen;

extern void (*pfOtherCMD)(void);
extern unsigned long int g_ulResetSec;

/*Local Define*/
#ifndef BOARDCODE
	#error Please define BOARDCODE!!!
#endif

#ifndef PMTYPENUM
	#error Please define PMTYPENUM!!!
#endif

#ifndef ALMTYPENUM
	#error Please define ALMTYPENUM!!!
#endif

#ifndef LINENUM  	
	#error Please define LINENUM!!!
#endif

#ifndef ALMNUM    	
	#error Please define ALMNUM!!!	
#endif

#ifndef PMNUM		
	#error Please define PMNUM!!!	
#endif

#ifndef CONFLEN		
	#error Please define CONFLEN!!!
#endif

#ifndef STATELEN	
	#error Please define STATELEN!!!
#endif

#if(STATELEN==0)
	#error STATELEN MUST >1!!!
#endif

#if(LINENUM==0)
	#error LINENUM MUST >1!!!
#endif

#if(CONFLEN==0)
	#error CONFLEN MUST >1!!!
#endif

#if(CONFLEN > MAXLEN)
	#error CONFLEN TOO LARGE!!!
#endif

#if(STATELEN > MAXLEN)
	#error STATELEN TOO LARGE!!!
#endif

#if LINENUM!=0
    unsigned char g_ucLineCode[LINENUM],g_ucLineMask[LINENUM];
#else
    unsigned char g_ucLineCode[1],g_ucLineMask[1];
#endif

#if ALMTYPENUM!=0
    unsigned char g_ucAlmCode[ALMTYPENUM],g_ucAlmMask[ALMTYPENUM],g_ucAlmType[ALMTYPENUM];
#else
    unsigned char g_ucAlmCode[1],g_ucAlmMask[1],g_ucAlmType[1];
#endif

#if PMTYPENUM!=0
    unsigned char g_ucPmCode[PMTYPENUM],g_ucPmMask[PMTYPENUM];
#else
    unsigned char g_ucPmCode[1],g_ucPmMask[1];
#endif

union INT_CHAR
{
	unsigned char uchar[2];
	unsigned short int usint;
};

struct PERFORMANCE
{
	unsigned char ucLineIndex;
  unsigned char ucPmIndex;
	union INT_CHAR Value;
	union INT_CHAR HisValue;
};

#if PMNUM!=0
    struct PERFORMANCE g_stuPm[PMNUM];
	unsigned char g_ucLinePmMask[PMNUM];
#else
    struct PERFORMANCE g_stuPm[1];
	unsigned char g_ucLinePmMask[1];
#endif

struct ALARM
{
	unsigned char ucLineIndex;
	unsigned char ucAlmIndex;
    unsigned char ucState;
	unsigned char ucLastState;
    /*start time: [0]; 
	    end time: [1];
	    record continous alarming stopping time from last 15 minutes:[2]*/
	unsigned char ucMinute[3];	
	unsigned char ucSecond[3];
	unsigned char ucTimes;        	/*alarming times*/

	unsigned char ucHisMinute[3];	
	unsigned char ucHisSecond[3];
	unsigned char ucHisTimes;        	/*alarming times*/
};
#if ALMNUM!=0
    struct ALARM g_stuAlm[ALMNUM];
#else
	struct ALARM g_stuAlm[1];
#endif

unsigned char g_ucConfData[CONFLEN],g_ucState[STATELEN];

#ifdef _MICROTEC
const unsigned short g_usConfLen=CONFLEN;
const unsigned short g_usStateLen=STATELEN;
const unsigned char g_ucPmTypeNum=PMTYPENUM;
const unsigned char g_ucAlmTypeNum=ALMTYPENUM;
const unsigned char g_ucBoardCode=BOARDCODE;
const unsigned short g_usAlmNum=ALMNUM;
const unsigned short g_usPmNum=PMNUM;
const unsigned char g_ucLineNum=LINENUM;
#endif

#ifdef __C51__
unsigned short code g_usConfLen=CONFLEN;
unsigned short code g_usStateLen=STATELEN;
unsigned char code g_ucPmTypeNum=PMTYPENUM;
unsigned char code g_ucAlmTypeNum=ALMTYPENUM;
unsigned char code g_ucBoardCode=BOARDCODE;
unsigned short code g_usAlmNum=ALMNUM;
unsigned short code g_usPmNum=PMNUM;
unsigned char code g_ucLineNum=LINENUM;
#endif

/*end -- wyh*/