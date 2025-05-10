#include "headfile.h" 
int16 ad_valu1[4];
float L_Adc,R_Adc,L_S_Adc,R_S_Adc,M_Adc;//���ֵ
float Erro=0;
int16 g_ValueOfAD[4] = {0};       //��ȡ�ĵ��ֵ
int16 g_ValueOfADFilter[5] = {0};
int P_5[10];
void Read_ADC(void)
{
		int16 i,j;
	int16 ad_valu[5][10],ValueOfADOld[5],ValueOfADNew[5];

	//*******************************���ֵ�˲�����*******************************//    
   for(i=0;i<10;i++)
     {
				 P_5[i] = adc_once(ADC_P05,ADC_12BIT);   
         ad_valu[0][i]=adc_once(ADC_P05,ADC_12BIT);      //you  heng  	
         ad_valu[1][i]=adc_once(ADC_P14,ADC_12BIT);      //you   shu
				 ad_valu[2][i]=adc_once(ADC_P16,ADC_12BIT);  	   //you    xie
         ad_valu[3][i]=adc_once(ADC_P13,ADC_12BIT);      //you    zhong
				 ad_valu[4][i]=adc_once(ADC_P00,ADC_12BIT);      //zhong  heng
     }
		 /*===========================�����˲�=================================*/
for(i = 0;i<5;i++)
{
	int min=4000,max=0,sum = 0;
	int times = 10;
	for(j = 0;j<times;j++)
	{

		if(ad_valu[i][j]>max)
			max = ad_valu[i][j];
		if(min > ad_valu[i][j])
			min = ad_valu[i][j];
		  sum+=ad_valu[i][j];
		  ad_valu1[i] = (sum-max-min)/(times-2);
	}
}
	 for(i=0;i<5;i++)            //����ֵ��,��λ������
	 {
	 	g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);
		//�ɼ��ݶ�ƽ����ÿ�βɼ����仯40
		ValueOfADOld[i] = g_ValueOfADFilter[i];
		ValueOfADNew[i] = g_ValueOfAD[i];
		if(ValueOfADNew[i]>=ValueOfADOld[i])
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);
		else
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-50?(ValueOfADOld[i]-50):ValueOfADNew[i]);
	 } 
}

float Left,LeftXie,RightXie,Right,Mid;
void InductorNormal (void)
{
	  Read_ADC();
   Left =  (float)(g_ValueOfADFilter[0]) / (3200.0 - 0.0) * 100.0;		// ��й�һ��   ���ֵ��ȥ��Сֵ�ǵø�
		LeftXie =  (float)(g_ValueOfADFilter[1]) / (3200.0 - 0.0) * 100.0;
		RightXie = (float)(g_ValueOfADFilter[2]) / (3200.0 - 0.0) * 100.0;		
		Right = (float)(g_ValueOfADFilter[3]) / (3200.0 - 0.0) * 100.0;		
    Mid = (float)(g_ValueOfADFilter[4]) / (3200.0 - 0.0) * 100.0;
	

		L_Adc = Left;       //��������ֵ
		L_S_Adc = LeftXie;   //�����������ֵ
		R_S_Adc = RightXie;  //�����������ֵ
		R_Adc = Right;	    //�ҵ������ֵ	
		M_Adc = Mid;	
    
	  if(L_Adc<=1) L_Adc = 1;
		else if(L_Adc > 100) L_Adc=100;
    if(L_S_Adc<=1) L_S_Adc = 1;
		else if(L_S_Adc > 100) L_S_Adc=100;
		if(R_S_Adc<=1) R_S_Adc = 1;
		else if(R_S_Adc > 100) R_S_Adc=100;
		if(R_Adc<=1) R_Adc = 1;
		else if(R_Adc > 100) R_Adc=100;
		if(M_Adc<=1) M_Adc = 1;
		else if(M_Adc> 100) M_Adc=100;
	}



float error_get(int flag)
{
	float L,R,Err;
	float Mid_M = 50;
	float M = Mid_M/M_Adc;
	if(flag==normal||flag==cross)
	{
		Err=(float)(L_Adc-R_Adc)/(float)(L_Adc+R_Adc)/**M*/;
		//P67=1;
	}
	else
	{
		L=sqrt((float)L_Adc*(float)L_Adc+(float)L_S_Adc*(float)L_S_Adc);
		R=sqrt((float)R_S_Adc*(float)R_S_Adc+(float)R_Adc*(float)R_Adc);
		Err=(L-R)/(L+R)*M*0.5; //M*0.5  
		//P67=0;
	}
		Err=Err>1.2?1.2:Err;
		Err=Err<-1.2?-1.2:Err;
		Erro = Err;
	  return Err;
}
