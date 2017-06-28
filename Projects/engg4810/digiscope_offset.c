/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 24, 2016
 * ============================================================================
 * Filename: digiscope_offset.c
 * ============================================================================
 */

#include "digiscope_offset.h"

/**
 * Takes a channel and value and sets the DC offset in hardware
 * @param ch (1: CH1, 2: CH2, 3: Function Gen)
 * @param value (Floating point value for voltage)
 */
void digiscope_offset_set(int ch, float value) {
    
//    printf("Setting DC Offset to %d\n", (int)(value * 1000));
    
    /* Check values are not out of range */
    if((ch == 1 || ch == 2) && (value > 4.5 || value < -4.5)) {
        printf("Invalid range\n");
        return;
    }
    
    /* Check values are not out of range */
    if(ch == 3 && (value > 2.5 || value < -2.5)) {
        printf("Invalid range\n");
        return;
    }
    
    if (ch == 1) {
        
        float v1 = 0.0, v2 = 0.0;
        
        if(value == 0) {
            v1 = 5.0;
            v2 = 0.0;
        } else if (value > 0) {
            v1 = 5.0;
            v2 = value;     // Correct
        } else if (value < 0) {
            v1 = 0.0;
            v2 = value + 5.0; // Correct
        }
        
        lowlevel_dac_setvalue(LOWLEVEL_DAC_CH1_ID_2, (uint16_t)((v1/5.0)*1023));
        vTaskDelay(100);
            
        lowlevel_dac_setvalue(LOWLEVEL_DAC_CH1_ID_1, (uint16_t)((v2/5.0)*1023));
        vTaskDelay(100);
        
    } else if (ch == 2) {
        
    }
}