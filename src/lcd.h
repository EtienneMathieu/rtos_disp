/*
 * lcd.h
 *
 *  Created on: 31 ������. 2013 ��.
 *      Author: danil
 */

#ifndef LCD_H_
#define LCD_H_
#include <stdio.h>
#include <stdint.h>

/**
 * \defgroup ili9341_group ILI9341 Display Controller Component Driver
 *
 * See \ref common_ili9341_qs
 *
 * This is the low level component driver for the ILI9341 display controller.
 * It provides basic functions for initializing, writing and reading the
 * controller. In addition to hardware control and use of the LCD controller
 * internal functions.
 *
 * \warning This component driver is not reentrant and can not be used in
 * interrupt service routines without extra care.
 *
 * Before writing data to the display call \ref ili9341_init() which will set up
 * the physical interface and the display. A configuration file conf_ili9341.h
 * is needed to define which interface to use, pin connections and communication
 * speed.
 *
 * An example conf_ili9341.h file for the XMEGA could look like:
 * \code
 * #define CONF_ILI9341_USART_SPI     &USARTC0
 * #define CONF_ILI9341_CLOCK_SPEED   8000000UL
 *
 * #define CONF_ILI9341_CS_PIN        IOPORT_CREATE_PIN(PORTC, 5)
 * #define CONF_ILI9341_DC_PIN        IOPORT_CREATE_PIN(PORTC, 4)
 * #define CONF_ILI9341_BACKLIGHT_PIN IOPORT_CREATE_PIN(PORTA, 5)
 * #define CONF_ILI9341_RESET_PIN     IOPORT_CREATE_PIN(PORTA, 7)
 * \endcode
 *
 * \section dependencies Dependencies
 * This component driver depends on the following modules:
 * - \ref gpio_group for IO port control.
 * - \ref sysclk_group for clock speed and functions.
 * - \ref usart_spi_group or \ref spi_group for communication with the
 * controller.
 * @{
 */

/** This macro generates a 16-bit native color for the display from a
 *  24-bit RGB value.
 */
/* Level 1 Commands (from the display Datasheet) */
#define ILI9341_CMD_NOP                             0x00
#define ILI9341_CMD_SOFTWARE_RESET                  0x01
#define ILI9341_CMD_READ_DISP_ID                    0x04
#define ILI9341_CMD_READ_DISP_STATUS                0x09
#define ILI9341_CMD_READ_DISP_MADCTRL               0x0B
#define ILI9341_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ILI9341_CMD_READ_DISP_IMAGE_FORMAT          0x0D
#define ILI9341_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ILI9341_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ILI9341_CMD_ENTER_SLEEP_MODE                0x10
#define ILI9341_CMD_SLEEP_OUT                       0x11
#define ILI9341_CMD_PARTIAL_MODE_ON                 0x12
#define ILI9341_CMD_NORMAL_DISP_MODE_ON             0x13
#define ILI9341_CMD_DISP_INVERSION_OFF              0x20
#define ILI9341_CMD_DISP_INVERSION_ON               0x21
#define ILI9341_CMD_GAMMA_SET                       0x26
#define ILI9341_CMD_DISPLAY_OFF                     0x28
#define ILI9341_CMD_DISPLAY_ON                      0x29
#define ILI9341_CMD_COLUMN_ADDRESS_SET              0x2A
#define ILI9341_CMD_PAGE_ADDRESS_SET                0x2B
#define ILI9341_CMD_MEMORY_WRITE                    0x2C
#define ILI9341_CMD_COLOR_SET                       0x2D
#define ILI9341_CMD_MEMORY_READ                     0x2E
#define ILI9341_CMD_PARTIAL_AREA                    0x30
#define ILI9341_CMD_VERT_SCROLL_DEFINITION          0x33
#define ILI9341_CMD_TEARING_EFFECT_LINE_OFF         0x34
#define ILI9341_CMD_TEARING_EFFECT_LINE_ON          0x35
#define ILI9341_CMD_MEMORY_ACCESS_CONTROL           0x36
#define ILI9341_CMD_VERT_SCROLL_START_ADDRESS       0x37
#define ILI9341_CMD_IDLE_MODE_OFF                   0x38
#define ILI9341_CMD_IDLE_MODE_ON                    0x39
#define ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET         0x3A
#define ILI9341_CMD_WRITE_MEMORY_CONTINUE           0x3C
#define ILI9341_CMD_READ_MEMORY_CONTINUE            0x3E
#define ILI9341_CMD_SET_TEAR_SCANLINE               0x44
#define ILI9341_CMD_GET_SCANLINE                    0x45
#define ILI9341_CMD_WRITE_DISPLAY_BRIGHTNESS        0x51
#define ILI9341_CMD_READ_DISPLAY_BRIGHTNESS         0x52
#define ILI9341_CMD_WRITE_CTRL_DISPLAY              0x53
#define ILI9341_CMD_READ_CTRL_DISPLAY               0x54
#define ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS  0x55
#define ILI9341_CMD_READ_CONTENT_ADAPT_BRIGHTNESS   0x56
#define ILI9341_CMD_WRITE_MIN_CAB_LEVEL             0x5E
#define ILI9341_CMD_READ_MIN_CAB_LEVEL              0x5F
#define ILI9341_CMD_READ_ID1                        0xDA
#define ILI9341_CMD_READ_ID2                        0xDB
#define ILI9341_CMD_READ_ID3                        0xDC

