/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 20, 2016
 * ============================================================================
 * Filename: digiscope_lcd.c
 * ============================================================================
 */

#include "digiscope_lcd.h"

volatile int TP_X, TP_Y;

int accept_flag;
int menu;
int backlight;
int chan;
char char_chan;
float v_range;
float h_range;
float pk_pk;
float off;
int frequency;
float thresh;
int s_thresh_count;
int s_off_count;
char ipaddress[20];

char *vrange[2][5] = {
    {"     "},
    {"     "}};
char *hrange[2][5] = {
    {"    "},
    {"    "}};
char *pktopk = "     ";
char freq[] = "                ";
char offset[] = "        ";
char threshold[] = "        ";
int freq_count;
int off_count;
int thresh_count;
char *coupling[2][2] = {
    {"  "},
    {"  "}};
char *trigger_mode = "      ";
char *trigger_type = "       ";
char *output = "   ";
char *wave = "        ";

/**
 * Starts any of the required tasks for the LCD module
 * @param None?
 */
void digiscope_lcd_init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    /*initialise pins*/
    GPIO_InitStructure.Pin = DIGISCOPE_LCD_RS_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_RS_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_WR_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_WR_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_CS_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_CS_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_RST_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_RST_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB0_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB0_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB1_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB1_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB2_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB2_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB3_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB3_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB4_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB4_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB5_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB5_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB6_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB6_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB7_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB7_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB8_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB8_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB9_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB9_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB10_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB10_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB11_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB11_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB12_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB12_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB13_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB13_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB14_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB14_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_LCD_DB15_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_LCD_DB15_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_TS_CLK_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_TS_CLK_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_TS_CS_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_TS_CS_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_TS_DIN_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_TS_DIN_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_TS_DOUT_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //Input Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_TS_DOUT_PORT, &GPIO_InitStructure); //Initialise Pin

    GPIO_InitStructure.Pin = DIGISCOPE_TS_IRQ_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //Input Mode
    GPIO_InitStructure.Pull = GPIO_NOPULL; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_TS_IRQ_PORT, &GPIO_InitStructure); //Initialise Pin

    digiscope_lcd_setrst();
    HAL_Delay(200);
    digiscope_lcd_clearrst();
    HAL_Delay(200);
    digiscope_lcd_setrst();
    digiscope_lcd_setcs();
    digiscope_lcd_setwr();
    digiscope_lcd_writecommanddata(0x0000, 0x0001);
    digiscope_lcd_writecommanddata(0x0003, 0xA8A4);
    digiscope_lcd_writecommanddata(0x000C, 0x0000);
    digiscope_lcd_writecommanddata(0x000D, 0x080C);
    digiscope_lcd_writecommanddata(0x000E, 0x2B00);
    digiscope_lcd_writecommanddata(0x001E, 0x00B7);
    digiscope_lcd_writecommanddata(0x0001, 0x2B3F);
    digiscope_lcd_writecommanddata(0x0002, 0x0600);
    digiscope_lcd_writecommanddata(0x0010, 0x0000);
    digiscope_lcd_writecommanddata(0x0011, 0x6070);
    digiscope_lcd_writecommanddata(0x0005, 0x0000);
    digiscope_lcd_writecommanddata(0x0006, 0x0000);
    digiscope_lcd_writecommanddata(0x0016, 0xEF1C);
    digiscope_lcd_writecommanddata(0x0017, 0x0003);
    digiscope_lcd_writecommanddata(0x0007, 0x0233);
    digiscope_lcd_writecommanddata(0x000B, 0x0000);
    digiscope_lcd_writecommanddata(0x000F, 0x0000);
    digiscope_lcd_writecommanddata(0x0041, 0x0000);
    digiscope_lcd_writecommanddata(0x0042, 0x0000);
    digiscope_lcd_writecommanddata(0x0048, 0x0000);
    digiscope_lcd_writecommanddata(0x0049, 0x013F);
    digiscope_lcd_writecommanddata(0x004A, 0x0000);
    digiscope_lcd_writecommanddata(0x004B, 0x0000);
    digiscope_lcd_writecommanddata(0x0044, 0xEF00);
    digiscope_lcd_writecommanddata(0x0045, 0x0000);
    digiscope_lcd_writecommanddata(0x0046, 0x013F);
    digiscope_lcd_writecommanddata(0x0030, 0x0707);
    digiscope_lcd_writecommanddata(0x0031, 0x0204);
    digiscope_lcd_writecommanddata(0x0032, 0x0204);
    digiscope_lcd_writecommanddata(0x0033, 0x0502);
    digiscope_lcd_writecommanddata(0x0034, 0x0507);
    digiscope_lcd_writecommanddata(0x0035, 0x0204);
    digiscope_lcd_writecommanddata(0x0036, 0x0204);
    digiscope_lcd_writecommanddata(0x0037, 0x0502);
    digiscope_lcd_writecommanddata(0x003A, 0x0302);
    digiscope_lcd_writecommanddata(0x003B, 0x0302);
    digiscope_lcd_writecommanddata(0x0023, 0x0000);
    digiscope_lcd_writecommanddata(0x0024, 0x0000);
    digiscope_lcd_writecommanddata(0x0025, 0x8000);
    digiscope_lcd_writecommanddata(0x004f, 0x0000);
    digiscope_lcd_writecommanddata(0x004e, 0x0000);
    digiscope_lcd_writecommand(0x0022);

    digiscope_lcd_settscs();
    digiscope_lcd_settsclk();
    digiscope_lcd_settsdin();
    digiscope_lcd_settsclk();
    digiscope_lcd_paint(0x0000);
	backlight = 1;
    chan = 0;
    char_chan = '1';
    digiscope_lcd_write_menu_home();
}

void digiscope_lcd_setcs() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_CS_PORT, DIGISCOPE_LCD_CS_PIN, 0x01);
}

void digiscope_lcd_clearcs() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_CS_PORT, DIGISCOPE_LCD_CS_PIN, 0x00);
}

void digiscope_lcd_setrs() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_RS_PORT, DIGISCOPE_LCD_RS_PIN, 0x01);
}

void digiscope_lcd_clearrs() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_RS_PORT, DIGISCOPE_LCD_RS_PIN, 0x00);
}

void digiscope_lcd_setwr() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_WR_PORT, DIGISCOPE_LCD_WR_PIN, 0x01);
}

void digiscope_lcd_clearwr() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_WR_PORT, DIGISCOPE_LCD_WR_PIN, 0x00);
}

