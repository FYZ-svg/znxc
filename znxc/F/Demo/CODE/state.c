#include "headfile.h"
char state[30]={Track,Track,Track,Track,Track,Track};
int state_lead=0;
int Flag_Circ=0;
int Flag_Obstacle=0;
int flag_turn=0;
int typee = 0;
int po_time = 0;
int count_i = 0;
float Sum_Distance=0,Sum_Angle=0;
float BZ_sp1 = 120,BZ_err1 = 0.9,BZ_sum1 = 110,BZ_dis1 = 8500;
float BZ_sp2 = 120,BZ_err2 = -0.25,BZ_sum2 = -145,BZ_dis2 = 12000;
float BZ_sp3 = 120,BZ_err3 = 0.03,BZ_sum3 = 0,BZ_dis3 = 10000;
float LC_dis1 = 10,LC_err2 = 0.2,LC_sum2 = 75,LC_sum3 = 1350,LC_dis4 = 55;
float RC_dis1 = 10,RC_err2 = -0.3,RC_sum2 = -95,RC_sum3 = -1390,RC_dis4 = 20	;
void state_detect()
{
	InductorNormal();
	switch(state[state_lead])
	{
		case Track:
			if(Track_Jump())state_lead++;break;
		case Big_Circ_Left:
			if(Big_Circ_Left_Jump(&Flag_Circ))state_lead++;break;
		case Big_Circ_Right:
			if(Big_Circ_Right_Jump(&Flag_Circ))state_lead++;break;
		case Small_Circ_Left:
			if(Small_Circ_Left_Jump(&Flag_Circ))state_lead++;break;
		case Small_Circ_Right:
			if(Small_Circ_Right_Jump(&Flag_Circ))state_lead++;break;
		case Obstacle:
			if(Obstacle_Jump(&Flag_Obstacle))state_lead++;break;	
		default:break;
	}
}
int Track_Jump()
{
	switch(state[state_lead+1])
		{
			case Big_Circ_Left:
			if(L_Adc>90&&R_Adc>55)
				{Flag_Circ=1;return 1;}
					break;
			case Small_Circ_Left:
			if(L_Adc>80&&R_Adc>40&&M_Adc>60)
				{Flag_Circ=1;return 1;}
					break;
			case Big_Circ_Right:
				if(R_Adc>90&&L_Adc>5555)
				{Flag_Circ=1;return 1;}
				break;
			case Small_Circ_Right:
//				if((temp[0]+temp[3]>3000)&&(temp[1]+temp[2]>2000))
			if(R_Adc>80&&L_Adc>40&&M_Adc>60)
				{Flag_Circ=1;return 1;}
					break;
			case Obstacle:
				//ips114_showstr(0,4,"jinle");//½øÀ´ÁË
			if(dl1a_distance_mm<1000&&Flag_Obstacle==0 && Erro<0.3)//
			{
				Flag_Obstacle=1;
			//	ips114_showstr(0,5,"success");
			return 1;}
			break;
			case Stop:
			return 1;break;
			case Garage_In:
				if(P26==0)
				{return 1;}
				break;
			default:break;		
			}
		

		return 0;
}
int Small_Circ_Left_Jump(int* Flag)
{
	switch((*Flag)){
		case 1:
			Sum_Distance+=(motor_L_pid.ActValue+motor_R_pid.ActValue)*isr_time*0.5;
			//P67=1;
			if(Sum_Distance>LC_dis1){
					Sum_Distance=0;
					(*Flag)++;}break;
		case 2:
			Sum_Angle+=Single_Angle_Get();
			P67=0;
			
			if(Sum_Angle>LC_sum2){
					Sum_Angle=0;
					(*Flag)++;}break;
		case 3:
			Sum_Angle+=Single_Angle_Get();
			//P67=1;
			if(Sum_Angle>LC_sum3){
					Sum_Angle=0;
					(*Flag)++;}	break;
		case 4:
			Sum_Distance+=(motor_L_pid.ActValue+motor_R_pid.ActValue)*isr_time*0.5;
			typee	 = 4;
			P67=0;
				if(Sum_Distance>LC_dis4){
					Sum_Distance=0;
					(*Flag)=0;
					return 1;	}
		default:break;}
		return 0;
}
int Small_Circ_Right_Jump(int* Flag)
{
switch((*Flag)){
		case 1:
		//P67=1;
	  Sum_Distance+=(motor_L_pid.ActValue+motor_R_pid.ActValue)*isr_time*0.5;	
		if(Sum_Distance>RC_dis1){
	  Sum_Distance=0;
		(*Flag)++;}break;
		case 2:
			P67=0;
			Sum_Angle+=Single_Angle_Get();
			if(abs(Sum_Angle<RC_sum2)){
					Sum_Angle=0;
					(*Flag)++;
				}	
				break;
		case 3:
	//	P67=1;
			Sum_Angle+=Single_Angle_Get();
			if(Sum_Angle<RC_sum3)
				{
					Sum_Angle=0;
					(*Flag)++;
					
				}break;
		case 4:
			P67=0;
			Sum_Distance+=(motor_L_pid.ActValue+motor_R_pid.ActValue)*isr_time*0.5;
				if(Sum_Distance>RC_dis4)
				{
					Sum_Distance=0;
					(*Flag)=0;
					typee	 = 5;
					return 1;
				}
		default:break;
		}
//	printf("%d\r\n",(*Flag));
	return 0;
}
int Obstacle_Jump(int *Flag)
{
	switch (*Flag)
	{
		
		case 1:
			r_speed_add += r_speed_add_temp;
		  Sum_Angle+=Single_Angle_Get();
			
			if(r_speed_add>BZ_dis1&&abs(Sum_Angle)>BZ_sum1)
				{
					Sum_Angle=0;
					r_speed_add=0;
					(*Flag)++;
				}break;
		case 2:
			Sum_Angle+=Single_Angle_Get();
		  l_speed_add += l_speed_add_temp;
		
			Sum_Distance+=(motor_L_pid.ActValue+motor_R_pid.ActValue)*isr_time*0.5;
				if(/*l_speed_add>BZ_dis2*/Sum_Angle<BZ_sum2)
		  {
				l_speed_add=0;
				r_speed_add=0;
				Sum_Angle=0;
			  (*Flag)++;	
			}break;
			case 3:
			r_speed_add += r_speed_add_temp;
			
				if(r_speed_add>BZ_dis3||((L_Adc>7&&L_S_Adc>3)||(R_Adc>7&&R_S_Adc>3)))
				{   
					r_speed_add=0;
					(*Flag)++;
					
					
				}break;
			case 4:
			{
				if((L_Adc>7&&L_S_Adc>3)||(R_Adc>7&&R_S_Adc>3))
				{	
					(*Flag)=0;
					po_time=1;
					return 1;
				}
			}break;

		}
	return 0;
}

int Big_Circ_Left_Jump(int* Flag)
{
	
		return 0;
}

int Big_Circ_Right_Jump(int* Flag)
{
	
		return 0;
}
//int Ramp_Jump(int *Flag)
//{
//	
//}