/******************************************************************
** 文件名：STAND.C
** Copyright (c) 2002 烽火通信传输产品部BCT软件组
** 创建人：翁益晖
** 文件编号：
** 描　述：标准模版程序
**         
** 日　期：2002.6.28
** 修改人：
** 日　期：
** 文件编号：
** 描　述：
**
**
** 编译器：Keil C51 V5.5 或 MCC68K V4.4
** 公共库：NewLib9.LIB for Mcs51
**         或 NewLib1.LIB for Mc68302
** 版  本：1.0
********************************************************************/
//#define  NEED_LP_RFI   /**  !!! 单盘起不来了  */
#define AIS_CONDITIONS      /** 与 AIS_CONDITIONS_AUTO 二选一  */
//#define AIS_CONDITIONS_AUTO
#define SKIP_DEBUG_CHECK

#define UASNUM  	63      	//要计算误码的线路数63//
#define LINENUM  	64      	/*告警线路数为64*/
#ifdef NEED_LP_RFI
	#define ALMNUM    	822     /*总共63*(ALMTYPENUM-3)+3=759个告警*/
	#define ALMTYPENUM      16      /*16种告警类型*/
#else
	#define ALMNUM    	759     /*总共63*(ALMTYPENUM-3)+3=759个告警*/
	#define ALMTYPENUM      15      /*15种告警类型*/
#endif

#define PMNUM		441     		/*总共63*PMTYPENUM个性能*/

#define PMTYPENUM       7       /*7种性能类型*/
#define CONFLEN		2560	/*配置数据长度为2560*/
#define STATELEN	338		/*状态数据长度为338*/
#define BOARDCODE	0x96	/*盘代码*/
#define WGS21891A 0xB000

#define SW1021	0xB000    //TODO：confirm

#define SW1021_CHIP_ADDR(num)  (SW1021 + 0x1000*(num))
#define CHIPS_ON_BOARD          3


#include "commvar9.h"
#include "uasE63.h"

#include "sw1021.h"

char code SysName[]="GF2488-01F";
char code BoardName[]="E1-63A";
char code BoardSn[]="WKE2.170.609R1B";
char code SoftVer[]="R170609R1B/01F    ";

char code PcbSn[]="WKE7.822.828R1B";
char code PcbTime[]="2004 05 02";

char code BoardSoftVer[]="RP0100            ";

char code SoftDate[]=__DATE__;
char code SoftTime[]=__TIME__;

extern int sprintf(char *, const char *, ...);

#define RSTB IO0
#define TUALMO IO2
#define TUXAB IO5 //TUXAB为主备交叉时钟盘XCU有效指示信号,主用XCU有效时TUXAB=0,备用XCU有效TUXAB=1
#define WP IO6     //E1-63A盘在正常槽位时WP=1,在保护槽位时WP=0//
#define TUSWTI IO7 //本盘almnumber大于一定值时,TUALMO=1,XCU盘收到TUALMO=1时,产生TUSWTI=0//
#define P2V5 IO8
#define P5VI1 IO9
#define P5VI2 IO10
#define DZBI1 IO11
#define DZBI2 IO12
extern unsigned char g_AllClrFlag;
extern unsigned char g_LampEn;
unsigned int ulEsLimit,ulSesLimit;
//unsigned int WGS21891,wgs21891reg0[4],wgs21891reg1[4];
unsigned int sw1021Chip;
unsigned int N1000[63],BIP[63],BIP_2_1S[63],BIP_2_1000S[63];
unsigned char actnumber,almnumber,first_conf;
unsigned char g_haveTux;
/*
code unsigned char slot[64]={1,22,43,4,25,46,7,
28,49,10,31,52,13,34,
55,16,37,58,19,40,61,

2,23,44,5,26,47,8,
29,50,11,32,53,14,35,
56,17,38,59,20,41,62,

3,24,45,6,27,48,9,
30,51,12,33,54,15,36,
57,18,39,60,21,42,63,0};
		*/
/*
code unsigned char slot[64]={0x00,0x20,0x40,0x01,0x21,0x41,0x02,
                     0x22,0x42,0x03,0x23,0x43,0x04,0x24,
                     0x44,0x05,0x25,0x45,0x06,0x26,0x46,

                     0x07,0x27,0x47,0x08,0x28,0x48,0x09,
                     0x29,0x49,0x0a,0x2a,0x4a,0x0b,0x2b,
                     0x4b,0x0c,0x2c,0x4c,0x0d,0x2d,0x4d,

                     0x0e,0x2e,0x4e,0x0f,0x2f,0x4f,0x10,
                     0x30,0x50,0x11,0x31,0x51,0x12,0x32,
                     0x52,0x13,0x33,0x53,0x14,0x34,0x54,
										 0
                };		
*/
code unsigned char slot[64]={0x00,0x07,0x0e,0x01,0x08,0x0f,0x02,
                     0x09,0x10,0x03,0x0a,0x11,0x04,0x0b,
                     0x12,0x05,0x0c,0x13,0x06,0x0d,0x14,

                     0x20,0x27,0x2e,0x21,0x28,0x2f,0x22,
                     0x29,0x30,0x23,0x2a,0x31,0x24,0x2b,
                     0x32,0x25,0x2c,0x33,0x26,0x2d,0x34,

                     0x40,0x47,0x4e,0x41,0x48,0x4f,0x42,
                     0x49,0x50,0x43,0x4a,0x51,0x44,0x4b,
                     0x52,0x45,0x4c,0x53,0x46,0x4d,0x54,
										 0
                };

		
static void writeTxJ2(char chip, char witchJ2, char Slot, char j2, char busB)
{
	int copOk = 0;
	int busBase = A_BUS_BASE;
	int i = 5;
	if(busB)
		busBase = B_BUS_BASE;
	// write  Tx J2 data
	XBYTE[SW1021_CHIP_ADDR(chip) + J2_TX_DATA_REG(busBase)] = j2;
	XBYTE[SW1021_CHIP_ADDR(chip) + J2_TX_WHICH_REG(busBase)] = witchJ2;
	XBYTE[SW1021_CHIP_ADDR(chip) + J2_TX_SLOT_REG(busBase)] = RW_SEL | Slot;
	while(!copOk){
		copOk = XBYTE[SW1021_CHIP_ADDR(chip) + J2_TX_RAM_STATUS_REG(busBase)] & 0x01;
		i--;
		if(i == 0)
			break;
		//Delay(1);
	}
}

