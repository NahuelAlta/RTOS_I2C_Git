#ifndef FREERTOS_TASK_H
#define FREERTOS_TASK_H

void gpio_check             (void * pvParameters);
void Enviar_muestras       (void * pvParameters);
void MAX30100_Read_data     (void * pvParameters);

#endif