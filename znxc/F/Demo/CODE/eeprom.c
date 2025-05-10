#include "headfile.h"
#include "eeprom.h"
float pitch_angle = 10;
float yaw_angle = 10;
float roll_angle = 10;
float AngleTh = 30;
uint16 PodaoFlag = 0; 
uint16 BarrierFlag = 0;
uint16 RingFlag = 0;
uint8 datapage = 0;//0����ʾID��Ӧ�ĺ�����1���޸�arrow��Ӧ�Ĳ���
float mul = 1;
int Pm=0;
int f_lag = 0;

//**************************************************************************//
//**************************************************************************//
//���尴������
#define KEY1    P74  
#define KEY2    P77     
#define KEY3    P52        
#define KEY4    P61     
#define KEY5    P46
#define Switch_Pin_1       P76
#define Switch_Pin_2       P75

typedef enum
{
	key1_flag = 1,
	key2_flag =	2,
	key3_flag =	3,
	key4_flag =	4,
	key5_flag =	5
};


//***************�����궨��****(������Щ�������޸ĺ궨��Ϊ��Ӧ��GPIO�⺯������)***********
#define KEY_INT(key_x)           gpio_mode(key_x,GPO_PP)//����ΪGPO_PP:������� 
#define SWITCH_INT(switch_x)     gpio_mode(switch_x,GPO_PP)//����ΪGPO_PP:�������
#define READ_GPIO(Pin_X)         Pin_X
#define TiaoCan_DelayMs(M_S)     delay_ms(M_S)   //��ʱ����

//����״̬����
int key1_status = 1,key2_status = 1,key3_status = 1, key4_status = 1,key5_status = 1;
//��һ�ο���״̬����
int key1_last_status, key2_last_status, key3_last_status, key4_last_status,key5_last_status;
//���ر�־λ
//uint8 key1_flag=0,key2_flag=0,key3_flag=0, key4_flag=0,key5_flag=0;
unsigned char Switch1=0,Switch2=0;

void Switch_Init()
{
	//���뿪�س�ʼ��  �������޸ģ������޸ģ�
	SWITCH_INT(Switch_Pin_1) ;
	SWITCH_INT(Switch_Pin_2) ;
}

int Temp_Swtich=0;
void Strategy_Slect()
{
	
	//��ȡ���뿪��״̬
      if(!READ_GPIO(Switch_Pin_1))
      {
				Switch1=1;
				//Pm=1;
				UI();
      }
      if(!READ_GPIO (Switch_Pin_2))
      {
				Switch2=1;
				ips114_print();
      }
}

void Key_Init()
{
	//������ʼ�� �������޸ģ������޸ģ�
  KEY_INT(KEY1);
	KEY_INT(KEY2);
	KEY_INT(KEY3);
	KEY_INT(KEY4);
	KEY_INT(KEY5);
}
uint8  Key_Scan_Deal ()													
{
    //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
    //���水��״̬
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
	
		//��ȡ��ǰ����ֵ
		key1_status = KEY1;
    key2_status = KEY2;
    key3_status = KEY3;
    key4_status = KEY4;
    key5_status = KEY5;
	
		delay_ms(10);
		//��⵽��������֮��   ���ſ���λ��־λ
		if(key1_status && !key1_last_status)  return key1_flag;
		else if(key2_status && !key2_last_status)  return key2_flag;
		else if	(key3_status && !key3_last_status)  return key3_flag;
		else if(key4_status && !key4_last_status)  return key4_flag;
		else if(key5_status && !key5_last_status)  return key5_flag;
		else return 0;
		
		
}




//********************************************************//
//********************************************************//
int8 arrow = 0;//���������ڵ��У���ʼ���ڵ�һ��
void DisplayCursor()
{
	ips114_showstr(0,arrow,">");//�ڲ���ǰ����ʾ>
}

void UI()
{
	if(!datapage)
	{
		UI_Content();//��ʾID��Ӧ�ĺ���
		UI_ContentKey();//����ID
	}
	else
	{
		UI_Datapage();  //����ҳ
    UI_DatapageKey();   //���ݰ�������
	}
	DisplayCursor();//�����ʾ
}

