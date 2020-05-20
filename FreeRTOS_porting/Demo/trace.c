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
		#ifdef configMEASURE_PREEMPTION_TIME
	    printHex("Preemption Time=", ulCurrentTimeInMicroSecond-switchOutTime, BLUE_TEXT);	
	    #endif	
	} else if(switchPriority == priority) {
		#ifdef configMEASURE_THREAD_SWITCH_TIME
	    printHex("Thread Switch Time=", ulCurrentTimeInMicroSecond-switchOutTime, BLUE_TEXT);		
	    #endif
	}
}

void vThreadContextSwitchOut(unsigned portBASE_TYPE priority) {
    portGET_CURRENT_TIME_IN_MICROSECOND();
    extern volatile unsigned portLONG ulCurrentTimeInMicroSecond;
	switchOutTime = ulCurrentTimeInMicroSecond;
	switchPriority = priority;
}
