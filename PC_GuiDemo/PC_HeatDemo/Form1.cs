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
    public partial class Form1 : Form
    {
       
        private void button6_Click(object sender, EventArgs e)
        {
           
        }
       
      
        public Form1()
        {
            InitializeComponent();
            FormInit();
            //thCommThread.Start();
            myGuiUpdata = new GuiUpdata(DoUpdate);//定义委托更新新界面
            string[] strarg = SerialPort.GetPortNames();//获得串口描述值
            ComPort.Items.Clear();//清除指令
            ComPort.Text = "";//清除文本
            foreach (var item in strarg)
            {
                ComPort.Items.Add(item);
                //groupBox1.Enabled = true;
                ComPort.SelectedIndex = 0;
                comboBox2.SelectedIndex = 3;
                comboBox3.SelectedIndex = 3;
                comboBox4.SelectedIndex = 0;
                comboBox5.SelectedIndex = 0;

            }
        }
        
       
        int stateSetCount = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {

#if false
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
            label_Version.Text = "000";
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
                    //if(thComm.ApartmentState== ApartmentState.)
                    // ThreadState Unstarted   System.Threading.ThreadState
                    Thread.Sleep(500);
                    if (thComm.ThreadState == ThreadState.Suspended)
                    {
                        thComm.Resume();
                    }
                    else
                    {
                        thComm.Start();

                    }
                   //更新版本信息
                       QueueInfo     myQueueInfoTx = new QueueInfo();
                        myQueueInfoTx.Type = 'v';
                        myQueue.AddQueue(myQueueInfoTx); //添加获取版本号事件
                    //更新版本信息
                }
                else
                {
                    sp.Close();
                    button2.Text = "打开串口";
                    //groupBox3.Enabled = false;
                    //label_Tip.Text = Convert.ToString(sp.PortName) + "已关闭！";
                  thComm.Suspend();
                }
            }
            catch (Exception er)
            {
                MessageBox.Show("Error:" + er.Message, "Error");
                return;
            } 
        }
       
        private void button4_Click(object sender, EventArgs e)
        {
            textBox_Rx.Text = "";
        }

 
       //开机 
       private void bT_Start_Click(object sender, EventArgs e)
       {
            // keyState = 1;
            // stateSendFlag = true;
            MyQueue.QueueInfo myQuInfo = new MyQueue.QueueInfo();
            myQuInfo.Type = 'k';
            myQuInfo.Msg = 1;
            myQueue.AddQueue(myQuInfo);
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

            MyQueue.QueueInfo myQuInfo = new MyQueue.QueueInfo();
            myQuInfo.Type = 'l';
            myQuInfo.Msg = tem1;
            myQueue.AddQueue(myQuInfo);

        }  
        //关机
       private void bT_Stop_Click(object sender, EventArgs e)
       {
            MyQueue.QueueInfo myQuInfo = new MyQueue.QueueInfo();
            myQuInfo.Type = 'k';
            myQuInfo.Msg = 3;
            myQueue.AddQueue(myQuInfo);
       }
        private void bT_Wind_Click(object sender, EventArgs e)
        {
            MyQueue.QueueInfo myQuInfo = new MyQueue.QueueInfo();
            myQuInfo.Type = 'k';
            myQuInfo.Msg = 2;
            myQueue.AddQueue(myQuInfo);
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

               // stateSetParmFlag = true;
            }
            catch {
                MessageBox.Show("检查参数不能为空");
                //stateSetParmFlag = false;
            }


        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void bT_Debug_Click(object sender, EventArgs e)
        {
            keyState = 8;
            
        }
        
        private void button3_Click(object sender, EventArgs e)
        {
            
            Thread thParm = new Thread(ThreadSendParm);
            thParm.Start(); //启动线程
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
