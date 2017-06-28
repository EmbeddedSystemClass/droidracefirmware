/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 25, 2016
 * ============================================================================
 * Filename: digiscope_cli.c
 * ============================================================================
 */

extern struct netif xnetif;

#include "digiscope_cli.h"
#include "digiscope_vertical.h"
#include "digiscope_offset.h"

/* CLI Commands */
static CLI_Command_Definition_t xEcho = {
    "echo",
    "echo: Echo the input.\r\n",
    prvEchoCommand,
    1
};

/* Function Generator */
static CLI_Command_Definition_t xFreq = {
    "setfreq",
    "setfreq: Set the signal gen freq.\r\n",
    prvSigFreqCommand,
    1
};

static CLI_Command_Definition_t xWave = {
    "setwave",
    "setwave: sine, square, triangle, noise, sawtooth\r\n",
    prvSigWaveCommand,
    1
};

static CLI_Command_Definition_t xAmp = {
    "setamp",
    "setamp: 0.1 0.2 0.5 1 or 2\r\n",
    prvSigAmpCommand,
    1
};

/* LwIP */
static CLI_Command_Definition_t xGetIP = {
    "getip",
    "getip: Returns the current ip\r\n",
    prvGetIpCommand,
    0
};

/* PGA Set */
static CLI_Command_Definition_t xPGASet = {
    "setpga",
    "setpga: <1,2> <Gain> Set the gain (0.02 -> 2) (1,2,5) \r\n",
    prvSetPGACommand,
    2
};

static CLI_Command_Definition_t xOffsetSet = {
    "setoffset",
    "setoffset: <1,2,F> <Voltage> Set the DC offset for each channel\r\n",
    prvSetOffsetCommand,
    2
};

static CLI_Command_Definition_t xCoupling = {
    "setcoupling",
    "setcoupling: <1, 2> <DC, AC>\r\n",
    prvSetCouplingCommand,
    2
};

void digiscope_cli_init() {
    xTaskCreate((void *) &CLI_Task, (const signed char *) "CLI", mainCLI_TASK_STACK_SIZE, NULL, mainCLI_PRIORITY, NULL);

    FreeRTOS_CLIRegisterCommand(&xEcho);
    FreeRTOS_CLIRegisterCommand(&xFreq);
    FreeRTOS_CLIRegisterCommand(&xWave);
    FreeRTOS_CLIRegisterCommand(&xAmp);
    FreeRTOS_CLIRegisterCommand(&xGetIP);
    FreeRTOS_CLIRegisterCommand(&xPGASet);
    FreeRTOS_CLIRegisterCommand(&xOffsetSet);
    FreeRTOS_CLIRegisterCommand(&xCoupling);
}

/**
 * @brief  CLI Receive Task.
 * @param  None
 * @retval None
 */