void digiscope_lcd_setrst() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_RST_PORT, DIGISCOPE_LCD_RST_PIN, 0x01);
}

void digiscope_lcd_clearrst() {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_RST_PORT, DIGISCOPE_LCD_RST_PIN, 0x00);
}

void digiscope_lcd_cleartsdin() {
    HAL_GPIO_WritePin(DIGISCOPE_TS_DIN_PORT, DIGISCOPE_TS_DIN_PIN, 0x00);
}

void digiscope_lcd_settsdin() {
    HAL_GPIO_WritePin(DIGISCOPE_TS_DIN_PORT, DIGISCOPE_TS_DIN_PIN, 0x01);
}

void digiscope_lcd_cleartsclk() {
    HAL_GPIO_WritePin(DIGISCOPE_TS_CLK_PORT, DIGISCOPE_TS_CLK_PIN, 0x00);
}

void digiscope_lcd_settsclk() {
    HAL_GPIO_WritePin(DIGISCOPE_TS_CLK_PORT, DIGISCOPE_TS_CLK_PIN, 0x01);
}

void digiscope_lcd_cleartscs() {
    HAL_GPIO_WritePin(DIGISCOPE_TS_CS_PORT, DIGISCOPE_TS_CS_PIN, 0x00);
}

void digiscope_lcd_settscs() {
    HAL_GPIO_WritePin(DIGISCOPE_TS_CS_PORT, DIGISCOPE_TS_CS_PIN, 0x01);
}

void digiscope_lcd_writebus(uint16_t data) {
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB0_PORT, DIGISCOPE_LCD_DB0_PIN, (data & 0x01));
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB1_PORT, DIGISCOPE_LCD_DB1_PIN, (data & 0x02) >> 1);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB2_PORT, DIGISCOPE_LCD_DB2_PIN, (data & 0x04) >> 2);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB3_PORT, DIGISCOPE_LCD_DB3_PIN, (data & 0x08) >> 3);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB4_PORT, DIGISCOPE_LCD_DB4_PIN, (data & 0x10) >> 4);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB5_PORT, DIGISCOPE_LCD_DB5_PIN, (data & 0x20) >> 5);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB6_PORT, DIGISCOPE_LCD_DB6_PIN, (data & 0x40) >> 6);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB7_PORT, DIGISCOPE_LCD_DB7_PIN, (data & 0x80) >> 7);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB8_PORT, DIGISCOPE_LCD_DB8_PIN, (data & 0x0100) >> 8);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB9_PORT, DIGISCOPE_LCD_DB9_PIN, (data & 0x0200) >> 9);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB10_PORT, DIGISCOPE_LCD_DB10_PIN, (data & 0x0400) >> 10);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB11_PORT, DIGISCOPE_LCD_DB11_PIN, (data & 0x0800) >> 11);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB12_PORT, DIGISCOPE_LCD_DB12_PIN, (data & 0x1000) >> 12);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB13_PORT, DIGISCOPE_LCD_DB13_PIN, (data & 0x2000) >> 13);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB14_PORT, DIGISCOPE_LCD_DB14_PIN, (data & 0x4000) >> 14);
    HAL_GPIO_WritePin(DIGISCOPE_LCD_DB15_PORT, DIGISCOPE_LCD_DB15_PIN, (data & 0x8000) >> 15);
}

void digiscope_lcd_writecommand(unsigned int c) {
    digiscope_lcd_clearcs();
    digiscope_lcd_clearrs();
    digiscope_lcd_writebus(c);
    digiscope_lcd_clearwr();
    HAL_Delayus(1);
    digiscope_lcd_setwr();
    digiscope_lcd_setcs();
}

void digiscope_lcd_writedata(unsigned int c) {
    digiscope_lcd_clearcs();
    digiscope_lcd_setrs();
    digiscope_lcd_writebus(c);
    digiscope_lcd_clearwr();
    HAL_Delayus(1);
    digiscope_lcd_setwr();
    digiscope_lcd_setcs();
}

void digiscope_lcd_writecommanddata(unsigned int cmd, unsigned int dat) {
    digiscope_lcd_writecommand(cmd);
    digiscope_lcd_writedata(dat);
}

void digiscope_lcd_setxy(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
    digiscope_lcd_writecommanddata(0x0044, (x1 << 8) + x0);
    digiscope_lcd_writecommanddata(0x0045, y0);
    digiscope_lcd_writecommanddata(0x0046, y1);
    digiscope_lcd_writecommanddata(0x004e, x0);
    digiscope_lcd_writecommanddata(0x004f, y0);
    digiscope_lcd_writecommand(0x0022);
}

void digiscope_lcd_paint(unsigned int color) {
    int i, j;
    digiscope_lcd_setxy(0, 0, 239, 319);
    for (i = 0; i < 320; i++) {
        for (j = 0; j < 240; j++) {
            digiscope_lcd_writedata(color);
        }
    }
}

void digiscope_lcd_rectangle(unsigned int color, int x1, int y1, int x2, int y2) {
    int i, j;
    digiscope_lcd_setxy(x1, y1, x2, y2);
    for (i = x1; i <= x2; i++) {
        for (j = y1; j <= y2; j++) {
            digiscope_lcd_writedata(color);
        }
    }
}

void digiscope_lcd_clear() {
    unsigned int i, j;
    digiscope_lcd_setxy(0, 0, 239, 319);
    for (i = 0; i < 320; i++) {
        for (j = 0; j < 240; j++) {
            digiscope_lcd_writedata(BLACK);
        }
    }
}

void digiscope_lcd_write_char(int x, int y, unsigned int colour, char c) {
    unsigned int i, j, temp, l;
    l = 0;
    for (i = 0; i < 48; i++) {
        if (c == lookup[i]) {
            l = i;
            break;
        }
    }
    for (i = 0; i < 12; i++) {
        temp = alphabet[l][i];
        for (j = 0; j < 8; j++) {
            if (temp & 0x01) {
                digiscope_lcd_setxy(x + i, y + j, x + i, y + j);
                digiscope_lcd_writedata(colour);
            }
            temp = temp >> 1;
        }
    }
}

void digiscope_lcd_write_string(int x, int y, unsigned int colour, char* c, int len) {
    int i;
    for (i = 0; i < len; i++) {
        digiscope_lcd_write_char(x, y - (8 * i), colour, c[i]);
    }
}

