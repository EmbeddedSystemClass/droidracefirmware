/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 24, 2016
 * ============================================================================
 * Filename: lowlevel_mcp48fvb12.c
 * ============================================================================
 */

#include "lowlevel_dac.h"

extern SPI_HandleTypeDef SpiHandle;
extern SemaphoreHandle_t SPI1Mutex;

/* ==== Static Functions ==== */
void lowlevel_dac_cs(int id, int value) {
    switch(id) {
        case 1:
        case 2:
            HAL_GPIO_WritePin(LOWLEVEL_DAC_CH1_CS_PORT, LOWLEVEL_DAC_CH1_CS_PIN, value);
            break;
        case 3:
        case 4:
            HAL_GPIO_WritePin(LOWLEVEL_DAC_CH2_CS_PORT, LOWLEVEL_DAC_CH2_CS_PIN, value);
            break;
        case 5:
        case 6:
            HAL_GPIO_WritePin(LOWLEVEL_DAC_CH3_CS_PORT, LOWLEVEL_DAC_CH3_CS_PIN, value);
            break;
    }
}

/* ==== Public Functions ==== */

/**
 * Init all the DACs:
 *  - CH1 Offset
 *  - CH2 Offset
 *  - LCD Backlight
 */
void lowlevel_dac_init() {
    GPIO_InitTypeDef GPIO_spi;

    LOWLEVEL_DAC_CH1_CS_CLK();
    LOWLEVEL_DAC_CH2_CS_CLK();
    LOWLEVEL_DAC_CH3_CS_CLK();

    /* Configure GPIO PIN for SPI Chip select */
    GPIO_spi.Pin = LOWLEVEL_DAC_CH1_CS_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(LOWLEVEL_DAC_CH1_CS_PORT, &GPIO_spi); //Initialise Pin
    
    /* Configure GPIO PIN for SPI Chip select */
    GPIO_spi.Pin = LOWLEVEL_DAC_CH2_CS_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(LOWLEVEL_DAC_CH2_CS_PORT, &GPIO_spi); //Initialise Pin
    
    /* Configure GPIO PIN for SPI Chip select */
    GPIO_spi.Pin = LOWLEVEL_DAC_CH3_CS_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(LOWLEVEL_DAC_CH3_CS_PORT, &GPIO_spi); //Initialise Pin
    
    lowlevel_dac_cs(1, 1);
    lowlevel_dac_cs(3, 1);
    lowlevel_dac_cs(5, 1);
    
    lowlevel_dac_config(1);
    lowlevel_dac_config(3);
    lowlevel_dac_config(5);
}

void lowlevel_dac_setvalue(int id, int value) {
#ifdef DEBUG_LOWLEVEL_DAC
    printf("Lowlevel: %d, %d\n", id, value);
#endif
    if (xSemaphoreTake(SPI1Mutex, (TickType_t) 10) == pdTRUE) {
        
        // Assert SPI CS Line
        lowlevel_dac_cs(id, 0);
        
        /* Check which DAC value we are changing */
        int write_command = 0;
        if (id % 2 == 0) { // Even number
            write_command = LOWLEVEL_DAC_WRITE1_CMD;
        } else {
            write_command = LOWLEVEL_DAC_WRITE0_CMD;
        }

        uint8_t send[3] = {write_command, (value & 0xFF00) >> 8, value & 0x00FF};
        
#ifdef DEBUG_LOWLEVEL_DAC
        printf("Sending: %X %X %X\n", send[0], send[1], send [2]);
#endif /* DEBUG_LOWLEVEL_DAC */
        
        HAL_SPI_Transmit(&SpiHandle, send, 3, 1000);
        
        vTaskDelay(1);

        // De-Assert SPI CS Line
        lowlevel_dac_cs(id, 1);

        xSemaphoreGive(SPI1Mutex);
    } else {
        printf("Could not get SPIBus semph\n");
    }
}

static void lowlevel_dac_config(int id) {
    if (xSemaphoreTake(SPI1Mutex, (TickType_t) 10) == pdTRUE) {
        
        // Assert SPI CS Line
        lowlevel_dac_cs(id, 0);

        uint8_t send[3] = {0x0A, 0x00, 0x00};
        
        HAL_SPI_Transmit(&SpiHandle, send, 3, 1000);
        
        vTaskDelay(1);

        // De-Assert SPI CS Line
        lowlevel_dac_cs(id, 1);

        xSemaphoreGive(SPI1Mutex);
    } else {
        printf("Could not get SPIBus semph\n");
    }
}