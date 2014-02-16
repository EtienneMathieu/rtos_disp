
#include "FreeRTOS.h"
#include "timers.h"
#include "delay.h"
#include "stm32f4xx.h"

uint8_t state;
xTimerHandle t;

void vAutoReloadTimerFunction(xTimerHandle xTimer) {
	state=0;
	xTimerDelete(t,0);
}

void delay_ms(uint8_t x){
	const signed char name = "lcd";
	t = xTimerCreate(
	                     &name,
	                     // The timer period in ticks.
	                     (x * 1),
	                     // The timers will auto-reload themselves when they expire.
	                     pdFALSE,
	                     // Assign each timer a unique id equal to its array index.
	                     ( void * ) x,
	                     // Each timer calls the same callback when it expires.
	                     vAutoReloadTimerFunction
	                   );
	state=1;
	xTimerStart(t,0);
	while(state==1);
}
