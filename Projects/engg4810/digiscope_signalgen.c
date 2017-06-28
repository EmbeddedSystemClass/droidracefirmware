/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 21, 2016
 * ============================================================================
 * Filename: digiscope_signalgen.c
 * ============================================================================
 * Hardware Used
 * - DAC
 * - TIM4
 */

#include "digiscope_signalgen.h"

int digiscope_signalgen_counter;
DAC_ChannelConfTypeDef sConfig;
DAC_HandleTypeDef DacHandle;
static TIM_HandleTypeDef TIMDACTrig;
TIM_MasterConfigTypeDef TIMMasterConfig;

static SignalgenWaveform currentwaveform = WAVE_SINE;
static int currentfreq = 25000;

void Error_Handler() {
    printf("Error\n");
    while (1) {
        asm("nop");
    }
}

void digiscope_signalgen_setenabled(int enabled) {
    if (enabled) {
        HAL_TIM_Base_Start(&TIMDACTrig);
    } else {
        HAL_TIM_Base_Stop(&TIMDACTrig);
    }
}

/**
 * Initalize the signalgen component of the Digiscope
 */
void digiscope_signalgen_init() {

    /* Power up the periph we need */
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DAC_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();

    DIGISCOPE_SIGNALGEN_MUX_1_CLK();
    DIGISCOPE_SIGNALGEN_MUX_2_CLK();
    DIGISCOPE_SIGNALGEN_MUX_3_CLK();

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin = DIGISCOPE_SIGNALGEN_MUX_1_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_SIGNALGEN_MUX_1_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_SIGNALGEN_MUX_2_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_SIGNALGEN_MUX_2_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_SIGNALGEN_MUX_3_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_SIGNALGEN_MUX_3_PORT, &GPIO_InitStructure); //Initialise Pin

    /* Set up the timer struct */
    TIMDACTrig.Instance = TIM4;

    TIMDACTrig.Init.Period = 1;
    TIMDACTrig.Init.Prescaler = ((SystemCoreClock / 2) / 1000000) - 1;
    TIMDACTrig.Init.ClockDivision = 0;
    TIMDACTrig.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&TIMDACTrig);

    /* Set up the master trigger to trigger the DMA at the required frequency */
    TIMMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    TIMMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&TIMDACTrig, &TIMMasterConfig);

    /* Start the timer */
    HAL_TIM_Base_Start(&TIMDACTrig);

    /* Init DAC & DMA Settings */
    DacHandle.Instance = DAC;
    DMA_Init(&DacHandle);

    //* Init the DAC */
    if (HAL_DAC_Init(&DacHandle) != HAL_OK) {
        Error_Handler();
    }

    /* Set the trigger and output buffer settings */
    sConfig.DAC_Trigger = DAC_TRIGGER_T4_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    /* Set up the DAC channel as DAC_CHANNEL_1 For PA4 */
    if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
        /* Channel configuration Error */
        Error_Handler();
    }

    /* Enable the DAC (and therefore the DMA) as a sine wave output */
    if (HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_sine, DIGISCOPE_SIGNALGEN_NUMVALUES, DAC_ALIGN_8B_R) != HAL_OK) {
        /* Start DMA Error */
        Error_Handler();
    }

    //    /* Set the signal gen frequency to 25kHz */
    digiscope_signalgen_setfreq(currentfreq);

}

/**
 * Sets the Signal Generator frequency
 * @param freq must be greater than 0 and less than DIGISCOPE_SIGNALGEN_MAX_FREQ
 */
void digiscope_signalgen_setfreq(int freq) {
    /* Check if frequency is within range */
    if (freq < 0 || freq > DIGISCOPE_SIGNALGEN_MAX_FREQ) {
        printf("Invalid freq\n");
        return;
    }

    HAL_DAC_DeInit(&DacHandle);

    HAL_DAC_Init(&DacHandle);

    /* Set the trigger and output buffer settings */
    sConfig.DAC_Trigger = DAC_TRIGGER_T4_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    /* Set up the DAC channel as DAC_CHANNEL_1 For PA4 */
    if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
        /* Channel configuration Error */
        Error_Handler();
    }

    uint32_t prescaler = ((SystemCoreClock / 2) / (1000 * 2.0 * freq)) - 1;
    if (prescaler < 2000) {
        // We cannot use the large look up table.
        TIMDACTrig.Init.Prescaler = ((SystemCoreClock / 2) / (DIGISCOPE_SIGNALGEN_NUMVALUES * 2.0 * freq)) - 1;

        switch (currentwaveform) {
            case WAVE_SINE:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_sine, DIGISCOPE_SIGNALGEN_NUMVALUES, DAC_ALIGN_8B_R);
                break;
            case WAVE_SQUARE:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_square, DIGISCOPE_SIGNALGEN_NUMVALUES, DAC_ALIGN_8B_R);
                break;
            case WAVE_SAWTOOTH:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_sawtooth, DIGISCOPE_SIGNALGEN_NUMVALUES, DAC_ALIGN_8B_R);
                break;
            case WAVE_TRIANGLE:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_triangle, DIGISCOPE_SIGNALGEN_NUMVALUES, DAC_ALIGN_8B_R);
                break;
            case WAVE_NOISE:
                HAL_DAC_Stop_DMA(&DacHandle, DAC_CHANNEL_1);
                HAL_DACEx_NoiseWaveGenerate(&DacHandle, DAC_CHANNEL_1, DAC_LFSRUNMASK_BITS11_0);
                HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1);
                break;
            default:
                // Do nothing
                break;
        }
    } else {
        /* Set the prescaler for the trigger timer */
        TIMDACTrig.Init.Prescaler = prescaler;

        switch (currentwaveform) {
            case WAVE_SINE:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_sine_1000, 1000, DAC_ALIGN_8B_R);
                break;
            case WAVE_SQUARE:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_square_1000, 1000, DAC_ALIGN_8B_R);
                break;
            case WAVE_SAWTOOTH:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_sawtooth_1000, 1000, DAC_ALIGN_8B_R);
                break;
            case WAVE_TRIANGLE:
                HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_1, (uint32_t*) signalgen_triangle_1000, 1000, DAC_ALIGN_8B_R);
                break;
            case WAVE_NOISE:
                HAL_DAC_Stop_DMA(&DacHandle, DAC_CHANNEL_1);
                HAL_DACEx_NoiseWaveGenerate(&DacHandle, DAC_CHANNEL_1, DAC_LFSRUNMASK_BITS11_0);
                HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1);
                break;
            default:
                // Do nothing
                break;
        }
    }

    // Update the locally stored frequency
    currentfreq = freq;
    HAL_TIM_Base_Init(&TIMDACTrig);
}

