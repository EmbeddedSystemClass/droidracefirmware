/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: main.c
 * ============================================================================
 */


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void) {
    Hardware_init();

    digiscope_cli_init();
    digiscope_net_init();

    /* Start the LED Task */
    xTaskCreate((void *) &LEDTask, (const signed char *) "LED", LEDTASK_STACK_SIZE, NULL, LEDTASK_PRIORITY, NULL);

    /* Start scheduler */
    vTaskStartScheduler();

    return 0;
}

/**
 * Keep alive LED Task.
 * @param pvParam
 */
void LEDTask(void* pvParam) {
    int i = 0;

    for (;;) {
        vTaskDelay(1000);
        BSP_LED_Toggle(LED2);
    }
}

/**
 * @brief  Hardware Initialisation.
 * @param  None
 * @retval None
 */
void Hardware_init(void) {
    portDISABLE_INTERRUPTS();

    SystemClock_Config();

    /* Enable USER Led */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Enable HAL */
    if (HAL_Init() != HAL_OK) {
        /* If error turn on Red LED*/
        BSP_LED_On(LED3);
    }

    debugger_printf_init();

    portENABLE_INTERRUPTS();
}

/**
 * @brief  Idle Application Task (Disabled)
 * @param  None
 * @retval None
 */
void vApplicationIdleHook(void) {
    static TickType_t xLastTx = 0;

    // BRD_LEDOff();
    /* The idle hook simply prints the idle tick count */
    if ((xTaskGetTickCount() - xLastTx) > (1000 / portTICK_RATE_MS)) {
        xLastTx = xTaskGetTickCount();
        // debug_printf("IDLE Tick %d\n", xLastTx);
    }
}

/**
 * @brief  vApplicationStackOverflowHook
 * @param  Task Handler and Task Name
 * @retval None
 */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
    /* This function will get called if a task overflows its stack.   If the
    parameters are corrupt then inspect pxCurrentTCB to find which was the
    offending task. */

    // BRD_LEDOff();
    (void) pxTask;
    (void) pcTaskName;

    for (;;);
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        while (1) {
        };
    }

    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        while (1) {
        };
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        while (1) {
        };
    }
}