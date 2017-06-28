/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 20, 2016
 * ============================================================================
 * Filename: digiscope_lcd.h
 * ============================================================================
 */

#ifndef DIGISCOPE_LCD__H
#define DIGISCOPE_LCD__H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_nucleo_144.h"
#include <stdint.h>
#include "debugger_printf.h"
#include "lowlevel_dac.h"
#include "digiscope_settings.h"
#include "digiscope_adc.h"


#include "tcpip.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip_init.h"
#include "debugger_printf.h"
#include <math.h>

    extern struct netif xnetif;

#define DIGISCOPE_LCD_CLK()     {__GPIOD_CLK_ENABLE(); __GPIOE_CLK_ENABLE(); __GPIOF_CLK_ENABLE(); __GPIOG_CLK_ENABLE(); __GPIOC_CLK_ENABLE();}

#define X_CONST 240
#define Y_CONST 320

#define PREC_TOUCH_CONST 10

#define PixSizeX 15.01
#define PixOffsX 221

#define PixSizeY 11.01
#define PixOffsY 370

    /* Data bus*/
#define DIGISCOPE_LCD_DB0_PIN   GPIO_PIN_7
#define DIGISCOPE_LCD_DB0_PORT  GPIOD

#define DIGISCOPE_LCD_DB1_PIN   GPIO_PIN_6
#define DIGISCOPE_LCD_DB1_PORT  GPIOD

#define DIGISCOPE_LCD_DB2_PIN   GPIO_PIN_5
#define DIGISCOPE_LCD_DB2_PORT  GPIOD

#define DIGISCOPE_LCD_DB3_PIN   GPIO_PIN_4
#define DIGISCOPE_LCD_DB3_PORT  GPIOD

#define DIGISCOPE_LCD_DB4_PIN   GPIO_PIN_3
#define DIGISCOPE_LCD_DB4_PORT  GPIOF

#define DIGISCOPE_LCD_DB5_PIN   GPIO_PIN_3
#define DIGISCOPE_LCD_DB5_PORT  GPIOA

#define DIGISCOPE_LCD_DB6_PIN   GPIO_PIN_9
#define DIGISCOPE_LCD_DB6_PORT  GPIOB

#define DIGISCOPE_LCD_DB7_PIN   GPIO_PIN_8
#define DIGISCOPE_LCD_DB7_PORT  GPIOB

#define DIGISCOPE_LCD_DB8_PIN   GPIO_PIN_4
#define DIGISCOPE_LCD_DB8_PORT  GPIOB

#define DIGISCOPE_LCD_DB9_PIN   GPIO_PIN_7
#define DIGISCOPE_LCD_DB9_PORT  GPIOC

#define DIGISCOPE_LCD_DB10_PIN  GPIO_PIN_15
#define DIGISCOPE_LCD_DB10_PORT GPIOA

#define DIGISCOPE_LCD_DB11_PIN  GPIO_PIN_12
#define DIGISCOPE_LCD_DB11_PORT GPIOB

#define DIGISCOPE_LCD_DB12_PIN  GPIO_PIN_15
#define DIGISCOPE_LCD_DB12_PORT GPIOB

#define DIGISCOPE_LCD_DB13_PIN  GPIO_PIN_6
#define DIGISCOPE_LCD_DB13_PORT GPIOC

#define DIGISCOPE_LCD_DB14_PIN  GPIO_PIN_8
#define DIGISCOPE_LCD_DB14_PORT GPIOC

#define DIGISCOPE_LCD_DB15_PIN  GPIO_PIN_9
#define DIGISCOPE_LCD_DB15_PORT GPIOC

    /* Control Pins */
#define DIGISCOPE_LCD_RD_PIN    GPIO_PIN_12
#define DIGISCOPE_LCD_RD_PORT   GPIOC

#define DIGISCOPE_LCD_WR_PIN    GPIO_PIN_2
#define DIGISCOPE_LCD_WR_PORT   GPIOD

#define DIGISCOPE_LCD_RS_PIN    GPIO_PIN_2
#define DIGISCOPE_LCD_RS_PORT   GPIOG

#define DIGISCOPE_LCD_CS_PIN    GPIO_PIN_10
#define DIGISCOPE_LCD_CS_PORT   GPIOC

