/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 27, 2016
 * ============================================================================
 * Filename: digiscope_adc.c
 * ============================================================================
 */

#include "digiscope_adc.h"
#include "Core/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h"

#define ADC_BLOCK_SIZE 2000
#define ADC_BLOCK_SIZE_12B 1000
//#define ADC_TRIG_OUT //Enable an external pin to act as trigger measurement
//#define ADC_TRIG_OUT_ERR // Enable an external pin to act as trigger missed measurement
#define DEBUG_TRIGGER

ADC_HandleTypeDef ADCHandleCH1;
DMA_HandleTypeDef DMAHandleCH1;

ADC_HandleTypeDef ADCHandleCH2;
DMA_HandleTypeDef DMAHandleCH2;

TIM_HandleTypeDef TIMADCTrig;
TIM_MasterConfigTypeDef TIMMasterConfig;
SemaphoreHandle_t NetSendTrigger;

static TriggerSettings_t triggersettings = {SETTINGS_TRIGGER_MODE_AUTO, SETTINGS_TRIGGER_RISING, 125, 1};

int triggered = 0, forcetrigger = 0;
int paused = 0;

int g_ADCValue;
uint8_t DMABlockCH1[ADC_BLOCK_SIZE];
uint8_t DMABlockCH2[ADC_BLOCK_SIZE];

int channel1_beforetrigger_counter, channel1_aftertrigger_counter = 0;
uint8_t* channel1_beforetrigger;
uint8_t* channel1_aftertrigger;

int channel2_beforetrigger_counter, channel2_aftertrigger_counter = 0;
uint8_t* channel2_beforetrigger;
uint8_t* channel2_aftertrigger;

int samplingmode = 8;

/**
 * Updates the trigger settings for both ADCs
 * @param newsettings
 */
void digiscope_adc_trigger_settings(TriggerSettings_t *newsettings) {
    memcpy(&triggersettings, newsettings, sizeof (TriggerSettings_t));
}

void digiscope_adc_samplingrate(int rate) {
    TIMADCTrig.Init.Prescaler = ((SystemCoreClock / 2) / (int) (rate * 1.4285 * 3)) - 1;
    HAL_TIM_Base_Init(&TIMADCTrig);

    HAL_TIM_Base_Start(&TIMADCTrig);
}

///**
// * 8 bit/12 bit mode
// * @param mode
// */
//void digiscope_adc_samplingmode(int mode) {
////    HAL_TIM_Base_Stop(&TIMADCTrig);
////
////    // Tear down the old values
////    digiscope_adc_deinit();
////
////    samplingmode = mode;
////
////    digiscope_adc_init_ch1();
////    digiscope_adc_init_ch1();
////
////    HAL_TIM_Base_Start(&TIMADCTrig);
//}

void digiscope_adc_trigger_init() {
    NetSendTrigger = xSemaphoreCreateBinary();

    __HAL_RCC_TIM2_CLK_ENABLE();

    channel1_beforetrigger = pvPortMalloc(sizeof (uint8_t) * 25000);
    channel1_aftertrigger = pvPortMalloc(sizeof (uint8_t) * 25000);

    channel2_beforetrigger = pvPortMalloc(sizeof (uint8_t) * 25000);
    channel2_aftertrigger = pvPortMalloc(sizeof (uint8_t) * 25000);

    if (channel1_aftertrigger == NULL || channel1_beforetrigger == NULL || channel2_beforetrigger == NULL || channel2_aftertrigger == NULL) {
        printf("Malloc failed\n");
        for (;;);
    }

    /* Set up the timer struct */
    TIMADCTrig.Instance = TIM2;

    TIMADCTrig.Init.Period = 1;
    TIMADCTrig.Init.Prescaler = ((SystemCoreClock / 2) / 14285000) - 1;
    TIMADCTrig.Init.ClockDivision = 0;
    TIMADCTrig.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&TIMADCTrig);

    /* Set up the master trigger to trigger the DMA at the required frequency */
    TIMMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    TIMMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&TIMADCTrig, &TIMMasterConfig);

    /* Start the timer */
    HAL_TIM_Base_Start(&TIMADCTrig);
}

void digiscope_adc_deinit() {
    // Stop the DMA for both channels
    HAL_DMA_DeInit(&DMAHandleCH1);
    HAL_DMA_DeInit(&DMAHandleCH2);

    // De init the ADCs
    HAL_ADC_DeInit(&ADCHandleCH1);
    HAL_ADC_DeInit(&ADCHandleCH2);
}

