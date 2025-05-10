/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2023,杰智易科技
 * All rights reserved.
 * 以下所有内容版权均属杰智易所有，未经允许不得用于商业用途，
 * @file       		main
 * @company	   		南京杰智易科技有限公司
 * @author     		杰智易科技
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @date       		2023-11-30
 ********************************************************************************************************************/
#include "headfile.h"
#include "speed.h"
//float cs_motor_l_pid[3]={50,10.5,15};//float cs_motor_r_pid[3]={50,10,0};
//float cs_motor_r_pid[3]={50,10.5,15};
//float cs_dir_loop_pid[3]={160,0,300};
int adc_value[8]={0};

//float sum=0;
float speed=0;

int16 P05_C;
int main()
{
	board_init();
	
	
	pit_timer_ms(TIM_1,5); 
//	pit_timer_ms(TIM_4,5);
	NVIC_SetPriority(	TIMER1_IRQn,2);
	NVIC_SetPriority(	UART4_IRQn,3);	
	InitPID();
		delay_ms(500);
		EnableGlobalIRQ(); // 开启总中断 
		motor_L_pid.SetValue=0;
		motor_R_pid.SetValue=0;
		 // int xL,xR,yT,yB;
			//int errorX,errorY;
	while(1)
	{
			
//			P05_C = adc_once(ADC_P05,ADC_12BIT);
//			if(P05_C > 2600) duty(L_C,R_C);
//			else duty(-L_C,-R_C);
//		
		
		/*  errorX = xL - xR;
		  errorY = yT - yB;
		  if(errorX > 0){
				errorX *= 10;
				duty(L,R+errorX);
			}
			else(errorx < 0){
				errorX *= -10;
				duty(L+errorX,R);
			}
			if(errorY > 0){
				errorY *= 10;
				duty(L+errorY,R);
			}
			else if(errorY < 0){
				errorY *= -10;
				duty(L,errorY+R);
			}
			
			duty_set(9999,9999);      */
			
		
		//P52=0;
			//P67=1;
		//printf("%d\r\n",dl1a_distance_mm);
		//printf("%f\r\n",Single_Angle_Get());
	//	printf("%d\r     ",encoder_L_get());
	//	printf("     %d\r\n",encoder_R_get());
	}
}