void CLI_Task(void) {

    char cRxedChar;
    char cInputString[100];
    int InputIndex = 0;
    char *pcOutputString;
    BaseType_t xReturned;

    /* Initialise pointer to CLI output buffer. */
    memset(cInputString, 0, sizeof (cInputString));
    pcOutputString = FreeRTOS_CLIGetOutputBuffer();

    for (;;) {

        /* Receive character */
        cRxedChar = debugger_printf_getc();

        /* Process if char if not Null */
        if (cRxedChar != '\0') {

            /* Put byte into USB buffer */
            debugger_printf_putc(cRxedChar);

            /* Process only if return is received. */
            if (cRxedChar == '\r') {

                //Put new line and transmit buffer
                debugger_printf_putc('\n');

                /* Put null character in command input string. */
                cInputString[InputIndex] = '\0';

                xReturned = pdTRUE;
                /* Process command input string. */
                while (xReturned != pdFALSE) {

                    /* Returns pdFALSE, when all strings have been returned */
                    xReturned = FreeRTOS_CLIProcessCommand(cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE);

                    /* Display CLI output string */
                    printf("%s\n\r", pcOutputString);
                    vTaskDelay(5); //Must delay between debug_printfs.
                }

                memset(cInputString, 0, sizeof (cInputString));
                InputIndex = 0;

            } else if (cRxedChar == '\b') {

                /* Backspace was pressed.  Erase the last character in the
                 string - if any.*/
                if (InputIndex > 0) {
                    InputIndex--;
                    cInputString[ InputIndex ] = '\0';
                }

                debugger_printf_putc(8);
                printf(" ");
                debugger_printf_putc(8);

            } else if (cRxedChar == 3) {
                /* Ctrl-C was pressed */
                printf(".Cancelled...\n\n");
                InputIndex = 0;
                memset(cInputString, 0, sizeof (cInputString));

            } else {

                /* A character was entered.  Add it to the string
                   entered so far.  When a \n is entered the complete
                   string will be passed to the command interpreter. */
                if (InputIndex < 20) {
                    cInputString[ InputIndex ] = cRxedChar;
                    InputIndex++;
                }
            }
        }

        vTaskDelay(10);
    }
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvSigFreqCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

    digiscope_signalgen_setfreq(atoi(cCmd_string));

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rDone\n\r");

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvSigWaveCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

    SignalgenWaveform wave;
    if (strcmp(cCmd_string, "sine") == 0) {
        wave = WAVE_SINE;
    } else if (strncmp(cCmd_string, "sq", 2) == 0) {
        wave = WAVE_SQUARE;
    } else if (strncmp(cCmd_string, "no", 2) == 0 || strncmp(cCmd_string, "rand", 4) == 0) {
        wave = WAVE_NOISE;
    } else if (strncmp(cCmd_string, "sa", 2) == 0) {
        wave = WAVE_SAWTOOTH;
    } else if (strncmp(cCmd_string, "tr", 2) == 0 || strncmp(cCmd_string, "ramp", 4) == 0) {
        wave = WAVE_TRIANGLE;
    } else {
        // Invalid wave
        /* Write command echo output string to write buffer. */
        xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rInvalid Wave.\n\r");
        return pdFALSE;
    }

    digiscope_signalgen_setwave(wave);

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rDone\n\r");

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvGetIpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%d.%d.%d.%d\n\r", 0xff & xnetif.ip_addr.addr, 0xff & (xnetif.ip_addr.addr >> 8), 0xff & (xnetif.ip_addr.addr >> 16), 0xff & (xnetif.ip_addr.addr >> 24));
    return pdFALSE;
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvSetPGACommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);
    char number = cCmd_string[0];
        
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 2, &lParam_len);
    float value;
    sscanf(cCmd_string, "%f", &value);
    
    switch(number) {
        case '1':
            systemsettings.channel1settings.verticalrange = value;
            break;
        case '2':
            systemsettings.channel2settings.verticalrange = value;
            break;
        default:
            xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rInvalid Channel %d\n\r", number);
            return pdFALSE;
    }

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rDone\n\r");

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvSetOffsetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);
    char number = 0;
    
    memcpy(&number, cCmd_string, 1);
        
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 2, &lParam_len);
    float value;
    sscanf(cCmd_string, "%f", &value);
    
    switch(number) {
        case '1':
            systemsettings.channel1settings.offset = value;
            break;
        case '2':
            systemsettings.channel2settings.offset = value;
            break;
        case 'F':
        case 'f':
            systemsettings.signalgensettings.offset = value;
            break;
        default:
           xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rInvalid Channel\n\r %d", number);
           return pdFALSE;
    }
    
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rDone\n\r");

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

/**
 * @brief  Echo Command.
 * @param  writebuffer, writebuffer length and command strength
 * @retval None
 */
static BaseType_t prvSigAmpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

    float value;
    sscanf(cCmd_string, "%f", &value);

    digiscope_signalgen_setamp(value);

    /* Write command echo output string to write buffer. */
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rDone: %d\n\r", (int) (value * 100));

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}

static BaseType_t prvSetCouplingCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {

    long lParam_len;
    const char *cCmd_string;

    /* Get parameters from command string */
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);
    char number = cCmd_string[0];
        
    cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 2, &lParam_len);
    
    int coupling;
    if (strncmp(cCmd_string, "AC", 2)) {
        coupling = SETTINGS_AC_COUPLING;
    } else if (strncmp(cCmd_string, "DC", 2)) {
        coupling = SETTINGS_DC_COUPLING;
    }
    
    switch(number) {
        case '1':
            systemsettings.channel1settings.coupling = coupling;
            break;
        case '2':
            systemsettings.channel2settings.coupling = coupling;
            break;
        default:
           xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rInvalid Channel %d\n\r", number);
           return pdFALSE;
    }
    
    xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\rDone\n\r");

    /* Return pdFALSE, as there are no more strings to return */
    /* Only return pdTRUE, if more strings need to be printed */
    return pdFALSE;
}