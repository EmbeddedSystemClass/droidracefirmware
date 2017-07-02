//
// Created by Adam Stacey on 29/6/17.
//

#include "control_hal.h"

static TIM_HandleTypeDef TIM3_Init;
static TIM_HandleTypeDef TIM4_Init;

TIM_OC_InitTypeDef PWMConfig;
TIM_OC_InitTypeDef PWMConfig2;
TIM_HandleTypeDef TIM_Init;

void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void motor_output_init();

float dc_value_ch1 = 50;
float dc_value_ch2 = 12.5;

void control_init() {
	control_channel1_init();
	control_channel2_init();
	motor_output_init();
}

void control_channel1_init() {

	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_IC_InitTypeDef TIM_ICInitStruct;

	uint16_t prescalerValue = 1;

	/* Clocks */
	__TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	prescalerValue = (uint16_t) ((SystemCoreClock / 2) / 50000) - 1;

	/* Configure Timer 3 settings */
	TIM3_Init.Instance = TIM3;					//Enable Timer 3
	TIM3_Init.Init.Period = 0xFFFF;
	TIM3_Init.Init.Prescaler = prescalerValue;	//Set prescale value
	TIM3_Init.Init.ClockDivision = 0;			//Set clock division
	TIM3_Init.Init.RepetitionCounter = 0; 		// Set Reload Value
	TIM3_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Configure TIM3 Input capture */
	TIM_ICInitStruct.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
	TIM_ICInitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStruct.ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.ICFilter = 0;

	/* Set priority of Timer 3 Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	HAL_NVIC_SetPriority(TIM3_IRQn, 10, 0);	//Set Main priority ot 10 and sub-priority ot 0.

	//Enable Timer 3 interrupt
	NVIC_EnableIRQ(TIM3_IRQn);

	/* Enable input capture for Timer 3, channel 1 */
	HAL_TIM_IC_Init(&TIM3_Init);
	HAL_TIM_IC_ConfigChannel(&TIM3_Init, &TIM_ICInitStruct, TIM_CHANNEL_1);

	/* Start Input Capture */
	HAL_TIM_IC_Start_IT(&TIM3_Init, TIM_CHANNEL_1);
}

void control_channel2_init() {

	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_IC_InitTypeDef TIM_ICInitStruct;

	uint16_t prescalerValue = 1;

	/* Clocks */
	__TIM4_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	prescalerValue = (uint16_t) ((SystemCoreClock / 2) / 50000) - 1;

	/* Configure Timer 3 settings */
	TIM4_Init.Instance = TIM4;					//Enable Timer 3
	TIM4_Init.Init.Period = 0xFFFF;
	TIM4_Init.Init.Prescaler = prescalerValue;	//Set prescale value
	TIM4_Init.Init.ClockDivision = 0;			//Set clock division
	TIM4_Init.Init.RepetitionCounter = 0; 		// Set Reload Value
	TIM4_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Configure TIM3 Input capture */
	TIM_ICInitStruct.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
	TIM_ICInitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStruct.ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.ICFilter = 0;

	/* Set priority of Timer 3 Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	HAL_NVIC_SetPriority(TIM4_IRQn, 10, 0);	//Set Main priority ot 10 and sub-priority ot 0.

	//Enable Timer 3 interrupt
	NVIC_EnableIRQ(TIM4_IRQn);

	/* Enable input capture for Timer 3, channel 1 */
	HAL_TIM_IC_Init(&TIM4_Init);
	HAL_TIM_IC_ConfigChannel(&TIM4_Init, &TIM_ICInitStruct, TIM_CHANNEL_4);

	/* Start Input Capture */
	HAL_TIM_IC_Start_IT(&TIM4_Init, TIM_CHANNEL_4);
}

