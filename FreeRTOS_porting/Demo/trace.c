//trace.c
//authored by Edge

#include "trace.h"
#include <FreeRTOS.h>
#include "video.h"

static portTickType switchOutTime;
static portBASE_TYPE switchPriority;

void vThreadContextSwitchIn(unsigned portBASE_TYPE priority) {
	if(switchPriority < priority) {
	    printHex("Preemption Time=", xTaskGetTickCount()-switchOutTime, BLUE_TEXT);		
	} else if(switchPriority == priority) {
	    printHex("Thread Switch Time=", xTaskGetTickCount()-switchOutTime, BLUE_TEXT);		
	}
}

void vThreadContextSwitchOut(unsigned portBASE_TYPE priority) {
	switchOutTime = xTaskGetTickCount();
	switchPriority = priority;
}