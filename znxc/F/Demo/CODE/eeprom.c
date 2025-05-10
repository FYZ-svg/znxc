#include "headfile.h"
#include "eeprom.h"
float pitch_angle = 10;
float yaw_angle = 10;
float roll_angle = 10;
float AngleTh = 30;
uint16 PodaoFlag = 0; 
uint16 BarrierFlag = 0;
uint16 RingFlag = 0;
uint8 datapage = 0;//0：显示ID对应的函数，1：修改arrow对应的参数
float mul = 1;
int Pm=0;
int f_lag = 0;

//**************************************************************************//
//**************************************************************************//
//定义按键引脚
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


//***************函数宏定义****(下面这些函数请修改宏定义为对应的GPIO库函数操作)***********
#define KEY_INT(key_x)           gpio_mode(key_x,GPO_PP)//配置为GPO_PP:推挽输出 
#define SWITCH_INT(switch_x)     gpio_mode(switch_x,GPO_PP)//配置为GPO_PP:推挽输出
#define READ_GPIO(Pin_X)         Pin_X
#define TiaoCan_DelayMs(M_S)     delay_ms(M_S)   //延时函数

//开关状态变量
int key1_status = 1,key2_status = 1,key3_status = 1, key4_status = 1,key5_status = 1;
//上一次开关状态变量
int key1_last_status, key2_last_status, key3_last_status, key4_last_status,key5_last_status;
//开关标志位
//uint8 key1_flag=0,key2_flag=0,key3_flag=0, key4_flag=0,key5_flag=0;
unsigned char Switch1=0,Switch2=0;

void Switch_Init()
{
	//拨码开关初始化  （无需修改，请勿修改）
	SWITCH_INT(Switch_Pin_1) ;
	SWITCH_INT(Switch_Pin_2) ;
}

int Temp_Swtich=0;
void Strategy_Slect()
{
	
	//读取拨码开关状态
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
	//按键初始化 （无需修改，请勿修改）
  KEY_INT(KEY1);
	KEY_INT(KEY2);
	KEY_INT(KEY3);
	KEY_INT(KEY4);
	KEY_INT(KEY5);
}
uint8  Key_Scan_Deal ()													
{
    //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
    //保存按键状态
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
	
		//读取当前按键值
		key1_status = KEY1;
    key2_status = KEY2;
    key3_status = KEY3;
    key4_status = KEY4;
    key5_status = KEY5;
	
		delay_ms(10);
		//检测到按键按下之后   并放开置位标志位
		if(key1_status && !key1_last_status)  return key1_flag;
		else if(key2_status && !key2_last_status)  return key2_flag;
		else if	(key3_status && !key3_last_status)  return key3_flag;
		else if(key4_status && !key4_last_status)  return key4_flag;
		else if(key5_status && !key5_last_status)  return key5_flag;
		else return 0;
		
		
}




//********************************************************//
//********************************************************//
int8 arrow = 0;//定义光标所在的行，初始化在第一行
void DisplayCursor()
{
	ips114_showstr(0,arrow,">");//在参数前面显示>
}

void UI()
{
	if(!datapage)
	{
		UI_Content();//显示ID对应的函数
		UI_ContentKey();//更新ID
	}
	else
	{
		UI_Datapage();  //数据页
    UI_DatapageKey();   //数据按键处理
	}
	DisplayCursor();//光标显示
}

