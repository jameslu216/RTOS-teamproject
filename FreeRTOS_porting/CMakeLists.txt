CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project(FreeRTOS)
include_directories("FreeRTOS/Source/include")
include_directories("FreeRTOS/Source/portable/GCC/RaspberryPi")
include_directories("Drivers")
include_directories("Drivers/lan9514")
include_directories("Drivers/FreeRTOS-Plus-TCP")
