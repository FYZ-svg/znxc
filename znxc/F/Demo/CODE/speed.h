#ifndef SPEED_H
#define SPEED_H
//extern float speed_goal;
extern int Zhi,Wan,Shi;
void duty_set(int left_duty,int right_duty);
void Speed_Loop();
void Get_Actual_Speed();
float speed_section(int flag);
extern float error;
extern int duty_L,duty_R;
extern float inc_L, inc_R;
#endif