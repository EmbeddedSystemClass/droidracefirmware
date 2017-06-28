/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 21, 2016
 * ============================================================================
 * Filename: digiscope_settings.c
 * ============================================================================
 */

#include "digiscope_settings.h"

static int sock = 0;
static int tcpconn = 0;
static long size = 0;
static int ret = 0;
static int i;
char *output;
char *set;
int out;
int seti;
static uint8_t send_buffer[500];
static uint8_t recv_buffer[500];
float float_value;

static struct sockaddr_in address, remotehost;

void digiscope_settings_init() {
    xTaskCreate((void *) &SettingsTask, (const signed char *) "SET", SETTING_TASK_STACK_SIZE, NULL, SETTING_TASK_PRIORITY, NULL);
    xTaskCreate((void *) &SettingsPropTask, (const signed char *) "SET2", SETTING_TASK_STACK_SIZE, NULL, SETTING_TASK_PRIORITY, NULL);
    digiscope_settings_default();
}

void digiscope_settings_default() {
    systemsettings.channel1settings.coupling = SETTINGS_DC_COUPLING;
    systemsettings.channel1settings.horizontalrange = 0.1; // 0.1s per div (1s per screen)
    systemsettings.channel1settings.samplingmode = SETTINGS_SAMPLING_MODE_LPF;
    systemsettings.channel1settings.verticalrange = 0.5; // 2v per div
    systemsettings.channel1settings.offset = 0.0;

    systemsettings.channel2settings.coupling = SETTINGS_DC_COUPLING;
    systemsettings.channel2settings.horizontalrange = 0.1; // 0.1s per div (1s per screen)
    systemsettings.channel2settings.samplingmode = SETTINGS_SAMPLING_MODE_LPF;
    systemsettings.channel2settings.verticalrange = 0.5; // 2v per div
    systemsettings.channel2settings.offset = 0.0;

    systemsettings.trigger.mode = SETTINGS_TRIGGER_MODE_NORMAL;
    systemsettings.trigger.level = 125;
    systemsettings.trigger.type = SETTINGS_TRIGGER_RISING;
    systemsettings.trigger.channel = 1;

    systemsettings.backlightenabled = 5;
    systemsettings.backlightlevel = 1;
    systemsettings.samplerate = 1000000;
    systemsettings.samplemode = 8;

    systemsettings.signalgensettings.enabled = 1;
    systemsettings.signalgensettings.frequency = 1000;
    systemsettings.signalgensettings.offset = 0;
    systemsettings.signalgensettings.voltage = 2; //2v
    systemsettings.signalgensettings.wave = WAVE_SINE;
}

/**
 * Sends all settings to all modules
 */
void digiscope_settings_propogate() {

    /* System Settings */
    digiscope_lcd_backlight(systemsettings.backlightlevel);
    digiscope_adc_samplingrate(systemsettings.samplerate);

    /* Trigger settings */
    digiscope_adc_trigger_settings(&(systemsettings.trigger));

    /* Channel 1 Settings */
    digiscope_mux_coupling(1, systemsettings.channel1settings.coupling);
    digiscope_vertical_set(1, systemsettings.channel1settings.verticalrange);
    // Horizontal Range
    digiscope_mux_filtering(systemsettings.channel1settings.samplingmode);
    digiscope_offset_set(1, systemsettings.channel1settings.offset);

    /* Channel 2 Settings */
    digiscope_mux_coupling(2, systemsettings.channel2settings.coupling);
    digiscope_vertical_set(2, systemsettings.channel2settings.verticalrange);
    // Horizontal Range
    digiscope_offset_set(2, systemsettings.channel2settings.offset);


    /* Signal Gen Settings */
    digiscope_signalgen_setenabled(systemsettings.signalgensettings.enabled);
    digiscope_signalgen_setamp(systemsettings.signalgensettings.voltage * 10);
    digiscope_signalgen_setfreq(systemsettings.signalgensettings.frequency);
    digiscope_signalgen_setoffset(systemsettings.signalgensettings.offset);
    digiscope_signalgen_setwave(systemsettings.signalgensettings.wave);
}

void SettingsPropTask() {
    for (;;) {
        digiscope_settings_propogate();
    }
}

