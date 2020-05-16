// main.c
// Authored by Jared Hull
// Modified by Roope Lindström & Emil Pirinen
//
// Main initialises the devices
// Tasks simulate car lights

#include <FreeRTOS.h>
#include <task.h>
#include <string.h>

#include "interrupts.h"
#include "gpio.h"
#include "video.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

//Only for debug, normally should not 
//   include private header
#include "FreeRTOS_IP_Private.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef long int32_t;


void taskMeasureTest() {
	println("Measure Test - start", 0x00);
	portTickType startTick = xTaskGetTickCount();
	long i;
	for(i = 0;i < 1e7;++i) {
		// NOP
	}
	portTickType endTick = xTaskGetTickCount();
	println("Measure Test - end", 0x00);
	vTaskDelete(NULL);
}

void taskInterruptLatency() {
	println("Measuring Interrupt Latency", 0x00);
	portTickType startTick = xTaskGetTickCount();
	portTickType endTick = xTaskGetTickCount();
	println("Interrupt Latency: ", endTick-startTick);
	vTaskDelete(NULL);
}

int main(void) {

	DisableInterrupts();
	InitInterruptController();

	xTaskCreate(taskMeasureTest, "MEASURE_TEST", 128, NULL, 0, NULL);
	xTaskCreate(taskInterruptLatency, "MEASURE_INTERRUPT_LATENCY", 128, NULL, 0, NULL);

	println("Starting task scheduler", GREEN_TEXT);

	vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1) {
		;
	}
}

