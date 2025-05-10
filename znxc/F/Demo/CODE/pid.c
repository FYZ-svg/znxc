#include "headfile.h"
PID motor_L_pid={0};
PID motor_R_pid={0};
PID speed_ctrl_pid={0};
PID dir_loop_pid={0};
void pid_set(float *arr,PID *pid)
{
	pid->KP=arr[0];
	pid->KI=arr[1];
	pid->KD=arr[2];
}

void InitPID(void)
{
    motor_L_pid.SetValue	=	0.0;
    motor_L_pid.ActValue	=	0.0;
    motor_L_pid.KP			=2.6;   //1.6的时候跑着会突然一卡一卡
	  motor_L_pid.KI			=0;
    motor_L_pid.KD			=120;
    motor_L_pid.ek			=	0.0;
    motor_L_pid.ek_1		=	0.0;
    motor_L_pid.ek_2		=	0.0;
    motor_L_pid.ek_sum		=	0.0;
    motor_L_pid.PIDmax		=	1200;
    motor_L_pid.PIDmin		=	-1200;
    motor_L_pid.PIDout		=	0;

    motor_R_pid.SetValue	=	0.0;
    motor_R_pid.ActValue	=	0.0;
    motor_R_pid.KP			=2.6;    //1.6的时候跑着会突然一卡一卡
            motor_R_pid.KI			=0;
    motor_R_pid.KD			=120; 
    motor_R_pid.ek			=	0.0;
    motor_R_pid.ek_1		=	0.0;
    motor_R_pid.ek_2		=	0.0;
    motor_R_pid.ek_sum		=	0.0;
    motor_R_pid.PIDmax		=	motor_L_pid.PIDmax;
    motor_R_pid.PIDmin		=	motor_L_pid.PIDmin;
//    motor_R_pid.Sum_max		=	motor_L_pid.Sum_max;
//    motor_R_pid.Sum_min		=	motor_L_pid.Sum_min;
    motor_R_pid.PIDout		=	0;

    dir_loop_pid.SetValue = 	0.00;
    dir_loop_pid.ActValue = 	0.00;
    dir_loop_pid.KP 			=  	340;
    dir_loop_pid.KI			  =	  0;
    dir_loop_pid.KD			  =   1;//KP
    dir_loop_pid.ek			  =	  0.00;
    dir_loop_pid.ek_1			=	  0.00;
    dir_loop_pid.ek_2			=	  0.00;
    dir_loop_pid.ek_sum		=	  0.00;
    dir_loop_pid.Sum_max 	= 	1.00;
    dir_loop_pid.Sum_min 	= 	-1.00;
    dir_loop_pid.PIDmax 	= 	30.00;
    dir_loop_pid.PIDmin 	= 	-30.00;
}

float PID_Control_Inc(PID* pid,int flag) 
{
    float inc;

    pid->ek = pid->SetValue - pid->ActValue;
//	if(pid->SetValue!=0&& pid->ek<9)
//	{
//	return 0;
//	}
		inc = pid->KP * (pid->ek - pid->ek_1) + pid->KI * pid->ek+pid->KD*(pid->ek-2*pid->ek_1+pid->ek_2);

    pid->ek_2 = pid->ek_1; 
    pid->ek_1 = pid->ek; 

    if(flag == 1)
    {
        if(inc > pid->PIDmax)
            inc = pid->PIDmax;
        if(inc < pid->PIDmin)
            inc = pid->PIDmin;
    }
    pid->PIDout = inc;
    return inc;
}
float PID_Control_Pos(PID* pid,int flag) 
{
    float Pos ;

    pid->ek = pid->SetValue - pid->ActValue;
    pid->ek_sum += pid->ek;

    Pos = pid->KP * pid->ek + pid->KI * pid->ek_sum + pid->KD * (pid->ek - pid->ek_1);

    pid->ek_2 = pid->ek_1; 
    pid->ek_1 = pid->ek; 

    if(flag == 1)
    {
        if(Pos > pid->PIDmax)
            Pos = pid->PIDmax;
        if(Pos < pid->PIDmin)
            Pos = pid->PIDmin;
    }
    pid->PIDout = Pos;
	
    return Pos;
}