void digiscope_lcd_set_accept(){
	accept_flag = 1;
	digiscope_lcd_write_menu_home();
}

void digiscope_lcd_write_menu_home() {
    int i;
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 174, WHITE, "HOME", 4);
    digiscope_lcd_rectangle(WHITE, 5, 14, 50, 82);
    digiscope_lcd_write_string(20, 48, BLACK, "CH", 2);
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 272, BLACK, "SIGNAL", 6);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "TRIGGER", 7);
    digiscope_lcd_rectangle(WHITE, 150, 208, 210, 292);
    digiscope_lcd_write_string(174, 274, BLACK, "FUNCTION", 8);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "DISPLAY", 7);
    sprintf((char *) ipaddress, "IP: %d.%d.%d.%dg", 0xff & xnetif.ip_addr.addr, 0xff & (xnetif.ip_addr.addr >> 8), 0xff & (xnetif.ip_addr.addr >> 16), 0xff & (xnetif.ip_addr.addr >> 24));
    for (i = 0; i < 100; i++) {
        if (ipaddress[i] == 'g') {
            break;
        }
    }
    digiscope_lcd_write_string(220, 310, GREEN, ipaddress, i + 1);
	if (accept_flag){
		digiscope_lcd_write_string(220, 164, RED, "ACCEPTED", 8);
	}
    digiscope_lcd_write_string(220, 90, WHITE, "CHANNEL:", 8);
    digiscope_lcd_write_char(220, 25, WHITE, char_chan);
    menu = 0;
}

void digiscope_lcd_write_menu_warning() {
    digiscope_lcd_clear();
    digiscope_lcd_paint(RED);
    digiscope_lcd_write_string(20, 248, BLACK, "WARNING EXCESS VOLTAGE", 22);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "RETURN", 6);
    menu = -1;
}

void digiscope_lcd_write_menu_signal() {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 190, WHITE, "SIGNAL", 6);
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 278, BLACK, "COUPLING", 8);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "H RANGE", 7);
    digiscope_lcd_rectangle(WHITE, 150, 208, 210, 292);
    digiscope_lcd_write_string(174, 278, BLACK, "V RANGE", 7);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "BACK", 4);
    menu = 1;
}

void digiscope_lcd_write_menu_signal_coupling(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "COUPLING:", 9);
    if (flag) {
        digiscope_lcd_write_string(20, 160, RED, *coupling[chan], 2);
    }
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 278, BLACK, "AC", 2);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "DC", 2);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "BACK", 4);
    menu = 9;
}

void digiscope_lcd_write_menu_display() {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 190, WHITE, "DISPLAY", 7);
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 278, BLACK, "BRIGHT +", 8);
    digiscope_lcd_rectangle(WHITE, 150, 208, 210, 292);
    digiscope_lcd_write_string(174, 278, BLACK, "BRIGHT -", 8);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "OFF", 3);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "BACK", 4);
    menu = 2;
}

void digiscope_lcd_write_menu_trigger() {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 190, WHITE, "TRIGGER", 8);
    digiscope_lcd_rectangle(WHITE, 60, 224, 120, 310);
    digiscope_lcd_write_string(84, 272, BLACK, "MODE", 4);
    digiscope_lcd_rectangle(WHITE, 60, 118, 120, 204);
    digiscope_lcd_write_string(84, 180, BLACK, "TYPE", 4);
    digiscope_lcd_rectangle(WHITE, 60, 12, 120, 98);
    digiscope_lcd_write_string(84, 80, BLACK, "THRESHOLD", 9);
    digiscope_lcd_rectangle(WHITE, 150, 224, 210, 310);
    digiscope_lcd_write_string(174, 272, BLACK, "ARM", 3);
    digiscope_lcd_rectangle(WHITE, 150, 118, 210, 204);
    digiscope_lcd_write_string(174, 180, BLACK, "FORCE", 5);
    digiscope_lcd_rectangle(WHITE, 150, 12, 210, 98);
    digiscope_lcd_write_string(174, 80, BLACK, "BACK", 4);
    menu = 3;
}

void digiscope_lcd_write_menu_trigger_mode(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "TRIGGER MODE:", 13);
    if (flag) {
        digiscope_lcd_write_string(20, 130, RED, trigger_mode, 6);
    }
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 278, BLACK, "AUTO", 4);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "NORMAL", 6);
    digiscope_lcd_rectangle(WHITE, 150, 208, 210, 292);
    digiscope_lcd_write_string(174, 278, BLACK, "SINGLE", 7);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "BACK", 4);
    menu = 5;
}

void digiscope_lcd_write_menu_trigger_type(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "TRIGGER TYPE:", 13);
    if (flag) {
        digiscope_lcd_write_string(20, 130, RED, trigger_type, 7);
    }
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 278, BLACK, "FALLING", 7);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "RISING", 6);
    digiscope_lcd_rectangle(WHITE, 150, 208, 210, 292);
    digiscope_lcd_write_string(174, 278, BLACK, "LEVEL", 5);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "BACK", 4);
    menu = 6;
}

void digiscope_lcd_write_menu_trigger_threshold(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 300, WHITE, "THRESHOLD:", 10);
    if (flag) {
        digiscope_lcd_write_string(20, 200, RED, threshold, 8);
    }
    digiscope_lcd_rectangle(WHITE, 5, 14, 50, 82);
    digiscope_lcd_write_string(20, 48, BLACK, "+/-", 3);
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 277, BLACK, "1", 1);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 201, BLACK, "2", 1);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 125, BLACK, "3", 1);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 48, BLACK, "4", 1);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 277, BLACK, "5", 1);
    digiscope_lcd_rectangle(WHITE, 120, 168, 170, 234);
    digiscope_lcd_write_string(141, 201, BLACK, "6", 1);
    digiscope_lcd_rectangle(WHITE, 120, 92, 170, 158);
    digiscope_lcd_write_string(141, 125, BLACK, "7", 1);
    digiscope_lcd_rectangle(WHITE, 120, 14, 170, 82);
    digiscope_lcd_write_string(141, 48, BLACK, "8", 1);
    digiscope_lcd_rectangle(WHITE, 180, 244, 230, 310);
    digiscope_lcd_write_string(201, 277, BLACK, "9", 1);
    digiscope_lcd_rectangle(WHITE, 180, 168, 230, 234);
    digiscope_lcd_write_string(201, 201, BLACK, "0", 1);
    digiscope_lcd_rectangle(WHITE, 180, 92, 230, 158);
    digiscope_lcd_write_string(201, 140, BLACK, "CLEAR", 5);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 70, BLACK, "ENTER", 5);
    menu = 13;
}