static void writeExpectJ2(char chip, char witchJ2, char Slot, char j2, char busB)
{
	int copOk = 0;
	int busBase = A_BUS_BASE;
	int i = 5;
	if(busB) 
		busBase = B_BUS_BASE;
	// write  Tx J2 data
	XBYTE[SW1021_CHIP_ADDR(chip) + J2_RX_DATA_REG(busBase)] = j2;
	XBYTE[SW1021_CHIP_ADDR(chip) + J2_RX_J2TYPE_CTRL_REG(busBase)] = EXPECT_J2 |  witchJ2;
	XBYTE[SW1021_CHIP_ADDR(chip) + J2_RX_CTRL_REG(busBase)] = RW_SEL | Slot;
	while(!copOk){
		copOk = XBYTE[SW1021_CHIP_ADDR(chip) + J2_RAM_STATUS_REG(busBase)] & READY;
		i--;
		if(i == 0)
			break;
		//Delay(1);
	}
}

/*****************************************************************
** 函数名:GETDATA
** 输　入: 无
** 输　出: 无
** 功能描述：采集单盘告警和性能
** 全局变量：无
** 调用模块：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void GetData()          
{
	unsigned char i,m,n,mi,ni,tmp,tmpi,ADCLK,PPILOS,TULOP,LPSLM,LPTIM,LPTIU,LPRDI,LPRFI,LPUNEQ,TUAIS,PPIAIS,
	LOOP,LOOPL,tv5;
	unsigned int  LPFEBE,HDB3;
	unsigned char lopCnt = 0;
	actnumber=0;
	almnumber=0;
 	sw1021Chip = SW1021;
	for(i=0;i<(LINENUM-1);i++)
	{          
		
		if(g_ucState[78+i]==0)
		{
			g_ucLineMask[i]=1;
			// ADCLK 告警 (A 路下行时钟告警 )
			ADCLK=XBYTE[sw1021Chip+E1_IF_STATUS_REG] & ADLOC; 

		}
		else   
		{
			m=i/21;
			n=i%21;
			mi=i/8;
			ni=i%8;
			
			sw1021Chip = SW1021_CHIP_ADDR(m);
			
			if(TULOP||TUAIS)
				g_ucLofFlag[i]=1;      //有TULOP,TUAIS,PPILOS告警之一时上报UAS
			else
				g_ucLofFlag[i]=0;
			
			actnumber=actnumber+g_ucConfData[64+i];
			
			ADCLK = XBYTE[sw1021Chip+E1_IF_STATUS_REG] & ADLOC;
			tmp = XBYTE[sw1021Chip+PORT_E1_ALARM_REG(n)];
			PPIAIS = tmp & E1AIS;
			PPILOS = tmp & E1LOS;
		/*
			tmp = XBYTE[sw1021Chip+TU12_J2_STATUS_REG2(A_BUS_BASE, n)];
			TULOP = tmp & LOPI;
			TUAIS = tmp & AISI;
			LPSLM = tmp & PLMI;
			LPTIM = tmp & RTIMI;
			LPTIU = tmp & RTIUI;
			LPRDI = tmp & RDII;
			LPRFI = tmp & RFII;
			LPUNEQ = tmp & UNEQI;
		*/
			
			tmpi = XBYTE[sw1021Chip+TU12_J2_STATUS_REG2(A_BUS_BASE, n)];
			tmp = XBYTE[sw1021Chip+TU12_J2_STATUS_REG(A_BUS_BASE, n)] | tmpi;
			TULOP = tmp & LOPV;
			TUAIS = tmp & AISV;
			LPSLM = tmp & PLMV;
			LPTIM = tmp & RTIMV;
			LPTIU = tmp & RTIUV;
			LPRDI = tmp & RDIV;
			LPRFI = tmp & RFIV;
			LPUNEQ = tmp & UNEQV;
			
			if(TULOP) lopCnt++;
			
		#ifdef AIS_CONDITIONS	
		  /** 方法1： 手动 */
			//AIS 产生条件 ---add by caijun.Li
			if(((g_ucConfData[2531] & 0x80) && LPTIM) || ((g_ucConfData[2531] & 0x40) && LPUNEQ ) || ((g_ucConfData[2531] & 0x20) && LPSLM ) /* || ((g_ucConfData[2531] & 0x10) && LPSD )*/ ){
				// 下插AIS
				XBYTE[sw1021Chip + PORT_CFG_REG(n)] |= RAISEN;
			}else{
				// 停止下插AIS
				XBYTE[sw1021Chip + PORT_CFG_REG(n)] &= ~RAISEN;
			}
				
		#endif
			
			
			//must be write BIPERR_COUNTER_REG ; see manual
			XBYTE[sw1021Chip+BIPERR_COUNTER_REG(A_BUS_BASE, n)] = 0x00;
			g_liError1s[i].ulDatlong += XBYTE[sw1021Chip+BIPERR_COUNTER_REG(A_BUS_BASE, n)] + (XBYTE[sw1021Chip+BIPERR_COUNTER_REG(A_BUS_BASE, n) + 1] * 256);   // BIP错误计数值
			//must be write REI_COUNTER_REG ; see manual
			XBYTE[sw1021Chip+REI_COUNTER_REG(A_BUS_BASE, n)] = 0x00;
			LPFEBE = XBYTE[sw1021Chip+REI_COUNTER_REG(A_BUS_BASE, n)] + (XBYTE[sw1021Chip+REI_COUNTER_REG(A_BUS_BASE, n) + 1] * 256); //REI(FEBE)计数值
			
			tmp = XBYTE[sw1021Chip + PORT_TEST_REG(n)];
						
			LOOPL =  tmp & E1LBK;
			LOOP = 	 tmp & LnLBK;
			
			if((g_ucConfData[200+mi]&(0x01<<ni))==0)
			{
				g_stuAlm[i*(ALMTYPENUM-3)+6].ucState=0;
				if(PPILOS) g_stuAlm[i*(ALMTYPENUM-3)].ucState=1;	 			   
				else                                              //E1输入口有信号时/// 
				{
					if(g_stuAlm[i*(ALMTYPENUM-3)].ucState==1)  //如果还存在PPILOS告警//
					if(WP==1)                        //非保护盘位时//
					{
						// 向第i通道上话方向插短时间的伪随机码
						XBYTE[sw1021Chip + PORT_TEST_REG(n)] |= PRBSEN | PRBST;
						g_stuAlm[i*(ALMTYPENUM-3)].ucState=0;//不告PPILOS//
		
						Delay(10);
						XBYTE[sw1021Chip + PORT_TEST_REG(n)] &=  ~(PRBSEN | PRBST); //停插伪随机码

					}
					else g_stuAlm[i*(ALMTYPENUM-3)].ucState=0;//不告PPILOS////保护盘位时//
				}

			}
			else
			{
				g_stuAlm[i*(ALMTYPENUM-3)].ucState=0;
				if(PPILOS) g_stuAlm[i*(ALMTYPENUM-3)+6].ucState=0;	 			       
				else  g_stuAlm[i*(ALMTYPENUM-3)+6].ucState=1;
			}	 			       
			//if(PPIAIS) g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;	 			       
			//else g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
			if(TULOP) g_stuAlm[i*(ALMTYPENUM-3)+1].ucState=1;	 			       
			else g_stuAlm[i*(ALMTYPENUM-3)+1].ucState=0;	
			if(TUAIS) g_stuAlm[i*(ALMTYPENUM-3)+5].ucState=1;	 			       
			else g_stuAlm[i*(ALMTYPENUM-3)+5].ucState=0;
			if(TULOP||TUAIS)
			{
				g_stuAlm[i*(ALMTYPENUM-3)+2].ucState=0;
				g_stuAlm[i*(ALMTYPENUM-3)+3].ucState=0;
				g_stuAlm[i*(ALMTYPENUM-3)+4].ucState=0;
				#ifdef NEED_LP_RFI
				g_stuAlm[i*(ALMTYPENUM-3)+15].ucState=0; 
				#endif
				g_stuAlm[i*(ALMTYPENUM-3)+9].ucState=0;	
			}
			else
			{
				if(LPSLM) g_stuAlm[i*(ALMTYPENUM-3)+2].ucState=1;	 			       
				else g_stuAlm[i*(ALMTYPENUM-3)+2].ucState=0;
				if(LPRDI) g_stuAlm[i*(ALMTYPENUM-3)+3].ucState=1;	 			       
				else g_stuAlm[i*(ALMTYPENUM-3)+3].ucState=0;
				if(LPUNEQ) g_stuAlm[i*(ALMTYPENUM-3)+4].ucState=1;	 			       
				else g_stuAlm[i*(ALMTYPENUM-3)+4].ucState=0;

				/* 烽火老设备未使用RFI,不使用时将下面两句屏蔽 */
#ifdef NEED_LP_RFI
				 if(LPRFI) g_stuAlm[i*(ALMTYPENUM-3)+15].ucState=1;	 			       
				 else g_stuAlm[i*(ALMTYPENUM-3)+15].ucState=0; 
#endif
				
				if(LPTIM) g_stuAlm[i*(ALMTYPENUM-3)+9].ucState=1;	 			       
				else g_stuAlm[i*(ALMTYPENUM-3)+9].ucState=0;	
			}
			if(LOOPL) g_stuAlm[i*(ALMTYPENUM-3)+10].ucState=1;	 			       
			else g_stuAlm[i*(ALMTYPENUM-3)+10].ucState=0;
			if(LOOP) g_stuAlm[i*(ALMTYPENUM-3)+11].ucState=1;	 			       
			else g_stuAlm[i*(ALMTYPENUM-3)+11].ucState=0;
			if(g_stuPm[i*PMTYPENUM+3].Value.usint>=(0xffff-LPFEBE)) g_stuPm[i*PMTYPENUM+3].Value.usint=0xffff;       
			else g_stuPm[i*PMTYPENUM+3].Value.usint+=LPFEBE;	 //REI(FEBE)性能值                    
			if(g_stuAlm[i*(ALMTYPENUM-3)].ucState==0)                               //无PPI_LOS告警时
			{
				 //HDB3编码违例计数值
				HDB3 = XBYTE[sw1021Chip + HDB3_CV_REG(n)];
				
				if(g_stuPm[i*PMTYPENUM+4].Value.usint>=(0xffff-HDB3)) g_stuPm[i*PMTYPENUM+4].Value.usint=0xffff;     
				else g_stuPm[i*PMTYPENUM+4].Value.usint+=HDB3;      // HDB3性能值         //g_stuPm[i][4]=HDB3CV
			}			      

			if(g_stuAlm[i*(ALMTYPENUM-3)+1].ucState||g_stuAlm[i*(ALMTYPENUM-3)+5].ucState)
			almnumber=almnumber+1;
			
			tv5 = XBYTE[sw1021Chip + TX_V5_REG(A_BUS_BASE,n)];
			if(g_stuAlm[i*(ALMTYPENUM-3)+1].ucState||g_stuAlm[i*(ALMTYPENUM-3)+5].ucState||g_stuAlm[i*(ALMTYPENUM-3)+4].ucState)
			{ //如果TUAIS,TULOP,LPUNEQ之一为"1" 则向上插VC12=AIS,LPRDI,向下插AIS
					if((tv5 & RDI_CNF) != RDI_CNF)
						XBYTE[sw1021Chip + TX_V5_REG(A_BUS_BASE,n)] = tv5 | RDI_CNF;
			}
			else
			{
				if((tv5 & RDI_CNF) == RDI_CNF)
					XBYTE[sw1021Chip + TX_V5_REG(A_BUS_BASE,n)] = tv5 & (~RDI_CNF);
			}
		}
	}

	for(i=0;i<(LINENUM-1);i++)
	{
		m=i/21;
		n=i%21;
		sw1021Chip = SW1021_CHIP_ADDR(m);
		// 获取N2 和 K4 值
		//TODO: get N2
		//g_ucState[204+2*i]=XBYTE[WGS21891+0x0100+0x10*n+0x0C]; 
		g_ucState[205+2*i]=XBYTE[sw1021Chip + RX_K4_REG(A_BUS_BASE,n)];
		
		
		mi=i/8;
		ni=i%8;
		if((g_ucConfData[200+mi]&(0x01<<ni))==0)
			g_ucState[330+mi]&=(~(0x01<<ni));

		else
			g_ucState[330+mi]|=(0x01<<ni);    

		//	tv5=XBYTE[WGS21891+0x0100+0x10*n+0x00];

	}

	 

	if(TUXAB) g_ucState[77]=0;
	else g_ucState[77]=1;
                     
       
	if((WP==0) && (TUSWTI==0))	 //保护盘位时,如果没有倒换ACT灯慢闪//			
	{
		g_ucState[76]=0;
		g_LampEn=0;        
		if(g_stuSysClock.ucSecond%2==0) ACT=ON;
		else ACT=OFF;
		for(i=0;i<(LINENUM-1);i++)
		{
			g_ucLineMask[i]=1;
			g_ucState[141+i]=1;
		}
	}
	else if(g_ucState[76]==0)
	{                           
		for(i=0;i<(LINENUM-1);i++)
		{
			if((g_ucConfData[64+i]==0)||(g_ucConfData[137+i]==01))
			{    
				g_ucLineMask[i]=1;
				g_ucState[141+i]=1;  
			}  	
			else
			{
				g_ucLineMask[i]=0;  
				g_ucState[141+i]=0;
			}
		}
		g_ucState[76]=1;     
		g_LampEn=1;
		WROUTPORT;           
	}
       
	UasClear();


	if(g_AllClrFlag)                //清性能
	{   
		g_bClearUas=1; 
		g_AllClrFlag=0;
	}

	if(g_SecondFlag)                   //秒标识，1：进入新的一秒
	{
		CalUas(ulSesLmt);           //uas计算        

		for(i=0; i<(LINENUM-1); i++)
		{
			g_stuPm[i*PMTYPENUM].Value.usint=g_usiBbe[i];         //bbe_lp性能的采集
			g_stuPm[i*PMTYPENUM+1].Value.usint=g_usiEs[i];        //es_lp性能的采集
			g_stuPm[i*PMTYPENUM+2].Value.usint=g_usiSes[i];       //ses_lp性能的采集
			if(g_usiEs[i]>=ulEsLimit) 
				g_stuAlm[i*(ALMTYPENUM-3)+7].ucState=1;    //curalm[i][8]=es_limit
			else 
				g_stuAlm[i*(ALMTYPENUM-3)+7].ucState=0;
			// if(g_usiSes[i]>=ulSesLimit) g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;    //curalm[i][9]=ses_limit
			// else g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
			g_stuPm[i*PMTYPENUM+5].Value.usint=g_usiUas[i];             //UAS_LP性能的采集
			g_stuPm[i*PMTYPENUM+6].Value.usint=g_usiCses[i];            //cses_lp性能的采集


			/******************************************************************************/
			if(g_stuAlm[i*(ALMTYPENUM-3)+5].ucState||g_stuAlm[i*(ALMTYPENUM-3)+1].ucState)
			{
				g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
				BIP[i]=0;
			}
			else
			{
				BIP_2_1S[i]=BIP[i];
				BIP_2_1000S[i]+=BIP[i];  
				BIP[i]=0;
				switch(g_ucConfData[129])
				{
					case 0x04:
					if(BIP_2_1S[i]>=217) 
						g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;
					else if(BIP_2_1S[i]<=23) 
						g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
					break;

					case 0x05:
					if(BIP_2_1S[i]>=22) 
						g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;
					else if(BIP_2_1S[i]<=3) 
						g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
					break;

					case 0x06:


					if((N1000[i]>=10)&&(N1000[i]<100))
					{
						if(BIP_2_1000S[i]>=22) 
							g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;
						else if(BIP_2_1000S[i]<=3) 
							g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
						N1000[i]=0;
						BIP_2_1000S[i]=0;
					}
					else N1000[i]++;

					break;

					case 0x07:


					if((N1000[i]>=100)&&(N1000[i]<1000))
					{
						if(BIP_2_1000S[i]>=22) 
							g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;
						else if(BIP_2_1000S[i]<=3) 
							g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
						N1000[i]=0;
						BIP_2_1000S[i]=0;
					}
					else N1000[i]++;
					break;

					case 0x08:


					if(N1000[i]>=1000)
					{
						if(BIP_2_1000S[i]>=22) 
							g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=1;
						else if(BIP_2_1000S[i]<=3) 
							g_stuAlm[i*(ALMTYPENUM-3)+8].ucState=0;
						N1000[i]=0;
						BIP_2_1000S[i]=0;
					}
					else 
						N1000[i]++;
					break;
				}
			}

		/********************************************************************************/

		}

		g_SecondFlag=0;      //秒标识清?

	}     

	//IO8=1;
	// WRHIO;
	//RDHIO;		
	//IO9=1;
	// WRHIO;
	//RDHIO;
	//IO10=1;
	// WRHIO;
	//RDHIO;
	//IO11=1;
	// WRHIO;
	//RDHIO;
	//IO12=1;
	// WRHIO;
	RDHIO;

   
	     
	if(ADCLK||(P2V5==0)||(first_conf==0)) 
		TUALMO=1; //如果工作支路中全部有告警或2.5V//
	else 
		TUALMO=0; //电源有告警或下配置前TUALMO=1,否则TUALMO=0//
	if((P5VI1==0)&&(DZBI1==0)) 
		g_stuAlm[63*(ALMTYPENUM-3)+1].ucState=1;
	else    
		g_stuAlm[63*(ALMTYPENUM-3)+1].ucState=0;               
	if((P5VI2==0)&&(DZBI2==0)) 
		g_stuAlm[63*(ALMTYPENUM-3)+2].ucState=1;
	else    
		g_stuAlm[63*(ALMTYPENUM-3)+2].ucState=0;       
	if(P2V5==0) 
		g_stuAlm[63*(ALMTYPENUM-3)+0].ucState=1;
	else    
		g_stuAlm[63*(ALMTYPENUM-3)+0].ucState=0;	         	         
 
}

