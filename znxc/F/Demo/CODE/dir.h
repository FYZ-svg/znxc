#ifndef DIF_H
#define DIF_H
extern float K_p1;
extern float K_p2;
extern float Z_Kp1,Z_Kp2,Z_KD,W_Kp1,W_Kp2,W_KD;
void Dir_Loop(float error,float speed_goal,int flag);
float  Kp_Select(float error);
float  Kp_Select_1(float error);
#endif