void digiscope_adc_init_ch1() {

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure the D0 pin as an output */
    GPIO_InitStructure.Pin = GPIO_PIN_11; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_PULLDOWN; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure); //Initialise Pin

    channel1_beforetrigger_counter = 0;
    channel1_aftertrigger_counter = 0;

    GPIO_InitTypeDef gpioInit;

    ADC_CH1_GPIO_CLK();
    __ADC1_CLK_ENABLE();

    gpioInit.Pin = ADC_CH1_GPIO_PIN;
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC_CH1_GPIO_PORT, &gpioInit);

    HAL_NVIC_SetPriority(ADC_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    ADC_ChannelConfTypeDef adcChannel;

    ADCHandleCH1.Instance = ADC_CH1_ADC;
    ADCHandleCH1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;

    if (samplingmode == 8) {
        ADCHandleCH1.Init.Resolution = ADC_RESOLUTION_8B;
    } else {
        ADCHandleCH1.Init.Resolution = ADC_RESOLUTION_12B;
    }

    ADCHandleCH1.Init.ScanConvMode = ENABLE;
    ADCHandleCH1.Init.ContinuousConvMode = DISABLE;
    ADCHandleCH1.Init.DiscontinuousConvMode = DISABLE;
    ADCHandleCH1.Init.NbrOfDiscConversion = 0;
    ADCHandleCH1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    ADCHandleCH1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
    ADCHandleCH1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ADCHandleCH1.Init.NbrOfConversion = 1;
    ADCHandleCH1.Init.DMAContinuousRequests = ENABLE;
    ADCHandleCH1.Init.EOCSelection = DISABLE;

    HAL_ADC_Init(&ADCHandleCH1);

    adcChannel.Channel = ADC_CH1_CHANNEL;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    adcChannel.Offset = 0;

    if (HAL_ADC_ConfigChannel(&ADCHandleCH1, &adcChannel) != HAL_OK) {
        printf("Config Channel Error\n");
    }

    __DMA2_CLK_ENABLE();
    DMAHandleCH1.Instance = DMA2_Stream4;
    DMAHandleCH1.Init.Channel = DMA_CHANNEL_0;
    DMAHandleCH1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMAHandleCH1.Init.PeriphInc = DMA_PINC_DISABLE;
    DMAHandleCH1.Init.MemInc = DMA_MINC_ENABLE;

    if (samplingmode == 8) {
        DMAHandleCH1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        DMAHandleCH1.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
    } else {
        DMAHandleCH1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        DMAHandleCH1.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
    }

    DMAHandleCH1.Init.Mode = DMA_CIRCULAR;
    DMAHandleCH1.Init.Priority = DMA_PRIORITY_HIGH;
    DMAHandleCH1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    DMAHandleCH1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMAHandleCH1.Init.MemBurst = DMA_MBURST_SINGLE;
    DMAHandleCH1.Init.PeriphBurst = DMA_PBURST_SINGLE;

    HAL_DMA_Init(&DMAHandleCH1);

    __HAL_LINKDMA(&ADCHandleCH1, DMA_Handle, DMAHandleCH1);

    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

    if (samplingmode == 8) {
        HAL_ADC_Start_DMA(&ADCHandleCH1, (uint32_t *) DMABlockCH1, ADC_BLOCK_SIZE);
    } else {
        /* Start DMA for ADC_BLOCK_SIZE_12B values from the ADC */
        HAL_ADC_Start_DMA(&ADCHandleCH1, (uint32_t *) DMABlockCH1, ADC_BLOCK_SIZE_12B);
    }

}

