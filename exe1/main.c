#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"

#define FIRST_GPIO 2
#define BUTTON_GPIO (FIRST_GPIO+7)

const int BTN_PIN_G = 22;
const int SW_PIN = 28;
volatile int flag_btn = 0;
volatile int flag_sw = 1;

int map[] = {0x000, 0x001, 0x003, 0x07, 0x0f, 0x01f};

void bar_init() {
    for (int gpio = FIRST_GPIO; gpio < FIRST_GPIO + 10; gpio++) {
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_OUT);
    }
}

void bar_display(int val) {
    // for (int gpio = FIRST_GPIO; gpio < FIRST_GPIO + 7; gpio++) {
    //     gpio_put(gpio, 0);
    // }
    int32_t mask = map[val] << FIRST_GPIO;
    gpio_clr_mask(0xfff);
    gpio_set_mask(mask);
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {         // fall edge
        if (gpio == BTN_PIN_G)
            flag_btn = 1;
        else
            flag_sw = 1;
    }
    else if (events == 0x8) {  // rise edge
        flag_sw = -1;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
    gpio_set_irq_enabled_with_callback(BTN_PIN_G,
        GPIO_IRQ_EDGE_FALL,
        true,
        &btn_callback);

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);
    gpio_set_irq_enabled_with_callback(SW_PIN,
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
        true,
        &btn_callback);

    int val = 0;
    bar_init();
    bar_display(0);

    // while(1) {
    //     printf("%d \n", gpio_get(SW_PIN));
    //     sleep_ms(10);

    // }

    while (true) {
        if (flag_btn) {
            flag_btn = 0 ;
            val += flag_sw;
            if (val < 0)
                val = 0;
            else if (val > 5)
                val = 5;

            printf("%d \n", val);
            bar_display(val);
        }

    }
}
