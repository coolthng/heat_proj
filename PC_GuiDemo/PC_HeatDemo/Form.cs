using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using ReadExcel;
using System.Text.RegularExpressions;
using System.Threading;
using System.IO;
using System.Runtime.InteropServices;
using MyQueue;

namespace PC_HeatDemo
{
    public partial class Form1:Form
    {
        SerialPort sp = new SerialPort();//新建串口对象
        FormProcess myFormProcess = new FormProcess();
        byte[] myStateByteArry = new byte[500];
        byte[] mySetByteArry = new byte[50];

        byte keyState = 0;//按键值状态
        byte levelState = 0;//正常工作模式档位
        Int16[] SetStateParmArry = new Int16[10];

        MyQueue.MyQueue myQueue = new MyQueue.MyQueue();
        StreamWriter swHeatLogFile;

        __HeatDis heatDis = new __HeatDis();

        public delegate void GuiUpdata();
        GuiUpdata myGuiUpdata;

      

        bool stateSendFlag = false;//状态下发完成标志
        bool stateSetParmFlag = false;//设置参数下发完成标志
        bool stateUpParmFlag = false;//更新参数标志
        bool stateBackParmFlag = false;//更新参数回传标志

        Int16[] SetUpParmArry = new Int16[10];

        [StructLayout(LayoutKind.Sequential)]
        //[StructLayout(LayoutKind.Explicit)]
        public struct __HeatDis
        {

            public byte comnd;//0
            public byte HsVoltage;//1 火花塞电压
            public byte parm1;//2  高四位状态机  低四位报警状态
            public byte parm2;//3  高四位挡位    低四位保留  
            public byte StateMachineRun_s;//4 状态机运行时间
            public byte JinKouTemp;//5 进口温度
            public UInt16 CurrentPrm;//6 当前转速
            public Int16 KeTiTemp;//8 壳体温度
            public UInt16 PowerVolatge;//10 电源电压
            public UInt16 YouBengHz;//12 油泵频率
            public Int16 resav3;//14 保留数据3
            public Int16 resav4;//16 保留数据4
            public byte tail1;//18 0x0d
            public byte tail2;//19 0x0a

            public byte AlarmState
            {
                get
                {
                    return (byte)(this.parm1 & 0x0f);
                }
                set
                {
                    this.parm1 = (byte)((this.parm1 & 0x0f) | (parm1 & 0xf0));
                }
            }
            public byte StateMachine
            {
                get
                {
                    return (byte)(this.parm1 >> 4 & 0x0f);
                }
                set
                {
                    this.parm1 = (byte)((this.parm1 & 0x0f) | (parm1 & 0xf0));
                }
            }
            public byte noused
            {
                get
                {
                    return (byte)(this.parm2 & 0x0f);
                }
                set
                {
                    this.parm2 = (byte)((this.parm2 & 0x0f) | (parm2 & 0xf0));
                }
            }
            public byte StateRunLevel
            {
                get
                {
                    return (byte)(this.parm2 >> 4 & 0x0f);
                }
                set
                {
                    this.parm2 = (byte)((this.parm2 & 0x0f) | (parm2 & 0xf0));
                }
            }

        };
        public byte[] StructToBytes(object obj)
        {
            int rawsize = Marshal.SizeOf(obj);
            IntPtr buffer = Marshal.AllocHGlobal(rawsize);
            Marshal.StructureToPtr(obj, buffer, false);
            byte[] rawdatas = new byte[rawsize];
            Marshal.Copy(buffer, rawdatas, 0, rawsize);
            Marshal.FreeHGlobal(buffer);
            return rawdatas;
        }

        public object BytesToStruct(byte[] buf, int len, Type type)
        {
            object rtn;
            IntPtr buffer = Marshal.AllocHGlobal(len);
            Marshal.Copy(buf, 0, buffer, len);
            rtn = Marshal.PtrToStructure(buffer, type);
            Marshal.FreeHGlobal(buffer);
            return rtn;
        }

