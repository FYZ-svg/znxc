#ifndef SYS_H
#define SYS_H
/***************������***************/
//������������
#define Encoder_L   CTIM0_P34
#define Encoder_R   CTIM3_P04
//���巽������
#define DIR_Encoder_L     P35
#define DIR_Encoder_R     P53

/***************���***************/
//������������
#define PWM_L   PWMA_CH4P_P66
#define PWM_R   PWMA_CH2P_P62
//���巽������
#define DIR_L   P64
#define DIR_R   P60

/****************�ж�ʱ��***********/

#define isr_time 0.005
#define ActSpeed_Rate 0.0088
#endif