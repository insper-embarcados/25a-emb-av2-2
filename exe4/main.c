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

QueueHandle_t xQueueInput;
QueueHandle_t xQueueLed2;
QueueHandle_t xQueueLed1;

SemaphoreHandle_t xSemaphoreLed2;

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
        vTaskDelay(1);
    }
}

/**
 * Seu código vem aqui!
 */

void main_task(void* p) {

    input_t test_case;
    while (true) {
        if (xQueueReceive(xQueueInput, &test_case, 1))
        {
            xQueueSend(xQueueLed1, &test_case.num_led1, 0);
            xQueueSend(xQueueLed2, &test_case.num_led2, 0);
        }
        vTaskDelay(1);
    }
}

void led_2_task(void* p) {
      gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    int num_led2;

    while (true) {
        if (xSemaphoreTake(xSemaphoreLed2, 1)) {
         if (xQueueReceive(xQueueLed2, &num_led2, 1))
        {
               
            while(num_led2 > 0){
                gpio_put(LED_PIN_Y, 1);
                vTaskDelay(250);
                gpio_put(LED_PIN_Y, 0);
                vTaskDelay(250);
                num_led2--;
            }
            
        }
        vTaskDelay(1);
    }
    }
}


void led_1_task(void* p) {
    
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);
    
    int num_led1;

    while (true) {
        
         if (xQueueReceive(xQueueLed1, &num_led1, 1))
        {
            while(num_led1 > 0){
                
                gpio_put(LED_PIN_B, 1);
                vTaskDelay(250);
                gpio_put(LED_PIN_B, 0);
                vTaskDelay(250);
                num_led1--;
                }
                xSemaphoreGive(xSemaphoreLed2);
            }
        
        vTaskDelay(1);
    }
}




int main() {
    stdio_init_all();

    /**
     * manter essas duas linhas!
     */
    xQueueInput = xQueueCreate(32, sizeof(input_t));
    xQueueLed2 = xQueueCreate(32, sizeof(int));
    xQueueLed1 = xQueueCreate(32, sizeof(int));

    xSemaphoreLed2 = xSemaphoreCreateBinary();

    xTaskCreate(input_task, "Input", 256, NULL, 1, NULL);
    xTaskCreate(main_task, "Main", 256, NULL, 1, NULL);
    xTaskCreate(led_2_task, "Led_2", 256, NULL, 1, NULL);
    xTaskCreate(led_1_task, "Led_1", 256, NULL, 1, NULL);

    /**
     * Seu código vem aqui!
     */

    vTaskStartScheduler();

    while (1) {}

    return 0;
}