void digiscope_lcd_write_menu_function() {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 190, WHITE, "FUNCTION", 8);
    digiscope_lcd_rectangle(WHITE, 60, 224, 120, 310);
    digiscope_lcd_write_string(84, 272, BLACK, "OUTPUT", 6);
    digiscope_lcd_rectangle(WHITE, 60, 118, 120, 204);
    digiscope_lcd_write_string(84, 180, BLACK, "WAVE", 4);
    digiscope_lcd_rectangle(WHITE, 60, 12, 120, 98);
    digiscope_lcd_write_string(84, 80, BLACK, "AMPLITUDE", 9);
    digiscope_lcd_rectangle(WHITE, 150, 224, 210, 310);
    digiscope_lcd_write_string(174, 272, BLACK, "OFFSET", 6);
    digiscope_lcd_rectangle(WHITE, 150, 118, 210, 204);
    digiscope_lcd_write_string(174, 180, BLACK, "FREQUENCY", 9);
    digiscope_lcd_rectangle(WHITE, 150, 12, 210, 98);
    digiscope_lcd_write_string(174, 80, BLACK, "BACK", 4);
    menu = 4;
}

void digiscope_lcd_write_menu_function_wave(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "WAVE TYPE:", 10);
    if (flag) {
        digiscope_lcd_write_string(20, 160, RED, wave, 8);
    }
    digiscope_lcd_rectangle(WHITE, 60, 224, 120, 310);
    digiscope_lcd_write_string(84, 272, BLACK, "SINE", 4);
    digiscope_lcd_rectangle(WHITE, 60, 118, 120, 204);
    digiscope_lcd_write_string(84, 180, BLACK, "SQUARE", 6);
    digiscope_lcd_rectangle(WHITE, 60, 12, 120, 98);
    digiscope_lcd_write_string(84, 80, BLACK, "TRIANGLE", 8);
    digiscope_lcd_rectangle(WHITE, 150, 224, 210, 310);
    digiscope_lcd_write_string(174, 285, BLACK, "SAWTOOTH", 8);
    digiscope_lcd_rectangle(WHITE, 150, 118, 210, 204);
    digiscope_lcd_write_string(174, 180, BLACK, "NOISE", 5);
    digiscope_lcd_rectangle(WHITE, 150, 12, 210, 98);
    digiscope_lcd_write_string(174, 80, BLACK, "BACK", 4);
    menu = 7;
}

void digiscope_lcd_write_menu_function_output(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "OUTPUT:", 7);
    if (flag) {
        digiscope_lcd_write_string(20, 160, RED, output, 3);
    }
    digiscope_lcd_rectangle(WHITE, 60, 208, 120, 292);
    digiscope_lcd_write_string(84, 278, BLACK, "ON", 2);
    digiscope_lcd_rectangle(WHITE, 60, 48, 120, 132);
    digiscope_lcd_write_string(84, 112, BLACK, "OFF", 3);
    digiscope_lcd_rectangle(WHITE, 150, 48, 210, 132);
    digiscope_lcd_write_string(174, 112, BLACK, "BACK", 4);
    menu = 8;
}

void digiscope_lcd_write_menu_function_pktopk(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 280, WHITE, "AMPLITUDE RANGE:", 16);
    if (flag) {
        digiscope_lcd_write_string(20, 100, RED, pktopk, 5);
    }
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 277, BLACK, "100mV", 5);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 201, BLACK, "200mV", 5);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 125, BLACK, "500mV", 5);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 48, BLACK, "1V", 2);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 277, BLACK, "2V", 2);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 70, BLACK, "ENTER", 5);
    menu = 15;
}

void digiscope_lcd_write_menu_function_offset(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 300, WHITE, "OFFSET:", 7);
    if (flag) {
        digiscope_lcd_write_string(20, 200, RED, offset, 8);
    }
    digiscope_lcd_rectangle(WHITE, 5, 14, 50, 82);
    digiscope_lcd_write_string(20, 48, BLACK, "+/-", 3);
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 277, BLACK, "1", 1);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 201, BLACK, "2", 1);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 125, BLACK, "3", 1);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 48, BLACK, "4", 1);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 277, BLACK, "5", 1);
    digiscope_lcd_rectangle(WHITE, 120, 168, 170, 234);
    digiscope_lcd_write_string(141, 201, BLACK, "6", 1);
    digiscope_lcd_rectangle(WHITE, 120, 92, 170, 158);
    digiscope_lcd_write_string(141, 125, BLACK, "7", 1);
    digiscope_lcd_rectangle(WHITE, 120, 14, 170, 82);
    digiscope_lcd_write_string(141, 48, BLACK, "8", 1);
    digiscope_lcd_rectangle(WHITE, 180, 244, 230, 310);
    digiscope_lcd_write_string(201, 277, BLACK, "9", 1);
    digiscope_lcd_rectangle(WHITE, 180, 168, 230, 234);
    digiscope_lcd_write_string(201, 201, BLACK, "0", 1);
    digiscope_lcd_rectangle(WHITE, 180, 92, 230, 158);
    digiscope_lcd_write_string(201, 140, BLACK, "CLEAR", 5);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 70, BLACK, "ENTER", 5);
    menu = 14;
}

void digiscope_lcd_write_menu_function_freq(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "FREQUENCY:", 10);
    if (flag) {
        digiscope_lcd_write_string(20, 160, RED, freq, 16);
    }
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 277, BLACK, "1", 1);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 201, BLACK, "2", 1);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 125, BLACK, "3", 1);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 48, BLACK, "4", 1);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 277, BLACK, "5", 1);
    digiscope_lcd_rectangle(WHITE, 120, 168, 170, 234);
    digiscope_lcd_write_string(141, 201, BLACK, "6", 1);
    digiscope_lcd_rectangle(WHITE, 120, 92, 170, 158);
    digiscope_lcd_write_string(141, 125, BLACK, "7", 1);
    digiscope_lcd_rectangle(WHITE, 120, 14, 170, 82);
    digiscope_lcd_write_string(141, 48, BLACK, "8", 1);
    digiscope_lcd_rectangle(WHITE, 180, 244, 230, 310);
    digiscope_lcd_write_string(201, 277, BLACK, "9", 1);
    digiscope_lcd_rectangle(WHITE, 180, 168, 230, 234);
    digiscope_lcd_write_string(201, 201, BLACK, "0", 1);
    digiscope_lcd_rectangle(WHITE, 180, 92, 230, 158);
    digiscope_lcd_write_string(201, 140, BLACK, "CLEAR", 5);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 70, BLACK, "ENTER", 5);
    menu = 10;
}

