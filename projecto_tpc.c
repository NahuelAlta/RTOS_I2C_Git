#include "definiciones.h"
#include "freertos_task.h"
#include "max30100.h"

   
QueueHandle_t           MAX_Data_Queue;
EventGroupHandle_t      Eventos_MAX30100;
Estructura_MAX30100_t   Struct_MAX30100;
SemaphoreHandle_t       I2C_Mutex;

void init_queues_and_stuff (){
    MAX_Data_Queue                  =   xQueueCreate(16,sizeof(MAX_Data_t));
    Eventos_MAX30100                =   xEventGroupCreate();
    I2C_Mutex                       =   xSemaphoreCreateMutex();
    Struct_MAX30100.Data_Queue      =   MAX_Data_Queue;
    Struct_MAX30100.Eventos         =   Eventos_MAX30100;
    Struct_MAX30100.Mutex           =   I2C_Mutex;
}

void uart_irq (){
    static uint8_t check=0;
    uart_read_blocking(uart0,&check,1);
    if (check==0xAA){
        xEventGroupSetBitsFromISR(Struct_MAX30100.Eventos,
                                Enviar_valores,
                                pdFALSE);
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

    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    irq_set_exclusive_handler(UART0_IRQ, uart_irq);
    irq_set_enabled(UART0_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

    gpio_init(PICO_INT_PIN);
    gpio_pull_up(PICO_INT_PIN);
    gpio_set_dir(PICO_INT_PIN,false);

    max30100_init_with_interrupts();
    init_queues_and_stuff();
    xTaskCreate(gpio_check, "int_check", configMINIMAL_STACK_SIZE*1, &Struct_MAX30100, 2, NULL);
    xTaskCreate(Enviar_muestras, "filtrar", configMINIMAL_STACK_SIZE*2, &Struct_MAX30100, 3, NULL);
    xTaskCreate(MAX30100_Read_data, "obtener_muestra", configMINIMAL_STACK_SIZE*2, &Struct_MAX30100, 4, NULL);
    vTaskStartScheduler();

    return 0;
}