/*****************************************************************
** 函数名:USERFUN
** 输　入: 无
** 输　出: 无
** 功能描述：用户实时处理函数，10毫秒被调用一次
** 全局变量：无
** 调用模块：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void UserFunc() using 1
{
	int i,j;
	int tmp;
	unsigned int chip  = SW1021;
	static int docnt = 0;
	if(g_haveTux){
		if(docnt > 10){
			docnt = 0;
			g_haveTux = 0;
		}
		for(i=0; i<CHIPS_ON_BOARD; i++){
			for(j=0; j< 21; j++){
				tmp = XBYTE[chip+PORT_E1_ALARM_REG(j)];
				if((tmp & RFFERR) == 0){
					continue;
				}else{
					//XBYTE[SW1021_CHIP_ADDR(k) + SOFTWARE_RST_REG] = SRST_E1 | SRST_SDH_A | SRST_SDH_B;
					XBYTE[SW1021 + SOFTWARE_RST_REG] = SRST_E1;
					XBYTE[SW1021 + SOFTWARE_RST_REG] = RST_CANCLE;
					
					XBYTE[SW1021 + 0x1000 + SOFTWARE_RST_REG] = SRST_E1;
					XBYTE[SW1021 + 0x1000 + SOFTWARE_RST_REG] = RST_CANCLE;
					
					XBYTE[SW1021 + 0x2000 + SOFTWARE_RST_REG] = SRST_E1;
					XBYTE[SW1021 + 0x2000 + SOFTWARE_RST_REG] = RST_CANCLE;
				
					g_haveTux = 0;
					docnt = 0;
					return;
				}
			}
			chip+=0x1000;
		}
		docnt++;
	}
	
	return;
}


//static void ChecktuxAB (void) interrupt 9 using 3
static void ChecktuxAB (void) interrupt 9
{

	EX3 = 0;
	EXIF &= 0xDF;
	g_haveTux = 1;


	EX3 = 1;
	return;
}


/*****************************************************************
** 函数名:CONF_SET
** 输　入: 无
** 输　出: 无
** 功能描述：单盘配置
** 全局变量：无
** 调用模块：无
** 作　者：caijun.Li
** 日　期：2016.3.19
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void ConfSet(void)
{
    /*Process ALMMASK*/
    /*Process PMMASK*/
	unsigned char i,j,ilong,m,n,temp;

	g_ucState[69]=g_ucConfData[5];
	g_ucState[70]=g_ucConfData[6];
	g_ucState[71]=g_ucConfData[7];
	g_ucState[72]=g_ucConfData[8];
	g_ucState[73]=g_ucConfData[9];
	g_ucState[74]=g_ucConfData[10];
	g_ucState[75]=g_ucConfData[11];
	for(i=0;i<(LINENUM-1);i++){
		m=i/21;
		n=i%21;
		sw1021Chip = SW1021_CHIP_ADDR(m);
	
		XBYTE[sw1021Chip + RXTU12_SLOT_REG(A_BUS_BASE,n)] = slot[i];	
		XBYTE[sw1021Chip + TXTU12_SLOT_REG(A_BUS_BASE,n)] = slot[i];	
		XBYTE[sw1021Chip + RXTU12_SLOT_REG(B_BUS_BASE,n)] = slot[i];	
		XBYTE[sw1021Chip + TXTU12_SLOT_REG(B_BUS_BASE,n)] = slot[i];	

		if(g_ucConfData[64+i]==0){  // 支路关闭
			// 上下话强发AIS
			//XBYTE[sw1021Chip + TX_CTRL_REG(B_BUS_BASE,n)] = AIS_EN;	
			temp = XBYTE[sw1021Chip + PORT_CFG_REG(n)];
			XBYTE[sw1021Chip + PORT_CFG_REG(n)] = temp | TAISEN | RAISEN;	
			XBYTE[sw1021Chip + TX_V5_REG(A_BUS_BASE,n)] = 0x00;
						
			g_ucState[78+i]=0;     
			g_ucLineMask[i]=1;
			g_ucState[141+i]=1;
		}else{  // 支路打开
			XBYTE[sw1021Chip + PORT_CFG_REG(n)] = A_UP_DOWN | RnEN;
			XBYTE[sw1021Chip + TX_V5_REG(A_BUS_BASE,n)] = (0x02 << 1);  // 注意信号标记值得配置
			if(g_ucState[78+i]==0)//刚打开支路开关时插伪随机码
			{
				//TODO: 启动误码计数器
				XBYTE[sw1021Chip + PORT_TEST_REG(n)] |= PRBSEN | PRBST;
				Delay(5);
				XBYTE[sw1021Chip + PORT_TEST_REG(n)] &= ~(PRBSEN | PRBST);
				g_ucState[78+i]=1;
				g_ucLineMask[i]=0; 
				g_ucState[141+i]=0;
			}  	     	
		}
		XBYTE[sw1021Chip + TX_K4_REG(A_BUS_BASE,n)]= g_ucConfData[263+4*i+2];               // K4 发 
		XBYTE[sw1021Chip + TX_K4_REG(B_BUS_BASE,n)]= g_ucConfData[263+4*i+2];               // K4 发 
		// TODO: N2 发
		//XBYTE[WGS21891+0x0100+0x10*n+0x01]=g_ucConfData[263+4*i];                 // N2 发
	
	#ifdef AIS_CONDITIONS_AUTO
		if((g_ucConfData[2531] & 0x80)){
			XBYTE[sw1021Chip + RCV_INSET_AIS_REG(A_BUS_BASE,n)] |= J2RTIME;
		}else{
			XBYTE[sw1021Chip + RCV_INSET_AIS_REG(A_BUS_BASE,n)] |= J2RTIME;
		}
		if(g_ucConfData[2531] & 0x40){
			XBYTE[sw1021Chip + RCV_INSET_AIS_REG(A_BUS_BASE,n)] |= UNEQAISE;
		}
		if(g_ucConfData[2531] & 0x20){
			XBYTE[sw1021Chip + RCV_INSET_AIS_REG(A_BUS_BASE,n)] |= PLMAISE;
		}
	#endif
	
		// J2 发 和 期望收
		for(j=0;j<16;j++)
		{
			if(j==0) temp=0x80;
			else temp=0;
			
			// A 
			writeTxJ2(m, j, slot[i],  temp|g_ucConfData[515+2*i*16+j], 0);
			writeExpectJ2(m, j, slot[i],  temp|g_ucConfData[515+16+2*i*16+j], 0);
			// B
			writeTxJ2(m, j, slot[i],  temp|g_ucConfData[515+2*i*16+j], 1);
			writeExpectJ2(m, j, slot[i],  temp|g_ucConfData[515+16+2*i*16+j], 1);
		}
		
		if(g_ucConfData[137+i]==0)   //confdata[137+i]==0不屏蔽支路i告警/否则屏蔽支路i告警//
		{
			g_ucLineMask[i]=0;
			g_ucState[141+i]=0;
		}
		else
		{
			g_ucLineMask[i]=1;
			g_ucState[141+i]=1;
		}
	//if(g_ucConfData[200+i]==0)     //confdata[200+i]==1支路i装载/否则不装载//
	//XBYTE[WGS21891+0x0100+0x10*n+0x00]=0x00;
	// else  XBYTE[WGS21891+0x0100+0x10*n+0x00]=0x04;

	}



	for(j=0;j<ALMTYPENUM;j++)	
	{
		 
		 for(ilong=29;ilong<(29+ALMTYPENUM);ilong++)
		 {  	
				if(g_ucConfData[ilong]==g_ucAlmCode[j])   //从网管设告警屏蔽//
				{
				 g_ucAlmMask[j]=1;   	   	                      
				 break; 		
				}
				else
				g_ucAlmMask[j]=0;

		 }
	}      
										
	for(j=0;j<PMTYPENUM;j++)	
	{

		for(ilong=54;ilong<(54+PMTYPENUM);ilong++)
		{  	
			if(g_ucConfData[ilong]==g_ucPmCode[j])   //从网管设性能屏蔽//
			{
				g_ucPmMask[j]=1;   	   	                      
				break; 		
			}
			else
				g_ucPmMask[j]=0;
		}
	}        

	ulEsLimit=256*g_ucConfData[131]+g_ucConfData[132];    //从网管设ulEsLimit门限//
	ulSesLimit=256*g_ucConfData[134]+g_ucConfData[135];   //从网管设ulSesLimit门限//


	//if((g_ucConfData[136]==0)&&TUALMO&&WP) TUALMO=0;  //IO2=TUALMO  confdata[108]=盘保护模式不保护


	// g_ucConfChange=0;	           	      			
				

	if(first_conf==0) first_conf=1;	
}


