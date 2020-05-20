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

#define ACCELERATE_LED_GPIO 23
int accelerateLedState = 1;

void taskOutputTest() {
	println("ABCDE abcde", GREEN_TEXT);

	portGET_CURRENT_TIME_IN_MICROSECOND();
	extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;
	printHex("Current time is=", ulCurrentTimeInMicroSecond, GREEN_TEXT);

	vTaskDelete(NULL);
}

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
	accelerateLedState = !accelerateLedState;
	portTickType startTick = xTaskGetTickCount();
	SetGpio(ACCELERATE_LED_GPIO, accelerateLedState);
	while(ReadGpio(ACCELERATE_LED_GPIO) != accelerateLedState);
	portTickType endTick = xTaskGetTickCount();
	printHex("Interrupt Latency: ", (int)(endTick-startTick), BLUE_TEXT);
	vTaskDelete(NULL);
}

void measure_workload_1ms(long workloadAmount) {
	long i;
	for(i = 0;i < workloadAmount;++i) {
		++i;
		--i;
	}
}

void workload_1ms() {
    measure_workload_1ms(0x22);
}

void taskMeasureWorkload1msStandard() {
	println("Getting 1ms workload", GREEN_TEXT);
	long averageWorkloadAmount = 0;
	int i;
	for(i = 0;i < 1000;++i) {	
		int workloadAmount = 0;
		portTickType startTick;
		portTickType endTick;
		do {
			++workloadAmount;
			startTick = xTaskGetTickCount();
            measure_workload_1ms(workloadAmount);
			endTick = xTaskGetTickCount();
		}while(endTick-startTick < 1);
//		printHex("Admissible workload=", workloadAmount, BLUE_TEXT);
		averageWorkloadAmount += workloadAmount;
	}
	printHex("Average admissible workload=", averageWorkloadAmount/1000, BLUE_TEXT);
	vTaskDelete(NULL);
}

int main(void) {

	SetGpioFunction(ACCELERATE_LED_GPIO, 1);

	initFB();

	SetGpio(ACCELERATE_LED_GPIO, accelerateLedState);

	DisableInterrupts();
	InitInterruptController();

	xTaskCreate(taskOutputTest, "OUTPUT_TEST", 128, NULL, 0, NULL);
	xTaskCreate(taskMeasureTest, "MEASURE_TEST", 128, NULL, 4, NULL);
	xTaskCreate(taskInterruptLatency, "MEASURE_INTERRUPT_LATENCY", 128, NULL, 3, NULL);
    // Admissible Workload for 1ms is 34
	xTaskCreate(taskMeasureWorkload1msStandard, "MEASURE_1MS_WORKLOAD", 128, NULL, 1, NULL);

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

