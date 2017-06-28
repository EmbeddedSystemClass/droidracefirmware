/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 21, 2016
 * ============================================================================
 * Filename: digiscope_settings.h
 * ============================================================================
 */

#ifndef DIGISCOPE_SETTINGS_H
#define DIGISCOPE_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "digiscope_signalgen.h"
#include "stm32f4xx_nucleo_144.h"
#include "stm32f4xx_it.h"
#include "digiscope_offset.h"
#include "digiscope_vertical.h"
#include "digiscope_net.h"
#include "digiscope_lcd.h"
#include "debugger_printf.h"

#include "netconfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "tcpip.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip_init.h"

#define SETTING_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 2 )
#define SETTING_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )

    typedef struct trigset {
        int mode;
        int type;
        float level; // In volts
        int channel;
    } TriggerSettings_t;

    typedef struct chset {
        int coupling;
        int samplingmode;
        float verticalrange; // Volts per division
        float horizontalrange; // Time per division
        float offset;
    } ChannelSettings_t;

    typedef struct sigset {
        int enabled;
        SignalgenWaveform wave;
        float voltage;
        float offset;
        int frequency;
    } SignalGenSettings_t;

    typedef struct sysset {
        int backlightlevel;
        int backlightenabled;
        int samplerate;
        int samplemode;
        ChannelSettings_t channel1settings;
        ChannelSettings_t channel2settings;
        SignalGenSettings_t signalgensettings;
        TriggerSettings_t trigger;
    } SystemSettings_t;

#define SETTINGS_SAMPLING_8BIT 1
#define SETTINGS_SAMPLING_12BIT 2

#define SETTINGS_DC_COUPLING 1
#define SETTINGS_AC_COUPLING 0

#define SETTINGS_SAMPLING_MODE_LPF 0
#define SETTINGS_SAMPLING_MODE_BPF 1

#define SETTINGS_TRIGGER_MODE_OFF 0
#define SETTINGS_TRIGGER_MODE_AUTO 1
#define SETTINGS_TRIGGER_MODE_NORMAL 2
#define SETTINGS_TRIGGER_MODE_SINGLE 3

#define SETTINGS_TRIGGER_RISING 1
#define SETTINGS_TRIGGER_FALLING 2
#define SETTINGS_TRIGGER_LEVEL 3

    SystemSettings_t systemsettings;

    void SettingsTask();
    void digiscope_settings_propogate();
    void digiscope_settings_default();
    void send_func_settings();
    void SettingsPropTask();
	void process_recv_settings(int iden, int contents);


#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_SETTINGS_H */

