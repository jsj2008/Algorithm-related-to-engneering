#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"

typedef struct
{
 float Sv;//�û��趨ֵ
 float Pv;
 
 float Kp;
 float T;  //PID��������--��������
 float Ti;
 float Td; 
	
	
	
 float Ek;  //����ƫ��
 float Ek_1;//�ϴ�ƫ��
 float SEk; //��ʷƫ��֮��
	
		float Iout;
	float Pout;
	float Dout;
	
 float OUT0;

 float OUT;
	
	
 u16 C10ms;
	
 u16 pwmcycle;//pwm����
 
 u32 times;
	
}PID;

extern PID pid; //���PID�㷨����Ҫ������
void PID_Calc(void); //pid����

void PID_Calc_overshoot(void);  //pid����-���崦��

#endif