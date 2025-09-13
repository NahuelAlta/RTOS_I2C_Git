#include "freertos_task.h"
#include "max30100.h"
#include "definiciones.h"

void gpio_check (void*pvParameters){
    Estructura_MAX30100_t *Estructura = (Estructura_MAX30100_t *) pvParameters;
    uint8_t PTR_Read    = 0;
    uint8_t PTR_Write   = 0;
    while(1){
        if (gpio_get(PICO_INT_PIN)==0){
            xSemaphoreTake(Estructura->Mutex,portMAX_DELAY);
            max30100_read_interrupt_status();
            PTR_Write=max30100_read_register(REG_FIFO_WR_PTR);
            if (PTR_Write>0){
                PTR_Read=PTR_Write-1;
            }
            else{
                PTR_Read=0xFF;
            }
            PTR_Read=max30100_write_register(REG_FIFO_RD_PTR,PTR_Read);
            xSemaphoreGive(Estructura->Mutex);
            xEventGroupSetBits(Estructura->Eventos, Obtener_muestras);
        }
        else{
            vTaskDelay(10/portTICK_PERIOD_MS);
        }
    }
    
} 

void Enviar_muestras(void *pvParameters){
    Estructura_MAX30100_t *Estructura = (Estructura_MAX30100_t *) pvParameters;
    MAX_Data_t Datos;
    printf("Entre a la subrutina de filtrar los datos antes del while\n");
    uint8_t     i=0;
    uint16_t    Valores[2]={0};
    EventBits_t bits=0;
    while(1){
        xQueueReceive(Estructura->Data_Queue,&Datos,portMAX_DELAY);
        bits=xEventGroupWaitBits(Estructura->Eventos,
                                Enviar_valores,
                                pdTRUE,
                                pdTRUE,
                                250/portTICK_PERIOD_MS);
        Valores[0]=Datos.R_LED;
        Valores[1]=Datos.IR_LED;
        if ((bits & Enviar_valores)!=0){
            uart_write_blocking(uart0,(uint8_t*)Valores,4);
        }
    }

}

void MAX30100_Read_data (void * pvParameters) {
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
            xSemaphoreTake(Estructura->Mutex,portMAX_DELAY);
            i2c_write_blocking  (i2c_default, MAX30100_I2C_ADDRESS, &reg, 1, true);
            Obtenidos=i2c_read_blocking   (i2c_default, MAX30100_I2C_ADDRESS, fifo_buffer, 4, false);
            if (Obtenidos == 4){
                Valores_obtenidos.IR_LED    = (fifo_buffer[0] << 8) | fifo_buffer[1];
                Valores_obtenidos.R_LED     = (fifo_buffer[2] << 8) | fifo_buffer[3];
                xQueueSend(Estructura->Data_Queue,&Valores_obtenidos,100/portTICK_PERIOD_MS);
            }
            else{
                printf("No se pudo obtener correctamente\n");
            }
            xSemaphoreGive(Estructura->Mutex);
        }
    }
}