void digiscope_lcd_write_menu_signal_hrange(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "HORIZONTAL RANGE:", 17);
    if (flag) {
        digiscope_lcd_write_string(20, 100, RED, *hrange[chan], 5);
    }
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 280, BLACK, "1e-6", 4);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 204, BLACK, "2e-6", 4);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 128, BLACK, "5e-6", 4);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 51, BLACK, "1e-5", 4);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 280, BLACK, "2e-5", 4);
    digiscope_lcd_rectangle(WHITE, 120, 168, 170, 234);
    digiscope_lcd_write_string(141, 204, BLACK, "5e-5", 4);
    digiscope_lcd_rectangle(WHITE, 120, 92, 170, 158);
    digiscope_lcd_write_string(141, 128, BLACK, "1e-4", 4);
    digiscope_lcd_rectangle(WHITE, 120, 14, 170, 82);
    digiscope_lcd_write_string(141, 51, BLACK, "2e-4", 4);
    digiscope_lcd_rectangle(WHITE, 180, 244, 230, 310);
    digiscope_lcd_write_string(201, 280, BLACK, "5e-4", 4);
    digiscope_lcd_rectangle(WHITE, 180, 168, 230, 234);
    digiscope_lcd_write_string(201, 204, BLACK, "1e-3", 4);
    digiscope_lcd_rectangle(WHITE, 180, 92, 230, 158);
    digiscope_lcd_write_string(201, 125, BLACK, ">", 1);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 70, BLACK, "ENTER", 5);
    menu = 11;
}

void digiscope_lcd_write_menu_signal_hrange2(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "HORIZONTAL RANGE:", 17);
    if (flag) {
        digiscope_lcd_write_string(20, 100, RED, *hrange[chan], 4);
    }
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 280, BLACK, "2e-3", 4);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 204, BLACK, "5e-3", 4);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 128, BLACK, "1e-2", 4);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 51, BLACK, "2e-2", 4);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 280, BLACK, "5e-2", 4);
    digiscope_lcd_rectangle(WHITE, 120, 168, 170, 234);
    digiscope_lcd_write_string(141, 204, BLACK, "0.1", 3);
    digiscope_lcd_rectangle(WHITE, 120, 92, 170, 158);
    digiscope_lcd_write_string(141, 128, BLACK, "0.2", 3);
    digiscope_lcd_rectangle(WHITE, 120, 14, 170, 82);
    digiscope_lcd_write_string(141, 51, BLACK, "0.5", 3);
    digiscope_lcd_rectangle(WHITE, 180, 244, 230, 310);
    digiscope_lcd_write_string(201, 277, BLACK, "1", 1);
    digiscope_lcd_rectangle(WHITE, 180, 168, 230, 234);
    digiscope_lcd_write_string(201, 201, BLACK, "2", 1);
    digiscope_lcd_rectangle(WHITE, 180, 92, 230, 158);
    digiscope_lcd_write_string(201, 125, BLACK, "5", 1);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 44, BLACK, "<", 1);
    menu = 16;
}

void digiscope_lcd_write_menu_signal_vrange(int flag) {
    digiscope_lcd_clear();
    digiscope_lcd_write_string(20, 240, WHITE, "VERTICAL RANGE:", 15);
    if (flag) {
        digiscope_lcd_write_string(20, 100, RED, *vrange[chan], 5);
    }
    digiscope_lcd_rectangle(WHITE, 60, 244, 110, 310);
    digiscope_lcd_write_string(81, 277, BLACK, "10mV", 4);
    digiscope_lcd_rectangle(WHITE, 60, 168, 110, 234);
    digiscope_lcd_write_string(81, 201, BLACK, "20mV", 4);
    digiscope_lcd_rectangle(WHITE, 60, 92, 110, 158);
    digiscope_lcd_write_string(81, 125, BLACK, "50mV", 4);
    digiscope_lcd_rectangle(WHITE, 60, 14, 110, 82);
    digiscope_lcd_write_string(81, 48, BLACK, "100mV", 5);
    digiscope_lcd_rectangle(WHITE, 120, 244, 170, 310);
    digiscope_lcd_write_string(141, 277, BLACK, "200mV", 5);
    digiscope_lcd_rectangle(WHITE, 120, 168, 170, 234);
    digiscope_lcd_write_string(141, 201, BLACK, "500mV", 5);
    digiscope_lcd_rectangle(WHITE, 120, 92, 170, 158);
    digiscope_lcd_write_string(141, 125, BLACK, "1V", 2);
	digiscope_lcd_rectangle(WHITE, 120, 14, 170, 82);
    digiscope_lcd_write_string(141, 51, BLACK, "2V", 2);
    digiscope_lcd_rectangle(WHITE, 180, 14, 230, 82);
    digiscope_lcd_write_string(201, 70, BLACK, "ENTER", 5);
    menu = 12;
}

void digiscope_lcd_touch_write_data(unsigned char data) {
    unsigned char temp;
    unsigned char nop;
    unsigned char count;

    temp = data;
    digiscope_lcd_cleartsclk();

    for (count = 0; count < 8; count++) {
        if (temp & 0x80) {
            digiscope_lcd_settsdin();
        } else {
            digiscope_lcd_cleartsdin();
        }
        temp = temp << 1;
        digiscope_lcd_cleartsclk();
        nop++;
        digiscope_lcd_settsclk();
        nop++;
    }
}

unsigned int digiscope_lcd_touch_read_data() {
    unsigned char nop;
    unsigned int data = 0;
    unsigned char count;
    for (count = 0; count < 12; count++) {
        data <<= 1;
        digiscope_lcd_settsclk();
        nop++;
        digiscope_lcd_cleartsclk();
        nop++;
        if (HAL_GPIO_ReadPin(DIGISCOPE_TS_DOUT_PORT, DIGISCOPE_TS_DOUT_PIN)) {
            data++;
        }
    }
    return (data);
}

