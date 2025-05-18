/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

const int LED_PIN_B = 8;
const int LED_PIN_Y = 13;

typedef struct input {
    int num_led1;
    int num_led2;
} input_t;

 QueueHandle_t xQueueLed1;
 QueueHandle_t xQueueLed2;
 
 SemaphoreHandle_t xSemaphore_led1;

QueueHandle_t xQueueInput;

/**
 * NÃO MEXER!
 */
void input_task(void* p) {
    input_t test_case;

    test_case.num_led1 = 3;
    test_case.num_led2 = 4;
    xQueueSend(xQueueInput, &test_case, 0);

    test_case.num_led1 = 0;
    test_case.num_led2 = 2;
    xQueueSend(xQueueInput, &test_case, 0);

    while (true) {

    }
}

/**
 * Seu código vem aqui!
 */



 void main_task(void* p) {
     input_t test_case;
 
     int num_led1;
     int num_led2;
 
     while(1) {
         if (xQueueReceive(xQueueInput, &test_case, portMAX_DELAY)){
             num_led1 = test_case.num_led1;
             num_led2 = test_case.num_led2;
             xQueueSend(xQueueLed1, &num_led1, 0);
             vTaskDelay(1000);
             xQueueSend(xQueueLed2, &num_led2, 0);
         }
     }
 }
 
 void led_1_task(void* p) {
     gpio_init(LED_PIN_B);
     gpio_set_dir(LED_PIN_B, GPIO_OUT);
 
     int delay = 250;
     int num = 0;
     while (true) {
         if (xQueueReceive(xQueueLed1, &num, portMAX_DELAY)){
             printf("led1 %d \n", num);
             for (int i = 0; i < num; i++) {
                 gpio_put(LED_PIN_B, 1);
                 vTaskDelay(pdMS_TO_TICKS(delay));
                 gpio_put(LED_PIN_B, 0);
                 vTaskDelay(pdMS_TO_TICKS(delay));
             }
             xSemaphoreGive(xSemaphore_led1);
             printf("Fim 1\n");
         }
     }
 }
 
 void led_2_task(void* p) {
     gpio_init(LED_PIN_Y);
     gpio_set_dir(LED_PIN_Y, GPIO_OUT);
 
     int delay = 250;
     int num = 0;
     while (true) {
         if (xQueueReceive(xQueueLed2, &num, portMAX_DELAY)){
             xSemaphoreTake(xSemaphore_led1, portMAX_DELAY);
             printf("led2 %d \n", num);
             for (int i = 0; i < num; i++) {
                 gpio_put(LED_PIN_Y, 1);
                 vTaskDelay(pdMS_TO_TICKS(delay));
                 gpio_put(LED_PIN_Y, 0);
                 vTaskDelay(pdMS_TO_TICKS(delay));
             }
             printf("Fim 2\n");
         }
     }
 }

int main() {
    stdio_init_all();

    /**
     * manter essas duas linhas!
     */
    xQueueInput = xQueueCreate(32, sizeof(input_t));
    xQueueLed1 = xQueueCreate(32, sizeof(int));
     xQueueLed2 = xQueueCreate(32, sizeof(int));
     xSemaphore_led1 = xSemaphoreCreateBinary();
 
     xTaskCreate(input_task, "Input", 256, NULL, 1, NULL);
 
     xTaskCreate(main_task, "Main", 256, NULL, 1, NULL);
     xTaskCreate(led_1_task, "LED_Task 1", 256, NULL, 1, NULL);
     xTaskCreate(led_2_task, "LED_Task 2", 256, NULL, 1, NULL);
    /**
     * Seu código vem aqui!
     */

    vTaskStartScheduler();

    while (1) {}

    return 0;
}