/*****************************************************************
** 函数名:UserHdlc
** 输　入: 无
** 输　出: 无
** 功能描述：单盘特殊命令处理
** 全局变量：无
** 调用模块：无
** 作　者：caijun.Li
** 日　期：2016.3.19
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void UserHdlc(void)
{
	unsigned char i,m,n,temp;
	unsigned int addr;

	if (g_ucHdlcBuf[CMD_TYPE]==COMM_CATA)
	{
		if (g_ucHdlcBuf[CMD_CODE]==RTRV_OCONF)
		{
			g_ucHdlcBuf[CHARACTER1]=CMD_RIGHT;
		}
		else if (g_ucHdlcBuf[CMD_CODE]==SET_OCONF)
		{
			g_ucHdlcBuf[CHARACTER1]=CMD_RIGHT;                                               
		}
	}


	switch(g_ucHdlcBuf[5])
	{
		case 0x00:		/*SET_LOOP*/
			if(g_ucHdlcBuf[6]==0x34)
			{ // 设备环回
				if(((g_usRxDataLen)==0)||((g_usRxDataLen)>63)) 
					g_ucHdlcBuf[10]=0x80; 
				else			          
					for(i=0;i<(g_usRxDataLen);i++)
					{
						temp=g_ucHdlcBuf[14+i]-1;
						m=temp/21;
						n=temp%21;

						sw1021Chip = SW1021_CHIP_ADDR(m);
						if(g_ucHdlcBuf[7]==0x01){
							XBYTE[sw1021Chip + PORT_TEST_REG(n)] |= LnLBK;
						}else{
							XBYTE[sw1021Chip + PORT_TEST_REG(n)] &= (~LnLBK);
						}
						g_ucHdlcBuf[10]=0x00;
					}
			}
			else if(g_ucHdlcBuf[6]==0x33)       /*SET_LOOPL*/
			{ // 线路环回 
				if(((g_usRxDataLen)==0)||((g_usRxDataLen)>63)) 
					g_ucHdlcBuf[10]=0x80; 
				else			            
					for(i=0;i<(g_usRxDataLen);i++)
					{
						temp=g_ucHdlcBuf[14+i]-1;
						m=temp/21;
						n=temp%21;
						
						sw1021Chip = SW1021_CHIP_ADDR(m);
						if(g_ucHdlcBuf[7]==0x01){
							XBYTE[sw1021Chip + PORT_TEST_REG(n)] |= E1LBK;
						}else{
							XBYTE[sw1021Chip + PORT_TEST_REG(n)] &= (~E1LBK);
						}
						g_ucHdlcBuf[10]=0x00;
					}
			}

			break;

		case 0x77:                /**透明帧命令**/
			addr=256*g_ucHdlcBuf[14]+g_ucHdlcBuf[15];  // commbuf[14]，commbuf[15]所存放 的 地址
			g_ucHdlcBuf[10]=0x00;
			g_ucHdlcBuf[11]=0x00;
			g_ucHdlcBuf[12]=0x00;
			g_ucHdlcBuf[13]=0x11;
			//读16个字节//	addr 地址开始读取16个字节数据//
			for(i=0;i<16;i++) 
				g_ucHdlcBuf[14+i]=XBYTE[addr+i]; 
			g_ucHdlcBuf[30]=P1;
			break;                   
		
		case 0x88:
			addr=256*g_ucHdlcBuf[14]+g_ucHdlcBuf[15];
			XBYTE[addr]=g_ucHdlcBuf[16];       //将commbuf[16]存放的数据送入commbuf[14]，//
			g_ucHdlcBuf[10]=0x00;              //commbuf[15]所存放的WGS21891的地址单元中//
			g_ucHdlcBuf[11]=0x00;
			g_ucHdlcBuf[12]=0x00;
			g_ucHdlcBuf[13]=0x10;              //读16个字节//
			for(i=0;i<16;i++) 
				g_ucHdlcBuf[14+i]=XBYTE[addr+i]; //从commbuf[14]，commbuf[15]的地址开始读取16个字节数据//
			break;

		case 0X99:
			while(1)
			{;}
			if((g_usRxDataLen==0)||(g_usRxDataLen>63)) 
				g_ucHdlcBuf[10]=0x80; 
			else			          
				for(i=0;i<(g_usRxDataLen);i++)
				{
					temp=g_ucHdlcBuf[14+i]-1;
					m=temp/21;
					n=temp%21;
					sw1021Chip = SW1021_CHIP_ADDR(m);
										
					g_ucHdlcBuf[17]=XBYTE[sw1021Chip + PORT_TEST_REG(n)];
					g_ucHdlcBuf[18]=0xbb;
					
					if(g_ucHdlcBuf[7]==0x01)
					{
						XBYTE[sw1021Chip + PORT_TEST_REG(n)] |= LnLBK;
						if(n<8)
							g_ucHdlcBuf[18]=0xff;
					}
					else
					{
						XBYTE[sw1021Chip + PORT_TEST_REG(n)] &= (~LnLBK);
					}
							
				}
			g_ucHdlcBuf[10]=0x00;
			g_ucHdlcBuf[13]=6;
			g_ucHdlcBuf[14]=0xaa;
			g_ucHdlcBuf[15]=m;
			g_ucHdlcBuf[16]=n;
			g_ucHdlcBuf[19]=XBYTE[sw1021Chip + PORT_TEST_REG(n)];
			
			break;
			

	}

}

