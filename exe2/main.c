
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int PIN_LED_Y = 5;
const int PIN_LED_B = 9;
const int PIN_BTN_G = 28;

volatile int flag_btn = 0;
volatile int f_timer_y = 0;
volatile int f_timer_b = 0;
volatile int timer_fired = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {         // fall edge
        flag_btn = 1;
    }
}

bool timer_y_callback(repeating_timer_t* rt) {
    f_timer_y = 1;
    return true; // keep repeating
}

bool timer_b_callback(repeating_timer_t* rt) {
    f_timer_b = 1;
    return true; // keep repeating
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    timer_fired = 1;
    return 0;
}

int main() {
    stdio_init_all();

    gpio_init(PIN_BTN_G);
    gpio_set_dir(PIN_BTN_G, GPIO_IN);
    gpio_pull_up(PIN_BTN_G);
    gpio_set_irq_enabled_with_callback(PIN_BTN_G,
        GPIO_IRQ_EDGE_FALL,
        true,
        &btn_callback);

    gpio_init(PIN_LED_Y);
    gpio_set_dir(PIN_LED_Y, GPIO_OUT);
    gpio_init(PIN_LED_B);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);

    repeating_timer_t timer_y;
    repeating_timer_t timer_b;

    if (!add_repeating_timer_ms(300, 
        timer_y_callback,
                                NULL, 
                                &timer_y)) {
        printf("Failed to add timer\n");
    }

    if (!add_repeating_timer_ms(150, 
        timer_b_callback,
                                NULL, 
                                &timer_b)) {
        printf("Failed to add timer\n");
    }
    
    int enable_y = 0;
    int enable_b = 0;

    int status_y = 0;
    int status_b = 0;

    while (1) {

        if (flag_btn) {
            flag_btn = 0;
            alarm_id_t alarm = add_alarm_in_ms(1000, alarm_callback, NULL, false);
            enable_y = 1;
            enable_b = 1;
        }

        if (timer_fired){
            enable_y = 0;
            enable_b = 0;     
            timer_fired = 0;     
            gpio_put(PIN_LED_Y, 0);
            gpio_put(PIN_LED_B, 0);
        }

        if (enable_y && f_timer_y){
            f_timer_y = 0;
            status_y = !status_y;
            gpio_put(PIN_LED_Y, status_y);
        }

        if (enable_b && f_timer_b){
            f_timer_b = 0;
            status_b = !status_b;
            gpio_put(PIN_LED_B, status_b);
        }
    }
}
