/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 
 const int buzzer = 12;
 
 volatile int g_timer_0 = 0;
 
 bool timer_0_callback(repeating_timer_t *rt) {
     g_timer_0 = 1;
     return true; // keep repeating
 }
 
 
 // Baremetal  -- ok
 // Timer  -- ok
 // ADC   -- ok
 // IRQ
 
 
 // modifica frequencia de acordo com potenciometro
 // timer de acordo com valor de potenciometro
 
 
 
 
 
 void buzzer_tone(int freq, int duration_ms) {
     gpio_init(buzzer);
     gpio_set_dir(buzzer, GPIO_OUT);
     gpio_put(buzzer,0);
     int half_period_us = 1000000 / (2 * freq); // usar no timer
     int cycles = (duration_ms * 1000) / (2 * half_period_us); // fixo
     repeating_timer_t timer_0;
     int timer_0_hz = 5;
     int counter = 0;
     // cria timer
     if (!add_repeating_timer_us(half_period_us / timer_0_hz, 
                                           timer_0_callback,
                                           NULL, 
                                           &timer_0)) {
                   printf("Failed to add timer\n");
           }
     while (counter < cycles){
       if(g_timer_0){
         gpio_put(buzzer, !gpio_get(buzzer));
         counter ++;
         g_timer_0 = 0;
       }
     }
     cancel_repeating_timer(&timer_0);
 
     }
 
 
 int main() {
     stdio_init_all();
     
 
     adc_init();
 
     // Make sure GPIO is high-impedance, no pullups etc
     adc_gpio_init(26);
     adc_select_input(0);
     uint16_t result = 0;
     const float conversion_factor = 3.3f / (1 << 12);
     while(1){
       result = adc_read();
       if(result > 0){
 
 
 
       //0v  ->  0hz
       //3,3 v -> 3,300 hz
 
       uint16_t result = adc_read();
       // printf("%f \n", result * conversion_factor);
       buzzer_tone((int)result * conversion_factor * 1000,50);
 
       }
           
     }
 }