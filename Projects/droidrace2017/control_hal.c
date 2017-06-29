//
// Created by Adam Stacey on 29/6/17.
//

#include "control_hal.h"

static TIM_HandleTypeDef TIM3_Init;
static TIM_HandleTypeDef TIM4_Init;

void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

void control_init() {
	control_channel1_init();
	control_channel2_init();
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
