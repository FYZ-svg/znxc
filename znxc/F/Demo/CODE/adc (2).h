#ifndef DATA_H
#define DATA_H
//extern float error;
extern int flag_turn;
extern float Erro;
extern float L_Adc,R_Adc,L_S_Adc,R_S_Adc,M_Adc;//µç¸ÐÖµ
void Read_ADC();
void InductorNormal (void);

float error_get(int flag);
#endif