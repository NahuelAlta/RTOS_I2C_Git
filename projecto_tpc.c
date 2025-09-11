// #include "MAX30100.h"
// #include "Filtro.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "fastmath.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "definiciones.h"


uint8_t     max30100_write_register         (uint8_t reg, uint8_t value) ;
uint8_t     max30100_read_register          (uint8_t reg);
uint8_t     max30100_read_interrupt_status  ();
bool        max30100_enable_interrupts      (uint8_t interrupt_mask);
void        max30100_init_with_interrupts   ();

typedef struct Data_modulo_s {
    uint16_t R_LED;
    uint16_t IR_LED;
} MAX_Data_t;


typedef struct Estructura_Modulo_s{
    QueueHandle_t       Data_Queue;
    QueueHandle_t       Data_F_Queue;
    QueueHandle_t       SPO2_HR;
    EventGroupHandle_t  Eventos;
} Estructura_MAX30100_t;

QueueHandle_t           MAX_Data_Queue;
QueueHandle_t           Constantes_vitales;
QueueHandle_t           Valores_Filtrados;
EventGroupHandle_t      Eventos_MAX30100;
Estructura_MAX30100_t   Struct_MAX30100;

// // void gpio_check (void*pvParameters);
void init_queues_and_stuff (){
    MAX_Data_Queue                  = xQueueCreate(1,sizeof(MAX_Data_t));
    Valores_Filtrados               = xQueueCreate(1,sizeof(MAX_Data_t));
    Constantes_vitales              = xQueueCreate(1,sizeof(MAX_Data_t));
    Eventos_MAX30100                = xEventGroupCreate();
    Struct_MAX30100.Data_Queue      =MAX_Data_Queue;
    Struct_MAX30100.Data_F_Queue    =Valores_Filtrados;
    Struct_MAX30100.SPO2_HR         =Constantes_vitales;
    Struct_MAX30100.Eventos         =Eventos_MAX30100;
}

void gpio_check (void*pvParameters){
    Estructura_MAX30100_t *Estructura = (Estructura_MAX30100_t *) pvParameters;
    while(1){
       // printf("Entre a la del gpio check\n");
        if (gpio_get(PICO_INT_PIN)==0){
            max30100_read_interrupt_status();
            xEventGroupSetBits(Estructura->Eventos, Obtener_muestras);
        }
        else{
            vTaskDelay(50/portTICK_PERIOD_MS);
        }
    }
    
} 

void Filtrar_muestras(void *pvParameters){
    Estructura_MAX30100_t *Estructura = (Estructura_MAX30100_t *) pvParameters;
    MAX_Data_t Datos;
    printf("Entre a la subrutina de filtrar los datos antes del while\n");
    while(1){
        xQueueReceive(Estructura->Data_Queue,&Datos,portMAX_DELAY);
        printf("Los valores son: %u para el led IR y %u para el led R\n",Datos.IR_LED,Datos.R_LED);
        }
}

void max30100_read_fifo_data (void * pvParameters) {
    Estructura_MAX30100_t *Estructura = (Estructura_MAX30100_t *) pvParameters;
    uint8_t fifo_buffer[4];  // 2 bytes por muestra (IR + RED)
    MAX_Data_t Valores_obtenidos;
    uint8_t Obtenidos=0;
    uint8_t reg = REG_FIFO_DATA;
    EventBits_t bits=0;
    printf("Entre a la subrutina de obtener los datos antes del while\n");
    while(1){
        bits=xEventGroupWaitBits(Estructura->Eventos,
            Obtener_muestras,
            pdTRUE,
            pdTRUE,
            (250/portTICK_PERIOD_MS));
        if ((bits & Obtener_muestras) !=0){
            i2c_write_blocking  (i2c_default, MAX30100_I2C_ADDRESS, &reg, 1, true);
            Obtenidos=i2c_read_blocking   (i2c_default, MAX30100_I2C_ADDRESS, fifo_buffer, 4, false);
            if (Obtenidos == 4){
                Valores_obtenidos.IR_LED    = (fifo_buffer[0] << 8) | fifo_buffer[1];
                Valores_obtenidos.R_LED     = (fifo_buffer[2] << 8) | fifo_buffer[3];
                xQueueSend(Estructura->Data_Queue,&Valores_obtenidos,portMAX_DELAY);
            }
            else{
                printf("No se pudo obtener correctamente\n");
            }
        }
    }
}


int main() {
    stdio_init_all();

    i2c_init(i2c_default,400000);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN,GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN,GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    gpio_init(PICO_INT_PIN);
    gpio_pull_up(PICO_INT_PIN);
    gpio_set_dir(PICO_INT_PIN,false);

    max30100_init_with_interrupts();
    init_queues_and_stuff();
    xTaskCreate(gpio_check, "int_check", configMINIMAL_STACK_SIZE*1, &Struct_MAX30100, 2, NULL);
    xTaskCreate(Filtrar_muestras, "filtrar", configMINIMAL_STACK_SIZE*2, &Struct_MAX30100, 3, NULL);
    xTaskCreate(max30100_read_fifo_data, "obtener_muestra", configMINIMAL_STACK_SIZE*2, &Struct_MAX30100, 4, NULL);
    
    vTaskStartScheduler();

    return 0;
}

uint8_t    max30100_write_register(uint8_t reg, uint8_t value) {
    uint8_t result;
    uint8_t registro=reg;
    uint8_t valor=value;
    uint8_t datos[2]={registro,valor};
    result=i2c_write_blocking(i2c_default, MAX30100_I2C_ADDRESS, datos, 2, false);
    sleep_ms(100);  
    return result==2;
}

// Leer registro
uint8_t max30100_read_register(uint8_t reg) {
    uint8_t registro=reg;
    uint8_t value      =0;
    i2c_write_blocking(i2c_default, MAX30100_I2C_ADDRESS, &registro, 1, true);
    i2c_read_blocking(i2c_default, MAX30100_I2C_ADDRESS, &value, 1,false);  
    return value;
}

bool max30100_enable_interrupts(uint8_t interrupt_mask){
    uint8_t status;
    status=max30100_write_register(REG_INTR_ENABLE, interrupt_mask);
    return status;
}

uint8_t max30100_read_interrupt_status(){
    uint8_t status;
    status = max30100_read_register(REG_INTR_STATUS);
    return status;
}

void   max30100_init_with_interrupts(){
    static uint8_t i=0;
    uint8_t status=0;
    sleep_ms(3000);
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
        printf("Se pudo configurar el SPO2\n");
    }  // SPO2_CONFIG = 100Hz, 16bit

    // Configurar corriente de LEDs
    if(max30100_write_register(REG_LED_CONFIG, 0xFF)){
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