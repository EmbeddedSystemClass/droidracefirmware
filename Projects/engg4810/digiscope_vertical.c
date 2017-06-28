/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 14, 2016
 * ============================================================================
 * Filename: digiscope_vertical.c
 * ============================================================================
 */

#include "digiscope_vertical.h"

float voltages[7] =  {0.02, 0.05, 0.1, 0.2, 0.5, 1, 2};
int gains1[7] =      {  10,    2,   2,   2,   2, 1, 1};
int gains2[7] =      {  10,   20,  10,   5,   2, 2, 1};

void digiscope_vertical_set(int channel, float voltsperdiv) {
    
    int i, gain1, gain2;
    for(i = 0; i < 7; i++) {
        if(voltages[i] == voltsperdiv) {
            gain1 = gains1[i];
            gain2 = gains2[i];
            break;
        }
    }
    
    if (gain1 == 0 || gain2 == 0) {
        printf("Invalid gain setting");
        return;
    }
    
    lowlevel_pga_setgain(channel, gain1, gain2);
}