/*****************************************************************
** 函数名:SELF_CONF
** 输　入: 无
** 输　出: 无
** 功能描述：单盘上电默认配置
** 全局变量：g_ucConfData[]
** 调用模块：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
** 注  意：所有告警和性能的初始化需安线路号顺序
****************************************************************/
void SelfConf()
{

	unsigned int i,j;
	char ctt[18];
	unsigned char ucii;
	//  first_conf=0;
	g_ucConfData[22]=BOARDCODE;/*BDINF5*/
	g_ucConfData[0]=0xfe;


	sprintf(ctt,BoardSn);
	for(ucii=0;ucii<15;ucii++) g_ucState[ucii]=ctt[ucii];  /*盘号*/

	sprintf(ctt,PcbSn);   
	for(ucii=0;ucii<15;ucii++) g_ucState[15+ucii]=ctt[ucii]; /*板号*/

	sprintf(ctt,PcbTime);   
	for(ucii=0;ucii<10;ucii++) g_ucState[30+ucii]=ctt[ucii]; /*制板时间*/

	for(ucii=0;ucii<18;ucii++) ctt[ucii]=0;    /*版本号buf清零*/
	//sprintf(ctt,"R170609R1B/01F    "); 
	//for(ucii=0;ucii<18;ucii++) g_ucState[40+ucii]=ctt[ucii];
	sprintf(ctt,BoardSoftVer); 
	for(ucii=0;ucii<18;ucii++) g_ucState[40+ucii]=ctt[ucii];
	sprintf(ctt,__DATE__);   
	for(ucii=0;ucii<11;ucii++) g_ucState[58+ucii]=ctt[ucii]; /*编程时间*/

	//g_ucState[69]=0x07;
	//g_ucState[70]=0xD1;
	//g_ucState[71]=0x06;
	//g_ucState[72]=0x01;
	//g_ucState[73]=0x08;
	//g_ucState[74]=0x00;
	//g_ucState[75]=0x00;
	g_ucState[76]=0x01;
	//g_ucState[77]=0x01;



	for(i=0;i<LINENUM;i++)           /*linecode=1..63*/
		g_ucLineCode[i]=i+1;
	for(i=0;i<LINENUM;i++)
	{
		g_ucLineMask[i]=0;          

	}



	/*for(i=0;i<LINENUM;i++)
	for(j=0;j<PMNUM;j++) linepmmask[i][j]=0;*/         


	actnumber=63;             
	almnumber=0;     

	ulEsLimit=0xff;
	ulSesLimit=0xff;

	g_ucAlmCode[0]=0x03;                        /*PPI_LOS*/
	g_ucAlmCode[1]=0x27;                        /*TU_LOP*/
	g_ucAlmCode[2]=0x22;                        /*LP_SLM*/
	g_ucAlmCode[3]=0x20;                        /*LP_RDI*/
	g_ucAlmCode[4]=0x24;                        /*LP_UNEQ*/
	//g_ucAlmCode[8]=0x04;                        /*PPI_AIS*/
	g_ucAlmCode[5]=0x02;                        /*TU_AIS*/
#ifdef NEED_LP_RFI
	g_ucAlmCode[15]=0x0c;                        /*LP_RFI*/
#endif
	g_ucAlmCode[7]=0x51;                        /*ES_LIMIT*/
	//g_ucAlmCode[8]=0x52;			/*SES _LIMIT*/
	g_ucAlmCode[9]=0x28;                       /*LP_TIM*/
	g_ucAlmCode[10]=0x4f;                       /*LOOPL*/
	g_ucAlmCode[11]=0x55;			/*LOOP*/
	g_ucAlmCode[12]=0x01;                       //2V5电源故障告警//
	g_ucAlmCode[13]=0x05;                       //5VI1电源故障告警//
	g_ucAlmCode[14]=0x06;                       //5VI2电源故障告警//
	g_ucAlmCode[6]=0x07;                 //trafic-onload//
	g_ucAlmCode[8]=0x08;                /*LP_SD*/

	g_ucPmCode[0]=0x08;                   /*BBE_LP*/
	g_ucPmCode[1]=0x02;                   /*ES_LP*/
	g_ucPmCode[2]=0x03;                   /*SES_LP*/
	g_ucPmCode[3]=0x07;                   /*REI_LP*/
	g_ucPmCode[4]=0x0b;                   /*HDB3CV*/
	//g_ucPmCode[5]=0x06;                 /*UAS_LP*/
	g_ucPmCode[5]=0x0c;                   /*UAS_LP*/
	g_ucPmCode[6]=0xa2;                   /*CSES_LP*/

	g_ucAlmType[0]=0;		/*almtype：0急告；4非急告；8状态量*/
	g_ucAlmType[1]=0;
	g_ucAlmType[2]=0;
	g_ucAlmType[3]=4;
	g_ucAlmType[4]=4;
	g_ucAlmType[5]=4;
	g_ucAlmType[6]=8;
	g_ucAlmType[7]=4;
	g_ucAlmType[8]=4;
	g_ucAlmType[9]=0;      /*LP_TIM*/
	g_ucAlmType[10]=8;         
	g_ucAlmType[11]=8;  
	g_ucAlmType[12]=0;
	g_ucAlmType[13]=0;
	g_ucAlmType[14]=0;
	
#ifdef NEED_LP_RFI
	g_ucAlmType[15]=4;
#endif

	for(j=0;j<(LINENUM-1);j++)
	{
		for(i=0;i<(ALMTYPENUM-3);i++)
		{
			g_stuAlm[j*(ALMTYPENUM-3)+i].ucLineIndex=j;
			g_stuAlm[j*(ALMTYPENUM-3)+i].ucAlmIndex=i;
		}
	}

	g_stuAlm[63*(ALMTYPENUM-3)+0].ucLineIndex=63;
	g_stuAlm[63*(ALMTYPENUM-3)+0].ucAlmIndex=12;

	g_stuAlm[63*(ALMTYPENUM-3)+1].ucLineIndex=63;
	g_stuAlm[63*(ALMTYPENUM-3)+1].ucAlmIndex=13;

	g_stuAlm[63*(ALMTYPENUM-3)+2].ucLineIndex=63;
	g_stuAlm[63*(ALMTYPENUM-3)+2].ucAlmIndex=14;


	for(j=0;j<(LINENUM-1);j++)
	{
		for(i=0;i<PMTYPENUM;i++)
		{
			g_stuPm[j*PMTYPENUM+i].ucLineIndex=j;
			g_stuPm[j*PMTYPENUM+i].ucPmIndex=i;
		}
	}

	for(i=0;i<UASNUM;i++)
	{
		g_liError1s[i].ulDatlong=0;
		ulSesLmt[i]=700; //若某一秒内误码个数大于等于ulSesLmt[i]时,该秒是严重误码秒//
	}                  
}



