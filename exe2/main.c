#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int LED_PIN_Y = 5;
const int LED_PIN_B = 9;

const int BTN_PIN = 28;


volatile int g_timer_500ms, g_timer_150ms, g_timer_5s = 0;

volatile int btnStatus = 0;

void btn_callback(uint gpio, uint32_t events)
{
    if (events == 0x4)
        if (gpio == BTN_PIN)
            btnStatus = 1;
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
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    repeating_timer_t timer_500ms, timer_150ms, timer_5s;

    add_repeating_timer_ms( 500,
                            timer_500ms_callback,
                            NULL,
                            &timer_500ms);

    add_repeating_timer_ms( 150,
                            timer_150ms_callback,
                            NULL,
                            &timer_150ms);

    int inverteY,
    inverteB = 0;

    while (1)
    {

        if (btnStatus == 1)
        {
            g_timer_5s = 1;
            add_repeating_timer_ms( 5000,
                                    timer_5s_callback,
                                    NULL,
                                     &timer_5s);
            btnStatus = 0;
        }

        if (g_timer_5s == 1)
        {

            if (g_timer_500ms == 1)
            {
                g_timer_500ms = 0;
                gpio_put(LED_PIN_Y, inverteY);
                inverteY = !inverteY;
            }
            if (g_timer_150ms == 1)
            {
                g_timer_150ms = 0;
                gpio_put(LED_PIN_B, inverteB);
                inverteB = !inverteB;
            }
        }

        if (g_timer_5s == 0)
        {
            gpio_put(LED_PIN_Y, 0);
            gpio_put(LED_PIN_B, 0);
            cancel_repeating_timer(&timer_5s);
        }
    }

    return 0;
}