void SettingsTask() {

    /* create a TCP socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Could not create settings socket\n");
        return;
    }

    /* bind to port 80 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(3001);
    address.sin_addr.s_addr = INADDR_ANY;
    size = sizeof (remotehost);

    if (bind(sock, (const struct sockaddr *) (&address), sizeof (address)) < 0) {
        printf("Could not bind settings port\n");
        return;
    }

    // Listen on socket for 1 connection	
    listen(sock, 1);

    for (;;) {
        int counter = 0;

        tcpconn = accept(sock, (struct sockaddr *) (&remotehost), (socklen_t *) & size);

        digiscope_lcd_set_accept();
        while (1) {

            ret = lwip_recv(tcpconn, recv_buffer, sizeof (recv_buffer), MSG_DONTWAIT);
            if (ret >= 1) {
                set = strtok(recv_buffer, " ");
                output = strtok(NULL, " ");
                seti = atoi(set);
                out = atoi(output);
                printf("%d, %d", seti, out);
                process_recv_settings(seti, out);
            }
            if(counter > 200) {
                counter = 0;
                send_func_settings();
            }
            counter++;
            vTaskDelay(10);
        }
    }
}

void send_func_settings() {
    int functiongewave = systemsettings.signalgensettings.wave;
    int len = sprintf(send_buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n\0",
            systemsettings.backlightlevel, systemsettings.backlightenabled, systemsettings.samplerate,
            systemsettings.samplemode, systemsettings.channel1settings.coupling, systemsettings.channel1settings.samplingmode,
            (int) (systemsettings.channel1settings.verticalrange * 1000000), (int) (systemsettings.channel1settings.horizontalrange * 1000000.0) + 1,
            (int) (systemsettings.channel1settings.offset * 1000000), systemsettings.channel2settings.coupling, systemsettings.channel2settings.samplingmode,
            (int) (systemsettings.channel2settings.verticalrange * 1000000), 0, (int) (systemsettings.channel2settings.offset * 1000000), systemsettings.signalgensettings.enabled,
            functiongewave, (int) (systemsettings.signalgensettings.voltage * 1000000), (int) (systemsettings.signalgensettings.offset * 1000000), systemsettings.signalgensettings.frequency,
            systemsettings.trigger.mode, systemsettings.trigger.type, (int) (systemsettings.trigger.level * 1000000), systemsettings.trigger.channel);
    printf("%s", send_buffer);
    write(tcpconn, send_buffer, sizeof (uint8_t) * len);
    printf("Sent\n");
}

void process_recv_settings(int iden, int contents) {
    switch (iden) {
        case 0:
            systemsettings.backlightlevel = contents;
            break;
        case 1:
            systemsettings.backlightenabled = contents;
            break;
        case 2:
            systemsettings.samplerate = contents;
            break;
        case 3:
            systemsettings.samplemode = contents;
            break;
        case 4:
            systemsettings.channel1settings.coupling = contents;
            break;
        case 5:
            systemsettings.channel1settings.samplingmode = contents;
            break;
        case 6:
            float_value = (float) contents / 1000000.0;
            systemsettings.channel1settings.verticalrange = float_value;
            break;
        case 7:
            float_value = (float) contents / 1000000.0;
            printf("Setting horiz: %d\n", (int)contents);
            systemsettings.channel1settings.horizontalrange = float_value;
            break;
        case 8:
            float_value = (float) contents / 1000000.0;
            systemsettings.channel1settings.offset = float_value;
            break;
        case 9:
            systemsettings.channel2settings.coupling = contents;
            break;
        case 10:
            systemsettings.channel2settings.samplingmode = contents;
            break;
        case 11:
            float_value = (float) contents / 1000000.0;
            systemsettings.channel2settings.verticalrange = float_value;
            break;
        case 12:
            float_value = (float) contents / 1000000.0;
            systemsettings.channel2settings.horizontalrange = float_value;
            break;
        case 13:
            float_value = (float) contents / 1000000.0;
            systemsettings.channel2settings.offset = float_value;
            break;
        case 14:
            systemsettings.signalgensettings.enabled = contents;
            break;
        case 15:
            systemsettings.signalgensettings.wave = (SignalgenWaveform) contents;
            break;
        case 16:
            float_value = (float) contents / 1000000.0;
            systemsettings.signalgensettings.voltage = float_value;
            break;
        case 17:
            float_value = (float) contents / 1000000.0;
            systemsettings.signalgensettings.offset = float_value;
            break;
        case 18:
            systemsettings.signalgensettings.frequency = contents;
            break;
        case 19:
            systemsettings.trigger.mode = contents;
            break;
        case 20:
            systemsettings.trigger.type = contents;
            break;
        case 21:
            float_value = (float) contents / 1000000.0;
            systemsettings.trigger.level = float_value;
            break;
        case 22:
            systemsettings.trigger.channel = contents;
            break;
		case 23:
            digiscope_adc_forcetrigger();
            break;
    }
}
