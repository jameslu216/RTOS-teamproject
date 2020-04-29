/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Characters on the LCD are used to simulate LED's.  In this case the 'ParTest'
 * is really operating on the LCD display.
 *-----------------------------------------------------------*/
#include <FreeRTOS.h>
#include <task.h>
#include <stdint.h>
#include "Drivers/irq.h"
#include "Drivers/gpio.h"
#include "Drivers/uart.h"
#include "Drivers/led.h"


void task1(void *pParam) {
    print(" task1 ");
    int i = 0;
    while(1)
    {
        led(1);
        vTaskDelay(1000);
        led(0);
        vTaskDelay(1000);
        print(" task 1 run ");
    }
}

void task2(void *pParam) {

    print(" task2 ");
    int i = 0;
    while(1)
    {
        vTaskDelay(2000);
        print(" task2 run ");
    }
}


/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
void main (void)
{
    uart_init();
    led(1);
    xTaskCreate(task1, "LED_0", 128, NULL, 0, NULL);
    xTaskCreate(task2, "LED_1", 128, NULL, 0, NULL);
    vTaskStartScheduler();
    /*
     *	We should never get here, but just in case something goes wrong,
     *	we'll place the CPU into a safe loop.
     */
    while(1) {
        ;
    }
}
