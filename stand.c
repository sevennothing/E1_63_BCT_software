/******************************************************************
** 文件名：STAND.C
** Copyright (c) 2002 烽火通信传输产品部BCT软件组
** 创建人：翁益晖
** 文件编号：
** 描　述：标准模版程序
**
** 日　期：2002.8.15
**
** 修改人：
** 日　期：
** 文件编号：
** 描　述：
**
** 编译器：Keil C51 V5.5
** 公共库：NewLibV95.LIB，OldLibV95.LIB，MC68302.lib
** 版  本：1.0
********************************************************************/
/*定义单盘参数，以下为范例*/
#define LINENUM         3
#define ALMNUM          16
#define PMNUM           10
#define ALMTYPENUM  8
#define PMTYPENUM   4
#define CONFLEN         4000
#define STATELEN        800
#define BOARDCODE       04

#include "commvar9.h"

#ifdef UASNUM
#include "uas.h"
#endif

/*定义单盘软硬件版本号，及时间，以下为范例*/
#ifdef __C51__
char code SysName[]="DEMO SYSTEM";
char code BoardName[]="Debug";
char code BoardSn[]="WKE2.155.055R1A";
char code SoftVer[]="R155055R1A.01B";
char code PcbSn[]="WKE7.155.057R1A";
char code PcbTime[]="2002.1.2";

char code SoftDate[]=__DATE__;
char code SoftTime[]=__TIME__;

char code BoardSoftVer[] = "RP0101";

#endif

#ifdef _MICROTEC
const char SysName[]="DEMO SYSTEM";
const char BoardName[]="DEMO";
const char BoardSn[]="WKE2.155.055R1A";
const char SoftVer[]="R155055R1A01B";
const char PcbSn[]="WKE7.155.057R1A";
const char PcbTime[]="2002.01.02";

const char SoftDate[]=__DATE__;
const char SoftTime[]=__TIME__;

const char BoardSoftVer[] = "RP0101";
#endif

/*****************************************************************
** 函数名:GETDATA
** 输　入: 无
** 输　出: 无
** 功能描述：采集单盘告警和性能
** 全局变量：无
** 调用函数：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
GetData(void)
{
		/*以下为标准库的测试范例，编写单盘程序时请删除*/
        unsigned char i;

        /*0－15分钟段，告警0第9分钟发生，第11分钟结束*/
        if(g_stuSysClock.ucMinute == 9) 
                g_stuAlm[0].ucState = 1;
        if(g_stuSysClock.ucMinute == 11) 
                g_stuAlm[0].ucState = 0;
        
        /*0－15分钟段，告警1第5分钟发生，没有结束*/
        if(g_stuSysClock.ucMinute == 5) 
                g_stuAlm[1].ucState = 1;        

        /*0－15分钟段，告警2第3分钟发生，没有结束
         15－30分钟段，告警2没有发生，第18分钟结束*/  
        if(g_stuSysClock.ucMinute == 3) 
                g_stuAlm[2].ucState = 1;
        if(g_stuSysClock.ucMinute == 18) 
                g_stuAlm[2].ucState = 0;

        /*0－15分钟段，告警3第4分钟发生，第5分钟结束
         第6分钟发生，第7分钟结束*/  
        if(g_stuSysClock.ucMinute == 4) 
                g_stuAlm[3].ucState = 1;
        if(g_stuSysClock.ucMinute == 5) 
                g_stuAlm[3].ucState = 0;
        if(g_stuSysClock.ucMinute == 6) 
                g_stuAlm[3].ucState = 1;
        if(g_stuSysClock.ucMinute == 7) 
                g_stuAlm[3].ucState = 0;


        /*0－15分钟段，告警4第1分钟发生，第2分钟结束
         第3分钟发生，没有结束*/  
        if(g_stuSysClock.ucMinute == 1) 
                g_stuAlm[4].ucState = 1;
        if(g_stuSysClock.ucMinute == 2) 
                g_stuAlm[4].ucState = 0;
        if(g_stuSysClock.ucMinute == 3) 
                g_stuAlm[4].ucState = 1;
        

        /*15－30分钟段，告警4第16分钟结束，没有发生
         第18分钟发生，没有结束*/  
        if(g_stuSysClock.ucMinute == 16) 
                g_stuAlm[4].ucState = 0;
        if(g_stuSysClock.ucMinute == 18) 
                g_stuAlm[4].ucState = 1;


        /*0－15分钟段，告警5发生结束8次*/  
        if(g_stuSysClock.ucMinute == 4) 
                g_stuAlm[5].ucState = 1;
        if(g_stuSysClock.ucMinute == 5) 
                g_stuAlm[5].ucState = 0;
        if(g_stuSysClock.ucMinute == 6) 
                g_stuAlm[5].ucState = 1;
        if(g_stuSysClock.ucMinute == 7) 
                g_stuAlm[5].ucState = 0;
        if(g_stuSysClock.ucMinute == 8) 
                g_stuAlm[5].ucState = 1;
        if(g_stuSysClock.ucMinute == 9) 
                g_stuAlm[5].ucState = 0;
        if(g_stuSysClock.ucMinute == 10) 
                g_stuAlm[5].ucState = 1;
        if(g_stuSysClock.ucMinute == 11) 
                g_stuAlm[5].ucState = 0;

        /*性能*/        
        for(i=0;i<4;++i)
        {
                g_stuPm[i].Value.usint=0xe0e0+i*0x0101; 
                g_stuPm[i+4].Value.usint=0xf0f0+i*0x0101;       
        }
        
        /*应添加实际单盘处理代码*/
}

