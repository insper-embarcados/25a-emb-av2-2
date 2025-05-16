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
QueueHandle_t xQueueLed1;
QueueHandle_t xQueueLed2;



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

    }
}

/**
 * Seu código vem aqui!
 */

 void main_task(void* p){

    input_t dados;
    while(1){ 
        // direciona pacotes para as tasks certas
        if(xQueueReceive(xQueueInput, &dados, pdMS_TO_TICKS(100))){
            // envia dados de cada led para cada task
            xQueueSend(xQueueLed1, &dados.num_led1,0);
            xQueueSend(xQueueLed2, &dados.num_led2,0);
        }
    }
 }

void led_1_task(void* p){
    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);
    int pisca_vezes = 0;
    while(1){
        if(xQueueReceive(xQueueLed1, &pisca_vezes, pdMS_TO_TICKS(100))){
            for(int i = 0; i<= pisca_vezes; i++){
                gpio_put(LED_PIN_Y, 1);
                vTaskDelay(pdMS_TO_TICKS(250));
                gpio_put(LED_PIN_Y, 0);
                vTaskDelay(pdMS_TO_TICKS(250));
            }
            // envia semaforo pro outro piscar
        }
    }
}


int main() {
    stdio_init_all();

    /**
     * manter essas duas linhas!
     */
    xQueueInput = xQueueCreate(32, sizeof(input_t));
    xQueueLed1 = xQueueCreate(32, sizeof(input_t));
    xQueueLed2 = xQueueCreate(32, sizeof(input_t));
    xSemaphoreLed2 = xSemaphoreCreateBinary();
    xTaskCreate(input_task, "Input", 256, NULL, 1, NULL);
    xTaskCreate(main_task, "Input", 256, NULL, 1, NULL);

    /**
     * Seu código vem aqui!
     */

    vTaskStartScheduler();

    while (1) {}

    return 0;
}