void digiscope_adc_init_ch2() {
    GPIO_InitTypeDef gpioInit;

    ADC_CH2_GPIO_CLK();
    __ADC2_CLK_ENABLE();

    gpioInit.Pin = ADC_CH2_GPIO_PIN;
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC_CH2_GPIO_PORT, &gpioInit);

    HAL_NVIC_SetPriority(ADC_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    ADC_ChannelConfTypeDef adcChannel;

    ADCHandleCH2.Instance = ADC_CH2_ADC;
    ADCHandleCH2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;

    if (samplingmode == 8) {
        ADCHandleCH2.Init.Resolution = ADC_RESOLUTION_8B;
    } else {
        ADCHandleCH2.Init.Resolution = ADC_RESOLUTION_12B;
    }

    ADCHandleCH2.Init.ScanConvMode = ENABLE;
    ADCHandleCH2.Init.ContinuousConvMode = DISABLE;
    ADCHandleCH2.Init.DiscontinuousConvMode = DISABLE;
    ADCHandleCH2.Init.NbrOfDiscConversion = 0;
    ADCHandleCH2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    ADCHandleCH2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
    ADCHandleCH2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ADCHandleCH2.Init.NbrOfConversion = 1;
    ADCHandleCH2.Init.DMAContinuousRequests = ENABLE;
    ADCHandleCH2.Init.EOCSelection = DISABLE;

    HAL_ADC_Init(&ADCHandleCH2);

    adcChannel.Channel = ADC_CH2_CHANNEL;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    adcChannel.Offset = 0;

    if (HAL_ADC_ConfigChannel(&ADCHandleCH2, &adcChannel) != HAL_OK) {
        printf("Config Channel Error\n");
    }

    __DMA2_CLK_ENABLE();
    DMAHandleCH2.Instance = DMA2_Stream2;
    DMAHandleCH2.Init.Channel = DMA_CHANNEL_1;
    DMAHandleCH2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMAHandleCH2.Init.PeriphInc = DMA_PINC_DISABLE;
    DMAHandleCH2.Init.MemInc = DMA_MINC_ENABLE;
    DMAHandleCH2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMAHandleCH2.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
    DMAHandleCH2.Init.Mode = DMA_CIRCULAR;
    DMAHandleCH2.Init.Priority = DMA_PRIORITY_HIGH;
    DMAHandleCH2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    DMAHandleCH2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMAHandleCH2.Init.MemBurst = DMA_MBURST_SINGLE;
    DMAHandleCH2.Init.PeriphBurst = DMA_PBURST_SINGLE;

    HAL_DMA_Init(&DMAHandleCH2);

    __HAL_LINKDMA(&ADCHandleCH2, DMA_Handle, DMAHandleCH2);

    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    if (samplingmode == 8) {
        HAL_ADC_Start_DMA(&ADCHandleCH2, (uint32_t *) DMABlockCH2, ADC_BLOCK_SIZE);
    } else {
        /* Start DMA for ADC_BLOCK_SIZE_12B values from the ADC */
        HAL_ADC_Start_DMA(&ADCHandleCH2, (uint32_t *) DMABlockCH2, ADC_BLOCK_SIZE_12B);
    }
}

/**
 * Forces a trigger condition to occour.
 * @param channel
 */
void digiscope_adc_forcetrigger() {
    printf("Force trigger init\n");
    forcetrigger = 1;
}

/**
 * DMA Has completed a full capture
 * @param AdcHandle
 */
int prev;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
    if (forcetrigger == 1) {
        triggered = 1;
    }
    
    if (paused == 1) {
        return;
    }

    if (samplingmode == 8) {
        ADC_Trigger_FullConv_8B(AdcHandle);
    } else {
        ADC_Trigger_FullConv_12B(AdcHandle);
    }

}

