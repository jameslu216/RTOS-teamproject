#ifndef TRACE_H
#define TRACE_H

#include "portmacro.h"

void vThreadContextSwitchIn(unsigned portBASE_TYPE priority);

void vThreadContextSwitchOut(unsigned portBASE_TYPE priority);

#endif // TRACE_H