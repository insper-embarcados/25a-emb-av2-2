
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

/**
 * 0..1.0V: Desligado
 * 1..2.0V: 150 ms
 * 2..3.3V: 400 ms
 */

 const int LED_PIN_Y = 5;
 const int LED_PIN_B = 9;


 const int BTN_PIN = 28;

volatile int g_timer_500ms, g_timer_150ms, g_timer_5s = 0;


volatile int btnStatus = 0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if(gpio == BTN_PIN) btnStatus = 1;
  } else if (events == 0x8) { // rise edge
  }

  
}


bool timer_500ms_callback(repeating_timer_t *rt)
{
    g_timer_500ms = 1;
    return true; // keep repeating
}

bool timer_150ms_callback(repeating_timer_t *rt)
{
    g_timer_150ms = 1;
    return true; // keep repeating
}

bool timer_5s_callback(repeating_timer_t *rt)
{
    g_timer_5s = 0;
    return true; // keep repeating
}

int main()
{
    stdio_init_all();


    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);


    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);


        gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);
  
    gpio_set_irq_enabled_with_callback(
        BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);



    repeating_timer_t timer_500ms, timer_150ms, timer_5s;

    if (!add_repeating_timer_ms( 500, 
                                timer_500ms_callback,
                                NULL, 
                                &timer_500ms)) {
        printf("Failed to add timer\n");
    }

    if (!add_repeating_timer_ms(150,
                                timer_150ms_callback,
                                NULL, 
                                &timer_150ms)) {
        printf("Failed to add timer\n");
    }



    volatile int inverteY, inverteB = 0;


    while (1)
    {



        if(btnStatus == 1){
            g_timer_5s = 1;
                if (!add_repeating_timer_ms(5000, 
                                timer_5s_callback,
                                NULL, 
                                &timer_5s)) {
        printf("Failed to add timer\n");

        }
        btnStatus = 0;
        printf("btnStatus: %d\n", btnStatus);
        }

        if (g_timer_5s == 1)
        {
           // printf("g_timer_5s: %d\n", g_timer_5s);

            if(g_timer_500ms == 1){
                
                g_timer_500ms = 0;
                gpio_put(LED_PIN_Y, inverteY);
                inverteY = !inverteY;
                printf("500ms - %s\n", inverteY ? "ON" : "OFF");
                //gpio_put(LED_PIN_Y, 0);
            }
            if(g_timer_150ms == 1){
                
                g_timer_150ms = 0;
                gpio_put(LED_PIN_B, inverteB);
                inverteB = !inverteB;
                printf("150ms - %s\n", inverteB ? "ON" : "OFF");
            }  
            
            
            //g_timer_5s = 0;
            //

        }

        if(g_timer_5s == 0){
            gpio_put(LED_PIN_Y, 0);
            gpio_put(LED_PIN_B, 0);
            cancel_repeating_timer(&timer_5s);
        }

        //printf("g_timer_5s: %d\n", g_timer_5s);

        //cancel_repeating_timer(&timer_5s);

    }

    return 0;
}
