project(f3_blink C ASM)
cmake_minimum_required(VERSION 3.5.0)

add_definitions(-DSTM32F429xx -DFREERTOS -DENABLE_DEBUGGER_UART -DENABLE_ETHERNET)

file(GLOB_RECURSE USER_SOURCES "Projects/engg4810/*.c")
file(GLOB_RECURSE HAL_SOURCES "Core/STM32F4xx_HAL_Driver/Src/*.c")

add_library(CMSIS
Core/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
Core/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f429xx.s)

include_directories(Projects/engg4810)
include_directories(Core/STM32F4xx_HAL_Driver/Inc)
include_directories(Core/CMSIS/Include)
include_directories(Core/CMSIS/Device/ST/STM32F4xx/Include)
include_directories(Platforms/nucleof429zi)
include_directories(Core/FreeRTOS/Source/include)
include_directories(Core/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI)
include_directories(Core/FreeRTOS/Source/portable/GCC/ARM_CM4F)
include_directories(Core/tinyprintf)
include_directories(Core/LwIP/src/include)
include_directories(Core/LwIP/src/include)
include_directories(Core/LwIP/src/core/lwip)
include_directories(Core/LwIP/src/api)
include_directories(Core/LwIP/src/core/ipv4)
include_directories(Core/LwIP/src/netif)
include_directories(Core/LwIP/system/OS)
include_directories(Platforms/nucleof429zi/net)


add_executable(${PROJECT_NAME}.elf ${USER_SOURCES} ${HAL_SOURCES} ${LINKER_SCRIPT})

target_link_libraries(${PROJECT_NAME}.elf CMSIS)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")
set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")