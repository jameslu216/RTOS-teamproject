PREFIX=D:/Program Files (x86)/GNU Tools ARM Embedded/4.7 2013q3/lib/gcc/arm-none-eabi/4.7.4
ARMGNU ?= arm-none-eabi
INCLUDEPATH ?= FreeRTOS/Source/include
INCLUDEPATH2 ?= FreeRTOS/Source/portable/GCC/RaspberryPi
INCLUDEPATH3 ?= FreeRTOS/Demo/RaspberryPi_GCC/Drivers
INCLUDEPATH4 ?= FreeRTOS/Demo/RaspberryPi_GCC


COPS = -Wall -O2 -lm -lgcc  -ffreestanding  -std=gnu99   -mcpu=cortex-a7 -I  $(INCLUDEPATH) -I $(INCLUDEPATH2) -I $(INCLUDEPATH3) -I $(INCLUDEPATH4)

gcc : kernel7.img

OBJS = build/startup.o 

OBJS +=build/gpio.o  
OBJS +=build/irq.o
OBJS +=build/uart.o
OBJS +=build/led.o
OBJS +=build/main.o
	
OBJS +=build/port.o
OBJS +=build/portisr.o
#OBJS +=build/heap_1.o
#OBJS +=build/heap_2.o
#OBJS +=build/heap_3.o
OBJS +=build/heap_4.o
OBJS +=build/mem_man.o
OBJS +=build/croutine.o
OBJS +=build/list.o
OBJS +=build/queue.o
OBJS +=build/tasks.o
OBJS +=build/timers.o
OBJS +=build/event_groups.o

# For FAT filesystem
OBJS +=build/ff_crc.o
OBJS +=build/ff_dev_support.o
OBJS +=build/ff_dir.o
OBJS +=build/ff_error.o
OBJS +=build/ff_fat.o
OBJS +=build/ff_file.o
OBJS +=build/ff_format.o
OBJS +=build/ff_ioman.o
OBJS +=build/ff_locking.o
#OBJS +=build/ff_locking.org.o
OBJS +=build/ff_memory.o
OBJS +=build/ff_stdio.o
OBJS +=build/ff_string.o
OBJS +=build/ff_sys.o
OBJS +=build/ff_time.o

OBJS +=build/libc.a
OBJS +=build/libgcc.a

clean :
	rm -f build/*.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f build/*.bc

build/%.o : FreeRTOS/Demo/RaspberryPi_GCC/%.s
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c -o $@ $<

build/%.o : FreeRTOS/Demo/RaspberryPi_GCC/Drivers/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
		
build/%.o : FreeRTOS/Demo/RaspberryPi_GCC/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
	
build/%.o : FreeRTOS/Source/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
	
build/%.o : FreeRTOS/Source/portable/GCC/RaspberryPi/%.c
	$(ARMGNU)-gcc $(COPS) -c -o $@ $<
           
build/%.o : FreeRTOS/Source/portable/MemMang/%.c
	$(ARMGNU)-gcc $(COPS) -c -o $@ $<

kernel7.img : raspberrypi.ld $(OBJS)
	$(ARMGNU)-ld $(OBJS) -T raspberrypi.ld -o freertos_bcm2837.elf
	$(ARMGNU)-objdump -D freertos_bcm2837.elf > freertos_bcm2837.list
	$(ARMGNU)-objcopy freertos_bcm2837.elf -O ihex freertos_bcm2837.hex
	$(ARMGNU)-objcopy freertos_bcm2837.elf -O binary freertos_bcm2837.bin
	$(ARMGNU)-objcopy freertos_bcm2837.elf -O binary kernel7.img