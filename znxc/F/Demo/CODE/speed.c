#include "headfile.h"
//float speed_goal=0;
  int duty_L=0,duty_R=0;
	int Zhi=10,Wan=10,Shi=10;
float speed_section(int flag)
{
		if(L_Adc<5&&R_Adc<5&&L_S_Adc<5&&R_S_Adc<5/*&&po_time==1*/)
			return 0;
		else if(flag==normal)
			return Zhi;
		else if(flag==cross)
			return Shi;
		else
			return Wan;
}


void Get_Actual_Speed()
{
	motor_L_pid.ActValue=((float)encoder_L_get()*ActSpeed_Rate*200);
	motor_R_pid.ActValue=((float)encoder_R_get()*ActSpeed_Rate*200);
}


void Speed_Loop()
{     
  float inc_L, inc_R;
	Get_Actual_Speed();
    // 外轮速度限制
    if(motor_L_pid.SetValue > 5*speed_goal)
        {motor_L_pid.SetValue=5*speed_goal;}
    if(motor_R_pid.SetValue > 5*speed_goal)
        motor_R_pid.SetValue =5*speed_goal;
		if(motor_L_pid.SetValue <-5*speed_goal)
       {motor_L_pid.SetValue=-5*speed_goal;}
    if(motor_R_pid.SetValue <-5*speed_goal)
        motor_R_pid.SetValue=-5*speed_goal;

    inc_L = PID_Control_Pos(&motor_L_pid,0);
    inc_R = PID_Control_Pos(&motor_R_pid,0);
    duty_L += inc_L;
    duty_R += inc_R;

    if(duty_L < duty_min)
        duty_L = duty_min;
    if(duty_R< duty_min)
        duty_R = duty_min;

    if(duty_L> duty_max)
        duty_L = duty_max;
    if(duty_R > duty_max)
        duty_R = duty_max;
		duty_set(duty_L,duty_R);
//		printf("%f,%f,%f,%f\r\n",motor_L_pid.ActValue,motor_R_pid.ActValue,motor_L_pid.SetValue,motor_R_pid.SetValue);

//				duty_set(-2000,-2000);

}
void duty_set(int left_duty,int right_duty)
{
		if(left_duty >= 0) //正转
		{
			DIR_L = 1;       
			pwm_duty(PWM_L, left_duty);     
		}
		else         //反转
		{
			DIR_L = 0;           
			pwm_duty(PWM_L, -left_duty);  
		}
		if(right_duty >= 0) //正转
		{
			DIR_R = 1;       
			pwm_duty(PWM_R, right_duty);  			
		}
		else         //反转
		{
			DIR_R = 0;           
			pwm_duty(PWM_R, -right_duty);			
		}
}
