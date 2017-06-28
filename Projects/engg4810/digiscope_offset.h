/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 24, 2016
 * ============================================================================
 * Filename: digiscope_offset.h
 * ============================================================================
 */

#ifndef DIGISCOPE_OFFSET_H
#define DIGISCOPE_OFFSET_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "lowlevel_dac.h"

    void digiscope_offset_set(int ch, float voltage);

#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_OFFSET_H */