/* Level 2 Commands (from the display Datasheet) */
#define ILI9341_CMD_RGB_SIGNAL_CONTROL              0xB0
#define ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL       0xB1
#define ILI9341_CMD_FRAME_RATE_CONTROL_IDLE_8COLOR  0xB2
#define ILI9341_CMD_FRAME_RATE_CONTROL_PARTIAL      0xB3
#define ILI9341_CMD_DISPLAY_INVERSION_CONTROL       0xB4
#define ILI9341_CMD_BLANKING_PORCH_CONTROL          0xB5
#define ILI9341_CMD_DISPLAY_FUNCTION_CONTROL        0xB6
#define ILI9341_CMD_ENTRY_MODE_SET                  0xB7
#define ILI9341_CMD_BACKLIGHT_CONTROL_1             0xB8
#define ILI9341_CMD_BACKLIGHT_CONTROL_2             0xB9
#define ILI9341_CMD_BACKLIGHT_CONTROL_3             0xBA
#define ILI9341_CMD_BACKLIGHT_CONTROL_4             0xBB
#define ILI9341_CMD_BACKLIGHT_CONTROL_5             0xBC
#define ILI9341_CMD_BACKLIGHT_CONTROL_6             0xBD
#define ILI9341_CMD_BACKLIGHT_CONTROL_7             0xBE
#define ILI9341_CMD_BACKLIGHT_CONTROL_8             0xBF
#define ILI9341_CMD_POWER_CONTROL_1                 0xC0
#define ILI9341_CMD_POWER_CONTROL_2                 0xC1
#define ILI9341_CMD_VCOM_CONTROL_1                  0xC5
#define ILI9341_CMD_VCOM_CONTROL_2                  0xC7
#define ILI9341_CMD_POWER_ON_SEQ_CONTROL            0xCB
#define ILI9341_CMD_POWER_CONTROL_A                 0xCD
#define ILI9341_CMD_POWER_CONTROL_B                 0xCF
#define ILI9341_CMD_NVMEM_WRITE                     0xD0
#define ILI9341_CMD_NVMEM_PROTECTION_KEY            0xD1
#define ILI9341_CMD_NVMEM_STATUS_READ               0xD2
#define ILI9341_CMD_READ_ID4                        0xD3
#define ILI9341_CMD_POSITIVE_GAMMA_CORRECTION       0xE0
#define ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION       0xE1
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_1         0xE2
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_2         0xE3
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_A         0xE8
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_B         0xEA
#define ILI9341_CMD_ENABLE_3_GAMMA_CONTROL          0xF2
#define ILI9341_CMD_INTERFACE_CONTROL               0xF6
#define ILI9341_CMD_PUMP_RATIO_CONTROL              0xF7

#define ILI9341_COLOR(r, g, b)\
	((((uint16_t)b) >> 3) |\
	       ((((uint16_t)g) << 3) & 0x07E0) |\
	       ((((uint16_t)r) << 8) & 0xf800))

/** Type define for an integer type large enough to store a pixel color. */
typedef uint16_t ili9341_color_t;

/** Type define for an integer type large enough to store a pixel coordinate.
 */
typedef int16_t ili9341_coord_t;

/**
 * \name Display orientation flags
 * @{
 */

/** Bit mask for flipping X for ili9341_set_orientation() */
#define ILI9341_FLIP_X 1
/** Bit mask for flipping Y for ili9341_set_orientation() */
#define ILI9341_FLIP_Y 2
/** Bit mask for swapping X and Y for ili9341_set_orientation() */
#define ILI9341_SWITCH_XY 4

/** @} */

/** Height of display using default orientation */
#define ILI9341_DEFAULT_HEIGHT   240

/** Width of display using default orientation */
#define ILI9341_DEFAULT_WIDTH    320

/** Height of display using swapped X/Y orientation */
#define ILI9341_SWITCH_XY_HEIGHT 320

/** Width of display using swapped X/Y orientation */
#define ILI9341_SWITCH_XY_WIDTH  240

/**
 * \name Controller primitive graphical functions
 * @{
 */
ili9341_color_t ili9341_read_gram(void);

void ili9341_write_gram(ili9341_color_t color);

void ili9341_set_top_left_limit(ili9341_coord_t x, ili9341_coord_t y);

void ili9341_set_bottom_right_limit(ili9341_coord_t x, ili9341_coord_t y);

