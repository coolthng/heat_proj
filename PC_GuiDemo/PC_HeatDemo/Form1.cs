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


namespace PC_HeatDemo
{
    public partial class Form1 : Form
    {
        SerialPort sp = new SerialPort();//新建串口对象
        byte[] myStateByteArry = new byte[500];
        byte[] mySetByteArry=new byte[50];
        
        byte keyState = 0;//按键值状态
        byte levelState = 0;//正常工作模式档位
        bool stateSendFlag = false;//状态下发完成标志
        bool stateSetParmFlag = false;//设置参数下发完成标志
        bool stateUpParmFlag = false;//更新参数标志
        bool stateBackParmFlag = false;//更新参数回传标志
        Int16[] SetStateParmArry = new Int16[10];
        Int16[] SetUpParmArry = new Int16[10];

        StreamWriter swHeatLogFile;
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
        __HeatDis heatDis = new __HeatDis();
        private void button6_Click(object sender, EventArgs e)
        {
            byte[] test = new byte[20];
            test[0] =(byte) 'a';
            test[2] = 0xf;
            test[3] = 0x3;
            test[4] = 0x4;
            //packet = (TGSGetDataRequest)BytesToStruct((byte[])buf, Marshal.SizeOf(packet), packet.GetType());
            // heatDis = (__HeatDis)BytesToStruct((byte[])test, Marshal.SizeOf(heatDis), heatDis.GetType());
            thComm.Start();
        }
        private void DoUpdate()///希望被执行的函数（被委托）        
        {
            //01 04 09 22 34 00 22 00 33 00 44 71cB
            if (UpdataArr[0] == 0x61)
            {
                heatDis = (__HeatDis)BytesToStruct((byte[])UpdataArr, Marshal.SizeOf(heatDis), heatDis.GetType());
                string item_temp;
                //状态机状态
                switch(heatDis.StateMachine)
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
                label_DianYuan.Text = Convert.ToString(heatDis.PowerVolatge/100)+'.'+ Convert.ToString((heatDis.PowerVolatge % 100)/10)+Convert.ToString(heatDis.PowerVolatge % 10)+'V';  // item_temp;
                //转速
                label_FengShan.Text = heatDis.CurrentPrm.ToString() + "RPM";// item_temp;
                //壳体温度
                label_KeTi.Text = heatDis.KeTiTemp.ToString() + '℃';
                //火花塞调节占空比
                label_HuoSai_Adj_Pre.Text = "未显示";
                //风扇调节占空比
                label_FengShan_Adj_Pre.Text = "未显示";

                //活塞占空比
                label_HuoSai_Pre.Text =Convert.ToString(heatDis.HsVoltage/10)+'.'+Convert.ToString(heatDis.HsVoltage % 10) +'V' ;
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
            else if (UpdataArr[0] == 'p')
            {
                stateBackParmFlag = true;
            }
            if ((myStateByteArry[0] == 0x01) && (myStateByteArry[1] == 0x04))
            {
                keyState = 0;
                levelState = 0;
                stateSendFlag = false;
            }
        }

        public delegate void GuiUpdata();
        GuiUpdata myGuiUpdata;
        Thread thComm;
        const int RX_BUFFER_LENGTH = 36;
        int RxArrPtrHeader = 0;
        int RxArrPtrTail = 0;
        byte[] RxArr = new byte[RX_BUFFER_LENGTH];
        byte[] UpdataArr = new byte[RX_BUFFER_LENGTH];
        int ttttttt_count = 0;
        private void ThreadComm()
        {
            while (true)
            {
                int SDataLength = this.sp.BytesToRead;
                while(SDataLength > RX_BUFFER_LENGTH)
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
        }
       
        public Form1()
        {
            InitializeComponent();
            thComm= new Thread(ThreadComm);
            //thCommThread.Start();
            myGuiUpdata= new GuiUpdata(DoUpdate);//定义委托更新新界面

            string[] strarg = SerialPort.GetPortNames();//获得串口描述值
            ComPort.Items.Clear();//清除指令
            ComPort.Text = "";//清除文本
            foreach (var item in strarg)
            {
                ComPort.Items.Add(item);
                //groupBox1.Enabled = true;
                ComPort.SelectedIndex = 0;
                comboBox2.SelectedIndex = 6;
                comboBox3.SelectedIndex = 3;
                comboBox4.SelectedIndex = 0;
                comboBox5.SelectedIndex = 0;

            }
        }
        
       
        int stateSetCount = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {

#if true
            if (stateSendFlag)//有按键使能
            {
                mySetByteArry[0] = (byte)'k';// 0x01;
                mySetByteArry[1] = levelState;// 0x06;
                mySetByteArry[2] = keyState;// 0x00;
                mySetByteArry[3] = 0x0d;// 0x00;
                mySetByteArry[4] = 0x0a;// levelState;
                if (sp.IsOpen)
                {
                    sp.Write(mySetByteArry, 0, 5);
                }
                stateSendFlag = false;
                stateSetCount++;
                if (stateSetCount > 5)
                {
                    lB_SetInfo.Text = "操作功能下发失败";
                    stateSendFlag = false;
                }

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
                if (sp.IsOpen)
                {
                    sp.Write(mySetByteArry, 0, 20);

                }
                stateSetParmFlag = false;
                stateSetCount++;
                if (stateSetCount > 5)
                {
                    lB_SetInfo.Text = "操作功能下发失败";
                    stateSetParmFlag = false;
                }

            } else if (stateUpParmFlag)//配置初始化参数
            {
                mySetByteArry[0] = (byte)'p';
                int txLength = SetUpParmArry[0] % 16;
                mySetByteArry[1] = (byte)txLength;
                for (int i = 0; i <=txLength; i++)
                {
                    mySetByteArry[2 + 2 * i] = (byte)(SetUpParmArry[i] % 256);
                    mySetByteArry[2 + 2 * i + 1] = (byte)(SetUpParmArry[i] / 256);
                }
                mySetByteArry[4+2*txLength] = 0x0d;// (byte)(res % 256);
                mySetByteArry[5+2*txLength] = 0x0a;// (byte)(res / 256);
                if (sp.IsOpen)
                {
                    sp.Write(mySetByteArry, 0, 4+2*txLength);

                }
                stateUpParmFlag = false;
                stateSetCount++;
                if (stateSetCount > 5)
                {
                    lB_SetInfo.Text = "操作功能下发失败";
                    stateUpParmFlag = false;
                }
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
#endif
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            String str1 = ComPort.Text  ;//串口号             
            String str2 = comboBox2.Text;//波特率             
            String str3 = comboBox3.Text;//数据位             
            String str4 = comboBox4.Text;//停止位             
            String str5 = comboBox5.Text;//校验位              
            Int32 int2 = Convert.ToInt32(str2);//将字符串转为整型             
            Int32 int3 = Convert.ToInt32(str3);//将字符串转为整型
            try
            {
                if (button2.Text == "打开串口")
                {
                    if (str1 == null)
                    {
                        MessageBox.Show("请先选择串口！", "Error");
                        return;
                    }
                    sp.Close();
                    sp = new SerialPort();
                    sp.PortName = ComPort.Text;//串口编号                     
                    sp.BaudRate = int2;//波特率
                    sp.NewLine = "\n";
                    //sp.Encoding=Encoder.Equals
                    switch (str4)//停止位                     
                    {
                        case "1":
                            sp.StopBits = StopBits.One;
                            break;
                        case "1.5":
                            sp.StopBits = StopBits.OnePointFive;
                            break;
                        case "2":
                            sp.StopBits = StopBits.Two;
                            break;
                        default:
                            MessageBox.Show("Error：参数不正确", "Error");
                            break;
                    }
                    switch (str5) //校验位                    
                    {
                        case "None":
                            sp.Parity = Parity.None;
                            break;
                        case "Odd":
                            sp.Parity = Parity.Odd;
                            break;
                        case "Even":
                            sp.Parity = Parity.Even;
                            break;
                        default:
                            MessageBox.Show("Error：参数不正确", "Error");
                            break;
                    }
                    sp.DataBits = int3;//数据位 
                    sp.Parity = Parity.None;//设置串口属性                  
                    //sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);
                    sp.Open();//打开串口 
                    button2.Text = "关闭串口";
                   // label_Tip.Text = Convert.ToString(sp.PortName) + "已开启！";
                   // groupBox3.Enabled = true;

                }
                else
                {
                    sp.Close();
                    button2.Text = "打开串口";
                    //groupBox3.Enabled = false;
                    //label_Tip.Text = Convert.ToString(sp.PortName) + "已关闭！";
                }
            }

            catch (Exception er)
            {
                MessageBox.Show("Error:" + er.Message, "Error");
                return;
            } 
        }
       
        //自动接收数据
        void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            //int SDateTemp = this.sp.ReadByte();
            //int SDataLength = this.sp.BytesToRead;
        }
       
        private void button4_Click(object sender, EventArgs e)
        {
            textBox_Rx.Text = "";
        }

 
       //开机 
       private void bT_Start_Click(object sender, EventArgs e)
       {
            keyState = 1;
            stateSendFlag = true;
           
       }
        //挡位
       private void textBox1_TextChanged(object sender, EventArgs e)
       {
           int tem1 = Convert.ToInt16(textBox1.Text);
           if (tem1 > 5)
           {
               tem1 = 5;
           }
           else if (tem1 < 1)
           {
               tem1 = 1;
           }
            levelState = (byte)tem1;
            stateSendFlag = true;
       }  
        //关机
       private void bT_Stop_Click(object sender, EventArgs e)
       {
            keyState = 3;
            stateSendFlag = true;
            
       }

        private void bT_SetParm_Click(object sender, EventArgs e)
        {
            try
            {

                SetStateParmArry[0] = Convert.ToInt16(tB_SetPower.Text);
                SetStateParmArry[1] = Convert.ToInt16(tB_SetRunLevel.Text);
                SetStateParmArry[2] = Convert.ToInt16(tB_SetKeTi.Text);
                SetStateParmArry[3] = Convert.ToInt16(tB_SetJinKou.Text);
                SetStateParmArry[4] = Convert.ToInt16(tB_SetChuKou.Text);
                SetStateParmArry[5] = Convert.ToInt16(tB_SetHuoSaiFb.Text);
                SetStateParmArry[6] = Convert.ToInt16(tB_SetYouBengFb.Text);
                SetStateParmArry[7] = Convert.ToInt16(tB_SetFengShanPrm.Text);

                stateSetParmFlag = true;
            }
            catch {
                MessageBox.Show("检查参数不能为空");
                stateSetParmFlag = false;
            }


        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void bT_Debug_Click(object sender, EventArgs e)
        {
            keyState = 8;
            stateSendFlag = true;
        }
        
        private void button3_Click(object sender, EventArgs e)
        {
            
            Thread thParm = new Thread(ThreadSendParm);
            thParm.Start(); //启动线程
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
            Array arr ;
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

                Console.WriteLine("总长度:"+parmLength+"已写parmTxPostion:" + parmTxPostion);
                parmTxPostion += i;
                stateUpParmFlag = true;//使能发送
                stateBackParmFlag = false;//等待回传成功
            }
            Console.WriteLine("写入数据完成");
        }
        bool bTLogFlag = false;
        private void bTLog_Click(object sender, EventArgs e)
        {
            bTLogFlag = !bTLogFlag;
            if (bTLogFlag)
            {
                bTLog.Text = "保存文件";
                 string str= Environment.GetFolderPath(Environment.SpecialFolder.Desktop)+ "\\HeatLogFile" + DateTime.Now.ToString("yyyy-MM-dd-HHmmss") + ".txt";
                swHeatLogFile = new StreamWriter(str);
               // swHeatLogFile.Close();
               // swHeatLogFile = File.AppendText(str) ;
                textBox_Other.Text = "文件创建在"+str;
                String wstr = "start recoad\r\n";
                swHeatLogFile.Write(wstr);
                
            }
            else {
                bTLog.Text = "创建文件";
                swHeatLogFile.Write("end recond\r\n");
                swHeatLogFile.Close();
            }
        }

    }
}