////用来显示各级菜单
uint8 pagenum = 0;//显示page的数目
//int8 arrow = 0;//定义光标所在的行，初始化在第一行
int8 PageClear = 0;
void UI_Content(void)
{
	
	// 根据pagenum去显示菜单
	
	switch(pagenum)
    {
    case 0://显示主菜单
			//ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
		{
			DisplayMain();
		}break;
		case 1://电机PID
		{
			MotorPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		case 2://转向PID
		{
			TrackPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		case 3://速度设定
		{
			SpeedPara();
		if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
		PageClear = pagenum;
		}break;
		case 4://避障
		{
			BiZhang();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		case 5://左环
		{
			L_CirPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		case 6://右环
		{
			R_CirPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		case 7:
		{
			DisplayBarrierPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		case 8:
		{
			DisplayPodaoPara();
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		default:
		{
			DisplayMain();	
		}break;
	}
}
//返回上一级菜单
uint8 pagenumup(void)
{
	switch(pagenum)
	{
		case 0://在一级菜单
			return 0;
		case 1://在一级菜单
			return 0;
		case 2://在一级菜单
			return 0;
		case 3://在一级菜单
			return 0;
		case 4://在一级菜单
			return 0;
		case 5://在一级菜单
			return 0;
		//这里解释一下，当在二级菜单ID=6时，返回的上一级是ID=5
		//
		case 6://在二级菜单
			return 5;
		case 7://在二级菜单
			return 5;
		case 8://在二级菜单
			return 5;
		
		default:
			return 0;
	}
}

//进入下一级菜单
//需要判断当前ID：知道在那一页，有些什么参数
//还需要判断光标所在行：知道要进入哪一个变量里面
uint8 pagenumdown(void)
{
	switch(pagenum)
	{
		case 0://在主界面
		{
			switch(arrow)//判断在第几行
			{
				case 0://在第0行---->对应着参数LeftMotorPara
				{
					return 1;//返回LeftMotorPara的ID
				}
				case 1: return 2;//对应RightMotorPara
				case 2: return 3;//对应ServoPara
				case 3: return 4;//对应IMUPara
				case 4: return 5;//对应ElemnetPara
			}
		}break;
		case 1://在LeftMotorPara界面
		{
			return 1;//返回当前界面
			//在该界面下没有下一级，就直接break;
		}break;
		case 2: return 2;break;//在RightMotorPara界面
		case 3: return 3;break;//ServoPara界面
		case 4: return 4;break;//IMUPara界面
		case 5://ElemnetPara界面
		{
			switch(arrow)
			{
				case 0: return 6;//对应Ring
				case 1: return 7;//对应Barrier
				case 2: return 8;//对应podao
				default : return 5;//返回当前界面
			}
		}break;
		
		default:break;
	}
}


//接收按键值，并更新arrow
void UI_ContentKey()
{
	int8 key = 0 ;
			key = Key_Scan_Deal();//获取按键值
    if(key == key1_flag)
			{//向上 up
        ips114_showstr(0,arrow," ");  //需要将当前行的光标隐藏，不然当arrow更新之后，出现两行显示光标
				arrow--;
				//向上移动，arrow减小
			}
//		ips114_showuint16(40,7,key);
    if(key == key2_flag){//向下 down
        ips114_showstr(0,arrow," ");  arrow++;                                                                                          
		}//和up同理
	
/**********上面的key1,key2是用来上下移动光标的***********/
/**********下面的key3,key4是用来进入相应的父/子菜单的***********/

    if(key == key3_flag){//进入子页面  enter
    ips114_clear(WHITE);//需要将当前显示清屏，用于显示子界面    
		pagenum = pagenumdown();//进入子界面 
		arrow = 0;//从第0行开始
	}
    if(key == key4_flag){//返回上一个页面 back
    ips114_clear(WHITE);    
		pagenum = pagenumup();
		arrow = 0;
	}
	if(key == key5_flag)//更改参数
	{
		ips114_clear(WHITE);    
		datapage = 1;    
		mul = 1;
	}
	
	//对arrow限幅，OLED屏只能显示8行，所以需要限制 0 <= arrow <8
	if(arrow < 0)
		arrow = 7;//意思是：当按下up键，光标一直向上移动，当运动到第一行即arrow=0，
				  //再按下up时，光标直接跳到最后一行，arrow = 7
	else if(arrow >7)
		arrow = 0;

}

//显示具体的参数
int arrow_Clear = 0;
void UI_Datapage()
{
	uint8 x = 8,y = 80;
	//ips114_clear(WHITE);
	switch(pagenum)
	{
		case 0:break;//在ID为0的界面没有需要修改的参数
		case 1://对应LeftMotorPara
		{
			switch(arrow)
			{
				case 0://kp
				{
					
					ips114_showstr(x,0,"L_Mkp"); 
					ips114_showfloat(y,0,motor_L_pid.KP,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 1://ki
				{
					ips114_showstr(x,0,"L_Mki"); 
					ips114_showfloat(y,0,motor_R_pid.KP,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 2://kd
				{
					ips114_showstr(x,0,"D_Mkd"); 
					ips114_showfloat(y,0,motor_L_pid.KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"R_Mkp"); 
					ips114_showfloat(y,0,motor_R_pid.KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率
					
					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 4://TargetSpeed
				{
					ips114_showstr(x,0,"R_Mki"); 
					ips114_showfloat(y,0,motor_L_pid.KI,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率
					
					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"R_Mkd"); 
					ips114_showfloat(y,0,motor_R_pid.KI,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率		

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			if(PageClear != pagenum)
				ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
			PageClear = pagenum;
		}break;
		
		case 2://对应RightMotorPara
		{
			switch(arrow)
			{
				case 0://kp
				{
					ips114_showstr(x,0,"Z_Kp1"); 
					ips114_showfloat(y,0,Z_Kp1,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 1://ki
				{
					ips114_showstr(x,0,"Z_Kp2"); 
					ips114_showfloat(y,0,Z_Kp2,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 2://kd
				{
					ips114_showstr(x,0,"W_Kp1"); 
					ips114_showfloat(y,0,W_Kp1,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 3: 
				{
					ips114_showstr(x,0,"W_Kp2"); 
					ips114_showfloat(y,0,W_Kp2,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}
				break;
				case 4://TargetSpeed
				{
					ips114_showstr(x,0,"Z_KD"); 
					ips114_showfloat(y,0,Z_KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"W_KD"); 
					ips114_showfloat(y,0,W_KD,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			ips114_showfloat(40,7,mul,8,4);//显示倍率
		}break;
		case 3://对应ServoPara
		{
			switch(arrow)
			{
				case 0://kp
				{
					ips114_showstr(x,0,"Zhi"); 
					ips114_showfloat(y,0,Zhi,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 1://ki
				{
					ips114_showstr(x,0,"Wan"); 
					ips114_showfloat(y,0,Wan,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 2://kd
				{
					ips114_showstr(x,0,"Shi"); 
					ips114_showfloat(y,0,Shi,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"BZ_sp3"); 
					ips114_showfloat(y,0,BZ_sp3,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"BZ_err3"); 
					ips114_showfloat(y,0,BZ_err3,3,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"BZ_sum3"); 
					ips114_showfloat(y,0,BZ_sum3,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 6:
				{
					ips114_showstr(x,0,"BZ_dis3"); 
					ips114_showfloat(y,0,BZ_dis3,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			ips114_showfloat(40,7,mul,8,4);//显示倍率
		}break;			
		case 4:
		{
			switch(arrow)
			{
				case 0:
				{
					ips114_showstr(x,0,"BZ_sp1"); 
					ips114_showfloat(y,0,BZ_sp1,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;	
				case 1:
				{
					ips114_showstr(x,0,"BZ_err1"); 
					ips114_showfloat(y,0,BZ_err1,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 2:
				{
					ips114_showstr(x,0,"BZ_sum1"); 
					ips114_showfloat(y,0,BZ_sum1,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"BZ_dis1"); 
					ips114_showfloat(y,0,BZ_dis1,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"BZ_sp2"); 
					ips114_showfloat(y,0,BZ_sp2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 5:
				{
					ips114_showstr(x,0,"BZ_err2"); 
					ips114_showfloat(y,0,BZ_err2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 6:
				{
					ips114_showstr(x,0,"BZ_sum2"); 
					ips114_showfloat(y,0,BZ_sum2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 7:
				{
					ips114_showstr(x,0,"BZ_dis2"); 
					ips114_showfloat(y,0,BZ_dis2,5,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				default:break;
			}
			ips114_showfloat(40,7,mul,8,4);//显示倍率
		}break;
		case 5:
		{
			switch(arrow)
			{
				case 0:
				{
					ips114_showstr(x,0,"LC_dis1"); 
					ips114_showfloat(y,0,LC_dis1,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;	
				case 1:
				{
					ips114_showstr(x,0,"LC_err2"); 
					ips114_showfloat(y,0,LC_err2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 2:
				{
					ips114_showstr(x,0,"LC_sum2"); 
					ips114_showfloat(y,0,LC_sum2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"LC_sum3"); 
					ips114_showfloat(y,0,LC_sum3,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"LC_dis4"); 
					ips114_showfloat(y,0,LC_dis4,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				default:break;
			}
				ips114_showfloat(40,7,mul,8,4);//显示倍率
		}break;
		case 6:
		{
			switch(arrow)
			{
				case 0:
				{
					ips114_showstr(x,0,"RC_dis1"); 
					ips114_showfloat(y,0,RC_dis1,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;	
				case 1:
				{
					ips114_showstr(x,0,"RC_err2"); 
					ips114_showfloat(y,0,RC_err2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 2:
				{
					ips114_showstr(x,0,"RC_sum2"); 
					ips114_showfloat(y,0,RC_sum2,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 3:
				{
					ips114_showstr(x,0,"RC_sum3"); 
					ips114_showfloat(y,0,RC_sum3,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				case 4:
				{
					ips114_showstr(x,0,"RC_dis4"); 
					ips114_showfloat(y,0,RC_dis4,4,3);
					ips114_showfloat(40,7,mul,8,4);//显示倍率

					if(arrow_Clear != arrow)
					ips114_clear(WHITE);//在显示新一级菜单时，需要清屏
					arrow_Clear = arrow;
				}break;
				default:break;
			}
				ips114_showfloat(40,7,mul,8,4);//显示倍率
		}break;
		case 7://对应BarrierPara
		{
			//自行添加变量
			
		}break;
		case 8://对应podaoPara
		{
			//自行添加变量
		}break;
		
		default:break;
	}
}

////更改参数
void UI_DatapageKey()
{
	uint8 key = Key_Scan_Deal();
	if(key == key1_flag)//   +
	{
		switch(pagenum)//判断在哪一页
		{
			case 1://LeftMotorPara
			{
				switch(arrow)//哪一行
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
		switch(pagenum)//判断在哪一页
		{
			case 1://LeftMotorPara
			{
				switch(arrow)//哪一行
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
if(key == key3_flag)//更改倍率
    {
        mul = mul / 10;
    }
if(key == key4_flag)// 确认更改参数
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
	ips114_showstr(8,0,"1.MotorPara");//左电机参数     		
  ips114_showstr(8,1,"2.TrackPara");//右电机参数       	
  ips114_showstr(8,2,"3.SpeedPara");//舵机参数         	 
	ips114_showstr(8,3,"4.BiZhang");//IMU参数         	 
	ips114_showstr(8,4,"5.L_CirPara");//元素参数         	
	ips114_showstr(8,5,"6.R_CirPara");//启动 
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

