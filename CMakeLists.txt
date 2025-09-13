cmake_minimum_required(VERSION 3.13...3.27)

#Esto son buenas prácticas. Establece los estándares de C y C++
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Acá configuro la placa que quiera, en mi caso es pico2_w y rp2350.
set(PICO_BOARD pico2_w CACHE STRING "Board type")
set(PICO_PLATFORM rp2350 CACHE STRING "Board type")

#Acá incluyo los archivos cmake de los sdk que descargo
include(pico_sdk_import.cmake)
#El kernel que dio resultado esta en "portable/ThirdParty/GCC/RP2350_ARM_NTZ"
include(FreeRTOS_Kernel_import.cmake)

#----------- Creacion de projecto. NO VARIAR EL ORDEN SUPERIOR------------
project(projecto_tp CXX C ASM)

#Se inicializa el sdk de la raspberry pi pico
pico_sdk_init()

#Se añade el ejecutable.
add_executable(projecto_tp projecto_tpc.c freertos_task.c max30100.c)

#Esto, es importantísimo. No me corrió el FreeRTOS sin haber seteado esto. En otros lados aparecía como ${CMAKE_CURRENT_LIST_DIR}/... pero no
#Lo habia probado y no dada. no encontraba el destino de FreeRTOSConfig.h
#Una forma mas precisa de definirlo, es creando una carpeta include y ahi colocando el freertos config. Esto de abajo 
#va a funcionar siempre y cuando tenga: 
# proyecto/
# ├── build/
# ├── .vscode/
# ├── CMakeLists.txt
# ├── FreeRTOS_Kernel_import.cmake
# ├── FreeRTOSConfig.h        # ← Está aquí mismo
# ├── hello_world.c
# └── pico_sdk_import.cmake
# Al hacer {CMAKE_CURRENT_LIST_DIR} esto busca en el directorio "padre", que vendria a ser "proyecto". Si esta dentro de una carpeta, debo
# Aclararlo haciendo ${CMAKE_CURRENT_LIST_DIR}/include (por ejemplo) 
target_include_directories(projecto_tp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/includes
    ${FREERTOS_KERNEL_PATH}/include     # ← FreeRTOS headers
    ${FREERTOS_KERNEL_PATH}/portable/GCC/RP2350_ARM_NTZ  # ← Port específico
    )

pico_enable_stdio_usb(projecto_tp 1)
pico_enable_stdio_uart(projecto_tp 0)

target_link_libraries(projecto_tp 
    hardware_gpio
    hardware_i2c
    hardware_uart
    FreeRTOS-Kernel-Heap4
    pico_stdlib)
    
pico_add_extra_outputs(projecto_tp)