/**
 * Sets the amplitude for the output of the signal generator
 * @param amplitude Voltage
 * Ampitude must be 100mV 200mV 500mV 1V 2V
 */
void digiscope_signalgen_setamp(int amplitude) {
    if (amplitude == 1) {
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_1_PORT, DIGISCOPE_SIGNALGEN_MUX_1_PIN, 0);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_2_PORT, DIGISCOPE_SIGNALGEN_MUX_2_PIN, 0);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_3_PORT, DIGISCOPE_SIGNALGEN_MUX_3_PIN, 0);
    } else if (amplitude == 2) {
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_1_PORT, DIGISCOPE_SIGNALGEN_MUX_1_PIN, 1);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_2_PORT, DIGISCOPE_SIGNALGEN_MUX_2_PIN, 0);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_3_PORT, DIGISCOPE_SIGNALGEN_MUX_3_PIN, 0);
    } else if (amplitude == 5) {
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_1_PORT, DIGISCOPE_SIGNALGEN_MUX_1_PIN, 0);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_2_PORT, DIGISCOPE_SIGNALGEN_MUX_2_PIN, 1);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_3_PORT, DIGISCOPE_SIGNALGEN_MUX_3_PIN, 0);
    } else if (amplitude == 10) {
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_1_PORT, DIGISCOPE_SIGNALGEN_MUX_1_PIN, 1);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_2_PORT, DIGISCOPE_SIGNALGEN_MUX_2_PIN, 1);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_3_PORT, DIGISCOPE_SIGNALGEN_MUX_3_PIN, 0);
    } else if (amplitude == 20) {
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_1_PORT, DIGISCOPE_SIGNALGEN_MUX_1_PIN, 0);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_2_PORT, DIGISCOPE_SIGNALGEN_MUX_2_PIN, 0);
        HAL_GPIO_WritePin(DIGISCOPE_SIGNALGEN_MUX_3_PORT, DIGISCOPE_SIGNALGEN_MUX_3_PIN, 1);
    }
}

void digiscope_signalgen_setoffset(float amplitude) {
    
    if (amplitude < -2.5 || amplitude > 2.5) {
        return;
    }
    
    lowlevel_dac_setvalue(LOWLEVEL_DAC_FUNC_ID, (amplitude * (1024/5)) + 511);
}

/**
 * Sets the waveform for the signal generator
 * @param wave Options in digiscope_signalgen.h
 */
void digiscope_signalgen_setwave(SignalgenWaveform wave) {
    currentwaveform = wave;
    digiscope_signalgen_setfreq(currentfreq);
}

/**
 * Powers up and handles the DAC and DMA init.
 * Grabs values from digiscope_signalgen.h
 * @param hdac
 */
void DMA_Init(DAC_HandleTypeDef* hdac) {
    GPIO_InitTypeDef GPIO_InitStruct;
    static DMA_HandleTypeDef DMAHandle;

    /* Start the DAC Clock*/
    __HAL_RCC_DAC_CLK_ENABLE();
    DACx_CHANNEL1_GPIO_CLK_ENABLE();

    /* Start the DMA Clock*/
    DMAx_CLK_ENABLE();

    /* Initalise the pin for DMA Output */
    GPIO_InitStruct.Pin = DIGISCOPE_SIGNALGEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DIGISCOPE_SIGNALGEN_PORT, &GPIO_InitStruct);

    /* Sets up the DAC instance */
    DMAHandle.Instance = DACx_DMA_STREAM1;
    DMAHandle.Init.Channel = DACx_DMA_CHANNEL1;
    DMAHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    DMAHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    DMAHandle.Init.MemInc = DMA_MINC_ENABLE;
    DMAHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMAHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    DMAHandle.Init.Mode = DMA_CIRCULAR;
    DMAHandle.Init.Priority = DMA_PRIORITY_HIGH;
    DMAHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    DMAHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMAHandle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMAHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;

    /* Initalise the DMA */
    HAL_DMA_Init(&DMAHandle);

    /* Connect DMA to DAC */
    __HAL_LINKDMA(hdac, DMA_Handle1, DMAHandle);

    /* Enabled the DACx_DMA interupt */
    HAL_NVIC_SetPriority(DACx_DMA_IRQn1, 2, 0);
    HAL_NVIC_EnableIRQ(DACx_DMA_IRQn1);
}

/**
 * Handles the DMA requests
 */
void DACx_DMA_IRQHandler1(void) {
    HAL_DMA_IRQHandler(DacHandle.DMA_Handle1);
}

/**
 * NOT USED
 */
void DACx_DMA_IRQHandler2(void) {
    HAL_DMA_IRQHandler(DacHandle.DMA_Handle2);
}