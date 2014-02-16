/*
 * lcd.c
 *
 *  Created on: 31 ������. 2013 ��.
 *      Author: danil
 */

#include "lcd.h"
#include "stm32f4xx.h"
#include "delay.h"

static ili9341_coord_t limit_start_x, limit_start_y;
static ili9341_coord_t limit_end_x, limit_end_y;

void ili9341_send_command(uint8_t );
void ili9341_send_draw_limits(const uint8_t);

inline static void ili9341_select_chip(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_10);
}

inline static void ili9341_deselect_chip(void){
	GPIO_SetBits(GPIOE, GPIO_Pin_10);
}
inline static void ili9341_select_command_mode(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_15);
}

inline static void ili9341_select_data_mode(void){
	GPIO_SetBits(GPIOE, GPIO_Pin_15);
}


static void ili9341_reset_display(void){
	GPIO_SetBits(GPIOE, GPIO_Pin_11);
	delay_ms(10);
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	delay_ms(10);
	GPIO_SetBits(GPIOE, GPIO_Pin_11);
	delay_ms(150);
}

static void ili9341_exit_standby(void)
{
	ili9341_send_command(ILI9341_CMD_SLEEP_OUT);
	ili9341_deselect_chip();
	delay_ms(150);
	ili9341_send_command(ILI9341_CMD_DISPLAY_ON);
	ili9341_deselect_chip();
}

inline static void ili9341_send_byte(uint8_t data){
	send(data);
}

inline static void ili9341_wait_for_send_done(void){
	  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);
	  while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_BSY) != RESET);
}


void ili9341_send_command(uint8_t command)
{
	ili9341_select_command_mode();
	ili9341_select_chip();
	ili9341_send_byte(command);
	ili9341_wait_for_send_done();
	ili9341_select_data_mode();
}

void ili9341_set_limits(ili9341_coord_t start_x, ili9341_coord_t start_y,
		ili9341_coord_t end_x, ili9341_coord_t end_y)
{
	limit_start_x = start_x;
	limit_start_y = start_y;
	limit_end_x = end_x;
	limit_end_y = end_y;

	ili9341_send_draw_limits(1);
}


void ili9341_set_orientation(uint8_t flags)
{
	uint8_t madctl = 0x48;

	/* Pretend the display is in landscape mode by default to match other display drivers */
	flags ^= ILI9341_SWITCH_XY | ILI9341_FLIP_X;

	if (flags & ILI9341_FLIP_X) {
		madctl &= ~(1 << 6);
	}

	if (flags & ILI9341_FLIP_Y) {
		madctl |= 1 << 7;
	}

	if (flags & ILI9341_SWITCH_XY) {
		madctl |= 1 << 5;
	}

	ili9341_send_command(ILI9341_CMD_MEMORY_ACCESS_CONTROL);
	ili9341_send_byte(madctl);
	ili9341_wait_for_send_done();
	ili9341_deselect_chip();
}

void lcdInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Configure PD12, PD13, PD14 and PD15 in output push-pull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_SPI4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	  /*!< SPI configuration */
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI4, &SPI_InitStructure);
	  /*!< Enable the sFLASH_SPI  */
	  SPI_Cmd(SPI4, ENABLE);

		ili9341_reset_display();

		/* Send commands to exit standby mode */
		ili9341_exit_standby();

		ili9341_send_command(ILI9341_CMD_POWER_CONTROL_A);
		ili9341_send_byte(0x39);


		ili9341_send_byte(0x2C);
		ili9341_send_byte(0x00);
		ili9341_send_byte(0x34);
		ili9341_send_byte(0x02);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_POWER_CONTROL_B);
		ili9341_send_byte(0x00);
		ili9341_send_byte(0xAA);
		ili9341_send_byte(0XB0);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_PUMP_RATIO_CONTROL);
		ili9341_send_byte(0x30);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_POWER_CONTROL_1);
		ili9341_send_byte(0x25);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_POWER_CONTROL_2);
		ili9341_send_byte(0x11);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_VCOM_CONTROL_1);
		ili9341_send_byte(0x5C);
		ili9341_send_byte(0x4C);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_VCOM_CONTROL_2);
		ili9341_send_byte(0x94);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_DRIVER_TIMING_CONTROL_A);
		ili9341_send_byte(0x85);
		ili9341_send_byte(0x01);
		ili9341_send_byte(0x78);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_DRIVER_TIMING_CONTROL_B);
		ili9341_send_byte(0x00);
		ili9341_send_byte(0x00);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_send_command(ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET);
		ili9341_send_byte(0x05);
		ili9341_wait_for_send_done();
		ili9341_deselect_chip();

		ili9341_set_orientation(0);
		ili9341_set_limits(0, 0, ILI9341_DEFAULT_WIDTH,
							ILI9341_DEFAULT_HEIGHT);

}

void send(uint8_t byte){
	 /*!< Loop while DR register in not emplty */
	ili9341_wait_for_send_done();
	  /*!< Send byte through the SPI1 peripheral */
	  SPI_I2S_SendData(SPI4, byte);
}


