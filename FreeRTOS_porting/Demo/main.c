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

void taskOutputTest() {
	println("ABCDE abcde", GREEN_TEXT);

	extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;
	long i;
	for(i = 0;i < 5;++i) {
		portGET_CURRENT_TIME_IN_MICROSECOND();
		printHex("Current time is=", ulCurrentTimeInMicroSecond, GREEN_TEXT);
		portLONG startTime = ulCurrentTimeInMicroSecond;
		workload_1ms();
		portGET_CURRENT_TIME_IN_MICROSECOND();
		printHex("Current time is=", ulCurrentTimeInMicroSecond, GREEN_TEXT);
		printHex("Time difference is=", ulCurrentTimeInMicroSecond - startTime, GREEN_TEXT);		
	}


	vTaskDelete(NULL);
}


void taskInterruptLatency() {
	println("Measuring Interrupt Latency", GREEN_TEXT);
	accelerateLedState = !accelerateLedState;
	extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;

	portGET_CURRENT_TIME_IN_MICROSECOND();
	portLONG startTime = ulCurrentTimeInMicroSecond;
	SetGpio(ACCELERATE_LED_GPIO, accelerateLedState);
	while(ReadGpio(ACCELERATE_LED_GPIO) != accelerateLedState);
	portGET_CURRENT_TIME_IN_MICROSECOND();
	portLONG endTime = ulCurrentTimeInMicroSecond;

	printHex("Interrupt Latency: ", (int)(endTime-startTime), BLUE_TEXT);
	vTaskDelete(NULL);
}

void taskMeasureWorkload1msStandard() {
	println("Getting 1ms workload", GREEN_TEXT);
	extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;
	long averageWorkloadAmount = 0;
	int i;
	for(i = 0;i < 1000;++i) {	
		int workloadAmount = 0;
		portTickType startTime;
		portTickType endTime;
		do {
			++workloadAmount;
			portGET_CURRENT_TIME_IN_MICROSECOND();
			startTime = ulCurrentTimeInMicroSecond;
            measure_workload_1ms(workloadAmount);
            portGET_CURRENT_TIME_IN_MICROSECOND();
			endTime = ulCurrentTimeInMicroSecond;
		}while(endTime-startTime < 1000);
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
//	xTaskCreate(taskInterruptLatency, "MEASURE_INTERRUPT_LATENCY", 128, NULL, 3, NULL);
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

