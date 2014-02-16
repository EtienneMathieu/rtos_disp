#include "stm32f4xx.h"
#include <stdio.h>
#include "misc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "delay.h"
#define ARM_MATH_CM4
#include "arm_math.h"

void vTaskLED1(void *pvParameters);
void vTaskLED2(void *pvParameters);
void vTBUTTON(void *pvParameters);
void vTDISP(void *pvParameters);


int main(void){
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure pin in output push/pull mode */
	  GPIO_InitStructure.GPIO_Pin =
			  GPIO_Pin_12 |
			  GPIO_Pin_13 |
			  GPIO_Pin_14 |
			  GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  GPIO_InitTypeDef GPIO_InitStructureb;
	  GPIO_InitStructureb.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructureb.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructureb.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOA, &GPIO_InitStructureb);

	  //GPIO_SetBits(GPIOD, GPIO_Pin_13);
	  //delay_ms(5);
	  //GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	  /*delay_ms(10);
	  GPIO_SetBits(GPIOD, GPIO_Pin_12);
	  delay_ms(500);
	  GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	  delay_ms(500);*/


	  xTaskCreate( vTaskLED1, ( signed char * ) "LED1", ( unsigned short ) 120, NULL, 2,
	                          ( xTaskHandle * ) NULL);
	  /*xTaskCreate( vTaskLED2, ( signed char * ) "LED2", ( unsigned short ) 120, NULL, 2,
	                          ( xTaskHandle * ) NULL);
	  xTaskCreate( vTBUTTON, ( signed char * ) "BUTTON", ( unsigned short ) 120, NULL, 2,
	  	                          ( xTaskHandle * ) NULL);*/
	  xTaskCreate( vTDISP, ( signed char * ) "DISP", ( unsigned short ) 1024, NULL, 2,
	  	  	                          ( xTaskHandle * ) NULL);
	  vTaskStartScheduler();
}

void vTaskLED1(void *pvParameters) {

        while(1) {
               GPIO_SetBits(GPIOD, GPIO_Pin_12);
               vTaskDelay(500);
               GPIO_ResetBits(GPIOD, GPIO_Pin_12);
               vTaskDelay(500);
        }

}

void vTaskLED2(void *pvParameters) {

        while(1) {
                GPIO_SetBits(GPIOD, GPIO_Pin_14);
                vTaskDelay(321);
                GPIO_ResetBits(GPIOD, GPIO_Pin_14);
                vTaskDelay(321);
        }

}
void vTBUTTON(void *pvParameters) {
        while(1) {
                if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)!=0)
                	GPIO_SetBits(GPIOD, GPIO_Pin_15);
                vTaskDelay(200);
                GPIO_ResetBits(GPIOD, GPIO_Pin_15);
                vTaskDelay(200);
        }

}
void vTDISP(void *pvParameters) {
	lcdInit();
			const ili9341_color_t color_table[] = {
				ILI9341_COLOR(0, 0, 255), ILI9341_COLOR(0, 255, 0),
				ILI9341_COLOR(255, 0, 0), ILI9341_COLOR(255, 255, 0),
				ILI9341_COLOR(255, 0, 255), ILI9341_COLOR(0, 255, 255)
			};
		#define TOTAL_PIXELS ((uint32_t)ILI9341_DEFAULT_WIDTH * ILI9341_DEFAULT_HEIGHT)
		while(1){
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)!=0){
				uint8_t k = ILI9341_DEFAULT_HEIGHT/2-30;
				uint16_t period = ILI9341_DEFAULT_WIDTH/2;
				float32_t step = 3.14159/period;
				uint8_t pixels[ILI9341_DEFAULT_WIDTH];
				GPIO_SetBits(GPIOD, GPIO_Pin_15);
				for(uint16_t i=0; i<ILI9341_DEFAULT_WIDTH; i++){
					float32_t g = step*i;
					pixels[i] =  round(k*sin(g) + k + 15);
				}
				draw(pixels, color_table[1]);
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
			}
		}
}