void ili9341_send_draw_limits(const uint8_t send_end_limits)
{
	ili9341_send_command(ILI9341_CMD_COLUMN_ADDRESS_SET);
	ili9341_send_byte(limit_start_x >> 8);
	ili9341_send_byte(limit_start_x & 0xFF);
	if (send_end_limits) {
		ili9341_send_byte(limit_end_x >> 8);
		ili9341_send_byte(limit_end_x & 0xFF);
	}
	ili9341_wait_for_send_done();
	ili9341_deselect_chip();

	ili9341_send_command(ILI9341_CMD_PAGE_ADDRESS_SET);
	ili9341_send_byte(limit_start_y >> 8);
	ili9341_send_byte(limit_start_y & 0xFF);
	if (send_end_limits) {
		ili9341_send_byte(limit_end_y >> 8);
		ili9341_send_byte(limit_end_y & 0xFF);
	}
	ili9341_wait_for_send_done();
	for(u8 i=0;i<250;i++) ili9341_send_byte(0xAA);
	ili9341_deselect_chip();
}

void ili9341_duplicate_pixel(const ili9341_color_t color, uint32_t count)
{
	/* Sanity check to make sure that the pixel count is not zero */
	ili9341_send_command(ILI9341_CMD_MEMORY_WRITE);

	while (count--) {
		ili9341_send_byte(color);
		ili9341_send_byte(color >> 8);
	}

	ili9341_wait_for_send_done();
	ili9341_deselect_chip();
}

/**
 * \brief Set the display top left drawing limit
 *
 * Use this function to set the top left limit of the drawing limit box.
 *
 * \param x The x coordinate of the top left corner
 * \param y The y coordinate of the top left corner
 */
void ili9341_set_top_left_limit(ili9341_coord_t x, ili9341_coord_t y)
{
	limit_start_x = x;
	limit_start_y = y;

//	ili9341_send_draw_limits(0);
}

void ili9341_set_bottom_right_limit(ili9341_coord_t x, ili9341_coord_t y)
{
	limit_end_x = x;
	limit_end_y = y;

//	ili9341_send_draw_limits(1);
}

/*void draw(ili9341_color_t pixels[][]){
	ili9341_set_limits(0, 0, ILI9341_DEFAULT_WIDTH, ILI9341_DEFAULT_HEIGHT);

	ili9341_send_command(ILI9341_CMD_MEMORY_WRITE);
	for(uint8_t k = 0; k<ILI9341_DEFAULT_HEIGHT; k++){
		for(uint16_t i=0; i<ILI9341_DEFAULT_WIDTH; i++){
			ili9341_send_byte(pixels[i][k]);
			ili9341_send_byte(pixels[i][k] >> 8);
		}
	}

	ili9341_wait_for_send_done();

}*/
void draw_alt(uint8_t pixels[], ili9341_color_t color,uint8_t width){

	for(uint16_t i=0; i<ILI9341_DEFAULT_WIDTH; i++){
		ili9341_set_limits(i-width/2, pixels[i]-width/2, i+width/2,
									pixels[i]+width/2);


		ili9341_duplicate_pixel(color, width*width);


	}
}
void erase_alt(uint8_t pixels[], uint8_t width){
	for(uint16_t i=0; i<ILI9341_DEFAULT_WIDTH; i++){
		ili9341_set_limits(i-width/2, pixels[i]-width/2, i+width/2,
									pixels[i]+width/2);


		ili9341_duplicate_pixel(ILI9341_COLOR(0, 0, 0), width*width);


	}
}
void erase_draw_alt(uint8_t prev_pixels[], uint8_t pixels[], ili9341_color_t color, uint8_t width){
	for(uint16_t i=0; i<ILI9341_DEFAULT_WIDTH; i++){
		ili9341_set_limits(i-width/2, prev_pixels[i]-width/2, i+width/2,
										prev_pixels[i]+width/2);

		ili9341_duplicate_pixel(ILI9341_COLOR(0, 0, 0), width*width);

		ili9341_set_limits(i-width/2, pixels[i]-width/2, i+width/2,
													pixels[i]+width/2);

		ili9341_duplicate_pixel(color, width*width);

	}
}
void draw_rectangle(uint8_t xstart, uint8_t ystart, uint8_t xend, uint8_t yend, ili9341_color_t color){
	ili9341_set_limits(xstart, ystart, xend, yend);

	uint8_t width = xend-xstart;
	uint8_t height = yend-ystart;
	ili9341_duplicate_pixel(ILI9341_COLOR(0, 0, 0), width*height);

}

void black_screen(){
	ili9341_set_limits(0, 0, ILI9341_DEFAULT_WIDTH,
								ILI9341_DEFAULT_HEIGHT);
	ili9341_send_command(ILI9341_CMD_MEMORY_WRITE);
	ili9341_duplicate_pixel(ILI9341_COLOR(0, 0, 0), ILI9341_DEFAULT_WIDTH*ILI9341_DEFAULT_HEIGHT);
	ili9341_wait_for_send_done();
	ili9341_deselect_chip();
}