        public void BytesToStruct(byte[] buf, int len, object rtn)
        {
            IntPtr buffer = Marshal.AllocHGlobal(len);
            Marshal.Copy(buf, 0, buffer, len);
            Marshal.PtrToStructure(buffer, rtn);
            Marshal.FreeHGlobal(buffer);
        }

        public void BytesToStruct(byte[] buf, object rtn)
        {
            BytesToStruct(buf, buf.Length, rtn);
        }

        public object BytesToStruct(byte[] buf, Type type)
        {
            return BytesToStruct(buf, buf.Length, type);
        }

        Thread thComm;
        const int RX_BUFFER_LENGTH = 36;
        int RxArrPtrHeader = 0;
        int RxArrPtrTail = 0;
        byte[] RxArr = new byte[RX_BUFFER_LENGTH];
        byte[] UpdataArr = new byte[RX_BUFFER_LENGTH];
        int ttttttt_count = 0;
        public enum CommState
        {
            STATE_TX = 0,
            STATE_RX = 1,
            STATE_RX_TIME_OUT = 2,
        };
        public void FormInit()
        {
            thComm =new Thread(ThreadComm);


        }
        private void ThreadComm()
        {
            CommState myCommState = new CommState();
            myCommState = CommState.STATE_TX;
            QueueInfo myQueueInfoTx = new QueueInfo();
            int usart_tx_length = 0;
            int CommRxTimeOutCount = 0;
            while (true)
            {
                switch (myCommState)
                {
                    case CommState.STATE_TX:
                        //获取事件
                       myQueueInfoTx= myQueue.DecQune();
                        if (myQueueInfoTx==null)//没有新事件
                        {
                            mySetByteArry[0] = (byte)'a';//0x01;
                            mySetByteArry[1] = 0x0d;
                            mySetByteArry[2] = 0x0a;
                            usart_tx_length = 3;
                        }
                        else
                        {
                            switch (myQueueInfoTx.Type)
                            {
                                case 'k':
                                    object aaa = myQueueInfoTx.Msg;
                                    mySetByteArry[0] = (byte)'k';// 0x01;
                                    mySetByteArry[1] = 0;//levelState;// 0x06;
                                    mySetByteArry[2] = Convert.ToByte(aaa);// 0x00;
                                    mySetByteArry[3] = 0x0d;// 0x00;
                                    mySetByteArry[4] = 0x0a;// levelState;
                                    usart_tx_length = 4;
                                    break;
                                case 'l':
                                    break;
                                default:
                                    break;
                            }
                        }
                        if (sp.IsOpen)
                        {
                            sp.Write(mySetByteArry, 0, usart_tx_length);
                        }
                        myCommState = CommState.STATE_RX;//进入到接受
#if false
                        //start 填充发送数据
                        if (stateSendFlag)//有按键使能
                        {
                            mySetByteArry[0] = (byte)'k';// 0x01;
                            mySetByteArry[1] = levelState;// 0x06;
                            mySetByteArry[2] = keyState;// 0x00;
                            mySetByteArry[3] = 0x0d;// 0x00;
                            mySetByteArry[4] = 0x0a;// levelState;
                        }
                        else if (stateSetParmFlag)//设置参数
                        {
                            mySetByteArry[0] = (byte)'s';
                            mySetByteArry[1] = 0x10;
                            for (int i = 0; i < 8; i++)
                            {
                                mySetByteArry[2 + 2 * i] = (byte)(SetStateParmArry[i] % 256);
                                mySetByteArry[2 + 2 * i + 1] = (byte)(SetStateParmArry[i] / 256);
                            }
                            mySetByteArry[18] = 0x0d;// (byte)(res % 256);
                            mySetByteArry[19] = 0x0a;// (byte)(res / 256);
                        }
                        else if (stateUpParmFlag)//配置初始化参数
                        {
                            mySetByteArry[0] = (byte)'p';
                            int txLength = SetUpParmArry[0] % 16;
                            mySetByteArry[1] = (byte)txLength;
                            for (int i = 0; i <= txLength; i++)
                            {
                                mySetByteArry[2 + 2 * i] = (byte)(SetUpParmArry[i] % 256);
                                mySetByteArry[2 + 2 * i + 1] = (byte)(SetUpParmArry[i] / 256);
                            }
                            mySetByteArry[4 + 2 * txLength] = 0x0d;// (byte)(res % 256);
                            mySetByteArry[5 + 2 * txLength] = 0x0a;// (byte)(res / 256);

                        }
                        else//获取常规状态
                        {
                            stateSetCount = 0;
                            mySetByteArry[0] = 0x61;//'a';//0x01;
                            mySetByteArry[1] = 0x0d;
                            mySetByteArry[2] = 0x0a;
                            if (sp.IsOpen)
                            {
                                sp.Write(mySetByteArry, 0, 3);
                            }

                        }

                        //end  填充发送数据
#endif
                        break;
                    case CommState.STATE_RX:
                        
                        int SDataLength = this.sp.BytesToRead;
                        while (SDataLength > RX_BUFFER_LENGTH)
                        {
                            byte[] temp = new byte[RX_BUFFER_LENGTH];
                            sp.Read(temp, 0, RX_BUFFER_LENGTH);
                            SDataLength = this.sp.BytesToRead;
                        }
                        if ((RxArrPtrTail + SDataLength) > RX_BUFFER_LENGTH)
                        {
                            sp.Read(RxArr, RxArrPtrTail, RX_BUFFER_LENGTH - RxArrPtrTail);
                            sp.Read(RxArr, 0, SDataLength + RxArrPtrTail - RX_BUFFER_LENGTH);
                        }
                        else
                        {
                            sp.Read(RxArr, RxArrPtrTail, SDataLength);
                        }
                        RxArrPtrTail += SDataLength;

                        //RxArr[RxArrPtrTail++] = ArrTmp[j];//(byte)(SDateTemp);
                        if (RxArrPtrTail >= RX_BUFFER_LENGTH)
                        {
                            RxArrPtrTail = RxArrPtrTail % RX_BUFFER_LENGTH;//循环指针
                        }
                        if (RxArr[(RxArrPtrTail + RX_BUFFER_LENGTH - 2) % RX_BUFFER_LENGTH] == 0x0d && RxArr[(RxArrPtrTail + RX_BUFFER_LENGTH - 1) % RX_BUFFER_LENGTH] == 0x0a)
                        {
                            int i = 0;
                            int s_header = RxArrPtrHeader;
                            for (; i < (RxArrPtrTail + RX_BUFFER_LENGTH - s_header) % RX_BUFFER_LENGTH; i++)
                            {
                                UpdataArr[i] = RxArr[RxArrPtrHeader++];
                                if (RxArrPtrHeader >= RX_BUFFER_LENGTH)
                                {
                                    RxArrPtrHeader = 0;
                                }
                            }
                            UpdataArr[0] = UpdataArr[0];
                            CommRxTimeOutCount = 0;
                            this.Invoke(myGuiUpdata);
                            myCommState = CommState.STATE_TX;//接收成功，下一跳到发送状态
                        }
                        CommRxTimeOutCount++;
                        if (CommRxTimeOutCount > 100)
                        {
                            myCommState = CommState.STATE_RX_TIME_OUT;//吓一跳到，通讯超时状态
                        }
                        Thread.Sleep(5);
                        //Console.WriteLine("字符" + ttttttt_count++);
                        //this.Invoke(myGuiUpdata);
                        // this.label_Version.Text = ttttttt_count.ToString();
              
                break;
                    case CommState.STATE_RX_TIME_OUT://接收通讯超时
                        Console.WriteLine("通讯超时  timeout ");
                        CommRxTimeOutCount = 0;
                        myCommState = CommState.STATE_TX;
                        break;
                    default:
                        break;
                }
            }
#if false
            while (true)
            {
                int SDataLength = this.sp.BytesToRead;
                while (SDataLength > RX_BUFFER_LENGTH)
                {
                    byte[] temp = new byte[RX_BUFFER_LENGTH];
                    sp.Read(temp, 0, RX_BUFFER_LENGTH);
                    SDataLength = this.sp.BytesToRead;
                }
                if ((RxArrPtrTail + SDataLength) > RX_BUFFER_LENGTH)
                {
                    sp.Read(RxArr, RxArrPtrTail, RX_BUFFER_LENGTH - RxArrPtrTail);
                    sp.Read(RxArr, 0, SDataLength + RxArrPtrTail - RX_BUFFER_LENGTH);
                }
                else
                {
                    sp.Read(RxArr, RxArrPtrTail, SDataLength);
                }
                RxArrPtrTail += SDataLength;

                //RxArr[RxArrPtrTail++] = ArrTmp[j];//(byte)(SDateTemp);
                if (RxArrPtrTail >= RX_BUFFER_LENGTH)
                {
                    RxArrPtrTail = RxArrPtrTail % RX_BUFFER_LENGTH;//循环指针
                }
                if (RxArr[(RxArrPtrTail + RX_BUFFER_LENGTH - 2) % RX_BUFFER_LENGTH] == 0x0d && RxArr[(RxArrPtrTail + RX_BUFFER_LENGTH - 1) % RX_BUFFER_LENGTH] == 0x0a)
                {
                    int i = 0;
                    int s_header = RxArrPtrHeader;
                    for (; i < (RxArrPtrTail + RX_BUFFER_LENGTH - s_header) % RX_BUFFER_LENGTH; i++)
                    {
                        UpdataArr[i] = RxArr[RxArrPtrHeader++];
                        if (RxArrPtrHeader >= RX_BUFFER_LENGTH)
                        {
                            RxArrPtrHeader = 0;
                        }
                    }
                    UpdataArr[0] = UpdataArr[0];
                    // DoUpdate();
                    this.Invoke(myGuiUpdata);
                }

                Thread.Sleep(5);
                Console.WriteLine("字符" + ttttttt_count++);
                //this.Invoke(myGuiUpdata);
                // this.label_Version.Text = ttttttt_count.ToString();
            }
#endif
        }



