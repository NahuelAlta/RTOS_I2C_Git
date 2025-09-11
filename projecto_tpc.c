#include "definiciones.h"
#include "freertos_task.h"
#include "max30100.h"

   
QueueHandle_t           MAX_Data_Queue;
QueueHandle_t           Constantes_vitales;
QueueHandle_t           Valores_Filtrados;
EventGroupHandle_t      Eventos_MAX30100;
Estructura_MAX30100_t   Struct_MAX30100;

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
    xTaskCreate(MAX30100_Read_data, "obtener_muestra", configMINIMAL_STACK_SIZE*2, &Struct_MAX30100, 4, NULL);
    
    vTaskStartScheduler();

    return 0;
}

