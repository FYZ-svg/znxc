#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "HCSR04.h"
#include "Delay.h"
#include "Buzzer.h"
#include "MPU6050.h"
#include "LightSensor.h"
#include "PWM.h"
#include <math.h>       // 添加数学库支持
// 定义频率参数
#define FREQ_LOW    800    // 低频800Hz
#define FREQ_HIGH   1600   // 高频1600Hz
#define PWM_DUTY    50     // 占空比50%
uint16_t T;
uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

int main(void)
{
	LightSensor_Init();
	LED_Init();
	OLED_Init();
	PWM_Init();
	JQ_Init();
	HCSR04_Init();
	MPU6050_Init();		//MPU6050初始化
	Buzzer_Init();
	OLED_ShowString(1, 1, "Angle:");
	//OLED_ShowString(1, 13, "cm");
	int flag = 1;
	while (1)
	{
		PWM_SetCompare1(0);
		T = HCSR04_GetValue();
		if(T<50){
			k1();
			LED1_ON();
			//Buzzer_Res(GPIOB,GPIO_Pin_12);
			Delay_ms(100);
			LED1_OFF();
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
			//Buzzer_Set(GPIOB,GPIO_Pin_12);
		}
		
		if(Get_LightSensor() == 1){
			if(flag){
				k2();
				LED2_ON();
				flag = 0;
			}
		}else{
			LED2_OFF();
			flag=1;
		}
		//跌倒检测
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
		if(AX>=0)
		{
			AX=AX*2000/32768;
		}else
		{
			AX=-(-AX+1)*2000/32768;

		}		
		if(AY>=0)
		{
			AY=AY*2000/32768;
		}else
		{
			AY=-(-AY+1)*2000/32768;

		}		
		if(AZ>=0)
		{
			AZ=AZ*2000/32768;
		}else
		{
			AZ=-(-AZ+1)*2000/32768;

		}		
		int Angle = atan2(AZ, AY)*180/3.14f;
		OLED_ShowNum(1, 7, Angle,3);
		if(Angle <=60 || Angle >=130){
			PWM_SetCompare1(90);	
			Delay_ms(150);
			PWM_SetCompare1(50);	
			Delay_ms(150);
		}
	}
}



//int main(void)
//{
//	/*模块初始化*/
//	OLED_Init();		//OLED初始化
//	
//	Buzzer_Init();
//	/*显示ID号*/
//	OLED_ShowString(1, 1, "ID:");		//显示静态字符串
//	ID = MPU6050_GetID();				//获取MPU6050的ID号
//	OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号
//	
//	
//	while (1)
//	{
//		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
//		if(AX>=0)
//		{
//			AX=AX*2000/32768;
//		}else
//		{
//			AX=-(-AX+1)*2000/32768;

//		}		
//		if(AY>=0)
//		{
//			AY=AY*2000/32768;
//		}else
//		{
//			AY=-(-AY+1)*2000/32768;

//		}		
//		if(AZ>=0)
//		{
//			AZ=AZ*2000/32768;
//		}else
//		{
//			AZ=-(-AZ+1)*2000/32768;

//		}		
//		int Angle = atan2(AZ, AY)*180/3.14f;
//		
//		if(Angle <=50 || Angle >=150){
//			Buzzer_Res(GPIOB,GPIO_Pin_12);
//			Delay_ms(100);
//			
//		}
//	    else{
//			Buzzer_Set(GPIOB,GPIO_Pin_12);
//		}
////		OLED_ShowSignedNum(2, 1, AX, 5);					//OLED显示数据
//		OLED_ShowSignedNum(3, 1, AY, 5);
//		OLED_ShowSignedNum(4, 1, AZ, 5);
//		OLED_ShowSignedNum(2, 8, Angle, 5);
////		OLED_ShowSignedNum(3, 8, GY, 5);
////		OLED_ShowSignedNum(4, 8, GZ, 5);
//	}
//}