/**
 * DMA Has completed the first half of the capture. We should empty the first half of the buffer here.
 * @param AdcHandle
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    if (paused == 1) {
        return;
    }

    if (forcetrigger == 1) {
        triggered = 1;
    }

    if (samplingmode == 8) {
        ADC_Trigger_HalfConv_8B(AdcHandle);
    } else {
        ADC_Trigger_HalfConv_12B(AdcHandle);
    }
}

void DMA2_Stream4_IRQHandler() {
    HAL_DMA_IRQHandler(&DMAHandleCH1);
}

void DMA2_Stream2_IRQHandler() {
    HAL_DMA_IRQHandler(&DMAHandleCH2);
}

void ADC_IRQHandler(ADC_HandleTypeDef* hadc) {
    HAL_ADC_IRQHandler(hadc);
}

void ADC_Start() {
    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_SINGLE) {
        paused = 1;
    } else {
        paused = 0;
    }
    forcetrigger = 0;
    triggered = 0;
    channel1_aftertrigger_counter = 0;
    channel1_beforetrigger_counter = 0;
    channel2_aftertrigger_counter = 0;
    channel2_beforetrigger_counter = 0;

    memset(channel1_beforetrigger, 0x00, 25000);
    memset(channel2_beforetrigger, 0x00, 25000);
    memset(channel1_aftertrigger, 0x00, 25000);
    memset(channel2_aftertrigger, 0x00, 25000);
}

void ADC_Unpause() {
    paused = 1;
}

void ADC_Trigger_HalfConv_8B(ADC_HandleTypeDef* AdcHandle) {
    if (AdcHandle == &ADCHandleCH1) {
        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;
            prev = DMABlockCH1[0];

            if (triggersettings.channel == 1) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = 0; i < ADC_BLOCK_SIZE / 2; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (DMABlockCH1[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (DMABlockCH1[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((DMABlockCH1[i] < triggersettings.level && prev > triggersettings.level)
                                    || (DMABlockCH1[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = DMABlockCH1[i];
                }

                i = ADC_BLOCK_SIZE / 2;
                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1 + i, sizeof (uint8_t) * i);
                channel1_beforetrigger_counter += i;

                if (channel1_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel1_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel1_beforetrigger_counter >= 24999) {
                    channel1_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel1_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel1_aftertrigger + channel1_aftertrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }

    } else {

        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;
            prev = DMABlockCH2[0];

            if (triggersettings.channel == 2) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = 0; i < ADC_BLOCK_SIZE / 2; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (DMABlockCH2[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (DMABlockCH2[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((DMABlockCH2[i] < triggersettings.level && prev > triggersettings.level)
                                    || (DMABlockCH2[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = DMABlockCH2[i];
                }

                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2 + i, sizeof (uint8_t) * i);
                channel2_beforetrigger_counter += i;

                if (channel2_beforetrigger_counter >= 24999) {
                    channel2_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel2_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel2_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel2_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel2_aftertrigger + channel2_aftertrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }
    }
}

void ADC_Trigger_FullConv_8B(ADC_HandleTypeDef* AdcHandle) {
    /* If we are receiving samples for Ch1 on that ADC*/
    if (AdcHandle == &ADCHandleCH1) {

        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;
            prev = DMABlockCH1[ADC_BLOCK_SIZE / 2];

            if (triggersettings.channel == 1) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = ADC_BLOCK_SIZE / 2; i < ADC_BLOCK_SIZE; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (DMABlockCH1[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (DMABlockCH1[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((DMABlockCH1[i] < triggersettings.level && prev > triggersettings.level)
                                    || (DMABlockCH1[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = DMABlockCH1[i];
                }
                i = ADC_BLOCK_SIZE / 2;
                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1 + i, sizeof (uint8_t) * i);
                channel1_beforetrigger_counter += i;

                if (channel1_beforetrigger_counter >= 24999) {
                    channel1_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1 + (ADC_BLOCK_SIZE / 2), sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel1_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel1_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel1_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel1_aftertrigger + channel1_aftertrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }


    } else {

        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;
            prev = DMABlockCH2[ADC_BLOCK_SIZE / 2];

            if (triggersettings.channel == 2) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = ADC_BLOCK_SIZE / 2; i < ADC_BLOCK_SIZE; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (DMABlockCH2[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (DMABlockCH2[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((DMABlockCH2[i] < triggersettings.level && prev > triggersettings.level)
                                    || (DMABlockCH2[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = DMABlockCH2[i];
                }
                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2 + i, sizeof (uint8_t) * i);
                channel2_beforetrigger_counter += i;

                if (channel2_beforetrigger_counter >= 24999) {
                    channel2_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2 + (ADC_BLOCK_SIZE / 2), sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel2_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel2_beforetrigger_counter = 0;
                }
            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel2_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel2_aftertrigger + channel2_aftertrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }
    }
}

void ADC_Trigger_FullConv_12B(ADC_HandleTypeDef* AdcHandle) {
    if (AdcHandle == &ADCHandleCH1) {
        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;

            /* Cast to the 16 bit value 
             * There should be ADC_BLOCK_SIZE_12B values stored
             * We need to deal with the first ADC_BLOCK_SIZE_12B/2 values
             */
            uint16_t* buf = (uint16_t*) DMABlockCH1;
            prev = buf[0];

            if (triggersettings.channel == 1) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = 0; i < ADC_BLOCK_SIZE_12B / 2; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (buf[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (buf[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((buf[i] < triggersettings.level && prev > triggersettings.level)
                                    || (buf[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = buf[i];
                }

                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1 + i, sizeof (uint8_t) * i);
                channel1_beforetrigger_counter += i;

                if (channel1_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel1_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel1_beforetrigger_counter >= 24999) {
                    channel1_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel1_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel1_aftertrigger + channel1_aftertrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }

    } else {

        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;

            uint16_t* buf = (uint16_t*) DMABlockCH2;
            prev = buf[0];

            if (triggersettings.channel == 2) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = 0; i < ADC_BLOCK_SIZE_12B / 2; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (buf[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (buf[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((buf[i] < triggersettings.level && prev > triggersettings.level)
                                    || (buf[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = buf[i];
                }

                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2 + i, sizeof (uint8_t) * i);
                channel2_beforetrigger_counter += i;

                if (channel2_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel2_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel2_beforetrigger_counter >= 24999) {
                    channel2_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel2_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel2_aftertrigger + channel2_aftertrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }
    }
}

void ADC_Trigger_HalfConv_12B(ADC_HandleTypeDef* AdcHandle) {
    if (AdcHandle == &ADCHandleCH1) {
        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;
            uint16_t* buf = (uint16_t*) DMABlockCH1;
            prev = buf[0];

            if (triggersettings.channel == 1) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = 0; i < ADC_BLOCK_SIZE_12B / 2; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (buf[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (buf[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((buf[i] < triggersettings.level && prev > triggersettings.level)
                                    || (buf[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = buf[i];
                }

                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1 + i, sizeof (uint8_t) * i);
                channel1_beforetrigger_counter += i;

                if (channel1_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel1_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel1_beforetrigger + channel1_beforetrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel1_beforetrigger_counter >= 24999) {
                    triggered = 1; // TODO AUTO MODE
                    channel1_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel1_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel1_aftertrigger + channel1_aftertrigger_counter, DMABlockCH1, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel1_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }

    } else {

        /* Check if we have already triggered */
        if (triggered == 0) { // Not triggered
            int i;

            uint16_t* buf = (uint16_t*) DMABlockCH2;
            prev = buf[0];

            if (triggersettings.channel == 2) {
                /* If we should attempt to trigger on this channel 
                    search for the triggering condition */

                /* For every 10 samples */
                for (i = 0; i < ADC_BLOCK_SIZE / 2; i += 10) {
                    if (triggered) {
                        break;
                    }

                    /* Analyze the data for trigger conditions */
                    switch (triggersettings.type) {
                        case SETTINGS_TRIGGER_RISING:
                            if (buf[i] > triggersettings.level && prev < triggersettings.level) {
                                // Rising trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_FALLING:
                            if (buf[i] < triggersettings.level && prev > triggersettings.level) {
                                // Falling trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                        case SETTINGS_TRIGGER_LEVEL:
                            if ((buf[i] < triggersettings.level && prev > triggersettings.level)
                                    || (buf[i] > triggersettings.level && prev < triggersettings.level)) {
                                // Both edge trigger condition happened
                                // Therefore stop here and keep the data up until this point
                                BSP_LED_Toggle(LED1);

                                triggered = 1;
                            }
                            break;
                    }

                    /* Save the previous sample for comparison later */
                    prev = buf[i];
                }

                /* Save the samples up until the trigger (if it occoured) */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2 + i, sizeof (uint8_t) * i);
                channel2_beforetrigger_counter += i;

                if (channel2_beforetrigger_counter >= 24999) {
                    channel2_beforetrigger_counter = 0;
                }

            } else {

                /* Just save the samples into the before buffer */
                memcpy(channel2_beforetrigger + channel2_beforetrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_beforetrigger_counter += ADC_BLOCK_SIZE / 2;

                if (channel2_beforetrigger_counter >= 24999) {
                    if (triggersettings.mode == SETTINGS_TRIGGER_MODE_AUTO) {
                        triggered = 1;
                    }
                    channel2_beforetrigger_counter = 0;
                }

            }

        } else {
            /* We have already triggered so save samples into the after trigger buffer */
            if (channel2_aftertrigger_counter > 24999) {

                /* All the samples have been aquired so pause sampling */
                triggered = 0;
                paused = 1;

                /* Aquisition has completed so fly a sempahore to indicate we should send samples to the PC */
                xSemaphoreGiveFromISR(NetSendTrigger, NULL);
            } else {

                /* We need to save more samples */
                memcpy(channel2_aftertrigger + channel2_aftertrigger_counter, DMABlockCH2, sizeof (uint8_t) * ADC_BLOCK_SIZE / 2);
                channel2_aftertrigger_counter += ADC_BLOCK_SIZE / 2;

            }
        }
    }
}