
/********************************************************************************************************************************************************************
                                                      
													  ////////////////////////中值滤波算法////////////////////////
													  
1.方法：程序判断滤波存在一个潜在的隐患，在连续两个以上采样周期受到强干扰后，系统可能会不稳定。因此，程序判断滤波只能在基本没有干扰的场合下运行，他只能滤除极个别的偶发的毛刺性干扰。在环境恶化，干扰频繁的情况下，可以考虑用中值滤波算法来处理，该算法的描述是：
	连续进行奇数次采样，然后将采样得到的数据样本进行排序，取中间的数据作为有效采样值。例
如连续采样5次，得到5个采样数据样本(例如17、17、29、27、16)，然后进行排序(16/17/17/27/29)，最后去中间的(即第三个)采样数据样本作为有效采样值输出。由于受到干扰的采样值偏离有效采样值，排序后必然出于两端的位置，只要受到干扰的采样数据样本个数小于总采样数据样本数目的一半，就可以确保中值采样数据样本的有效性。
	采用中值滤波算法必须满足以下条件：
	(1)被采样的物理量本身在连续N次子采样周期期间是基本稳定的，其变化量小于系统精度要求，
可以忽略不计
    (2)每次干扰的最长持续时间已知，相邻两次干扰的间隔时间虽然不定，但远大于一次干扰的持续时间。这样，可以使得连续采样的数据样本中被干扰的数据样本数目不会超过总样本数目N的一半，确保中值样本的有效性。
		
2.例程：假设某数据采集系统所采集的对象为缓慢变化的物理量，每次启动采样任务需连续采集1h，获取60个数据样本，每个数据样本间隔时间为1min(即主采样周期为60s).采样工作环境存在干扰，但每次干扰的持续时间不会超过80ms，相邻两次干扰的间隔时间为5s~5min之间变化不定。按主采样间隔每隔1min工作一次，将子采样周期确定为40ms,每次工作连续采样5次(工作200ms).这样，在5次连续采样的样本中，可能出现的受干扰样本不会超过2个(干扰最长持续时间为80ms，故最多影响2次采样)，能够确保中值的有效性，将这个中值作为有效采样数据样本，用这种采集方式得到60个有效数据样本后就完成了数据采集任务。
		NOTE1：采样中值滤波算法时需要对采样数据进行排序，此处我们的排序的目的只是为了获得
中值，故只要排除前3名即可，故可以提前结束排序工作
		NOTE2：数据采集可以由人工按钮操作来启动，也可以由时钟系统启动或者遥控指令启动。假
设该数据采集系统采用了嵌入式实时操作系统μC/OS-II,数据采集任务微单次执行模式的任务，由人工按钮操作的键盘监控任务创建，任务结束后自行删除。
		(1)调用函数
		(2)变量说明：
		(3)常量说明：
		(4)入口：
		(5)出口：
********************************************************************************************************************************************************************/
typedef unsigned char INT8U;
typedef unsigned int INT16U;

INT16U Samp[60];      //定义保存有效样本数据的数组

void TaskSamp(void * pdata)
{
	INT8U i, j, k, n;
	INT16U R[6];
	
	for (n=0; n<60; n++)
	{
		//每个有效样本需要进行5次连续采样
		//每个样本都经过一次中值滤波算法获得
		for (i=1; i<=5; i++)
		{
			R[i] = SampAdc();
			OSTimeDly(2);
		}
		
		//进行排序将中间值作为有效采样值
		for (i=1; i<=3; i++)
		{
			k = i;
			
			//排序，排除前3名即可，获得中值，作为有效采样值
			for (j=i+1; j<=5; j++)
			{
				if (R[j] < R[k]) 
				{
					k = j;
				}
				
				if (k != i)
				{
					R[0] = R[i];
					R[i] = R[k];
					R[k] = R[0];
				}
			}
			
			Samp[n] = R[3];
			OSTimeDly(2990);	//再延时2990*20ms = 2.99*20s = 29.9*2 = 59.8s,使主采样周期为1s
		}
		
		OSTaskCreate(TaskSend, (void *)0, &TaskSendStk[TaskStk-1], 6);   //采集任务完成，故此时应该创建发送任务
		
		OSTaskDel(OS_PRIO_SELF);   //数据发送之后，采样任务删除自己
	}	
}




