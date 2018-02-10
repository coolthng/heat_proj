﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace PC_HeatDemo
{
    public partial class Form1 : Form
    {

        SerialPort sp = new SerialPort();//新建串口对象
        byte[] myStateByteArry = new byte[500];
        byte[] mySetByteArry=new byte[50];
        
        int commflag_count = 0;

        byte keyState = 0;//按键值状态
        byte levelState = 0;//正常工作模式档位
        bool stateSendFlag = false;//状态下发完成标志
        bool stateSetParmFlag = false;//设置参数下发完成标志
        Int16[] SetStateParmArry = new Int16[10];

        delegate void getstring(String mystring);//定义委托        
        getstring getmystring;///定义委托变量
        byte[] updata_temp = new byte[40];
        private void DoUpdate()///希望被执行的函数（被委托）        
        {
            commflag_count = 5;
          //01 04 09 22 34 00 22 00 33 00 44 71cB
            if ((UpdataArr[0] == 0x61) && (UpdataArr[1] == 0x00))
            {
                string item_temp;
                for (int i = 0; i < 24; i++)
                {
                    updata_temp[i] = UpdataArr[i+2];

                }
                //int temptest2 = updata_temp[0] / 16;
                //label_MachineState.Text = temptest2.ToString();
                //updata_temp[3] = 0xf0;
                //状态机状态
                switch (updata_temp[0] / 16)//高四位  状态机
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
                //int temptest1=updata_temp[0] % 16;
                //label_ErrorState.Text = temptest1.ToString();
                switch (updata_temp[0] % 16)//低4位  故障状态
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
                switch (updata_temp[1]/16)//高四位档位  
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
                switch (updata_temp[1] % 16)//低四位  保留  
                {
                    case 1:
                        break;
                    default:
                        break;
                }
                //运行时间
                int int_run_time = updata_temp[2];// updata_temp[12] * 256 + updata_temp[13];
                item_temp = int_run_time.ToString() + 's';
                label_RunTime.Text = item_temp;
                //电压


                int int_dianya = BitConverter.ToUInt16(updata_temp, 14);//myStateByteArry[5] * 256 + myStateByteArry[6];
                item_temp = Convert.ToString(int_dianya / 10) + '.' + Convert.ToString(int_dianya % 10) + 'V';
                label_DianYuan.Text = item_temp;
                //转速
                int int_zhuansu = BitConverter.ToUInt16(updata_temp, 4);// updata_temp[5] * 256 + updata_temp[4];
                item_temp =(int_zhuansu).ToString()+ "RPM"; 
                label_FengShan.Text = item_temp;
                //壳体温度
                int int_keti = BitConverter.ToUInt16(updata_temp, 6);// updata_temp[6] * 256 + updata_temp[7] - 200;//50基值  保证传递的是正数
                item_temp=int_keti.ToString()+ '℃'; 
                label_KeTi.Text = item_temp;
                //火花塞调节占空比
                int int_huosai_adj = BitConverter.ToUInt16(updata_temp, 8);// updata_temp[8] * 256 + updata_temp[9];
                item_temp =int_huosai_adj.ToString()+ '%'; 
                label_HuoSai_Adj_Pre.Text = item_temp;
                //风扇调节占空比
                int int_fengshan_adj = BitConverter.ToUInt16(updata_temp, 10);// updata_temp[10] * 256 + updata_temp[11];
                item_temp = int_fengshan_adj.ToString() + '%'; 
                label_FengShan_Adj_Pre.Text = item_temp;

              
                
                
                //活塞占空比
                //int int_huosai = updata_temp[14] * 256 + updata_temp[15];
                //item_temp =int_huosai.ToString()+ '%'; 
                //label_HuoSai_Pre.Text = item_temp;
                //风扇占空比
                //int int_fengshan = updata_temp[16] * 256 + updata_temp[17];
                //item_temp = int_fengshan.ToString() + '%';
                //label_FengShan_Pre.Text = item_temp;

                //油泵频率
                int int_youbeng = updata_temp[19];
                item_temp = Convert.ToString(int_youbeng / 10) + '.' + Convert.ToString(int_youbeng % 10) + "Hz";
                label_YouBeng_Pre.Text = item_temp;
                //工作模式
                //switch (updata_temp[19])
                //{
                //    case 1:
                //        label_work_mode.Text = "1KW模式";
                //        break;
                //    case 2:
                //        label_work_mode.Text = "2KW模式";
                //        break;
                //    case 3:
                //        label_work_mode.Text = "3KW模式";
                //        break;
                //    case 4:
                //        label_work_mode.Text = "4KW模式";
                //        break;
                //    case 5:
                //        label_work_mode.Text = "5KW模式";
                //        break;
                //    case 0x80:
                //        label_work_mode.Text = "测试模式";
                //        break;
                //    default:
                //        label_work_mode.Text = "模式错误";
                //        break;

                //}
                int int_huosai_FB = BitConverter.ToUInt16(updata_temp, 20);// updata_temp[20] * 256 + updata_temp[21];
                label_JinKou.Text = int_huosai_FB.ToString();

                int int_youbeng_FB = BitConverter.ToUInt16(updata_temp, 22);// updata_temp[22] * 256 + updata_temp[23];
                label_ChuKou.Text = int_youbeng_FB.ToString();

                //
            }

            if ((myStateByteArry[0] == 0x01) && (myStateByteArry[1] == 0x04))
            {
                keyState = 0;
                levelState = 0;
                stateSendFlag = false;
            }

#if false
                    {
                        case 'C': item_temp+='℃'; label_ChuKou.Text = item_temp;//ok
                            break;
                        case 'D':
                            int int_dianya = Convert.ToInt16(item_temp);
                            item_temp = Convert.ToString(int_dianya / 10) + '.' + Convert.ToString(int_dianya%10)+'V';
                            label_DianYuan.Text = item_temp;
                            break;
                        case 'F': item_temp += "Prm"; label_FengShan.Text = item_temp;
                            break;
                        case 'J':  item_temp+='℃';label_JinKou.Text = item_temp;
                            break;
                        case 'K':  item_temp+='℃';label_KeTi.Text = item_temp;
                            break;
                        case 'M': label_Compile_date.Text = item_temp;
                            break;
                        case 'N': label_Compile_time.Text = item_temp;
                            break;
                        case 'O': textBox_Other.Text = item_temp;
                            break;
                        case 'P': item_temp += '%'; label_HuoSai_Adj_Pre.Text = item_temp;
                            break;
                        case 'Q': item_temp += '%'; label_FengShan_Adj_Pre.Text= item_temp;
                            break;
                        case 'S':

                            switch (Convert.ToChar(item[1]))
                            {
                                case '1': label_MachineState.Text = "PowerOff";
                                    break;
                                case '2': label_MachineState.Text = "正常状态";
                                    break;
                                case '3': label_MachineState.Text = "通风状态";
                                    break;
                                case '4': label_MachineState.Text = "点火状态";
                                    break;
                                case '5': label_MachineState.Text = "空状态";
                                    break;
                                case '6': label_MachineState.Text = "关机状态";
                                    break;
                                case '7': label_MachineState.Text = "二次点火";
                                    break;
                                case '8': label_MachineState.Text = "故障状态";
                                    break;
                                case '9': label_MachineState.Text = "自动泵油";
                                    break;
                                default:
                                    break;
                            }
                        
                       // label_MachineState.Text = "nihaod的";//item_temp;
                            break;
                        case 'U': label_work_mode.Text = item_temp;
                            break;
                        case 'V': int int_Verision = Convert.ToInt16(item_temp);
                            item_temp ='V'+ Convert.ToString(int_Verision / 100) + '.' + Convert.ToString(int_Verision % 100);
                            label_Version.Text = item_temp;
                            break;
                        case 'T': item_temp+='s'; label_RunTime.Text = item_temp;
                            break;
                        case 'W': item_temp += '℃'; label_DianWeiQi.Text = item_temp;
                            break;
                        case 'X': item_temp+='%'; label_HuoSai_Pre.Text = item_temp;
                            break;
                        case 'Y': item_temp+='%';label_FengShan_Pre.Text = item_temp;
                            break;
                        case 'Z':
                            int int_youbeng = Convert.ToInt16(item_temp);
                            item_temp = Convert.ToString(int_youbeng / 10) + '.' + Convert.ToString(int_youbeng % 10) + "Hz";
                            label_YouBeng_Pre.Text = item_temp;
                            break;

                        default:
                            break;
                    }
#endif



            }

        public Form1()
        {
            InitializeComponent();

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

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void label18_Click(object sender, EventArgs e)
        {

        }
        int count;
        int stateSetCount = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {

            commflag_count--;
            if (commflag_count > 0)
            {
                count++;
                ////ovalShape1.Enabled = true;
                //if (count % 2 == 0)
                //  //  ovalShape1.FillColor = Color.Green;
                //else
                //    ovalShape1.FillColor = Color.Black;


            }
            else {
                // ovalShape1.FillColor = Color.Black;
#if false
                label_ChuKou.Text = "";
                label_DianWeiQi.Text = "";
                label_DianYuan.Text = "";
                label_FengShan.Text = "";
                label_FengShan_Pre.Text = "";
                label_HuoSai_Pre.Text = "";
                label_JinKou.Text = "";
                label_KeTi.Text = "";
                label_MachineState.Text = "";
                label_RunTime.Text = "";
                label_YouBeng_Pre.Text = "";
                label_Compile_date.Text = "";
                label_Compile_time.Text = "";
                label_FengShan_Adj_Pre.Text = "";
                label_HuoSai_Adj_Pre.Text = "";
                label_Version.Text = "";
                label_work_mode.Text = "";
#endif      
                
            }
#if true
            if (stateSendFlag)
            {


                mySetByteArry[0] = (byte)'k';// 0x01;
                mySetByteArry[1] = levelState;// 0x06;
                mySetByteArry[2] = keyState;// 0x00;
                mySetByteArry[3] = 0x0d;// 0x00;
                mySetByteArry[4] = 0x0a;// levelState;
                //mySetByteArry[5] = keyState;
                //int res = Crc16(mySetByteArry, 6);
                //mySetByteArry[6] = (byte)(res % 256);
                //mySetByteArry[7] = (byte)(res / 256);
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
            else if (stateSetParmFlag)
            {
                mySetByteArry[0] = (byte)'s';
                mySetByteArry[1] = 0x10;
                //mySetByteArry[2] = 0x00;
                //mySetByteArry[3] = 0x00;
                //mySetByteArry[4] = 0x00;
                //mySetByteArry[5] = 0x0a;
                //mySetByteArry[6] = 0x14;


                //SetStateParmArry[0] = Convert.ToInt16(tB_SetPower.Text);
                //SetStateParmArry[1] = Convert.ToInt16(tB_SetRunLevel.Text);
                //SetStateParmArry[2] = Convert.ToInt16(tB_SetKeTi.Text);
                //SetStateParmArry[3] = Convert.ToInt16(tB_SetJinKou.Text);
                //SetStateParmArry[4] = Convert.ToInt16(tB_SetChuKou.Text);
                //SetStateParmArry[5] = Convert.ToInt16(tB_SetHuoSaiFb.Text);
                //SetStateParmArry[6] = Convert.ToInt16(tB_SetYouBengFb.Text);
                //SetStateParmArry[7] = Convert.ToInt16(tB_SetFengShanPrm.Text);
                for (int i = 0; i < 8; i++)
                {
                    mySetByteArry[2 + 2 * i] = (byte)(SetStateParmArry[i]%256);
                    mySetByteArry[2 + 2 * i+1] = (byte)(SetStateParmArry[i] / 256);
                }
                //int res = Crc16(mySetByteArry, 27);
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

            }
            else
            {
                stateSetCount = 0;
                mySetByteArry[0] = 0x61;//'a';//0x01;
                mySetByteArry[1] = 0x0d;
                mySetByteArry[2] = 0x0a;
                //mySetByteArry[3] = 0x00;
                //mySetByteArry[4] = 0x00;
                //mySetByteArry[5] = 0x0c;

                //int res = Crc16(mySetByteArry, 6);
                //mySetByteArry[6] = (byte)(res % 256);
                //mySetByteArry[7] = (byte)(res / 256);
                if (sp.IsOpen)
                {
                    sp.Write(mySetByteArry, 0, 3);
                }

            }
#endif
            /*
            ovalShape1.FillColor = Color.Red;
            ovalShape1.Enabled = true;
            */
            
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
                    sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);
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
        const int RX_BUFFER_LENGTH = 36;
        int RxArrPtr = 0;
        int RxArrPtrHeader = 0;
        int RxArrPtrTail = 0;
        byte[] RxArr = new byte[RX_BUFFER_LENGTH];
        byte[] UpdataArr = new byte[RX_BUFFER_LENGTH];
        //自动接收数据
        void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            //int SDateTemp = this.sp.ReadByte();
            int SDataLength = this.sp.BytesToRead;
            //读取串口中一个字节的数据  
            this.Invoke(
             //在拥有此控件的基础窗口句柄的线程上执行委托Invoke(Delegate)  
             //即在textBox_ReceiveDate控件的父窗口form中执行委托.  
             new MethodInvoker(
             /*表示一个委托，该委托可执行托管代码中声明为 void 且不接受任何参数的任何方法。 在对控件的 Invoke    方法进行调用时或需要一个简单委托又不想自己定义时可以使用该委托。*/
             delegate
             {
                 /*匿名方法,<a href="http://lib.csdn.net/base/csharp" class='replace_word' title="C#知识库" target='_blank' style='color:#df3434; font-weight:bold;'>C#</a>2.0的新功能，这是一种允许程序员将一段完整代码区块当成参数传递的程序代码编写技术，通过此种方法可    以直接使用委托来设计事件响应程序以下就是你要在主线程上实现的功能但是有一点要注意，这里不适宜处理过多的方法，因为C#消息机制是消息流水线响应机制，如果这里在主线程上处理语句的时间过长会导致主UI线程阻塞，停止响应或响应不顺畅,这时你的主form界面会延迟或卡死      */
                 //    this.textBox_Other.AppendText(SDateTemp.ToString());//输出到主窗口文本控件  
                 //this.textBox_Other.Text += " ";
                 //byte[] ArrTmp = new byte[SDataLength];
                 //sp.Read(ArrTmp,0,SDataLength);
                 if ((RxArrPtrTail + SDataLength) > RX_BUFFER_LENGTH)
                 {
                     sp.Read(RxArr, RxArrPtrTail, RX_BUFFER_LENGTH - RxArrPtrTail);
                     sp.Read(RxArr, 0, SDataLength + RxArrPtrTail - RX_BUFFER_LENGTH);
                 }
                 else
                 {
                     sp.Read(RxArr,RxArrPtrTail,SDataLength);
                 }
                 RxArrPtrTail += SDataLength;
                
                     //RxArr[RxArrPtrTail++] = ArrTmp[j];//(byte)(SDateTemp);
                     if (RxArrPtrTail >= RX_BUFFER_LENGTH)
                     {
                         RxArrPtrTail = RxArrPtrTail%RX_BUFFER_LENGTH;//循环指针
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
                         DoUpdate();
                     }
                 }
             
             )
             );

     
        }
        void shell_set(int fengshan_f,int fengshan_p,int huosai_f,int huosai_p,int youbeng_f,int youbeng_p)
        { 
            int comend=2;
            string str = comend.ToString() + fengshan_f.ToString() + fengshan_p.ToString()
                + huosai_f.ToString() + huosai_p.ToString() + youbeng_f.ToString() + youbeng_p.ToString() + @"\\";
            sp.WriteLine(str);
        
        }
        void shell_get()
        {
            int comend = 3;
            string str = comend.ToString() + @"\\";
            sp.WriteLine(str);
        }

        private void label22_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            textBox_Rx.Text = "";
        }

       private static readonly byte[] aucCRCHi = {
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
            0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40
        };
       private static readonly byte[] aucCRCLo = {
            0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
            0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
            0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
            0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
            0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
            0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
            0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
            0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
            0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
            0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
            0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
            0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
            0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
            0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
            0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
            0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
            0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
            0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
            0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
            0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
            0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
            0x41, 0x81, 0x80, 0x40
        };

       /// <summary>
       /// CRC效验
       /// </summary>
       /// <param name="pucFrame">效验数据</param>
       /// <param name="usLen">数据长度</param>
       /// <returns>效验结果</returns>
       public static int Crc16(byte[] pucFrame, int usLen)
       {
           int i = 0;
           byte ucCRCHi = 0xFF;
           byte ucCRCLo = 0xFF;
           UInt16 iIndex = 0x0000;

           while (usLen-- > 0)
           {
               iIndex = (UInt16)(ucCRCLo ^ pucFrame[i++]);
               ucCRCLo = (byte)(ucCRCHi ^ aucCRCHi[iIndex]);
               ucCRCHi = aucCRCLo[iIndex];
           }
           
           return (ucCRCHi << 8 | ucCRCLo);
       }
        
       private void bT_Start_Click(object sender, EventArgs e)
       {
            keyState = 1;
            stateSendFlag = true;
       }

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
    }
}