#define DIGISCOPE_LCD_RST_PIN   GPIO_PIN_11
#define DIGISCOPE_LCD_RST_PORT  GPIOC

    /* Touch screen */
#define DIGISCOPE_TS_DOUT_PIN   GPIO_PIN_6
#define DIGISCOPE_TS_DOUT_PORT  GPIOA

#define DIGISCOPE_TS_BUSY_PIN   GPIO_PIN_15
#define DIGISCOPE_TS_BUSY_PORT  GPIOD

#define DIGISCOPE_TS_DIN_PIN    GPIO_PIN_5
#define DIGISCOPE_TS_DIN_PORT   GPIOA

#define DIGISCOPE_TS_CS_PIN     GPIO_PIN_14
#define DIGISCOPE_TS_CS_PORT    GPIOD

#define DIGISCOPE_TS_CLK_PIN    GPIO_PIN_12
#define DIGISCOPE_TS_CLK_PORT   GPIOF

#define DIGISCOPE_TS_IRQ_PIN    GPIO_PIN_3
#define DIGISCOPE_TS_IRQ_PORT   GPIOG


#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0

    void lcd_init();
    void digiscope_lcd_test();
	void digiscope_lcd_set_accept();
    void digiscope_lcd_write_menu_trigger();
    void digiscope_lcd_write_menu_home();
    void digiscope_lcd_write_menu_signal();
    void digiscope_lcd_write_menu_display();
    void digiscope_lcd_write_menu_function();
    void digiscope_lcd_write_menu_function_output();
    void digiscope_lcd_write_menu_function_wave();
    void digiscope_lcd_write_menu_trigger_type();
    void digiscope_lcd_write_menu_trigger_mode();
    void digiscope_lcd_write_menu_signal_coupling();
    void digiscope_lcd_write_menu_function_freq();
    void digiscope_lcd_write_menu_signal_hrange();
    void digiscope_lcd_write_menu_signal_vrange();
    void digiscope_lcd_setcs();
    void digiscope_lcd_clearcs();
    void digiscope_lcd_setrs();
    void digiscope_lcd_clearrs();
    void digiscope_lcd_setwr();
    void digiscope_lcd_clearwr();
    void digiscope_lcd_setrst();
    void digiscope_lcd_clearrst();
    void digiscope_lcd_writebus(uint16_t data);
    void digiscope_lcd_write_char(int x, int y, unsigned int colour, char c);
    void digiscope_lcd_write_string(int x, int y, unsigned int colour, char* c, int len);
    void digiscope_lcd_writecommand(unsigned int c);
    void digiscope_lcd_writedata(unsigned int c);
    void digiscope_lcd_writecommanddata(unsigned int cmd, unsigned int dat);
    void digiscope_lcd_setxy(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
    void digiscope_lcd_paint(unsigned int color);
    void digiscope_lcd_clear();
    int digiscope_lcd_touch_gety();
    int digiscope_lcd_touch_getx();
    char digiscope_lcd_touch_data_available();
    void digiscope_lcd_touch_read();
    unsigned int digiscope_lcd_touch_read_data();
    void digiscope_lcd_touch_write_data(unsigned char data);
    void digiscope_lcd_settscs();
    void digiscope_lcd_cleartscs();
    void digiscope_lcd_settsclk();
    void digiscope_lcd_cleartsclk();
    void digiscope_lcd_settsdin();
    void digiscope_lcd_cleartsdin();
    static void digiscope_lcd_delay(int x);
    void digiscope_lcd_rectangle(unsigned int color, int x1, int y1, int x2, int y2);
    void digiscope_lcd_backlight(int);
	void digiscope_lcd_backlight_off();

    static const uint8_t alphabet[48][12] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // Space
        {0x00, 0x00, 0x20, 0x20, 0x30, 0x50, 0x50, 0x78, 0x48, 0xCC, 0x00, 0x00}, // A
        {0x00, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x48, 0x48, 0x48, 0xF0, 0x00, 0x00}, // B
        {0x00, 0x00, 0x78, 0x88, 0x80, 0x80, 0x80, 0x80, 0x88, 0x70, 0x00, 0x00}, // C
        {0x00, 0x00, 0xF0, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0xF0, 0x00, 0x00}, // D
        {0x00, 0x00, 0xF8, 0x48, 0x50, 0x70, 0x50, 0x40, 0x48, 0xF8, 0x00, 0x00}, // E
        {0x00, 0x00, 0xF8, 0x48, 0x50, 0x70, 0x50, 0x40, 0x40, 0xE0, 0x00, 0x00}, // F
        {0x00, 0x00, 0x38, 0x48, 0x80, 0x80, 0x9C, 0x88, 0x48, 0x30, 0x00, 0x00}, // G
        {0x00, 0x00, 0xCC, 0x48, 0x48, 0x78, 0x48, 0x48, 0x48, 0xCC, 0x00, 0x00}, // H
        {0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00}, // I
        {0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x90, 0xE0, 0x00}, // J
        {0x00, 0x00, 0xEC, 0x48, 0x50, 0x60, 0x50, 0x50, 0x48, 0xEC, 0x00, 0x00}, // K
        {0x00, 0x00, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x44, 0xFC, 0x00, 0x00}, // L
        {0x00, 0x00, 0xD8, 0xD8, 0xD8, 0xD8, 0xA8, 0xA8, 0xA8, 0xA8, 0x00, 0x00}, // M
        {0x00, 0x00, 0xDC, 0x48, 0x68, 0x68, 0x58, 0x58, 0x48, 0xE8, 0x00, 0x00}, // N
        {0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00}, // O
        {0x00, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x40, 0x40, 0x40, 0xE0, 0x00, 0x00}, // P
        {0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0xE8, 0x98, 0x70, 0x18, 0x00}, // Q
        {0x00, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x50, 0x48, 0x48, 0xEC, 0x00, 0x00}, // R
        {0x00, 0x00, 0x78, 0x88, 0x80, 0x60, 0x10, 0x08, 0x88, 0xF0, 0x00, 0x00}, // S
        {0x00, 0x00, 0xF8, 0xA8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00}, // T
        {0x00, 0x00, 0xCC, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x30, 0x00, 0x00}, // U
        {0x00, 0x00, 0xCC, 0x48, 0x48, 0x50, 0x50, 0x30, 0x20, 0x20, 0x00, 0x00}, // V
        {0x00, 0x00, 0xA8, 0xA8, 0xA8, 0x70, 0x50, 0x50, 0x50, 0x50, 0x00, 0x00}, // W
        {0x00, 0x00, 0xD8, 0x50, 0x50, 0x20, 0x20, 0x50, 0x50, 0xD8, 0x00, 0x00}, // X
        {0x00, 0x00, 0xD8, 0x50, 0x50, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00}, // Y
        {0x00, 0x00, 0xF8, 0x90, 0x10, 0x20, 0x20, 0x40, 0x48, 0xF8, 0x00, 0x00}, // Z
        {0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00}, // 0
        {0x00, 0x00, 0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00}, // 1
        {0x00, 0x00, 0x70, 0x88, 0x88, 0x10, 0x20, 0x40, 0x80, 0xF8, 0x00, 0x00}, // 2
        {0x00, 0x00, 0x70, 0x88, 0x08, 0x30, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00}, // 3
        {0x00, 0x00, 0x10, 0x30, 0x50, 0x50, 0x90, 0x78, 0x10, 0x18, 0x00, 0x00}, // 4
        {0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00}, // 5
        {0x00, 0x00, 0x70, 0x90, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00}, // 6
        {0x00, 0x00, 0xF8, 0x90, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00}, // 7
        {0x00, 0x00, 0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00}, // 8
        {0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x78, 0x08, 0x48, 0x70, 0x00, 0x00}, // 9
        {0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xA8, 0xA8, 0xA8, 0xA8, 0x00, 0x00}, // m
        {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00}, // :
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x10, 0x20, 0x20, 0x78, 0x00, 0x00}, // z
        {0x00, 0x00, 0x20, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00}, // +
        {0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // -
        {0x00, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x80, 0x00}, // /
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00}, // .
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x78, 0x40, 0x38, 0x00, 0x00}, // e
        {0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00}, // <
        {0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00}, // >
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x40, 0x30, 0x08, 0x78, 0x00, 0x00}
    }; // s

    static const uint8_t lookup[48] = {
        ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0',
        '1', '2', '3', '4', '5', '6', '7', '8', '9', 'm', ':', 'z', '+', '-',
        '/', '.', 'e', '<', '>', 's'
    };


#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_LCD__H */
