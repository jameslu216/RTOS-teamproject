//trace.c
//authored by Edge

#include "trace.h"
#include <FreeRTOS.h>
#include "video.h"

static portLONG switchOutTime;
static portBASE_TYPE switchPriority;

static portLONG testTimes = 0;
static portLONG testData[256];

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

void recordTraceData(portLONG measureData) {
    testData[testTimes] = measureData;
    testTimes = ((testTimes + 1) & 0x100);
}

void outputTraceData() {
    if(testTimes > 128) {
        volatile int *timeStamp = (int *) 0x3f003004;
        for(int i = 0;i < 128;++i) {
            printHex("Record=", testData[i], BLUE_TEXT);
            int stop = *timeStamp + 20 * 1000;
            while (*timeStamp < stop) __asm__("nop");
        }
    }
}