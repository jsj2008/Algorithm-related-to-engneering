#include "pid.h"

PID pid; //���PID�㷨����Ҫ������


void PID_Calc()  //pid����
{
 float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
 if(pid.C10ms<(pid.T))  //��������δ��
 {
    return ;
 }

 pid.Ek=pid.Sv-pid.Pv;   //�õ���ǰ��ƫ��ֵ
 pid.Pout=pid.Kp*pid.Ek;      //�������
 
 pid.SEk+=pid.Ek;        //��ʷƫ���ܺ�
 
 DelEk=pid.Ek-pid.Ek_1;  //�������ƫ��֮��
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //�������

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //΢�����
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //���εļ�����
 pid.Ek_1=pid.Ek;  //����ƫ��
 
 pid.C10ms=0;
}


void PID_Calc_overshoot()  //pid����-���崦���
{
 float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
  float SvSave;
 if(pid.C10ms<(pid.T))  //��������δ����pid.C10ms��ÿ1ms�Լ�һ�����Կ���������PID�ļ���ʱ�����Ա�
 {
    return ;
 }

//
//�²�:�ó���������������Ӧ��û����һ��PID���㣬��pid.times++,�������¶�<65���ʱ��pid.times�����0������ʱ�����й��崦��
//�ڶ�ʱ��3���ж�ѭ���ж�pid.times����++������ÿ1s��pid.times�Լ�һ���Թ���Ĵ�����Ҫ��ͨ��ʱ��������
//�������¶����ߵ�65���Ժ���ʼ���崦��
//��ν��й��崦����?����������<120��һ�·ֽ׶ν���PID������
//0-60s����65-72��
//60-100s����72-96�ȵ�PID����
//100-200s����96-102�ȵ�PID����
//200-300s����102-108�ȵ�PID����
//300-400s����108-114�ȵ�PID����
//��ÿ6����¶��ݶȣ���Ҫ100s��ʱ�������д�����������ֹ����


//�ж�Sv
  SvSave=pid.Sv;     //Sv = 120
 
 if(pid.Pv<(SvSave*0.55))       //�¶ȴﵽ������ʼ 66��
 {
    pid.times=0;
 }
 
  if(pid.times<=60)//3               
 {
	//120*0.6 = 72
	pid.Sv=SvSave*0.6;
 }
 
 if((pid.times>60)&&(pid.times<=100))//3���� 96��  100s
 {
  pid.Sv=SvSave*0.8;
 }
 
 if((pid.times>100)&&(pid.times<=200))//2���� 102�� 100s
 {
  pid.Sv=SvSave*0.85;
 }
 
 if((pid.times>200)&&(pid.times<=300))//2���� 108�� 100s
 {
  pid.Sv=SvSave*0.9;
 }
 
  if((pid.times>300)&&(pid.times<=400))//2���� 114�� 100s
 {
  pid.Sv=SvSave*0.95;
 }
 
 if(pid.times>400)
 {
  pid.Sv=SvSave;
 }
//////////////////////////////////
/*
                         n
OUT=(Kp*Ek) + (Kp*(T/Ti)��Ek) + (Kp*(TD/T)(EK-Ek-1)) +OUT0           
                        k=0
Kp:�㷨�������
Ek:��ǰ����ƫ��
T: �������ڣ����������ڣ�ÿ��Tʱ��ν���һ��PID����
Ti:����ʱ��
TD:΢��ʱ�䳣��
Ek_1:�ϴ�ƫ��ֵ
OUT0:��һ��������Ŀ���Ǳ�֤OUT�����Ϊ0�����OUT=0����PIDʧ��

��:
Ki= Kp*(T/Ti);
KD=(Kp*(TD/T)
��:
 
                   n
OUT=(Kp*Ek) + (Ki��Ek) + (KD(EK-Ek-1)) +OUT0           
                   k=0

*/

	//����������ʽ�ĵ�һ��:(Kp*Ek)
 pid.Ek=pid.Sv-pid.Pv;   //�õ���ǰ��ƫ��ֵ
 pid.Pout=pid.Kp*pid.Ek;      //�������
    //����������ʽ�ĵڶ���: 
 pid.SEk+=pid.Ek;        //��ʷƫ���ܺ�
 
 DelEk=pid.Ek-pid.Ek_1;  //�������ƫ��֮��
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //�������

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //΢�����
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //���εļ�����
 pid.Ek_1=pid.Ek;  //����ƫ��
 
 pid.C10ms=0;
 pid.Sv=SvSave;
}











