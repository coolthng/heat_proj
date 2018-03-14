using System;
using System.Collections;


namespace MyQueue
{
    public class QueueInfo
    {
       public int Type { get; set; }
       public object Msg { get; set; }
    }
    public class MyQueue {

        public int key;
        
        private Queue ListQueue = new Queue();
        public int AddQueue(QueueInfo queue)
        {
            QueueInfo queueinfo = new QueueInfo();
            queueinfo.Type = queue.Type;
            queueinfo.Msg = queue.Msg;
            ListQueue.Enqueue(queueinfo);
            return 0;
           
        }
        public QueueInfo DecQune()
        {
            try
            {
                //从队列中取出  
                QueueInfo queueinfo = (QueueInfo)ListQueue.Dequeue();

                return queueinfo;
                //取出的queueinfo就可以用了，里面有你要的东西  
                //。。。。。。  

            }
            catch (Exception ex)
            {
                return null;
                //throw;
            }
            
        }

    }

}