void digiscope_lcd_touch_read() {
    unsigned long tx = 0;
    unsigned long ty = 0;
    int i;

    digiscope_lcd_cleartscs();

    for (i = 0; i < PREC_TOUCH_CONST; i++) {
        digiscope_lcd_touch_write_data(0x90);
        digiscope_lcd_settsclk();
        digiscope_lcd_cleartsclk();
        ty += digiscope_lcd_touch_read_data();

        digiscope_lcd_touch_write_data(0xD0);
        digiscope_lcd_settsclk();
        digiscope_lcd_cleartsclk();
        tx += digiscope_lcd_touch_read_data();
    }

    digiscope_lcd_settscs();

    TP_X = tx / PREC_TOUCH_CONST;
    TP_Y = ty / PREC_TOUCH_CONST;
}

char digiscope_lcd_touch_data_available() {
    char avail;
    avail = !HAL_GPIO_ReadPin(DIGISCOPE_TS_IRQ_PORT, DIGISCOPE_TS_IRQ_PIN);
    return avail;
}

int digiscope_lcd_touch_getx() {
    int value;
    value = ((TP_X - PixOffsX) / PixSizeX);
    if (value < 0) {
        value = 0;
    }
    return value;
}

int digiscope_lcd_touch_gety() {
    int value;
    value = ((TP_Y - PixOffsY) / PixSizeY);
    if (value < 0) {
        value = 0;
    } else {
        value = 320 - value;
    }
    return value;
}