/*****************************************************************
** 函数名:USERFUN
** 输　入: 无
** 输　出: 无
** 功能描述：用户实时处理函数，10毫秒被调用一次
** 全局变量：无
** 调用函数：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
#ifdef __C51__
void UserFunc() using 1
#endif

#ifdef _MICROTEC
void UserFunc()
#endif
{
        return;
}

/*****************************************************************
** 函数名:CONF_SET
** 输　入: 无
** 输　出: 无
** 功能描述：单盘配置
** 全局变量：
** 调用函数：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void ConfSet(void)
{
    /*Process ALMMASK*/
    /*Process PMMASK*/                          
}

/*****************************************************************
** 函数名:UserHdlc
** 输　入: 无
** 输　出: 无
** 功能描述：单盘特殊命令处理
** 全局变量：无
** 调用模块：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void UserHdlc(void)
{
	/*以下为标准库的测试范例，编写单盘程序时请删除*/
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
    
    /*应添加实际单盘处理代码*/
}

/*****************************************************************
** 函数名:SelfConf
** 输　入: 无
** 输　出: 无
** 功能描述：单盘上电默认配置
** 全局变量：
** 调用函数：无
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void SelfConf()
{
	/*以下为标准库的测试范例，编写单盘程序时请删除*/
        unsigned char i;
        
    g_ucConfData[0]=BOARDCODE;

        for(i=0;i<8;++i)
        {
	        g_stuAlm[i].ucLineIndex=0;
	        g_stuAlm[i].ucAlmIndex=i;
	        g_stuAlm[i+8].ucLineIndex=1;
	        g_stuAlm[i+8].ucAlmIndex=i;
        }
        
        g_ucLineCode[0]=1;
        g_ucLineCode[1]=2;
        g_ucLineCode[2]=3;

        g_ucLineMask[0]=0;
        g_ucLineMask[1]=1;
        g_ucLineMask[2]=0;
        
        g_ucAlmCode[0]=0xE0;
        g_ucAlmCode[1]=0xE1;
        g_ucAlmCode[2]=0xE2;
        g_ucAlmCode[3]=0xE3;
        g_ucAlmCode[4]=0xE4;
        g_ucAlmCode[5]=0xE5;
        g_ucAlmCode[6]=0xE6;
        g_ucAlmCode[7]=0xE7;

        g_ucAlmType[0]=0;
        g_ucAlmType[1]=0;
        g_ucAlmType[2]=0;
        g_ucAlmType[3]=0;
        g_ucAlmType[4]=0;
        g_ucAlmType[5]=0;
        g_ucAlmType[6]=0;
        g_ucAlmType[7]=0;

        g_ucPmCode[0]=0xF0;
        g_ucPmCode[1]=0xF1;
        g_ucPmCode[2]=0xF2;
        g_ucPmCode[3]=0xF3;

        for(i=0;i<4;++i)
        {
            g_stuPm[i].ucLineIndex=0;
            g_stuPm[i].ucPmIndex=i;
            g_stuPm[i+4].ucLineIndex=1;
            g_stuPm[i+4].ucPmIndex=i;
        }
        g_stuPm[8].ucLineIndex=2;
        g_stuPm[8].ucPmIndex=1;
        g_stuPm[9].ucLineIndex=2;
        g_stuPm[9].ucPmIndex=1;

	/*应添加实际单盘处理代码*/
}

/*****************************************************************
** 函数名:MAIN
** 输　入: 无
** 输　出: 无
** 功能描述：主函数
** 全局变量：
** 调用函数：
** 作　者：翁益晖
** 日　期：2002.5.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void main()
{
    if (!HotBoot_f) {    /*判断是否是上电*/
        SelfConf();
    }
	
    while (1)
    {
        GetData();
        ProcAlmData();  /*整理当前告警*/

        /*将当前告警和性能数据存为历史数据*/
        if (g_QuarterFlag) ProcHisData();
        Debug();
    }
}
