#include "parm.h"
void PARM_Set(struct __PARM_HandleTypeDef * phparm,union __HeatParm *pheatParm)
{
	phparm->HEAT_KT_START_TEMP=pheatParm->st.HEAT_KT_START_TEMP;
	phparm->HEAT_KT_START_TEMP=pheatParm->st.HEAT_KT_START_TEMP;
	//phparm->HEAT_KETI_RISE_TEMP=pheatParm->st.HEAT_KETI_RISE_TEMP;
	phparm->HEAT_FS_EN_Time=pheatParm->st.HEAT_FS_EN_Time;
	phparm->HEAT_FS_D1=pheatParm->st.HEAT_FS_D1;
	phparm->HEAT_FS_D2=pheatParm->st.HEAT_FS_D2;
	phparm->HEAT_FS_D3=pheatParm->st.HEAT_FS_D3;
	phparm->HEAT_FS_D4=pheatParm->st.HEAT_FS_D4;
	phparm->HEAT_FS_D5=pheatParm->st.HEAT_FS_D5;
	phparm->HEAT_HS_EN_Time=pheatParm->st.HEAT_HS_EN_Time;
	phparm->HEAT_HS_D1=pheatParm->st.HEAT_HS_D1;
	phparm->HEAT_HS_D2=pheatParm->st.HEAT_HS_D2;
	phparm->HEAT_HS_D3=pheatParm->st.HEAT_HS_D3;
	phparm->HEAT_HS_D4=pheatParm->st.HEAT_HS_D4;
	phparm->HEAT_HS_D5=pheatParm->st.HEAT_HS_D5;
	phparm->HEAT_YB_EN_Time=pheatParm->st.HEAT_YB_EN_Time;
	phparm->HEAT_YB_StaticHz=pheatParm->st.HEAT_YB_StaticHz;
	phparm->HEAT_YB_ADJ_Time=pheatParm->st.HEAT_YB_ADJ_Time;
	phparm->HEAT_YB_DynamicParm=pheatParm->st.HEAT_YB_DynamicParm;
	phparm->HEAT_KT_StartTime=pheatParm->st.HEAT_KT_StartTime;
	phparm->HEAT_KT_JudgeTime=pheatParm->st.HEAT_KT_JudgeTime;
	phparm->HEAT2_KT_DROPDOWN_START_HEAT=pheatParm->st.HEAT2_KT_DROPDOWN_START_HEAT;
	phparm->HEAT2_START_HEAT_Time=pheatParm->st.HEAT2_START_HEAT_Time;
	phparm->HEAT2_FS_D1=pheatParm->st.HEAT2_FS_D1;
	phparm->HEAT2_FS_D2=pheatParm->st.HEAT2_FS_D2;
	phparm->HEAT2_FS_D3=pheatParm->st.HEAT2_FS_D3;
	phparm->HEAT2_FS_D4=pheatParm->st.HEAT2_FS_D4;
	phparm->HEAT2_FS_D5=pheatParm->st.HEAT2_FS_D5;
	phparm->HEAT2_FS_OFST_D1=pheatParm->st.HEAT2_FS_OFST_D1;
	phparm->HEAT2_FS_OFST_D2=pheatParm->st.HEAT2_FS_OFST_D2;
	phparm->HEAT2_FS_OFST_D3=pheatParm->st.HEAT2_FS_OFST_D3;
	phparm->HEAT2_FS_OFST_D4=pheatParm->st.HEAT2_FS_OFST_D4;
	phparm->HEAT2_FS_OFST_D5=pheatParm->st.HEAT2_FS_OFST_D5;
	phparm->HEAT2_HS_D1=pheatParm->st.HEAT2_HS_D1;
	phparm->HEAT2_HS_D2=pheatParm->st.HEAT2_HS_D2;
	phparm->HEAT2_HS_D3=pheatParm->st.HEAT2_HS_D3;
	phparm->HEAT2_HS_D4=pheatParm->st.HEAT2_HS_D4;
	phparm->HEAT2_HS_D5=pheatParm->st.HEAT2_HS_D5;
	phparm->HEAT2_HS_OFST_D1=pheatParm->st.HEAT2_HS_OFST_D1;
	phparm->HEAT2_HS_OFST_D2=pheatParm->st.HEAT2_HS_OFST_D2;
	phparm->HEAT2_HS_OFST_D3=pheatParm->st.HEAT2_HS_OFST_D3;
	phparm->HEAT2_HS_OFST_D4=pheatParm->st.HEAT2_HS_OFST_D4;
	phparm->HEAT2_HS_OFST_D5=pheatParm->st.HEAT2_HS_OFST_D5;
	phparm->HEAT2_YB_EN_OFST_Time=pheatParm->st.HEAT2_YB_EN_OFST_Time;
	phparm->HEAT2_YB_ADJ_OFST_Time=pheatParm->st.HEAT2_YB_ADJ_OFST_Time;
	phparm->STOP_FS_D1=pheatParm->st.STOP_FS_D1;
	phparm->STOP_FS_D2=pheatParm->st.STOP_FS_D2;
	phparm->STOP_FS_D3=pheatParm->st.STOP_FS_D3;
	phparm->STOP_HS_D1=pheatParm->st.STOP_HS_D1;
	phparm->STOP_HS_D2=pheatParm->st.STOP_HS_D2;
	phparm->STOP_HS_D3=pheatParm->st.STOP_HS_D3;
	phparm->STOP_SW2OFF_Time=pheatParm->st.STOP_SW2OFF_Time;
	phparm->STOP_SW2OFF_KetiTemp=pheatParm->st.STOP_SW2OFF_KetiTemp;
	phparm->WIND_FS_D1=pheatParm->st.WIND_FS_D1;
	phparm->WIND_FS_D2=pheatParm->st.WIND_FS_D2;
	phparm->WIND_FS_D3=pheatParm->st.WIND_FS_D3;
}
void PARM_DefSet(struct __PARM_HandleTypeDef * phparm)
{
	//start 新加参数
	phparm->HEAT_KT_START_TEMP = 70;//默认70摄氏度
	phparm->HEAT_KT_RISE_TEMP = 6;//// 加热模式，二次点火，点火成功判定  温度上升3度  即认为点火成功。  8会误判为不点火成功
	phparm->HEAT_YB_StaticHz = 60;
	phparm->HEAT_YB_DynamicParm = 70;
	//end  新加参数
	//start 加热参数
	phparm->HEAT_FS_EN_Time = 0;
	phparm->HEAT_FS_D1.Start_s =1;
	phparm->HEAT_FS_D1.Stop_s = 10;
	phparm->HEAT_FS_D1.parm = 1000;// phparm->FS1000PrmPre;
	phparm->HEAT_FS_D2.Start_s = 50;
	phparm->HEAT_FS_D2.Stop_s = 60;
	phparm->HEAT_FS_D2.parm = 1500;// phparm->FS1500PrmPre;
	phparm->HEAT_FS_D3.Start_s = 90;
	phparm->HEAT_FS_D3.Stop_s = 120;
	phparm->HEAT_FS_D3.parm = 2000;// phparm->FS3000PrmPre;
	phparm->HEAT_FS_D4.Start_s = 0;
	phparm->HEAT_FS_D4.Stop_s = 0;
	phparm->HEAT_FS_D4.parm = 0;
	phparm->HEAT_FS_D5.Start_s = 0;
	phparm->HEAT_FS_D5.Stop_s = 0;
	phparm->HEAT_FS_D5.parm = 0;
	phparm->HEAT_HS_EN_Time = 4;
	phparm->HEAT_HS_D1.Start_s = 4;
	phparm->HEAT_HS_D1.Stop_s =8;
	phparm->HEAT_HS_D1.parm = 500;// phparm->HS14V0Prmparm;
	phparm->HEAT_HS_D2.Start_s = 8;
	phparm->HEAT_HS_D2.Stop_s = 12;
	phparm->HEAT_HS_D2.parm = 1400;
	phparm->HEAT_HS_D3.Start_s = 140;
	phparm->HEAT_HS_D3.Stop_s = 148;
	phparm->HEAT_HS_D3.parm = 0;
	phparm->HEAT_HS_D4.Start_s = 0;
	phparm->HEAT_HS_D4.Stop_s = 0;
	phparm->HEAT_HS_D4.parm = 0;
	phparm->HEAT_HS_D5.Start_s = 0;
	phparm->HEAT_HS_D5.Stop_s = 0;
	phparm->HEAT_HS_D5.parm = 0;
	phparm->HEAT_YB_EN_Time = 60;
	phparm->HEAT_YB_ADJ_Time = 65;
	phparm->HEAT_KT_StartTime = 60;
	phparm->HEAT_KT_JudgeTime = 150;
	//end加热参数
	//start 二次点火参数
	phparm->HEAT2_FS_EN_Time = 0;
	phparm->HEAT2_FS_D1.Start_s = 0;
	phparm->HEAT2_FS_D1.Stop_s = 10;
	phparm->HEAT2_FS_D1.parm = 1000;
	phparm->HEAT2_FS_D2.Start_s = 60;
	phparm->HEAT2_FS_D2.Stop_s = 70;
	phparm->HEAT2_FS_D2.parm = 2000;
	phparm->HEAT2_FS_D3.Start_s =0;
	phparm->HEAT2_FS_D3.Stop_s = 0;
	phparm->HEAT2_FS_D3.parm = 0;
	phparm->HEAT2_FS_D4.Start_s = 0;
	phparm->HEAT2_FS_D4.Stop_s = 0;
	phparm->HEAT2_FS_D4.parm = 0;
	phparm->HEAT2_FS_D5.Start_s = 0;
	phparm->HEAT2_FS_D5.Stop_s = 0;
	phparm->HEAT2_FS_D5.parm = 0;
	phparm->HEAT2_EN_OFST_Time = 120;
	phparm->HEAT2_EN_OFST_KT_Temp = 70;
	phparm->HEAT2_FS_OFST_D1.Start_s = 1;
	phparm->HEAT2_FS_OFST_D1.Stop_s = 10;
	phparm->HEAT2_FS_OFST_D1.parm = 1000;
	phparm->HEAT2_FS_OFST_D2.Start_s = 50;
	phparm->HEAT2_FS_OFST_D2.Stop_s = 60;
	phparm->HEAT2_FS_OFST_D2.parm = 1500;
	phparm->HEAT2_FS_OFST_D3.Start_s = 90;
	phparm->HEAT2_FS_OFST_D3.Stop_s = 120;
	phparm->HEAT2_FS_OFST_D3.parm = 2000;
	phparm->HEAT2_FS_OFST_D4.Start_s = 0;
	phparm->HEAT2_FS_OFST_D4.Stop_s = 0;
	phparm->HEAT2_FS_OFST_D4.parm = 0;
	phparm->HEAT2_FS_OFST_D5.Start_s = 0;
	phparm->HEAT2_FS_OFST_D5.Stop_s = 0;
	phparm->HEAT2_FS_OFST_D5.parm = 0;
	phparm->HEAT2_HS_D1.Start_s = 50;
	phparm->HEAT2_HS_D1.Stop_s = 55;
	phparm->HEAT2_HS_D1.parm = 1500;
	phparm->HEAT2_HS_D2.Start_s = 110;
	phparm->HEAT2_HS_D2.Stop_s = 115;
	phparm->HEAT2_HS_D2.parm = 0;
	phparm->HEAT2_HS_D3.Start_s = 0;
	phparm->HEAT2_HS_D3.Stop_s = 0;
	phparm->HEAT2_HS_D3.parm = 0;
	phparm->HEAT2_HS_D4.Start_s = 0;
	phparm->HEAT2_HS_D4.Stop_s = 0;
	phparm->HEAT2_HS_D4.parm = 0;
	phparm->HEAT2_HS_D5.Start_s = 0;
	phparm->HEAT2_HS_D5.Stop_s = 0;
	phparm->HEAT2_HS_D5.parm = 0;
	phparm->HEAT2_HS_OFST_D1.Start_s = 4;
	phparm->HEAT2_HS_OFST_D1.Stop_s = 10;
	phparm->HEAT2_HS_OFST_D1.parm = 1500;
	phparm->HEAT2_HS_OFST_D2.Start_s = 0;
	phparm->HEAT2_HS_OFST_D2.Stop_s = 0;
	phparm->HEAT2_HS_OFST_D2.parm = 0;
	phparm->HEAT2_HS_OFST_D3.Start_s = 0;
	phparm->HEAT2_HS_OFST_D3.Stop_s = 0;
	phparm->HEAT2_HS_OFST_D3.parm = 0;
	phparm->HEAT2_HS_OFST_D4.Start_s = 0;
	phparm->HEAT2_HS_OFST_D4.Stop_s = 0;
	phparm->HEAT2_HS_OFST_D4.parm = 0;
	phparm->HEAT2_HS_OFST_D5.Start_s = 0;
	phparm->HEAT2_HS_OFST_D5.Stop_s = 0;
	phparm->HEAT2_HS_OFST_D5.parm = 0;
	phparm->HEAT2_YB_EN_OFST_Time = 60;
	phparm->HEAT2_YB_ADJ_OFST_Time =65;
	phparm->HEAT2_KT_HeatOffStartTime = 5;
	phparm->HEAT2_KT_HeatOffJudgeTime = 100;
	phparm->HEAT2_KT_DROPDOWN_START_HEAT = 70;//2次点火壳体温度小于70度时，开启二次点火
	phparm->HEAT2_START_HEAT_Time = 90;//二次点火大于此时间，开始二次点火。
	//end　　二次点火参数
	//start 通风参数
	phparm->WIND_FS_D1.Start_s = 0;
	phparm->WIND_FS_D1.Stop_s = 10;
	phparm->WIND_FS_D1.parm = 2000;// phparm->FS3500Prmparm;
	phparm->WIND_FS_D2.Start_s = 0;
	phparm->WIND_FS_D2.Stop_s = 0;
	phparm->WIND_FS_D2.parm = 0;
	phparm->WIND_FS_D3.Start_s = 0;
	phparm->WIND_FS_D3.Stop_s = 0;
	phparm->WIND_FS_D3.parm = 0;
	//end 通风参数
	//start 正常模式参数
	phparm->NORMAL_FS_D1.Start_s = 0;
	phparm->NORMAL_FS_D1.Stop_s = 30;
	phparm->NORMAL_FS_D1.parm = 2000;// phparm->FS3500Prmparm;
	phparm->NORMAL_FS_D1.Start_s = 30;
	phparm->NORMAL_FS_D1.Stop_s = 130;
	phparm->NORMAL_FS_D1.parm = 1500;// phparm->FS4500Prmparm;
	phparm->NORMAL_HS_DIS_Time = 30;
	phparm->NORMAL_STATE_LEVEL_HOLD_Second = 120;//120秒
	phparm->NORMAL_DWQ_JK_TEMP_Cpmpare = 2;//2度，电位器调节温度与  进口温度比较
	phparm->NORMAL_STATE_SWITCH_LOW_LEVEL_DELAY_Second=10;
	phparm->NORMAL_STATE_SWITCH_HIGH_LEVEL_DELAY_Second=10;
	phparm->NORMAL_STATE_SWITCH_LEVEL_Second=40;
	phparm->NORMAL_STATE_HEAT_OUT_JUDGE_Time = 300;
	phparm->NORMAL_STATE_HEAT_OUT_JUDGE_KeTiTemp = 40;
	phparm-> NORMAL_STATE_CHUKOU_DROPDOWN_Temp = 15;
	phparm->NORMAL_STATE_KETI_DROPDOWN_Temp = 25;
	//end 正常模式参数
	//start stop模式参数
	phparm->STOP_FS_D1.Start_s = 0;
	phparm->STOP_FS_D1.Stop_s = 10;
	phparm->STOP_FS_D1.parm = 2000;//phparm->FS2000Prmparm;
	phparm->STOP_FS_D2.Start_s = 55;
	phparm->STOP_FS_D2.Stop_s = 60;
	phparm->STOP_FS_D2.parm = 1500;//phparm->FS2000Prmparm;
	phparm->STOP_HS_D1.Start_s = 10;
	phparm->STOP_HS_D1.Stop_s = 15;
	phparm->STOP_HS_D1.parm = 1500;//phparm->HS14V5Prmparm;
	phparm->STOP_HS_D2.Start_s = 50;
	phparm->STOP_HS_D2.Stop_s = 55;
	phparm->STOP_HS_D2.parm = 0;//phparm->HS14V5Prmparm;
	phparm->STOP_SW2OFF_Time = 60;
	phparm->STOP_SW2OFF_KetiTemp = 70;
	phparm->STOP_HS_DIS_Time = 40;
	//end stop模式参数
}
void PARM_Init(struct __PARM_HandleTypeDef * phparm)
{
	//从Flash 读取配置参数
	HeatParm heatParm;
	if (!readParmFromFlash(&heatParm))//正确读出配置
	{
		PARM_Set(phparm,&heatParm);
	}//没有读出配置
	else {
		PARM_DefSet(phparm);
	}
	//phparm->ParmSet = PARM_Set;
	//phparm->FS_PreToYB_Hz = 65;
	//phparm->ModeXkw = 5;//5kw 瓦
}
