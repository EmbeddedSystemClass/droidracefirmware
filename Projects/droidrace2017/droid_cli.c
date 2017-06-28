/*
 * ============================================================================
 * Created by Timothy Ryan Hadwen
 * Created: Mar 25, 2016
 * ============================================================================
 */

extern struct netif xnetif;

#include "droid_cli.h"

/* CLI Commands */
static CLI_Command_Definition_t xEcho = {
    "echo",
    "echo: Echo the input.\r\n",
    prvEchoCommand,
    1
};

void droid_cli_init() {
    xTaskCreate((void *) &CLI_Task, (const signed char *) "CLI", mainCLI_TASK_STACK_SIZE, NULL, mainCLI_PRIORITY, NULL);

    FreeRTOS_CLIRegisterCommand(&xEcho);
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

