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
	println("Measure Test - start", GREEN_TEXT);
	portTickType startTick = xTaskGetTickCount();
	long i, j = 1;
	for(i = 0;i < 100;++i) {
		j += i;
	}
	portTickType endTick = xTaskGetTickCount();
	println("Measure Test - end", GREEN_TEXT);


	println("Available printf Test - start", GREEN_TEXT);
	printHex("Output Int: ", j, BLUE_TEXT);
	printHex("Current Tick: ", startTick, BLUE_TEXT);
	printHex("Latency: ", (int)(endTick-startTick), BLUE_TEXT);

	vTaskDelete(NULL);
}

void taskInterruptLatency() {
	println("Measuring Interrupt Latency", GREEN_TEXT);
	portTickType startTick = xTaskGetTickCount();
	vTaskSuspend(NULL);
	portTickType endTick = xTaskGetTickCount();
	printHex("Current Tick: ", startTick, BLUE_TEXT);
	printHex("Interrupt Latency: ", (int)(endTick-startTick), BLUE_TEXT);
	vTaskDelete(NULL);
}

xTaskHandle xHandle;

void taskInterruptResume() {
	println("Resume Interrupt Latency", GREEN_TEXT);
	vTaskResume(xHandle);
	vTaskDelete(NULL);
}

int main(void) {

	initFB();

	DisableInterrupts();
	InitInterruptController();

	xTaskCreate(taskMeasureTest, "MEASURE_TEST", 128, NULL, 2, NULL);
	xTaskCreate(taskInterruptLatency, "MEASURE_INTERRUPT_LATENCY", 128, NULL, 1, &xHandle);
	xTaskCreate(taskInterruptResume, "MEASURE_INTERRUPT_RESUME", 128, NULL, 0, NULL);

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

