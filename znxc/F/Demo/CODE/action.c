#include "headfile.h"
float speed_goal=300;
float error=0;
float error_rate=0;
int count=0;
int Cross_Flag=0;
void Track_Action()
{
	if(Cross_Flag==1)
	{
		Sum_Distance+=(motor_L_pid.ActValue+motor_R_pid.ActValue)*isr_time*0.5;
		if(Sum_Distance<200)
		{
			flag_turn=cross;
			//P67=1;
		}
		else	
		{
		Cross_Flag=0;
			P67=0;
		}			
	}
	else
	{
		if(L_S_Adc<10&&R_S_Adc<10&&L_Adc>25&&R_Adc>25)
	{
		flag_turn=normal;
	//	P67=0;		
	}
	else if(L_S_Adc>20&&R_S_Adc>20&&L_Adc>20&&R_Adc>20&&M_Adc>40)//在十字的时候,电感值情况:中间和两边的水平电感数值变化小,中间两颗竖直电感的变化大
	{
	 Sum_Distance=0;
		Cross_Flag = 1;
		
	//	P67=0;		
	}
	else
	{
	//	P67=0;
		flag_turn=right_angle;	
		
	}			
	}
	speed_goal=speed_section(flag_turn);	
	error=error_get(flag_turn);
	Dir_Loop(error,speed_goal,flag_turn);
}

	
void Circ_Left_Action()
{
	speed_goal=speed_section(flag_turn);
	error=error_get(flag_turn);
	switch (Flag_Circ)
	{	
		case 1:
		speed_goal= 200;
		Dir_Loop(error,speed_goal,flag_turn);
		break;
		case 2:
			error=LC_err2;
	  speed_goal= 200;
		//speed_goal=speed_section(flag_turn);
			Dir_Loop(error,	speed_goal,flag_turn);
		break;
		case 3:
			speed_goal= 200;
		//speed_goal=speed_section(flag_turn);
		Dir_Loop(error,speed_goal,flag_turn);
		break;
		case 4:
		motor_L_pid.SetValue=speed_goal;
		motor_R_pid.SetValue=speed_goal;
		break;
	}
}
void Circ_Right_Action()
{
  	speed_goal=speed_section(flag_turn);
	error=error_get(flag_turn);
	switch (Flag_Circ)
	{	
		case 1:
		speed_goal= 200;
		Dir_Loop(error,speed_goal,flag_turn);
		break;
		case 2:
			error=RC_err2;
	  speed_goal= 200;
		//speed_goal=speed_section(flag_turn);
			Dir_Loop(error,	speed_goal,flag_turn);
		break;
		case 3:
			speed_goal= 200;
		//speed_goal=speed_section(flag_turn);
		Dir_Loop(error,speed_goal,flag_turn);
		break;
		case 4:
			//speed_goal= 200;
		motor_L_pid.SetValue=speed_goal;
		motor_R_pid.SetValue=speed_goal;
		break;
	}
}
void Obstacle_Action(void)
{
	switch(Flag_Obstacle)
	{
		case 1:
			error=BZ_err1;  
			speed_goal=BZ_sp1;
			//P67=1;
		//	speed_goal=speed_section(error,state[state_lead]);
			Dir_Loop(error,speed_goal,flag_turn);break;
		case 2:
			P67=0;
			error=BZ_err2;  
			speed_goal=BZ_sp2;
	//speed_goal=speed_section(error,state[state_lead]);
			Dir_Loop(error,speed_goal,flag_turn);break;
		case 3:
			error=BZ_err3;  
			speed_goal=BZ_sp3;
		Dir_Loop(error,speed_goal,flag_turn);break;
		case 4:
			error = 0;
		speed_goal=BZ_sp3;      
		Dir_Loop(error,speed_goal,flag_turn);break;
	}
}

void Stop_Action()
{
		motor_L_pid.SetValue=0;
		motor_R_pid.SetValue=0;
	  motor_L_pid.KP=2.5;
		motor_R_pid.KP=2.5;
		motor_L_pid.KI=0;
		motor_R_pid.KI=0;
	  motor_L_pid.KD=80;
		motor_R_pid.KD=80;
	
}
void beep_on()
{

	{
			//P67=1;
	}

}
void beep_off()
{
	P67=0;
	count=0;
}