#ifndef _PID_H
#define _PID_H
#define duty_max  9999
#define duty_min  -9999

typedef struct
{
    float SetValue; // 期望值 参考值
    float ActValue; // 实际值
    float KP; // 比例因子
    float KI; // 积分因子
    float KD; // 微分因子
    float ek; // 本级误差
    float ek_1; // 上一次
    float ek_2; // 上上次
    float ek_sum; // 误差累积
    float Sum_max; // 误差累和上限
    float Sum_min; // 误差累和下限
    float PIDmax; // max limit
    float PIDmin; // min limit
    float PIDout; // output
}PID;
extern PID motor_L_pid;
extern PID motor_R_pid;
extern PID speed_ctrl_pid;
extern PID dir_loop_pid;
void pid_set(float *arr,PID *pid);
float PID_Control_Inc(PID* pid,int flag);
float PID_Control_Pos(PID* pid,int flag);
void InitPID(void);
#endif