        private void ThreadSendParm()
        {
            const bool StaticHeatParmFlag = false;
            //textBox_Other.Text = "读heatparm.xel配置文件";
            Console.WriteLine("读heatparm.xel配置文件");
            ReadExcel.ReadExcel readExcel = new ReadExcel.ReadExcel();//定义读Excel
            string projDirStr = Environment.CurrentDirectory;//当前工程路径
            Regex regex = new Regex("PC_GuiDemo");//特定目录下
            Match mc = regex.Match(projDirStr);//正则表达式
            int position = mc.Index;//
            string heatParmDir = projDirStr.Substring(0, position);
            String filename = heatParmDir + "PC_GuiDemo" + "\\" + "heatparm.xlsx";//文件名
            int parmLength = 0;
            int[] testArr = { 0, 5, 0, 1, 10, 1000, 50, 60, 1500, 90, 120, 2000, 0, 0, 0, 0, 0, 0, 4, 4, 8, 500, 8, 12, 1400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 60, 65, 7, 60, 150, 70, 90, 0, 10, 1000, 60, 70, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 1000, 50, 60, 1500, 90, 120, 2000, 0, 0, 0, 0, 0, 0, 50, 55, 1500, 110, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 10, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 65, 0, 10, 2000, 55, 60, 1500, 0, 0, 0, 10, 15, 1500, 50, 55, 0, 0, 0, 0, 60, 70, 0, 10, 2000, 0, 0, 0, 0, 0, 0 };
            //new int[20];
            Array arr;
            if (StaticHeatParmFlag)//模拟参数
            {
                Console.WriteLine("读模拟数据");
                parmLength = testArr.Length;// arr.Length;
            }
            else
            {
                Console.WriteLine("读Excel数据");
                arr = readExcel.ReadXlsSheetName(filename, "heatparm");//读参数配置表
                parmLength = arr.Length;// arr.Length;
            }

            int parmTxPostion = 0;
            //textBox_Other.Text = "读取配置信息成功，正在写数据";
            Console.WriteLine("读取配置信息成功，正在写数据");
            stateBackParmFlag = true;
            int countThread = 0;
            while (parmTxPostion < parmLength)
            {
                while (!stateBackParmFlag)
                {
                    Thread.Sleep(100);
                    countThread++;
                    if (countThread > 10)
                    {
                        countThread = 0;
                        stateUpParmFlag = true;//使能发送
                    }

                }
                int i = 0;
                for (; i < 7 && (parmTxPostion + i) < parmLength; i++)
                {
                    if (StaticHeatParmFlag)
                    {
                        UInt16 tmp = (UInt16)testArr[parmTxPostion + i];
                        SetUpParmArry[i + 1] = (Int16)(tmp); //BitConverter.ToInt16(arr[parmTxPostion]);  // arr[parmTxPostion++];
                    }
                    else
                    {
                        double tmp = (double)arr.GetValue(parmTxPostion + i + 1, 1);//parmTxPostion++);
                        SetUpParmArry[i + 1] = (Int16)(tmp); //BitConverter.ToInt16(arr[parmTxPostion]);  // arr[parmTxPostion++];
                    }

                }
                SetUpParmArry[0] = (Int16)(parmTxPostion * 16 + i);

                Console.WriteLine("总长度:" + parmLength + "已写parmTxPostion:" + parmTxPostion);
                parmTxPostion += i;
                stateUpParmFlag = true;//使能发送
                stateBackParmFlag = false;//等待回传成功
            }
            Console.WriteLine("写入数据完成");
        }

