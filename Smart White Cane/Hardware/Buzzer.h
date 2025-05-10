#ifndef __BUZZER_H
#define __BUZZER_H

void Buzzer_Init(void);
void Buzzer_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Buzzer_Res(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Buzzer_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//void PWM_SetDutyCycle(uint16_t dutyCycle);
//void PWM_Init(void);

#endif