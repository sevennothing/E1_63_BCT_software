/******************************************************************
** 文件名：uasE63.h
** Copyright (c) 2002 烽火通信传输产品部BCT软件组
** 创建人：TAOY

** 描　述：为刘江明的E63单盘计算误码修改的uas.h文件，只有该单盘适用
**         
** 日　期：2004.10.08

**
** 编译器：Keil C51 V5.5 或 MCC68K V4.4
** 公共库：3.3v NewLib9.LIB for Mcs51
**         
** 版  本：1.0
********************************************************************/

#ifndef	INCuash
#define	INCuash

#ifndef UASNUM
#define UASNUM 63
#endif

#if(UASNUM>0)
/*****************************************************************
** 函数名:CalUas
** 输　入: ulSesLmt[](严重误码秒门限)
** 输　出: 无
** 功能描述：计算误码秒Es, 严重误码秒Ses, 不可用时间Uas, 背景块误码Bbe。
   形参为严重误码秒的门限seslimit。在1秒期间，累计误码到"秒误码计数error1s[UAS号].datlong"
   中、设置"帧丢失标志lofflag[UAS号]"。caluas函数负责变量error1s[UAS号].datlong和变量
   lofflag[UAS号]的清零。es,ses,uas,bbe的计算结果，可以在数据采集函数里赋值给对相应的
   g_stuAlm[].Value.usint。

** 全局变量：g_usiNoEs[],g_usiEs[],g_usiSes[],g_usiUas[],
			g_usiBbe[],g_usiNoUseBbe[],g_ucNoUseFlag[],
			g_ucLofFlag[],g_ucUseTime[],g_ucDefectFlag[],
			g_ucEsTime[],g_bClearUas;

** 调用模块：无
** 作　者：yang ying
** 日　期：2003.8.6
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
union LongInt
{
	unsigned short int usiDatint[2];
	unsigned long  ulDatlong;
};
union LongInt g_liError1s[UASNUM];
unsigned long int ulSesLmt[UASNUM];
unsigned short int g_usiNoEs[UASNUM],g_usiEs[UASNUM],g_usiSes[UASNUM],
                 g_usiCses[UASNUM],g_usiUas[UASNUM],g_usiOfs[UASNUM],
                 g_usiNoUseBbe[UASNUM],g_usiBbe[UASNUM];
unsigned char g_ucNoUseFlag[UASNUM],g_ucUseTime[UASNUM],g_ucEsTime[UASNUM],
              g_ucLofFlag[UASNUM],g_ucDefectFlag[UASNUM],g_ucNoUseTime[UASNUM];
unsigned char g_bClearUas;   
// #ifndef g_ucClearUas
//unsigned char g_ucClearUas;
//#endif        


void CalUas(unsigned long int * ulSesLmt)
{
	/*usetime,estime:record the corresponding time from NOUSE STATE to USE STATE*/
	unsigned char i;

	for(i=0;i<UASNUM;i++)
	{
		if(g_ucNoUseFlag[i])	/*Now is in the period of NOUSE STATE*/
		{
			if((g_ucLofFlag[i]!=0)||(g_ucDefectFlag[i]!=0)||(g_liError1s[i].ulDatlong>ulSesLmt[i]))	/*Defect or Severe Error*/
			{
			//	g_usiSes[i]+=(g_ucUseTime[i]+g_ucEsTime[i]+1);	/*g_usiUas+=usetime+1*/   ////
            g_usiUas[i]+=(g_ucUseTime[i]+g_ucEsTime[i]+1);	/*g_usiUas+=usetime+1*/
				g_ucUseTime[i]=0;	/*clear usetime*/
				g_ucEsTime[i]=0;	/*clear estime*/
				g_usiNoUseBbe[i]=0;	/*clear g_usiNoUseBbe*/
				if(g_ucLofFlag[i]!=0) {	g_usiOfs[i]++;	g_ucLofFlag[i]=0;}
			}
			else
			{
				if(g_liError1s[i].ulDatlong>0)	/*General Error*/
				{
					g_ucEsTime[i]++;	/*g_ucEsTime++*/
					/*cal g_usiNoUseBbe*/
					if(g_liError1s[i].usiDatint[0]>0) g_usiNoUseBbe[i]=0xffff;
					else
					{
						g_usiNoUseBbe[i]+=g_liError1s[i].usiDatint[1];
						if(g_usiNoUseBbe[i]<g_liError1s[i].usiDatint[1]) g_usiNoUseBbe[i]=0xffff;							
					}
				}
				else g_ucUseTime[i]++;	/*usetime++*/
				if((g_ucUseTime[i]+g_ucEsTime[i])>9)  	/*Jump from NOUSE STATE to USE STATE*/
				{
					g_ucNoUseFlag[i]=0;	/*g_ucNoUseFlag=0*/
					/*g_usiEs+=g_ucEsTime*/
					g_usiEs[i]+=g_ucEsTime[i];
					/*g_usiNoEs+=g_ucUseTime*/
					g_usiNoEs[i]+=g_ucUseTime[i];
					/*add g_usiNoUseBbe to usebbe*/
					g_usiBbe[i]+=g_usiNoUseBbe[i];
					if(g_usiBbe[i]<g_usiNoUseBbe[i]) g_usiBbe[i]=0xffff;
						g_usiNoUseBbe[i]=0;	/*clear g_usiNoUseBbe*/
					g_ucEsTime[i]=0;	/*clear g_ucEsTime*/
					g_ucUseTime[i]=0;	/*clear g_ucUseTime*/
				}
			}
		}
		else      	/*Now is in the period of USE STATE*/
		{
			
			if((g_ucLofFlag[i]!=0)||(g_ucDefectFlag[i]!=0)||(g_liError1s[i].ulDatlong>ulSesLmt[i]))	/*LOF or Severe Error*/
			{
				if(++g_ucNoUseTime[i]>9)    /*Jump from USE STATE to NOUSE STATE*/
				{
					g_ucNoUseFlag[i]=1;	/*SET g_ucNoUseFlag*/
					//g_usiSes[i]+=g_ucNoUseTime[i];
                g_usiUas[i]+=g_ucNoUseTime[i];
					g_ucNoUseTime[i]=0;	/*clear g_ucNoUseTime*/
				}
				
				if(g_ucLofFlag[i]!=0)  { g_usiOfs[i]++; g_ucLofFlag[i]=0; }
			}
			else
			{
				if(g_liError1s[i].ulDatlong>0)	/*General Error*/
				{
					g_usiEs[i]++;
					if(g_liError1s[i].usiDatint[0]!=0) g_usiBbe[i]=0xffff;
					else
					{
						g_usiBbe[i]+=g_liError1s[i].usiDatint[1];
						if(g_usiBbe[i]<g_liError1s[i].usiDatint[1]) g_usiBbe[i]=0xffff;
					}
				}
				else g_usiNoEs[i]++;
				
				g_usiSes[i]+=g_ucNoUseTime[i];
				g_usiEs[i]+=g_ucNoUseTime[i];
				if(g_ucNoUseTime[i]>1) g_usiCses[i]+=g_ucNoUseTime[i];
				g_ucNoUseTime[i]=0;	/*clear g_ucNoUseTime*/
			
		    }				
				
		}
		/*clear LOF and 1s Error counter*/
		g_ucDefectFlag[i]=0;
		g_liError1s[i].ulDatlong=0;
		
		if(g_usiNoEs[i]>900) g_usiNoEs[i]=900;
		if(g_usiEs[i]>900) g_usiEs[i]=900;
		if(g_usiSes[i]>900) g_usiSes[i]=900;
		if(g_usiCses[i]>900) g_usiCses[i]=900;
		if(g_usiUas[i]>900) g_usiUas[i]=900;
		if(g_usiOfs[i]>900)  g_usiOfs[i]=900;
		
	}
	
}