void digiscope_lcd_test() {
    unsigned int x, y;
    int act, i;
    act = 0;
    while (digiscope_lcd_touch_data_available() == 1) {
		if (backlight == 0){
			backlight = 1;
           	systemsettings.backlightlevel = backlight;
			HAL_Delay(1000);
			break;
		}
        digiscope_lcd_touch_read();
        x = digiscope_lcd_touch_getx();
        y = digiscope_lcd_touch_gety();
        if (y > 208 && y < 292 && x > 60 && x < 120) {
            switch (menu) {
                case 0:
                    digiscope_lcd_write_menu_signal();
                    act = 1;
                    break;
                case 1:
                    digiscope_lcd_write_menu_signal_coupling(0);
                    act = 1;
                    break;
                case 2:
                    if (backlight >=5){
						backlight = 5;
                    	systemsettings.backlightlevel = backlight;
						HAL_Delay(500);
					} else {
						backlight++;
                    	systemsettings.backlightlevel = backlight;
						HAL_Delay(500);
					}
                    act = 1;
                    break;
                case 5:
                    trigger_mode = "AUTO  ";
                    systemsettings.trigger.mode = SETTINGS_TRIGGER_MODE_AUTO;
                    digiscope_lcd_write_menu_trigger_mode(1);
                    act = 1;
                    break;
                case 6:
                    trigger_type = "FALLING";
                    systemsettings.trigger.type = SETTINGS_TRIGGER_FALLING;
                    digiscope_lcd_write_menu_trigger_type(1);
                    act = 1;
                    break;
                case 8:
                    output = "ON ";
                    systemsettings.signalgensettings.enabled = 1;
                    digiscope_lcd_write_menu_function_output(1);
                    act = 1;
                    break;
                case 9:
                    *coupling[chan] = "AC";
                    if (chan == 0) {
                        systemsettings.channel1settings.coupling = SETTINGS_AC_COUPLING;
                    } else {
                        systemsettings.channel2settings.coupling = SETTINGS_AC_COUPLING;
                    }
                    digiscope_lcd_write_menu_signal_coupling(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 48 && y < 132 && x > 60 && x < 120) {
            switch (menu) {
                case 0:
                    digiscope_lcd_write_menu_trigger();
                    act = 1;
                    break;
                case 1:
                    digiscope_lcd_write_menu_signal_hrange(0);
                    h_range = 0;
                    act = 1;
                    break;
                case 2:
					backlight = 0;
                    systemsettings.backlightlevel = 0;
					HAL_Delay(500);
                    act = 1;
                    break;
                case 5:
                    trigger_mode = "NORMAL";
                    systemsettings.trigger.mode = SETTINGS_TRIGGER_MODE_NORMAL;
                    digiscope_lcd_write_menu_trigger_mode(1);
                    act = 1;
                    break;
                case 6:
                    trigger_type = "RISING ";
                    systemsettings.trigger.type = SETTINGS_TRIGGER_RISING;
                    digiscope_lcd_write_menu_trigger_type(1);
                    act = 1;
                    break;
                case 8:
                    output = "OFF";
                    systemsettings.signalgensettings.enabled = 0;
                    digiscope_lcd_write_menu_function_output(1);
                    act = 1;
                    break;
                case 9:
                    *coupling[chan] = "DC";
                    if (chan == 0) {
                        systemsettings.channel1settings.coupling = SETTINGS_DC_COUPLING;
                    } else {
                        systemsettings.channel2settings.coupling = SETTINGS_DC_COUPLING;
                    }
                    digiscope_lcd_write_menu_signal_coupling(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 208 && y < 292 && x > 150 && x < 210) {
            switch (menu) {
                case 0:
                    digiscope_lcd_write_menu_function();
                    act = 1;
                    break;
                case 1:
                    digiscope_lcd_write_menu_signal_vrange(0);
                    v_range = 0;
                    act = 1;
                    break;
                case 2:
					if (backlight <=1){
						backlight = 1;
                    	systemsettings.backlightlevel = backlight;
						HAL_Delay(500);
					} else {
						backlight--;
                    	systemsettings.backlightlevel = backlight;
						HAL_Delay(500);
					}
                    act = 1;
                    break;
                case 5:
                    trigger_mode = "SINGLE";
                    systemsettings.trigger.mode = SETTINGS_TRIGGER_MODE_SINGLE;
                    digiscope_lcd_write_menu_trigger_mode(1);
                    act = 1;
                    break;
                case 6:
                    trigger_type = "LEVEL  ";
                    systemsettings.trigger.type = SETTINGS_TRIGGER_LEVEL;
                    digiscope_lcd_write_menu_trigger_type(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 48 && y < 132 && x > 150 && x < 210) {
            switch (menu) {
                case -1:
                    digiscope_lcd_write_menu_home();
                    act = 1;
                    break;
                case 0:
                    digiscope_lcd_write_menu_display();
                    act = 1;
                    break;
                case 1:
                    digiscope_lcd_write_menu_home();
                    act = 1;
                    break;
                case 2:
                    digiscope_lcd_write_menu_home();
                    act = 1;
                    break;
                case 5:
                    digiscope_lcd_write_menu_trigger();
                    act = 1;
                    break;
                case 6:
                    digiscope_lcd_write_menu_trigger();
                    act = 1;
                    break;
                case 7:
                    digiscope_lcd_write_menu_function();
                    act = 1;
                    break;
                case 8:
                    digiscope_lcd_write_menu_function();
                    act = 1;
                    break;
                case 9:
                    digiscope_lcd_write_menu_signal();
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 224 && y < 310 && x > 60 && x < 120) {
            switch (menu) {
                case 3:
                    digiscope_lcd_write_menu_trigger_mode(0);
                    act = 1;
                    break;
                case 4:
                    digiscope_lcd_write_menu_function_output(0);
                    act = 1;
                    break;
                case 7:
                    wave = "SINE    ";
                    systemsettings.signalgensettings.wave = WAVE_SINE;
                    digiscope_lcd_write_menu_function_wave(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 118 && y < 204 && x > 60 && x < 120) {
            switch (menu) {
                case 3:
                    digiscope_lcd_write_menu_trigger_type(0);
                    act = 1;
                    break;
                case 4:
                    digiscope_lcd_write_menu_function_wave(0);
                    act = 1;
                    break;
                case 7:
                    wave = "SQUARE  ";
                    systemsettings.signalgensettings.wave = WAVE_SQUARE;
                    digiscope_lcd_write_menu_function_wave(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 12 && y < 98 && x > 60 && x < 120) {
            switch (menu) {
                case 3:
                    digiscope_lcd_write_menu_trigger_threshold(0);
					thresh = 0;
                    thresh_count = 0;
                    s_thresh_count = 0;
                    act = 1;
                    break;
                case 4:
                    digiscope_lcd_write_menu_function_pktopk(0);
					pk_pk = 0;
                    act = 1;
                    break;
                case 7:
                    wave = "TRIANGLE";
                    systemsettings.signalgensettings.wave = WAVE_TRIANGLE;
                    digiscope_lcd_write_menu_function_wave(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 224 && y < 310 && x > 150 && x < 210) {
            switch (menu) {
                case 3:
                    digiscope_lcd_write_menu_trigger();
                    systemsettings.trigger.channel = chan+1;
                    digiscope_lcd_write_string(220, 300, RED, "TRIGGER ARMED", 13);
                    act = 1;
                    break;
                case 4:
                    digiscope_lcd_write_menu_function_offset(0);
					off = 0;
                    off_count = 0;
                    s_off_count = 0;
                    act = 1;
                    break;
                case 7:
                    wave = "SAWTOOTH";
                    systemsettings.signalgensettings.wave = WAVE_SAWTOOTH;
                    digiscope_lcd_write_menu_function_wave(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 118 && y < 204 && x > 150 && x < 210) {
            switch (menu) {
                case 3:
                    digiscope_lcd_write_menu_trigger();
                    systemsettings.trigger.channel = chan+1;
                    digiscope_adc_forcetrigger();
                    digiscope_lcd_write_string(220, 300, RED, "TRIGGER FORCED", 14);
                    act = 1;
                    break;
                case 4:
                    digiscope_lcd_write_menu_function_freq(0);
                    freq_count = 0;
					frequency = 0;
                    act = 1;
                    break;
                case 7:
                    wave = "NOISE   ";
                    systemsettings.signalgensettings.wave = WAVE_NOISE;
                    digiscope_lcd_write_menu_function_wave(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 12 && y < 98 && x > 150 && x < 210) {
            switch (menu) {
                case 3:
                    digiscope_lcd_write_menu_home();
                    act = 1;
                    break;
                case 4:
                    digiscope_lcd_write_menu_home();
                    act = 1;
                    break;
                case 7:
                    digiscope_lcd_write_menu_function();
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 244 && y < 310 && x > 60 && x < 110) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '1';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 1;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "1e-6s";
                    h_range = 0.000001;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "10mV ";
                    v_range = 0.01;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '1';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + (1 * pow(10, -s_thresh_count));
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '1';
                    offset[off_count + 1] = 'V';
                    off = off + (1 * pow(10, -s_off_count));
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 15:
                    pktopk = "100mV";
                    pk_pk = 0.1;
                    digiscope_lcd_write_menu_function_pktopk(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "2e-3s";
                    h_range = 0.002;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 168 && y < 234 && x > 60 && x < 110) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '2';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 2;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "2e-6s";
                    h_range = 0.000002;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "20mV ";
                    v_range = 0.02;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '2';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + (2 * pow(10, -s_thresh_count));
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '2';
                    offset[off_count + 1] = 'V';
                    off = off + (2 * pow(10, -s_off_count));
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 15:
                    pktopk = "200mV";
                    pk_pk = 0.2;
                    digiscope_lcd_write_menu_function_pktopk(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "5e-3s";
                    h_range = 0.005;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 92 && y < 158 && x > 60 && x < 110) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '3';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 3;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "5e-6s";
                    h_range = 0.000005;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "50mV";
                    v_range = 0.05;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '3';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + (3 * pow(10, -s_thresh_count));
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '3';
                    offset[off_count + 1] = 'V';
                    off = off + (3 * pow(10, -s_off_count));
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 15:
                    pktopk = "500mV";
                    pk_pk = 0.5;
                    digiscope_lcd_write_menu_function_pktopk(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "1e-2s";
                    h_range = 0.01;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 14 && y < 82 && x > 60 && x < 110) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '4';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 4;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "1e-5s";
                    h_range = 0.00001;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "100mV";
                    v_range = 0.1;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '4';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 4 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '4';
                    offset[off_count + 1] = 'V';
                    off = off + 4 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 15:
                    pktopk = "1V   ";
                    pk_pk = 1;
                    digiscope_lcd_write_menu_function_pktopk(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "2e-2s";
                    h_range = 0.02;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 244 && y < 310 && x > 120 && x < 170) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '5';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 5;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "2e-5s";
                    h_range = 0.00002;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "200mV";
                    v_range = 0.2;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '5';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 5 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '5';
                    offset[off_count + 1] = 'V';
                    off = off + 5 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 15:
                    pktopk = "2V   ";
                    pk_pk = 2;
                    digiscope_lcd_write_menu_function_pktopk(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "5e-2s";
                    h_range = 0.05;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 168 && y < 234 && x > 120 && x < 170) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '6';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 6;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "5e-5s";
                    h_range = 0.00005;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "500mV";
                    v_range = 0.5;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '6';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 6 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '6';
                    offset[off_count + 1] = 'V';
                    off = off + 6 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "0.1s  ";
                    h_range = 0.1;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 92 && y < 158 && x > 120 && x < 170) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '7';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 7;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "1e-4s";
                    h_range = 0.0001;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 12:
                    *vrange[chan] = "1V   ";
                    v_range = 1;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '7';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 7 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '7';
                    offset[off_count + 1] = 'V';
                    off = off + 7 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "0.2s  ";
                    h_range = 0.2;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 14 && y < 82 && x > 120 && x < 170) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '8';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 8;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "2e-4s";
                    h_range = 0.0002;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
				case 12:
                    *vrange[chan] = "2V   ";
                    v_range = 2;
                    digiscope_lcd_write_menu_signal_vrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '8';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 8 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '8';
                    offset[off_count + 1] = 'V';
                    off = off + 8 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "0.5s ";
                    h_range = 0.5;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 244 && y < 310 && x > 180 && x < 230) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                    }
                    freq[freq_count] = '9';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10) + 9;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "5e-4s";
                    h_range = 0.0005;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '9';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 9 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '9';
                    offset[off_count + 1] = 'V';
                    off = off + 9 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "1s   ";
                    h_range = 1;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 168 && y < 234 && x > 180 && x < 230) {
            switch (menu) {
                case 10:
                    if (!freq_count) {
                        for (i = 0; i < 16; i++) {
                            freq[i] = ' ';
                        }
                        act = 1;
                        break;
                    }
                    freq[freq_count] = '0';
                    freq[freq_count + 1] = 'H';
                    freq[freq_count + 2] = 'z';
                    freq_count++;
                    frequency = (frequency * 10);
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "1e-3s";
                    h_range = 0.001;
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
                case 13:
                    if (!thresh_count) {
                        for (i = 0; i < 8; i++) {
                            threshold[i] = ' ';
                        }
                        threshold[0] = '+';
                        thresh_count = 1;
                    }
                    if (thresh_count == 2) {
                        threshold[2] = '.';
                        thresh_count = 3;
                    }
                    threshold[thresh_count] = '0';
                    threshold[thresh_count + 1] = 'V';
                    thresh = thresh + 0 * pow(10, -s_thresh_count);
                    s_thresh_count++;
                    thresh_count++;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (!off_count) {
                        for (i = 0; i < 8; i++) {
                            offset[i] = ' ';
                        }
                        offset[0] = '+';
                        off_count = 1;
                    }
                    if (off_count == 2) {
                        offset[2] = '.';
                        off_count = 3;
                    }
                    offset[off_count] = '0';
                    offset[off_count + 1] = 'V';
                    off = off + 0 * pow(10, -s_off_count);
                    s_off_count++;
                    off_count++;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "2s   ";
                    h_range = 2;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 92 && y < 158 && x > 180 && x < 230) {
            switch (menu) {
                case 10:
                    for (i = 0; i < 16; i++) {
                        freq[i] = ' ';
                    }
                    freq_count = 0;
                    frequency = 0;
                    digiscope_lcd_write_menu_function_freq(1);
                    act = 1;
                    break;
                case 11:
                    *hrange[chan] = "    ";
                    digiscope_lcd_write_menu_signal_hrange2(0);
                    act = 1;
                    break;
                case 13:
                    for (i = 0; i < 8; i++) {
                        threshold[i] = ' ';
                    }
                    thresh = 0;
                    thresh_count = 0;
                    s_thresh_count = 0;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    for (i = 0; i < 8; i++) {
                        offset[i] = ' ';
                    }
                    off = 0;
                    off_count = 0;
                    s_off_count = 0;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
                case 16:
                    *hrange[chan] = "5s   ";
                    h_range = 5;
                    digiscope_lcd_write_menu_signal_hrange2(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 14 && y < 82 && x > 180 && x < 230) {
            switch (menu) {
                case 10:
                    digiscope_lcd_write_menu_function();
					if (frequency){
                    	systemsettings.signalgensettings.frequency = frequency;
					}
                    act = 1;
                    break;
                case 11:
                    digiscope_lcd_write_menu_signal();
                    if (h_range) {
                    	systemsettings.channel1settings.horizontalrange = h_range;
                    }
                    act = 1;
                    break;
                case 12:
                    digiscope_lcd_write_menu_signal();
                    if (v_range) {
                        if (chan == 0) {
                            systemsettings.channel1settings.verticalrange = v_range;
                        } else {
                            systemsettings.channel2settings.verticalrange = v_range;
                        }
                    }
                    act = 1;
                    break;
                case 13:
                    digiscope_lcd_write_menu_trigger();
                    if (thresh) {
                        systemsettings.trigger.level = thresh;
                    }
                    act = 1;
                    break;
                case 14:
                    digiscope_lcd_write_menu_function();
					if (off){
                    	systemsettings.signalgensettings.offset = off;
					}
                    act = 1;
                    break;
                case 15:
                    digiscope_lcd_write_menu_function();
					if (pk_pk){
                    	systemsettings.signalgensettings.voltage = pk_pk;
					}
                    act = 1;
                    break;
                case 16:
                    digiscope_lcd_write_menu_signal_hrange(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
        if (y > 14 && y < 82 && x > 5 && x < 50) {
            switch (menu) {
                case 0:
                    if (chan == 0) {
                        chan = 1;
                        char_chan = '2';
                    } else {
                        chan = 0;
                        char_chan = '1';
                    }
                    digiscope_lcd_write_menu_home();
                    act = 1;
                    break;
                case 13:
                    if (threshold[0] == '-') {
                        threshold[0] = '+';
                    } else {
                        threshold[0] = '-';
                    }
                    thresh = thresh * -1;
                    digiscope_lcd_write_menu_trigger_threshold(1);
                    act = 1;
                    break;
                case 14:
                    if (offset[0] == '-') {
                        offset[0] = '+';
                    } else {
                        offset[0] = '-';
                    }
                    off = off * -1;
                    digiscope_lcd_write_menu_function_offset(1);
                    act = 1;
                    break;
            }
            if (act) {
                act = 0;
                HAL_Delay(100);
                break;
            }
        }
    }
}

static void digiscope_lcd_delay(__IO int x) {
    while (x--) {
        asm("nop");
    }
}

int backlight_levels[] = {0, 600, 620, 637, 650, 1023};
void digiscope_lcd_backlight(int level) {
	lowlevel_dac_setvalue(6, backlight_levels[level]);
}