void motor_output_init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	uint16_t PrescalerValue = 0;

	/* Timer 3 clock enable */
	__TIM2_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure the D0 pin with TIM3 output*/
	GPIO_InitStructure.Pin = GPIO_PIN_10;				//Pin
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; //Set mode to be output alternate
	GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;//Set alternate function to be timer 2
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);	//Initialise Pin

	GPIO_InitStructure.Pin = GPIO_PIN_3;				//Pin
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; //Set mode to be output alternate
	GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;//Set alternate function to be timer 2
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);	//Initialise Pin

	/* Compute the prescaler value. SystemCoreClock = 168000000 - set for 50Khz clock */
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;

	/* Configure Timer settings */
	TIM_Init.Instance = TIM2;					//Enable Timer 2
	TIM_Init.Init.Period = 2 * 4000;			//Set for 20ms (5Hz) period
	TIM_Init.Init.Prescaler = PrescalerValue;	//Set presale value
	TIM_Init.Init.ClockDivision = 0;			//Set clock division
	TIM_Init.Init.RepetitionCounter = 0; 		// Set Reload Value
	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* PWM Mode configuration for Channel 2 - set pulse width*/
	PWMConfig.OCMode = TIM_OCMODE_PWM1;	//Set PWM MODE (1 or 2 - NOT CHANNEL)
	PWMConfig.Pulse = 2 * 4000 / 100;		//0.1ms pulse width to 1ms
	PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWMConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	PWMConfig.OCFastMode = TIM_OCFAST_DISABLE;
	PWMConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	PWMConfig2.OCMode = TIM_OCMODE_PWM1;//Set PWM MODE (1 or 2 - NOT CHANNEL)
	PWMConfig2.Pulse = 2 * 4000 / 100;		//0.1ms pulse width to 1ms
	PWMConfig2.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWMConfig2.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	PWMConfig2.OCFastMode = TIM_OCFAST_DISABLE;
	PWMConfig2.OCIdleState = TIM_OCIDLESTATE_RESET;
	PWMConfig2.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	/* Enable PWM for Timer 3, channel 2 */
	HAL_TIM_PWM_Init(&TIM_Init);
	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig2, TIM_CHANNEL_2);

	/* Start PWM */
	HAL_TIM_PWM_Start(&TIM_Init, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&TIM_Init, TIM_CHANNEL_2);
}

//int count_ch1, count_ch2, mode;
//void TIM2_IRQHandler(void) {
//	__HAL_TIM_CLEAR_IT(&TIM2_Init, TIM_IT_UPDATE);
//
//	count_ch1++;
//
//	switch (mode) {
//	case 0:
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
//		mode = 1;
//	case 1:
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
//		mode = 0;
//		count_ch1 = 0;
//		break;
//	}
//}

void TIM4_IRQHandler(void) {
	static unsigned int last_capture_val = 0;
	unsigned int input_capture_value;

	//Clear Input Capture Flag
	__HAL_TIM_CLEAR_IT(&TIM4_Init, TIM_IT_TRIGGER);

	/* Read and display the Input Capture value of Timer 3, channel 2 */
	input_capture_value = HAL_TIM_ReadCapturedValue(&TIM4_Init, TIM_CHANNEL_4);

	int tmp = input_capture_value - last_capture_val;

	if (tmp <= 100 && tmp >= 50) {
		input_channels[1] = tmp;
	}

	last_capture_val = input_capture_value;
}

void TIM3_IRQHandler(void) {
	static unsigned int last_capture_val = 0;
	unsigned int input_capture_value;

	//Clear Input Capture Flag
	__HAL_TIM_CLEAR_IT(&TIM3_Init, TIM_IT_TRIGGER);

	/* Read and display the Input Capture value of Timer 3, channel 2 */
	input_capture_value = HAL_TIM_ReadCapturedValue(&TIM3_Init, TIM_CHANNEL_1);

	int tmp = input_capture_value - last_capture_val;

	if (tmp <= 100 && tmp >= 50) {
		input_channels[0] = tmp;
	}

	last_capture_val = input_capture_value;
}


/**
 * @param val - 0 to 100
 */
void setDuty2(float val) {
	PWMConfig2.Pulse = (2 * 4000 / 100) * val + 1;
	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&TIM_Init, TIM_CHANNEL_2);
}

void dont() {
	setDuty2(9.3); //9.3
}

void goFast(int go) {
	if (go) {
		setDuty2(9.72); //9.72
	} else {
		dont();
	}
}

/**
 * @param val - 0 to 100
 */
void setDuty(float val) {
	PWMConfig.Pulse = (2 * 4000 / 100) * val + 1;
	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&TIM_Init, TIM_CHANNEL_3);
}

void setSteering(float val) {
	if (val < 20 || val > 80) {
		return;
	}

	float v = 6.8 + 0.05 * val;
	setDuty(v);
}