        //更新显示
        private void DoUpdate()///希望被执行的函数（被委托）        
        {
            //01 04 09 22 34 00 22 00 33 00 44 71cB
            //if (UpdataArr[0] == 0x61)
            if (true)//暂定
            {
                heatDis = (__HeatDis)BytesToStruct((byte[])UpdataArr, Marshal.SizeOf(heatDis), heatDis.GetType());
                string item_temp;
                //状态机状态
                switch (heatDis.StateMachine)
                {
                    case 1:
                        label_MachineState.Text = "加热状态";
                        break;
                    case 2:
                        label_MachineState.Text = "通风状态";
                        break;
                    case 3:
                        label_MachineState.Text = "正常状态";
                        break;
                    case 4:
                        label_MachineState.Text = "二次点火";
                        break;
                    case 5:
                        label_MachineState.Text = "关机状态";
                        break;
                    case 6:
                        label_MachineState.Text = "PowerOff";
                        break;
                    case 7:
                        label_MachineState.Text = "空状态";
                        break;
                    case 8:
                        label_MachineState.Text = "测试状态";
                        break;
                    case 10:
                        label_MachineState.Text = "调试状态";
                        break;
                    default:
                        label_MachineState.Text = "未知状态";
                        break;
                }
                /*
                 故障简单列表
                 1.火花塞故障（开路短路故障）
                 2.油泵故障（开路短路故障）
                 3.风扇故障（开路短路）
                 4.风扇过载故障（堵转）+霍尔故障
                 5.进口温度故障
                 6.电压故障（电源过压欠压）
                 7.通信故障
                 8.客体温度故障（客体温度超过）
                 9.二次点火（二次点火中）
                 10.二次点火失败

                1：长亮档位5/快闪二次点火失败/慢闪二次点火中
                2：长亮档位4/快闪客体温度故障/慢闪进口温度故障
                3：长亮档位3/快闪电压故障/慢闪通信故障
                4：长亮档位2/快闪风扇故障/慢闪风扇过载故障
                5：长亮档位1/快闪火花塞故障/慢闪油泵故障
                 */
                switch (heatDis.AlarmState)//低4位  故障状态
                {
                    case 1:
                        label_ErrorState.Text = "poweroff";

                        break;
                    case 2:
                        label_ErrorState.Text = "通信故障";
                        break;
                    case 3:
                        label_ErrorState.Text = "电机故障";
                        break;
                    case 4:
                        label_ErrorState.Text = "火花塞故障";
                        break;
                    case 5:
                        label_ErrorState.Text = "油泵故障";
                        break;
                    case 6:
                        label_ErrorState.Text = "温度过热";
                        break;
                    case 7:
                        label_ErrorState.Text = "电压故障";
                        break;
                    case 8:
                        label_ErrorState.Text = "二次点火失败";
                        break;
                    case 9:
                        label_ErrorState.Text = "二次点火";
                        break;
                    case 10:
                        label_ErrorState.Text = "加热状态";
                        break;
                    case 11:
                        label_ErrorState.Text = "关闭指示";
                        break;
                    case 12:
                        label_ErrorState.Text = "关机";
                        break;
                    case 13:
                        label_ErrorState.Text = "无状态切换";
                        break;
                    case 14:
                        label_ErrorState.Text = "空状态";
                        break;
                    default:
                        break;
                }
                //档位
                switch (heatDis.StateRunLevel)//高四位档位  
                {
                    case 1:
                        label_DianWeiQi.Text = "1档位";
                        break;
                    case 2:
                        label_DianWeiQi.Text = "2档位";
                        break;
                    case 3:
                        label_DianWeiQi.Text = "3档位";
                        break;
                    case 4:
                        label_DianWeiQi.Text = "4档位";
                        break;
                    case 5:
                        label_DianWeiQi.Text = "5档位";
                        break;
                    default:
                        label_DianWeiQi.Text = "无档位调节";
                        break;
                }
                switch (heatDis.noused)//低四位  保留  
                {
                    case 1:
                        break;
                    default:
                        break;
                }

                //运行时间
                label_RunTime.Text = heatDis.StateMachineRun_s.ToString() + 's';
                //电压
                label_DianYuan.Text = Convert.ToString(heatDis.PowerVolatge / 100) + '.' + Convert.ToString((heatDis.PowerVolatge % 100) / 10) + Convert.ToString(heatDis.PowerVolatge % 10) + 'V';  // item_temp;
                //转速
                label_FengShan.Text = heatDis.CurrentPrm.ToString() + "RPM";// item_temp;
                //壳体温度
                label_KeTi.Text = heatDis.KeTiTemp.ToString() + '℃';
                //火花塞调节占空比
                label_HuoSai_Adj_Pre.Text = "未显示";
                //风扇调节占空比
                label_FengShan_Adj_Pre.Text = "未显示";

                //活塞占空比
                label_HuoSai_Pre.Text = Convert.ToString(heatDis.HsVoltage / 10) + '.' + Convert.ToString(heatDis.HsVoltage % 10) + 'V';
                //风扇占空比
                label_FengShan_Pre.Text = "未显示";

                //油泵频率
                label_YouBeng_Pre.Text = Convert.ToString(heatDis.YouBengHz / 10) + '.' + Convert.ToString(heatDis.YouBengHz % 10) + "Hz";// item_temp;
                //火花塞反馈
                label_JinKou.Text = heatDis.resav3.ToString();
                //油泵反馈
                label_ChuKou.Text = heatDis.resav4.ToString();

                //
                //start recond log
                if (bTLogFlag)//文件可写入
                {
                    swHeatLogFile.Write(label_RunTime.Text + " " + label_FengShan.Text + "\r\n");
                }

                //end recond log

            }

        }


    }
    public class FormProcess
    {

    }

}