////������ʾ�����˵�
uint8 pagenum = 0;//��ʾpage����Ŀ
//int8 arrow = 0;//���������ڵ��У���ʼ���ڵ�һ��
int8 PageClear = 0;
void UI_Content(void)
{
	
	// ����pagenumȥ��ʾ�˵�
	
	switch(pagenum)
    {
    case 0://��ʾ���˵�
			//ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
		{
			DisplayMain();
		}break;
		case 1://���PID
		{
			MotorPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		case 2://ת��PID
		{
			TrackPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		case 3://�ٶ��趨
		{
			SpeedPara();
		if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
		PageClear = pagenum;
		}break;
		case 4://����
		{
			BiZhang();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		case 5://��
		{
			L_CirPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		case 6://�һ�
		{
			R_CirPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		case 7:
		{
			DisplayBarrierPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		case 8:
		{
			DisplayPodaoPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		default:
		{
			DisplayMain();	
		}break;
	}
}
//������һ���˵�
uint8 pagenumup(void)
{
	switch(pagenum)
	{
		case 0://��һ���˵�
			return 0;
		case 1://��һ���˵�
			return 0;
		case 2://��һ���˵�
			return 0;
		case 3://��һ���˵�
			return 0;
		case 4://��һ���˵�
			return 0;
		case 5://��һ���˵�
			return 0;
		//�������һ�£����ڶ����˵�ID=6ʱ�����ص���һ����ID=5
		//
		case 6://�ڶ����˵�
			return 5;
		case 7://�ڶ����˵�
			return 5;
		case 8://�ڶ����˵�
			return 5;
		
		default:
			return 0;
	}
}

//������һ���˵�
//��Ҫ�жϵ�ǰID��֪������һҳ����Щʲô����
//����Ҫ�жϹ�������У�֪��Ҫ������һ����������
uint8 pagenumdown(void)
{
	switch(pagenum)
	{
		case 0://��������
		{
			switch(arrow)//�ж��ڵڼ���
			{
				case 0://�ڵ�0��---->��Ӧ�Ų���LeftMotorPara
				{
					return 1;//����LeftMotorPara��ID
				}
				case 1: return 2;//��ӦRightMotorPara
				case 2: return 3;//��ӦServoPara
				case 3: return 4;//��ӦIMUPara
				case 4: return 5;//��ӦElemnetPara
			}
		}break;
		case 1://��LeftMotorPara����
		{
			return 1;//���ص�ǰ����
			//�ڸý�����û����һ������ֱ��break;
		}break;
		case 2: return 2;break;//��RightMotorPara����
		case 3: return 3;break;//ServoPara����
		case 4: return 4;break;//IMUPara����
		case 5://ElemnetPara����
		{
			switch(arrow)
			{
				case 0: return 6;//��ӦRing
				case 1: return 7;//��ӦBarrier
				case 2: return 8;//��Ӧpodao
				default : return 5;//���ص�ǰ����
			}
		}break;
		
		default:break;
	}
}


//���հ���ֵ��������arrow
void UI_ContentKey()
{
	int8 key = 0 ;
			key = Key_Scan_Deal();//��ȡ����ֵ
    if(key == key1_flag)
			{//���� up
        ips114_showstr(0,arrow," ");  //��Ҫ����ǰ�еĹ�����أ���Ȼ��arrow����֮�󣬳���������ʾ���
				arrow--;
				//�����ƶ���arrow��С
			}
//		ips114_showuint16(40,7,key);
    if(key == key2_flag){//���� down
        ips114_showstr(0,arrow," ");  arrow++;                                                                                          
		}//��upͬ��
	
/**********�����key1,key2�����������ƶ�����***********/
/**********�����key3,key4������������Ӧ�ĸ�/�Ӳ˵���***********/

    if(key == key3_flag){//������ҳ��  enter
    ips114_clear(WHITE);//��Ҫ����ǰ��ʾ������������ʾ�ӽ���    
		pagenum = pagenumdown();//�����ӽ��� 
		arrow = 0;//�ӵ�0�п�ʼ
	}
    if(key == key4_flag){//������һ��ҳ�� back
    ips114_clear(WHITE);    
		pagenum = pagenumup();
		arrow = 0;
	}
	if(key == key5_flag)//���Ĳ���
	{
		ips114_clear(WHITE);    
		datapage = 1;    
		mul = 1;
	}
	
	//��arrow�޷���OLED��ֻ����ʾ8�У�������Ҫ���� 0 <= arrow <8
	if(arrow < 0)
		arrow = 7;//��˼�ǣ�������up�������һֱ�����ƶ������˶�����һ�м�arrow=0��
				  //�ٰ���upʱ�����ֱ���������һ�У�arrow = 7
	else if(arrow >7)
		arrow = 0;

}

//��ʾ����Ĳ���
int arrow_Clear = 0;
void UI_Datapage()
{
	uint8 x = 8,y = 80;
	//ips114_clear(WHITE);
	switch(pagenum)
	{
		case 0:break;//��IDΪ0�Ľ���û����Ҫ�޸ĵĲ���
		case 1://��ӦLeftMotorPara
		{
			switch(arrow)
			{
				case 0://kp
				{
					
					ips114_showstr(x,0,"L_Mkp"); 
					ips114_showfloat(y,0,motor_L_pid.KP,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 1://ki
				{
					ips114_showstr(x,0,"L_Mki"); 
					ips114_showfloat(y,0,motor_R_pid.KP,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 2://kd
				{
					ips114_showstr(x,0,"D_Mkd"); 
					ips114_showfloat(y,0,motor_L_pid.KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"R_Mkp"); 
					ips114_showfloat(y,0,motor_R_pid.KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����
					
					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 4://TargetSpeed
				{
					ips114_showstr(x,0,"R_Mki"); 
					ips114_showfloat(y,0,motor_L_pid.KI,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����
					
					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"R_Mkd"); 
					ips114_showfloat(y,0,motor_R_pid.KI,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����		

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			if(PageClear != pagenum)
				ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
			PageClear = pagenum;
		}break;
		
		case 2://��ӦRightMotorPara
		{
			switch(arrow)
			{
				case 0://kp
				{
					ips114_showstr(x,0,"Z_Kp1"); 
					ips114_showfloat(y,0,Z_Kp1,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 1://ki
				{
					ips114_showstr(x,0,"Z_Kp2"); 
					ips114_showfloat(y,0,Z_Kp2,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 2://kd
				{
					ips114_showstr(x,0,"W_Kp1"); 
					ips114_showfloat(y,0,W_Kp1,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 3: 
				{
					ips114_showstr(x,0,"W_Kp2"); 
					ips114_showfloat(y,0,W_Kp2,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}
				break;
				case 4://TargetSpeed
				{
					ips114_showstr(x,0,"Z_KD"); 
					ips114_showfloat(y,0,Z_KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"W_KD"); 
					ips114_showfloat(y,0,W_KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			ips114_showfloat(40,7,mul,8,4);//��ʾ����
		}break;
		case 3://��ӦServoPara
		{
			switch(arrow)
			{
				case 0://kp
				{
					ips114_showstr(x,0,"Zhi"); 
					ips114_showfloat(y,0,Zhi,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 1://ki
				{
					ips114_showstr(x,0,"Wan"); 
					ips114_showfloat(y,0,Wan,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 2://kd
				{
					ips114_showstr(x,0,"Shi"); 
					ips114_showfloat(y,0,Shi,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"BZ_sp3"); 
					ips114_showfloat(y,0,BZ_sp3,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"BZ_err3"); 
					ips114_showfloat(y,0,BZ_err3,3,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"BZ_sum3"); 
					ips114_showfloat(y,0,BZ_sum3,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 6:
				{
					ips114_showstr(x,0,"BZ_dis3"); 
					ips114_showfloat(y,0,BZ_dis3,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			ips114_showfloat(40,7,mul,8,4);//��ʾ����
		}break;			
		case 4:
		{
			switch(arrow)
			{
				case 0:
				{
					ips114_showstr(x,0,"BZ_sp1"); 
					ips114_showfloat(y,0,BZ_sp1,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;	
				case 1:
				{
					ips114_showstr(x,0,"BZ_err1"); 
					ips114_showfloat(y,0,BZ_err1,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 2:
				{
					ips114_showstr(x,0,"BZ_sum1"); 
					ips114_showfloat(y,0,BZ_sum1,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"BZ_dis1"); 
					ips114_showfloat(y,0,BZ_dis1,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"BZ_sp2"); 
					ips114_showfloat(y,0,BZ_sp2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"BZ_err2"); 
					ips114_showfloat(y,0,BZ_err2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 6:
				{
					ips114_showstr(x,0,"BZ_sum2"); 
					ips114_showfloat(y,0,BZ_sum2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 7:
				{
					ips114_showstr(x,0,"BZ_dis2"); 
					ips114_showfloat(y,0,BZ_dis2,5,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			ips114_showfloat(40,7,mul,8,4);//��ʾ����
		}break;
		case 5:
		{
			switch(arrow)
			{
				case 0:
				{
					ips114_showstr(x,0,"LC_dis1"); 
					ips114_showfloat(y,0,LC_dis1,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;	
				case 1:
				{
					ips114_showstr(x,0,"LC_err2"); 
					ips114_showfloat(y,0,LC_err2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 2:
				{
					ips114_showstr(x,0,"LC_sum2"); 
					ips114_showfloat(y,0,LC_sum2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"LC_sum3"); 
					ips114_showfloat(y,0,LC_sum3,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"LC_dis4"); 
					ips114_showfloat(y,0,LC_dis4,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				default:break;
			}
				ips114_showfloat(40,7,mul,8,4);//��ʾ����
		}break;
		case 6:
		{
			switch(arrow)
			{
				case 0:
				{
					ips114_showstr(x,0,"RC_dis1"); 
					ips114_showfloat(y,0,RC_dis1,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;	
				case 1:
				{
					ips114_showstr(x,0,"RC_err2"); 
					ips114_showfloat(y,0,RC_err2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 2:
				{
					ips114_showstr(x,0,"RC_sum2"); 
					ips114_showfloat(y,0,RC_sum2,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"RC_sum3"); 
					ips114_showfloat(y,0,RC_sum3,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"RC_dis4"); 
					ips114_showfloat(y,0,RC_dis4,4,3);
					ips114_showfloat(40,7,mul,8,4);//��ʾ����

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//����ʾ��һ���˵�ʱ����Ҫ����
					arrow_Clear = arrow;
				}break;
				default:break;
			}
				ips114_showfloat(40,7,mul,8,4);//��ʾ����
		}break;
		case 7://��ӦBarrierPara
		{
			//������ӱ���
			
		}break;
		case 8://��ӦpodaoPara
		{
			//������ӱ���
		}break;
		
		default:break;
	}
}

////���Ĳ���
void UI_DatapageKey()
{
	uint8 key = Key_Scan_Deal();
	if(key == key1_flag)//   +
	{
		switch(pagenum)//�ж�����һҳ
		{
			case 1://LeftMotorPara
			{
				switch(arrow)//��һ��
				{
					case 0:motor_L_pid.KP += mul;break;
					case 1:motor_R_pid.KP += mul;break;
					case 2:motor_L_pid.KD += mul;break;
					case 3:motor_R_pid.KD += mul;break;
					case 4:motor_L_pid.KI += mul;break;
					case 5:motor_R_pid.KI += mul;break;
					default:break;
				}
			}break;
			case 2://RightMotorPara
			{
				switch(arrow)
				{
					case 0:Z_Kp1 += mul;break;
					case 1:Z_Kp2 += mul;break;
					case 2:W_Kp1 += mul;break;
					case 3:W_Kp2 += mul;break;
					case 4:Z_KD += mul;break;
					case 5:W_KD += mul;break;
					default:break;
				}
			}break;
			case 3://ServoPara
			{
				switch(arrow)
				{
					case 0:Zhi += mul;break;
					case 1:Wan += mul;break;
					case 2:Shi += mul;break;
					case 3:BZ_sp3 += mul;break;
					case 4:BZ_err3 += mul;break;
					case 5:BZ_sum3 += mul;break;
					case 6:BZ_dis3 += mul;break;
					default:break;
				}
			}break;
			case 4:
			{
				switch(arrow)
				{
					case 0:BZ_sp1 += mul;break;
					case 1:BZ_err1 += mul;break;
					case 2:BZ_sum1 += mul;break;
					case 3:BZ_dis1 += mul;break;
					case 4:BZ_sp2 += mul;break;
					case 5:BZ_err2 += mul;break;
					case 6:BZ_sum2 += mul;break;
					case 7:BZ_dis2 += mul;break;
					default:break;
				}
			}			
			case 5:
			{
				switch(arrow)
				{
					case 0:LC_dis1 += mul;break;
					case 1:LC_err2 += mul;break;
					case 2:LC_sum2 += mul;break;
					case 3:LC_sum3 += mul;break;
					case 4:LC_dis4 += mul;break;
					default:break;
				}
			}
			case 6:
			{
				switch(arrow)
				{
					case 0:RC_dis1 += mul;break;
					case 1:RC_err2 += mul;break;
					case 2:RC_sum2 += mul;break;
					case 3:RC_sum3 += mul;break;
					case 4:RC_dis4 += mul;break;
					default:break;
				}
			}
			default:break;
		}
	}
	if(key == key2_flag)// -
	 {
		switch(pagenum)//�ж�����һҳ
		{
			case 1://LeftMotorPara
			{
				switch(arrow)//��һ��
				{
					case 0:motor_L_pid.KP -= mul;break;
					case 1:motor_R_pid.KP -= mul;break;
					case 2:motor_L_pid.KD -= mul;break;
					case 3:motor_R_pid.KD -= mul;break;
					case 4:motor_L_pid.KI -= mul;break;
					case 5:motor_R_pid.KI -= mul;break;
					default:break;
				}
			}break;
			case 2://RightMotorPara
			{ 
				switch(arrow) 
				{
					case 0:Z_Kp1 -= mul;break;
					case 1:Z_Kp2 -= mul;break;
					case 2:W_Kp1 -= mul;break;
					case 3:W_Kp2 -= mul;break;
					case 4:Z_KD -= mul;break;
					case 5:W_KD -= mul;break;
					default:break;
				}
			}break;
			case 3://ServoPara
			{
				switch(arrow)
				{
					case 0:Zhi -= mul;break;
					case 1:Wan -= mul;break;
					case 2:Shi -= mul;break;
					case 3:BZ_sp3 -= mul;break;
					case 4:BZ_err3 -= mul;break;
					case 5:BZ_sum3 -= mul;break;
					case 6:BZ_dis3 -= mul;break;
					default:break;
				}
			}break;
			case 4:
			{
				switch(arrow)
				{
					case 0:BZ_sp1 -= mul;break;
					case 1:BZ_err1 -= mul;break;
					case 2:BZ_sum1 -= mul;break;
					case 3:BZ_dis1 -= mul;break;
					case 4:BZ_sp2 -= mul;break;
					case 5:BZ_err2 -= mul;break;
					case 6:BZ_sum2 -= mul;break;
					case 7:BZ_dis2 -= mul;break;
					default:break;
				}
			}
			case 5:
			{
				switch(arrow)
				{
					case 0:LC_dis1 -= mul;break;
					case 1:LC_err2 -= mul;break;
					case 2:LC_sum2 -= mul;break;
					case 3:LC_sum3 -= mul;break;
					case 4:LC_dis4 -= mul;break;
					default:break;
				}				
			}
			case 6:
			{
				switch(arrow)
				{
					case 0:RC_dis1 -= mul;break;
					case 1:RC_err2 -= mul;break;
					case 2:RC_sum2 -= mul;break;
					case 3:RC_sum3 -= mul;break;
					case 4:RC_dis4 -= mul;break;
					default:break;
				}
			}			
			default:break;
		}
	}
if(key == key3_flag)//���ı���
    {
        mul = mul / 10;
    }
if(key == key4_flag)// ȷ�ϸ��Ĳ���
    {
        mul = mul * 10;
    }
if(key == key5_flag)
	{
		datapage = 0;
	}
	
}

void DisplayMain(void)
{
	//ips114_showstr(0,0,">"); 
	ips114_showstr(8,0,"1.MotorPara");//��������     		
  ips114_showstr(8,1,"2.TrackPara");//�ҵ������       	
  ips114_showstr(8,2,"3.SpeedPara");//�������         	 
	ips114_showstr(8,3,"4.BiZhang");//IMU����         	 
	ips114_showstr(8,4,"5.L_CirPara");//Ԫ�ز���         	
	ips114_showstr(8,5,"6.R_CirPara");//���� 
}
void MotorPara(void)     	
{
	ips114_showstr(8,0,"L_Mkp");       	ips114_showfloat(70,0,motor_L_pid.KP,4,3);	
  ips114_showstr(8,1,"R_Mkp");      	ips114_showfloat(70,1,motor_R_pid.KP,4,3);
	ips114_showstr(8,2,"L_Mkd");      	ips114_showfloat(70,2,motor_L_pid.KD,4,3);	
	ips114_showstr(8,3,"R_Mkd");        ips114_showfloat(70,3,motor_R_pid.KD,4,3);
	ips114_showstr(8,4,"L_Mki");      	ips114_showfloat(70,4,motor_L_pid.KI,4,3);
	ips114_showstr(8,5,"R_Mki");      	ips114_showfloat(70,5,motor_R_pid.KI,4,3);
}
void TrackPara(void)
{
	ips114_showstr(8,0,"Z_Kp1");      	ips114_showfloat(70,0,Z_Kp1,4,3);
	ips114_showstr(8,1,"Z_Kp2");      	ips114_showfloat(70,1,Z_Kp2,4,3);
  ips114_showstr(8,2,"W_Kp1");        ips114_showfloat(70,2,W_Kp1,4,3);
  ips114_showstr(8,3,"W_Kp2");        ips114_showfloat(70,3,W_Kp2,4,3);
  ips114_showstr(8,4,"Z_KD");         ips114_showfloat(70,4,Z_KD,4,3); 
	ips114_showstr(8,5,"W_KD");         ips114_showfloat(70,5,W_KD,4,3);	
}
void SpeedPara(void)
{
	ips114_showstr(8,0,"Zhi");       		ips114_showfloat(70,0,Zhi,4,3);
	ips114_showstr(8,1,"Wan");      		ips114_showfloat(70,1,Wan,4,3);
  ips114_showstr(8,2,"Shi");      	  ips114_showfloat(70,2,Shi,4,3);	
	ips114_showstr(8,3,"BZ_sp3");				ips114_showfloat(70,3,BZ_sp3,4,3);
	ips114_showstr(8,4,"BZ_err3");			ips114_showfloat(70,4,BZ_err3,4,3);
	ips114_showstr(8,5,"BZ_sum3");			ips114_showfloat(70,5,BZ_sum3,4,3);
	ips114_showstr(8,6,"BZ_dis3");			ips114_showfloat(70,6,BZ_dis3,4,3);
}
void BiZhang(void)
{
	ips114_showstr(8,0,"BZ_sp1");			ips114_showfloat(70,0,BZ_sp1,4,3);
	ips114_showstr(8,1,"BZ_err1");		ips114_showfloat(70,1,BZ_err1,4,3);
	ips114_showstr(8,2,"BZ_sum1");		ips114_showfloat(70,2,BZ_sum1,4,3);
	ips114_showstr(8,3,"BZ_dis1");		ips114_showfloat(70,3,BZ_dis1,4,3);
	ips114_showstr(8,4,"BZ_sp2");			ips114_showfloat(70,4,BZ_sp2,4,3);
	ips114_showstr(8,5,"BZ_err2");		ips114_showfloat(70,5,BZ_err2,4,3);
	ips114_showstr(8,6,"BZ_sum2");		ips114_showfloat(70,6,BZ_sum2,4,3);
	ips114_showstr(8,7,"BZ_dis2");		ips114_showfloat(70,7,BZ_dis2,5,3);
}
void L_CirPara(void)
{
	ips114_showstr(8,0,"LC_dis1");		ips114_showfloat(70,0,LC_dis1,4,3);
	ips114_showstr(8,1,"LC_err2");		ips114_showfloat(70,1,LC_err2,4,3);
	ips114_showstr(8,2,"LC_sum2");		ips114_showfloat(70,2,LC_sum2,4,3);
	ips114_showstr(8,3,"LC_sum3");		ips114_showfloat(70,3,LC_sum3,4,3);
	ips114_showstr(8,4,"LC_dis4");		ips114_showfloat(70,4,LC_dis4,4,3);
}
void R_CirPara(void)
{
	ips114_showstr(8,0,"RC_dis1");		ips114_showfloat(70,0,RC_dis1,4,3);
	ips114_showstr(8,1,"RC_err2");		ips114_showfloat(70,1,RC_err2,4,3);
	ips114_showstr(8,2,"RC_sum2");		ips114_showfloat(70,2,RC_sum2,4,3);
	ips114_showstr(8,3,"RC_sum3");		ips114_showfloat(70,3,RC_sum3,4,3);
	ips114_showstr(8,4,"RC_dis4");		ips114_showfloat(70,4,RC_dis4,4,3);
}
void DisplayBarrierPara(void)
{
	ips114_showstr(8,0,"BarrierFlag");		//ips114_showint16(80,0,BarrierFlag);
}
void DisplayPodaoPara(void)
{
	ips114_showstr(8,0,"PodaoFlag");			//ips114_showint16(80,0
}

