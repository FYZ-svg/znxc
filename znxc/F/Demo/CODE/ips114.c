#include "headfile.h"
void ips114_print()
{
//printf("%d  %d  %d  %d  \r\n",adc_value[0],adc_value[1],adc_value[2],adc_value[3]);

////		ips114_showuint16(0,5,Pm);
	  //ips114_showuint16(0,0,P_5);
		ips114_showuint16(0,1,L_S_Adc);
		//ips114_showfloat(0,2,R_S_Adc);
		ips114_showuint16(0,2,R_S_Adc);
		ips114_showuint16(0,3,R_Adc);
		ips114_showuint16(0,4,M_Adc);
	  ips114_showfloat(0,5,Erro,5,2);
//	  ips114_showfloat(0,6,motor_L_pid.SetValue,5,2);
//	  ips114_showfloat(0,7,motor_R_pid.SetValue,5,2);
//		ips114_showuint16(0,0,adc_once(ADC_P05,ADC_12BIT));
//	ips114_showuint16(0,1,adc_once(ADC_P14,ADC_12BIT));
//	ips114_showuint16(0,0,adc_once(ADC_P05,ADC_12BIT));
//	ips114_showuint16(0,1,adc_once(ADC_P14,ADC_12BIT));
//	ips114_showuint16(0,2,adc_once(ADC_P16,ADC_12BIT));
//	ips114_showuint16(0,3,adc_once(ADC_P13,ADC_12BIT));
		//ips114_showuint16(0,3,adc_once(ADC_P00,ADC_12BIT));
//	  ips114_showuint16(0,0,l_speed_add_temp);
//    ips114_showuint16(0,1,r_speed_add_temp);
//		ips114_showuint16(0,4,imu660ra_gyro_x);
//		ips114_showuint16(0,5,imu660ra_gyro_y);
//		ips114_showuint16(0,6,imu660ra_gyro_z);
//		ips114_showuint16(0,5,typee);
//		ips114_showint16(0,6,Sum_Distance);
//		ips114_showint16(0,7,Sum_Angle);
		
//					ips114_showuint16(0,0,dl1a_distance_mm);
					//ips114_showuint16(0,0,Sum_Angle);
					//ips114_showuint16(0,1,Sum_Distance);
					//ips114_showfloat(0,2,Single_Angle_Get(),5,2);,
//					ips114_showfloat(0,5,imu660ra_gyro_transition(imu660ra_gyro_x),5,2);
//					ips114_showfloat(0,6,imu660ra_gyro_x,5,2);
}