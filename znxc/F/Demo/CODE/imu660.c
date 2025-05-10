#include "headfile.h"
//float Single_Angle=0;
float Single_Angle_Get(void)
{
//	imu660ra_get_gyro();
	//return -(imu660ra_gyro_z * isr_time/ 65.6);
		//static float Sum_XXX = 0;
	//Sum_XXX = () /** isr_time/ 65.6)*/;
	return imu660ra_gyro_z/65.6/45;//Sum_XXX
}
//float Single_Angle_Get_X(void)
//{
////	imu660ra_get_gyro();
//	static float Sum_XXX = 0;
//	Sum_XXX += (imu660ra_gyro_x * isr_time/ 65.6);
//	return Sum_XXX;
//}