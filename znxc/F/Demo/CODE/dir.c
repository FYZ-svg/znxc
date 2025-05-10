#include "headfile.h"
	float temp=0;
	float KD2=0;
	float cha_speed=0;
	float K_p1 = 0,K_p2 = 0,Z_Kp1=270,Z_Kp2 = 250,W_Kp1 = 480,W_Kp2=470,W_KD =265,Z_KD=255;
float  Kp_Select(float error)
{
	return fabs(error)*50;
}
void Dir_Loop(float error,float speed_goal,int flag)
{		

	if(flag==normal||flag==cross)
	{
		{
		K_p1 = Z_Kp1;  
		K_p2 = Z_Kp2;

    dir_loop_pid.ek =  error;//一般数值在直道和弯道的时候数值都是小于0.4,当ta到了直角的时候,这个数值就会突增,增加到1以上
    temp=fabs(dir_loop_pid.ek)*dir_loop_pid.ek* K_p2+	//这个式子是直角转向的关键	//KP2一般是小于KP1
				K_p1 *dir_loop_pid.ek
	//	-Single_Angle_Get()
			  +dir_loop_pid.KD * Z_KD*(dir_loop_pid.ek - dir_loop_pid.ek_1);
				dir_loop_pid.ek_1=dir_loop_pid.ek;
				cha_speed=temp;
		motor_L_pid.SetValue=speed_goal-cha_speed;
		motor_R_pid.SetValue=speed_goal+cha_speed;
	}
	}
	else
	{
		{
		K_p1 = W_Kp1; 
		K_p2 = W_Kp2;
    dir_loop_pid.ek =  error;//一般数值在直道和弯道的时候数值都是小于0.4,当ta到了直角的时候,这个数值就会突增,增加到1以上
    temp=fabs(dir_loop_pid.ek)*dir_loop_pid.ek* K_p2+	//这个式子是直角转向的关键	//KP2一般是小于KP1
				K_p1 *dir_loop_pid.ek
				+Single_Angle_Get()*0
				+dir_loop_pid.KD * W_KD*(dir_loop_pid.ek - dir_loop_pid.ek_1);
	dir_loop_pid.ek_1=dir_loop_pid.ek;
				cha_speed=temp;
		motor_L_pid.SetValue=speed_goal-cha_speed*1.5;
		motor_R_pid.SetValue=speed_goal+cha_speed*1.5;
	}
	}

}


