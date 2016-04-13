/******************************************************************
** �ļ�����STAND.C
** Copyright (c) 2002 ���ͨ�Ŵ����Ʒ��BCT�����
** �����ˣ�������
** �ļ���ţ�
** �衡������׼ģ�����
**
** �ա��ڣ�2002.8.15
**
** �޸��ˣ�
** �ա��ڣ�
** �ļ���ţ�
** �衡����
**
** ��������Keil C51 V5.5
** �����⣺NewLibV95.LIB��OldLibV95.LIB��MC68302.lib
** ��  ����1.0
********************************************************************/
/*���嵥�̲���������Ϊ����*/
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

/*���嵥����Ӳ���汾�ţ���ʱ�䣬����Ϊ����*/
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
** ������:GETDATA
** �䡡��: ��
** �䡡��: ��
** �����������ɼ����̸澯������
** ȫ�ֱ�������
** ���ú�������
** �����ߣ�������
** �ա��ڣ�2002.5.25
** �ޡ��ģ�
** �ա��ڣ�
** ��  ����1.0
****************************************************************/
GetData(void)
{
		/*����Ϊ��׼��Ĳ��Է�������д���̳���ʱ��ɾ��*/
        unsigned char i;

        /*0��15���ӶΣ��澯0��9���ӷ�������11���ӽ���*/
        if(g_stuSysClock.ucMinute == 9) 
                g_stuAlm[0].ucState = 1;
        if(g_stuSysClock.ucMinute == 11) 
                g_stuAlm[0].ucState = 0;
        
        /*0��15���ӶΣ��澯1��5���ӷ�����û�н���*/
        if(g_stuSysClock.ucMinute == 5) 
                g_stuAlm[1].ucState = 1;        

        /*0��15���ӶΣ��澯2��3���ӷ�����û�н���
         15��30���ӶΣ��澯2û�з�������18���ӽ���*/  
        if(g_stuSysClock.ucMinute == 3) 
                g_stuAlm[2].ucState = 1;
        if(g_stuSysClock.ucMinute == 18) 
                g_stuAlm[2].ucState = 0;

        /*0��15���ӶΣ��澯3��4���ӷ�������5���ӽ���
         ��6���ӷ�������7���ӽ���*/  
        if(g_stuSysClock.ucMinute == 4) 
                g_stuAlm[3].ucState = 1;
        if(g_stuSysClock.ucMinute == 5) 
                g_stuAlm[3].ucState = 0;
        if(g_stuSysClock.ucMinute == 6) 
                g_stuAlm[3].ucState = 1;
        if(g_stuSysClock.ucMinute == 7) 
                g_stuAlm[3].ucState = 0;


        /*0��15���ӶΣ��澯4��1���ӷ�������2���ӽ���
         ��3���ӷ�����û�н���*/  
        if(g_stuSysClock.ucMinute == 1) 
                g_stuAlm[4].ucState = 1;
        if(g_stuSysClock.ucMinute == 2) 
                g_stuAlm[4].ucState = 0;
        if(g_stuSysClock.ucMinute == 3) 
                g_stuAlm[4].ucState = 1;
        

        /*15��30���ӶΣ��澯4��16���ӽ�����û�з���
         ��18���ӷ�����û�н���*/  
        if(g_stuSysClock.ucMinute == 16) 
                g_stuAlm[4].ucState = 0;
        if(g_stuSysClock.ucMinute == 18) 
                g_stuAlm[4].ucState = 1;


        /*0��15���ӶΣ��澯5��������8��*/  
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

        /*����*/        
        for(i=0;i<4;++i)
        {
                g_stuPm[i].Value.usint=0xe0e0+i*0x0101; 
                g_stuPm[i+4].Value.usint=0xf0f0+i*0x0101;       
        }
        
        /*Ӧ���ʵ�ʵ��̴������*/
}

/*****************************************************************
** ������:USERFUN
** �䡡��: ��
** �䡡��: ��
** �����������û�ʵʱ��������10���뱻����һ��
** ȫ�ֱ�������
** ���ú�������
** �����ߣ�������
** �ա��ڣ�2002.5.25
** �ޡ��ģ�
** �ա��ڣ�
** ��  ����1.0
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
** ������:CONF_SET
** �䡡��: ��
** �䡡��: ��
** ������������������
** ȫ�ֱ�����
** ���ú�������
** �����ߣ�������
** �ա��ڣ�2002.5.25
** �ޡ��ģ�
** �ա��ڣ�
** ��  ����1.0
****************************************************************/
void ConfSet(void)
{
    /*Process ALMMASK*/
    /*Process PMMASK*/                          
}

/*****************************************************************
** ������:UserHdlc
** �䡡��: ��
** �䡡��: ��
** �����������������������
** ȫ�ֱ�������
** ����ģ�飺��
** �����ߣ�������
** �ա��ڣ�2002.5.25
** �ޡ��ģ�
** �ա��ڣ�
** ��  ����1.0
****************************************************************/
void UserHdlc(void)
{
	/*����Ϊ��׼��Ĳ��Է�������д���̳���ʱ��ɾ��*/
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
    
    /*Ӧ���ʵ�ʵ��̴������*/
}

/*****************************************************************
** ������:SelfConf
** �䡡��: ��
** �䡡��: ��
** ���������������ϵ�Ĭ������
** ȫ�ֱ�����
** ���ú�������
** �����ߣ�������
** �ա��ڣ�2002.5.25
** �ޡ��ģ�
** �ա��ڣ�
** ��  ����1.0
****************************************************************/
void SelfConf()
{
	/*����Ϊ��׼��Ĳ��Է�������д���̳���ʱ��ɾ��*/
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

	/*Ӧ���ʵ�ʵ��̴������*/
}

/*****************************************************************
** ������:MAIN
** �䡡��: ��
** �䡡��: ��
** ����������������
** ȫ�ֱ�����
** ���ú�����
** �����ߣ�������
** �ա��ڣ�2002.5.25
** �ޡ��ģ�
** �ա��ڣ�
** ��  ����1.0
****************************************************************/
void main()
{
    if (!HotBoot_f) {    /*�ж��Ƿ����ϵ�*/
        SelfConf();
    }
	
    while (1)
    {
        GetData();
        ProcAlmData();  /*����ǰ�澯*/

        /*����ǰ�澯���������ݴ�Ϊ��ʷ����*/
        if (g_QuarterFlag) ProcHisData();
        Debug();
    }
}
