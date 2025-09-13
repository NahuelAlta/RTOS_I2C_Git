#include "max30100.h"
#include "definiciones.h"



uint8_t     max30100_write_register(uint8_t reg, uint8_t value) {
    uint8_t result;
    uint8_t registro=reg;
    uint8_t valor=value;
    uint8_t datos[2]={registro,valor};
    result=i2c_write_blocking(i2c_default, MAX30100_I2C_ADDRESS, datos, 2, false);
    sleep_ms(10);  
    return result==2;
}

// Leer registro
uint8_t     max30100_read_register(uint8_t reg) {
    uint8_t registro=reg;
    uint8_t value      =0;
    i2c_write_blocking(i2c_default, MAX30100_I2C_ADDRESS, &registro, 1, true);
    i2c_read_blocking(i2c_default, MAX30100_I2C_ADDRESS, &value, 1,false);  
    return value;
}

bool        max30100_enable_interrupts(uint8_t interrupt_mask){
    uint8_t status;
    status=max30100_write_register(REG_INTR_ENABLE, interrupt_mask);
    return status;
}

uint8_t     max30100_read_interrupt_status(){
    uint8_t status;
    status = max30100_read_register(REG_INTR_STATUS);
    return status;
}

void        max30100_init_with_interrupts(){
    static uint8_t i=0;
    uint8_t status=0;
    max30100_read_interrupt_status();
    printf("El valor del status es: %u",status);

    if (max30100_write_register(REG_MODE_CONFIG, 0x40)){
        sleep_ms(100);
        printf("Entre a la subrutina y se pudo configurar\n");
    }
      // MODE_CONFIG reset bit
    if (gpio_get(PICO_INT_PIN)==0){
        max30100_read_interrupt_status();
    }
    // Configurar modo SPO2
    if (max30100_write_register(REG_MODE_CONFIG, 0x03)){
        printf("Se pudo configurar el modo\n");
    }
      // MODE_CONFIG = SPO2 mode
    
    // Configurar SPO2 (100Hz, 16 bits)
    if(max30100_write_register(REG_SPO2_CONFIG, 0x07)){
    //if(max30100_write_register(REG_SPO2_CONFIG, 0x1C)){    
        printf("Se pudo configurar el SPO2\n");
    }  // SPO2_CONFIG = 100Hz, 16bit

    // Configurar corriente de LEDs
    if(max30100_write_register(REG_LED_CONFIG, 0x88)){
        printf("Se pudo configurar el Led\n");
    }  // LED_CONFIG = 50mA RED, 27mA IR

    // Configurar FIFO (almost full interrupt después de 16 muestras)
    if(max30100_write_register(REG_FIFO_WR_PTR, 0x00)){
        printf("Se pudo vaciar la fifo\n");
    }  // FIFO_WR_PTR = 0

    if (max30100_write_register(REG_OVF_COUNTER, 0x00)){
        printf("Se pudo escribir el puntero de lectura\n");
    }  // FIFO_RD_PTR = 0

    if (max30100_write_register(0x0A, 0x00)){
        printf("Se pudo escribir el puntero de escritura\n");
    }  // FIFO_OVF_CNT = 0

    uint8_t interrupt_mask = INTR_SPO2_READY;     // + Interrupción cuando dato SPO2 listo
    if (max30100_enable_interrupts(interrupt_mask))
    {
        printf("Se pudo configurar la interrupción para spo2\n");
    }
}