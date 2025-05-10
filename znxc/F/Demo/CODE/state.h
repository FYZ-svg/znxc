#ifndef _STATE_H
#define _STATE_H
extern char state[30];
extern int state_lead;
extern int Flag_Circ;
extern int Flag_Obstacle;
extern int flag_turn;
extern int typee;
extern float Sum_Distance;
extern float Sum_Angle;
extern int po_time;
extern float BZ_sp1,BZ_err1,BZ_sum1,BZ_dis1,BZ_sp2,BZ_err2,BZ_sum2,BZ_dis2,BZ_sp3,BZ_err3,BZ_sum3,BZ_dis3;
extern float LC_dis1,LC_err2,LC_sum2,LC_sum3,LC_dis4;
extern float RC_dis1,RC_err2,RC_sum2,RC_sum3,RC_dis4;

void state_detect();
int Track_Jump();
int Big_Circ_Left_Jump(int* Flag);
int Small_Circ_Left_Jump(int* Flag);
int Big_Circ_Right_Jump(int* Flag);
int Small_Circ_Right_Jump(int* Flag);
int Obstacle_Jump(int* flag);
enum element
{
	Stop=0,
	Start=1,
	Track=2,
	Bend=3,
	Big_Circ_Left=4,
	Big_Circ_Right=5,
	Small_Circ_Left=6,
	Small_Circ_Right=7,
	Obstacle=8,
	Ramp=9,
	Garage_Out=10,
	Garage_In=11,
};
enum Track
{
	cross=1,
	right_angle=2,
	normal=3,
};
#endif