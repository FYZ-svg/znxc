#ifndef DATA_H
#define DATA_H
//extern float error;
extern int flag_turn;
extern float Erro;
extern float L_Adc,R_Adc,L_S_Adc,R_S_Adc,M_Adc;//µç¸ÐÖµ
extern int P_5[10];
void Read_ADC();
void InductorNormal (void);
int MeanFilter(int *arr, int n);
float error_get(int flag);

#endif