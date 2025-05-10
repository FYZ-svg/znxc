#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//蜂鸣器初始化
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}



//// 初始化PWM输出
//void PWM_Init(void)
//{
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 假设使用TIM3_CH3 (PB0)
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);

//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_TimeBaseStructure.TIM_Period = 999; // PWM频率 = 72MHz / (999 + 1) = 72kHz
//    TIM_TimeBaseStructure.TIM_Prescaler = 0;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//    TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 500; // 占空比 50%
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

//    TIM_Cmd(TIM3, ENABLE);
//}

//// 设置PWM占空比
//void PWM_SetDutyCycle(uint16_t dutyCycle)
//{
//    TIM3->CCR3 = dutyCycle; // 设置占空比
//}

//关闭蜂鸣器
void Buzzer_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOx,GPIO_Pin);
}

//开启蜂鸣器
void Buzzer_Res(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx,GPIO_Pin);
}

//将对应IO口的状态翻转
void Buzzer_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadOutputDataBit(GPIOx,GPIO_Pin) == 0)
	{
		GPIO_SetBits(GPIOx,GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(GPIOx,GPIO_Pin);
	}
}