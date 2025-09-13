#ifndef DEFINICIONES_H
#define DEFINICIONES_H
    #include <stdio.h>
    #include "pico/stdlib.h"
    #include "hardware/gpio.h"
    #include "hardware/i2c.h"
    #include "hardware/uart.h"
    #include "pico/binary_info.h"

    #include "machine/fastmath.h"
    #include "FreeRTOS.h"
    #include "FreeRTOSConfig.h"

    #include "semphr.h"
    #include "task.h"
    #include "queue.h"
    #include "event_groups.h"

    // Dirección I2C del MAX30100
    #define     MAX30100_I2C_ADDRESS        0x57
    #define     MAX30100_I2C_READ           0XAF
    #define     MAX30100_I2C_WRITE          0XAE
    // Registros del MAX30100
    #define     REG_INTR_STATUS         0x00
    #define     REG_INTR_ENABLE         0x01
    #define     REG_FIFO_WR_PTR         0x02
    #define     REG_OVF_COUNTER         0x03
    #define     REG_FIFO_RD_PTR         0x04
    #define     REG_FIFO_DATA           0x05
    #define     REG_MODE_CONFIG         0x06
    #define     REG_SPO2_CONFIG         0x07
    #define     REG_LED_CONFIG          0x09
    #define     REG_TEMP_INTG           0x16
    #define     REG_TEMP_FRAC           0x17
    #define     REG_REV_ID              0xFE
    #define     REG_PART_ID             0xFF

    // Configuraciones
    #define     MODE_HR_ONLY            0x02
    #define     MODE_SPO2               0x03

    #define     SAMPLING_RATE_100HZ     0x03
    #define     LED_PULSE_AMP_27mA      0x1F

    #define     PICO_INT_PIN 6

    #define     INTR_SPO2_READY             (1 << 5)  // SPO2 data ready
    #define     INTR_HR_READY               (1 << 6)  // Heart rate data ready
    #define     INTR_FIFO_FULL              (1 << 7)  // FIFO almost full
    #define     INTR_PWR_RDY                (1 << 0)  // Power ready

    #define     Obtener_muestras            (1 << 0)
    #define     Filtrar                     (1 << 2)
    #define     Calcular_valores            (1 << 3)
    #define     Enviar_valores              (1 << 4)

    #define UART_ID     uart0
    #define BAUD_RATE   115200

    // We are using pins 0 and 1, but see the GPIO function select table in the
    // datasheet for information on which other pins can be used.
    #define UART_TX_PIN 0
    #define UART_RX_PIN 1

    typedef struct Data_modulo_s {
    uint16_t R_LED;
    uint16_t IR_LED;
    } MAX_Data_t;


    typedef struct Estructura_Modulo_s{
        QueueHandle_t       Data_Queue;
        EventGroupHandle_t  Eventos;
        SemaphoreHandle_t   Mutex;
    } Estructura_MAX30100_t;



#endif