/*****************************************************************
** 函数名:InitioSw1021
** 输　入: 无
** 输　出: 无
** 功能描述：SW1021初始化
** 全局变量：
** 调用模块：
** 作　者：caijun.Li
** 日　期：2016.3.18
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void InitioSw1021()
{

	unsigned int i,j;
  unsigned char m,n;
  first_conf=0;

	for(i=0; i<CHIPS_ON_BOARD; i++)
	{
		XBYTE[SW1021_CHIP_ADDR(i) + SOFTWARE_RST_REG] = SRST_E1 | SRST_SDH_A | SRST_SDH_B;
		Delay(1);
		XBYTE[SW1021_CHIP_ADDR(i) + SOFTWARE_RST_REG] = RST_CANCLE;
		
		// TODO: 并行总线时钟选择
		// SW1021只支持19.44Mbit/s 的传输速率
		
		
		XBYTE[SW1021_CHIP_ADDR(i) + BUS_CTRL_REG(A_BUS_BASE)] = UPBUS_TIMER | 0x02;  // 上行总线定时  + V5 期望值 0x02
		XBYTE[SW1021_CHIP_ADDR(i) + BUS_CTRL_REG(B_BUS_BASE)] = UPBUS_TIMER | 0x02;  // 上行总线定时
		
		XBYTE[SW1021_CHIP_ADDR(i) + SOMESET_REG(A_BUS_BASE)] = BLKBIP_BLOCK;  // BIP-2按块进行校验并计数
		
		XBYTE[SW1021_CHIP_ADDR(i) + RDI_RFI_CTRL_REG(A_BUS_BASE)] = MREI_AUTO | AISV_RDIEN | LOPV_RDIEN | LOMV_RDIEN | UNEQV_RDIEN | PLMV_RDIEN | RTIMV_RDIEN | RTIUV_RDIEN;
		
		// 上下话数据总线均选择偶校验
		XBYTE[SW1021_CHIP_ADDR(i) + SDH_CTRL_REG(A_BUS_BASE)] = UP_DELAY_2;
		XBYTE[SW1021_CHIP_ADDR(i) + SDH_CTRL_REG(B_BUS_BASE)] = UP_DELAY_2;		
		
		// 21个端口从A总线上下话, 使用 +- HDB3模式
		for(j=0; j<21; j++){
			XBYTE[SW1021_CHIP_ADDR(i) + PORT_CFG_REG(j)] = A_UP_DOWN | RnEN;
			// 关闭线路环回和系统环回
			XBYTE[SW1021_CHIP_ADDR(i) + PORT_TEST_REG(j)] = 0x00;
			// TODO: 上下话交叉状态选择 ： 不交叉
			
			// TODO: TU通道LOP\AIS告警检测设置： EN_TUAIS  使能TU-AIS 中断
			//                             EN_TULOP  使能TU-LOP 中断
			//														 
			XBYTE[SW1021_CHIP_ADDR(i) + PORT_E1_ALARM_INTERRPUT_MASK_REG(j)] = E1AISE | E1LOCE;	//??
			XBYTE[SW1021_CHIP_ADDR(i) + INTERRUPT_ENABLE_REG(A_BUS_BASE,j)] = LOPE | AISE;	
			XBYTE[SW1021_CHIP_ADDR(i) + INTERRUPT_ENABLE_REG(B_BUS_BASE,j)] = LOPE | AISE;	
			//TODO： 读清中断指示
			
			//TODO: 不启动随机码测试； 启动误码计数
			
			//TODO: 上话V5-TX2 设置 LP-REI
			XBYTE[SW1021_CHIP_ADDR(i) + TX_V5_REG(A_BUS_BASE,j)] = (0x02 << 1);
			
			//TODO： 上下话通道号编码
			XBYTE[SW1021_CHIP_ADDR(i) + RXTU12_SLOT_REG(A_BUS_BASE,j)] = slot[i*21+j];	
			XBYTE[SW1021_CHIP_ADDR(i) + TXTU12_SLOT_REG(A_BUS_BASE,j)] = slot[i*21+j];	
			XBYTE[SW1021_CHIP_ADDR(i) + RXTU12_SLOT_REG(B_BUS_BASE,j)] = slot[i*21+j];	
			XBYTE[SW1021_CHIP_ADDR(i) + TXTU12_SLOT_REG(B_BUS_BASE,j)] = slot[i*21+j];	
			
			
		}
		// TODO:上下话时钟数据同相
		XBYTE[SW1021_CHIP_ADDR(i) + E1_CTRL_REG] = TRZSEL | RRZSEL;
		XBYTE[SW1021_CHIP_ADDR(i) + SDH_IF_CLK_REG] = 0x00;

	}
	
	// 填充所有支路的J2字节（初始化清0）
	for(i=0;i<63;i++)            
	{
		m=i/21;
		n=i%21;
		for(j=0;j<16;j++)
		{
			if(j==0)
			{ // A 
				writeTxJ2(m, j, slot[i],  0x80, 0);
				writeExpectJ2(m, j, slot[i],  0x80, 0);
				// B
				writeTxJ2(m, j, slot[i],  0x80, 1);
				writeExpectJ2(m, j, slot[i],  0x80, 1);           
			}
			else
			{
				// A 
				writeTxJ2(m, j, slot[i],  0x00, 0);
				writeExpectJ2(m, j, slot[i],  0x00, 0);
				// B
				writeTxJ2(m, j, slot[i],  0x00, 1);
				writeExpectJ2(m, j, slot[i],  0x00, 1); 
			}
		}
	}

	 IO8=1;
	 IO9=1;
	 IO10=1;
	 IO11=1;
	 IO12=1;
	 WRHIO;

}
/*****************************************************************
** 函数名:checkSW1021Init
** 输　入: 无
** 输　出: 无
** 功能描述：检查SW1021初始化成功
** 全局变量：
** 调用模块：
** 作　者：caijun.Li
** 日　期：2016.3.18
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
static int checkSW1021Init(void)
{
	int ret = 0;
	int i,j;
	unsigned int reg;
	for(i=0;i<CHIPS_ON_BOARD;i++)
	{
		reg = XBYTE[SW1021_CHIP_ADDR(i) + BUS_CTRL_REG(A_BUS_BASE)];
		if( (reg & (UPBUS_TIMER | 0x02)) != (UPBUS_TIMER | 0x02)){
			ret = -1;
			break;
		}
		reg = XBYTE[SW1021_CHIP_ADDR(i) + BUS_CTRL_REG(B_BUS_BASE)];
		if( (reg & (UPBUS_TIMER | 0x02)) != (UPBUS_TIMER | 0x02)){
			ret = -1;
			break;
		}
		
		reg = XBYTE[SW1021_CHIP_ADDR(i) + SDH_CTRL_REG(A_BUS_BASE)];
		if( (reg & UP_DELAY_2) != UP_DELAY_2){
			ret = -1;
			break;
		}
		
		reg = XBYTE[SW1021_CHIP_ADDR(i) + SDH_CTRL_REG(B_BUS_BASE)];
		if( (reg & UP_DELAY_2) != UP_DELAY_2){
			ret = -1;
			break;
		}
		
		for(j=0; j<21; j++){
			reg = XBYTE[SW1021_CHIP_ADDR(i) + PORT_CFG_REG(j)];
			if((reg & (A_UP_DOWN | RnEN)) != (A_UP_DOWN | RnEN)){
				ret = -1;
				goto quit;
			}
		}

	}
quit:
	return ret;
}

/*****************************************************************
** 函数名:MAIN
** 输　入: 无
** 输　出: 无
** 功能描述：主函数
** 全局变量：g_QuarterFlag
** 调用模块：SelfConf(),GetData(),ProcHisData(),Debug();
** 作　者：caijun.Li
** 日　期：2016.3.18
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void main()
{   
	unsigned char i;  
	int ret = 0;
	//Delay(50);

	SelfConf(); 
	
	ret = checkSW1021Init();
	if(ret != 0)
		InitioSw1021();
	
	// 使能外部中断3
	EX3 = 1;
	PX3 = 0;  // 低优先级

	while (1)
	{
		#ifdef SKIP_DEBUG_CHECK
			#ifdef NEED_LP_RFI				
					XBYTE[0x7fe5] = 1;
			#else
				//	XBYTE[0x7b74] = 1;
					XBYTE[0x7b79] = 1;
			#endif
		#endif
		
		//ACT = ~ACT;
		//RED = ~RED;
		//YELLOW = ~YELLOW;
				
		
		ret = checkSW1021Init();
		if(ret != 0){	
			SelfConf(); 
			InitioSw1021();
			ConfSet();	
		}
	
		GetData();
		ProcAlmData();  

		if (g_QuarterFlag) 
		{
			UasQuarter();
			for(i=0;i<LINENUM;i++)
			{
				g_stuPm[i*PMTYPENUM].Value.usint=g_usiBbe[i];
				g_stuPm[i*PMTYPENUM+1].Value.usint=g_usiEs[i];
				g_stuPm[i*PMTYPENUM+2].Value.usint=g_usiSes[i];
			}
			ProcHisData();  //将当前告警和性能数据存为历史数据//
			g_QuarterFlag=0;
		}
		
		Debug();
	}
}