void ili9341_set_limits(ili9341_coord_t start_x, ili9341_coord_t start_y,
		ili9341_coord_t end_x, ili9341_coord_t end_y);

void ili9341_set_orientation(uint8_t flags);

void ili9341_copy_pixels_to_screen(const ili9341_color_t *pixels,
		uint32_t count);

#if XMEGA
void ili9341_copy_progmem_pixels_to_screen(
		ili9341_color_t PROGMEM_PTR_T pixels, uint32_t count);
#endif

void ili9341_copy_pixels_from_screen(ili9341_color_t *pixels, uint32_t count);

void ili9341_duplicate_pixel(const ili9341_color_t color, uint32_t count);

/** @} */

/**
 * \name Controller and display initialization and management
 * @{
 */

void ili9341_init(void);

/**
 * \brief Function to turn on the display back light
 *
 * Use this function to simply set the pin controlling the back light high to
 * turn on the back light.
 *
 * \note It is up to the user application if other means of controlling this pin
 * should be used, e.g. a PWM signal to be able to dim the display.

static inline void ili9341_backlight_on(void)
{
	ioport_set_pin_level(CONF_ILI9341_BACKLIGHT_PIN, true);
}
 */
/**
 * \brief Function to turn off the display back light
 *
 * Use this function to simply set the pin controlling the back light low to
 * turn off the back light.

static inline void ili9341_backlight_off(void)
{
	ioport_set_pin_level(CONF_ILI9341_BACKLIGHT_PIN, false);
}
 */
/** @} */

/** @} */

/**
 * \page common_ili9341_qs Quick Start Guide for the ILI9341 Display Controller Component Driver
 *
 * This is the quick start guide for the \ref ili9341_group, with step-by-step
 * instructions on how to configure and use the component driver for specific
 * use cases.
 *
 * The section described below can be compiled into e.g. the main application
 * loop or any other function that will need to interface non-volatile memory.
 *
 * \section common_ili9341_qs_basic Basic usage of the ILI9341 component driver
 * This section will present one use cases of the ILI9341 component driver. It
 * will first turn on the backlight and then fill the screen with a solid
 * color.
 *
 * \section common_ili9341_qs_basic_case Use case 1: Fill Screen Color
 *
 * The ILI9341 component driver contains a basic backlight control as well as
 * functions to draw pixels of various colors within a given pixel boundary.
 * This use case will turn on the backlight and fill the entire display with a
 * single color.
 *
 * \section common_ili9341_qs_basic_case_prereq Prerequisites
 * The conf_ili9341.h configuration file must be present in your application,
 * and should be set to use the correct SPI interface of your chosen
 * microcontroller.
 *
 * Your application should configure the display SPI, reset, command/data and
 * backlight pins as appropriate. If you are using the mXT143E Xplained kit,
 * this can be achieved by defining:
 * \code
 * #define CONF_BOARD_MXT143E_XPLAINED
 * \endcode
 * in your conf_board.h header file.
 *
 * \section common_ili9341_qs_basic_case_setup Setup
 *
 * \subsection common_ili9341_qs_basic_case_setup_steps Code
 * \code
 * ili9341_init();
 * ili9341_backlight_on();
 * \endcode
 *
 * \subsection common_ili9341_qs_basic_case_setup_workflow Workflow
 * -# First, we must initialize the display so that it is correctly configured
 *    and ready to receive commands:
 *    \code
 *    ili9341_init();
 *    \endcode
 * -# Next, the display backlight is enabled, turning it on:
 *    \code
 *    ili9341_backlight_on();
 *    \endcode
 *
 * \section common_ili9341_qs_basic_case_example_code Example code
 *
 * \code
 * ili9341_set_top_left_limit(0, 0);
 * ili9341_set_bottom_right_limit(240, 320);
 *
 * ili9341_duplicate_pixel(ILI9341_COLOR(255, 0, 0), 240UL * 320UL);
 * \endcode
 *
 * \subsection common_ili9341_qs_basic_case_workflow Workflow
 *
 * -# First the screen drawing limits are set to start from the upper-left
 *    pixel coordinate, and end at the lower right screen boundary:
 *     - \code
 *       ili9341_set_top_left_limit(0, 0);
 *       ili9341_set_bottom_right_limit(240, 320);
 *       \endcode
 *    \note If both the top-left and bottom-right limits are to be set at the
 *          same time, it is faster to call \ref  ili9341_set_limits() instead.
 * -# Next, the display is filled with 240*320 (the screen's pixel dimensions)
 *    pixels of the same RGB color of (255, 0, 0), a bright red.
 *     - \code
 *       ili9341_duplicate_pixel(ILI9341_COLOR(255, 0, 0), 240UL * 320UL);
 *       \endcode
 *    \note You must encode the RGB color value to use in the display's native
 *          pixel color format using the \ref ILI9341_COLOR() macro.
 */

void lcdInit();
 void send(uint8_t);
void draw(uint8_t[], ili9341_color_t);

#endif /* LCD_H_ */
