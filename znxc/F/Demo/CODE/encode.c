#include "headfile.h"
int l_speed_add=0;
int r_speed_add=0;
int l_speed_add_temp=0;
int r_speed_add_temp=0;
int encoder_L_get(void)
{
	int temp;
	   if(DIR_Encoder_L == 1)
	   {
			temp = ctimer_count_read(Encoder_L);
	   }	   // �����ֵ�ǰ�ٶ�
	   else
	   {
			temp = 0-ctimer_count_read(Encoder_L);
	   }
		 l_speed_add_temp=temp;
		 ctimer_count_clean(Encoder_L); // ����������
//		 l_speed_add = l_speed_add_temp;
//		 ips114_showuint16(0,0,temp);
//	printf("%d\r\n",temp);
	return temp;
}


//�ɼ��ұ�����ֵ


int encoder_R_get(void)
{
int temp;
if(DIR_Encoder_R == 0)
{
    temp = ctimer_count_read(Encoder_R) ; }// �����ֵ�ǰ�ٶ�
else
{
	temp = 0-ctimer_count_read(Encoder_R);
}
//printf("%d\r\n",temp);
	r_speed_add_temp=temp;
	ctimer_count_clean(Encoder_R); // ����������
//	r_speed_add = r_speed_add_temp;
	return temp;
}











