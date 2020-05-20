//trace.c
//authored by Edge

#include "trace.h"
#include <FreeRTOS.h>
#include "video.h"

static portLONG switchOutTime;
static portBASE_TYPE switchPriority;

void vThreadContextSwitchIn(unsigned portBASE_TYPE priority) {
    portGET_CURRENT_TIME_IN_MICROSECOND();
    extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;
	if(switchPriority < priority) {
	    printHex("Preemption Time=", ulCurrentTimeInMicroSecond-switchOutTime, BLUE_TEXT);		
	} else if(switchPriority == priority) {
	    printHex("Thread Switch Time=", ulCurrentTimeInMicroSecond-switchOutTime, BLUE_TEXT);		
	}
}

void vThreadContextSwitchOut(unsigned portBASE_TYPE priority) {
    portGET_CURRENT_TIME_IN_MICROSECOND();
    extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;
	switchOutTime = ulCurrentTimeInMicroSecond;
	switchPriority = priority;
}