void UasClear()
{
	unsigned char i;
	
	if(g_bClearUas!=0)
	{
		for(i=0;i<UASNUM;i++)   
		{
			g_ucNoUseFlag[i]=0;
			g_ucDefectFlag[i]=0;
			g_ucLofFlag[i]=0;
			g_ucUseTime[i]=0;
			g_ucNoUseTime[i]=0;
			g_ucEsTime[i]=0;
			g_usiNoEs[i]=0;
			g_usiEs[i]=0;			
			g_usiSes[i]=0;
			g_usiCses[i] = 0;
			g_usiUas[i]=0;
			g_usiOfs[i] = 0;
			g_usiBbe[i]=0;
			g_usiNoUseBbe[i]=0;			
			
		}
		g_bClearUas=0;
	}
}
/*****************************************************************
** 函数名UasQuarter()
** 输　入无
** 输　出: 无
** 功能描述:处理15分钟临界点的误码

** 全局变量：g_ucNoUseFlag[],g_usiUas[],g_ucUseTime[],g_ucEsTime[],
			g_usiSes[],g_usiEs[],g_usiSes[],g_usiUas[],
			g_bClearUas
** 调用模块：无
** 作　者：FU YUAN
** 日　期：2002.7.25
** 修　改：
** 日　期：
** 版  本：1.0
****************************************************************/
void UasQuarter()
{
	unsigned char i;
	for(i=0;i<UASNUM;i++)
	{
		if(g_ucNoUseFlag[i])
		{
			g_ucNoUseFlag[i]=0;	/*initiate to USE STATE*/
			//g_usiSes[i]+=(g_ucUseTime[i]+g_ucEsTime[i]);	/*g_usiUas+=usetime*/
         g_usiUas[i]+=(g_ucUseTime[i]+g_ucEsTime[i]);	/*g_usiUas+=usetime*/
		}
		else 
		{
			g_usiNoEs[i]+=g_ucUseTime[i];
			g_usiSes[i]+=g_ucNoUseTime[i];	/*g_usiSes+=g_ucNoUseTime*/		
			if(g_ucNoUseTime[i]>1) g_usiCses[i]+=g_ucNoUseTime[i];
			
		}
		
		if(g_usiNoEs[i]>900) g_usiNoEs[i]=900;
		if(g_usiEs[i]>900) g_usiEs[i]=900;
		if(g_usiSes[i]>900) g_usiSes[i]=900;
		if(g_usiCses[i]>900) g_usiCses[i]=900;
		if(g_usiUas[i]>900) g_usiUas[i]=900;
		if(g_usiOfs[i]>900)  g_usiOfs[i]=900;
		
	}
	g_bClearUas=1;
}

#endif 	/*UASNUM*/
#endif	/*INCuash*/
