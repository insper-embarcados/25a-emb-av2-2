#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

const int LED_BAR_0 = 2;
const int LED_BAR_1 = 3;
const int LED_BAR_2 = 4;
const int LED_BAR_3 = 5;
const int LED_BAR_4 = 6;

const int SW_PIN = 28;
const int BTN_PIN = 22;


volatile int btnStatus = 0;
volatile int swStatus = 1;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if(gpio == BTN_PIN) btnStatus = 1;
    else if(gpio == SW_PIN) swStatus = 1;
  } else if (events == 0x8) { // rise edge
    if(gpio == SW_PIN) swStatus = 0;
  }

  
}

void bar_init(){
    gpio_init(LED_BAR_0);
    gpio_set_dir(LED_BAR_0, GPIO_OUT);

    gpio_init(LED_BAR_1);
    gpio_set_dir(LED_BAR_1, GPIO_OUT);

    gpio_init(LED_BAR_2);
    gpio_set_dir(LED_BAR_2, GPIO_OUT);

    gpio_init(LED_BAR_3);
    gpio_set_dir(LED_BAR_3, GPIO_OUT);

    gpio_init(LED_BAR_4);
    gpio_set_dir(LED_BAR_4, GPIO_OUT);


    gpio_put(LED_BAR_0, 0);
    gpio_put(LED_BAR_1, 0);
    gpio_put(LED_BAR_2, 0);
    gpio_put(LED_BAR_3, 0);
    gpio_put(LED_BAR_4, 0);

  
}

void bar_display(int val){
    gpio_put(LED_BAR_0, 0);
    gpio_put(LED_BAR_1, 0);
    gpio_put(LED_BAR_2, 0);
    gpio_put(LED_BAR_3, 0);
    gpio_put(LED_BAR_4, 0);

if(val == 0){
    gpio_put(LED_BAR_0, 0);
    gpio_put(LED_BAR_1, 0);
    gpio_put(LED_BAR_2, 0);
    gpio_put(LED_BAR_3, 0);
    gpio_put(LED_BAR_4, 0);
}


else if(val == 1){
   gpio_put(LED_BAR_0, 1);
    gpio_put(LED_BAR_1, 0);
    gpio_put(LED_BAR_2, 0);
    gpio_put(LED_BAR_3, 0);
    gpio_put(LED_BAR_4, 0);
}

else if(val == 2){
  gpio_put(LED_BAR_0, 1);
    gpio_put(LED_BAR_1, 1);
    gpio_put(LED_BAR_2, 0);
    gpio_put(LED_BAR_3, 0);
    gpio_put(LED_BAR_4, 0);
}

else if(val == 3){
    gpio_put(LED_BAR_0, 1);
    gpio_put(LED_BAR_1, 1);
    gpio_put(LED_BAR_2, 1);
    gpio_put(LED_BAR_3, 0);
    gpio_put(LED_BAR_4, 0);
}

else if(val == 4){
    gpio_put(LED_BAR_0, 1);
    gpio_put(LED_BAR_1, 1);
    gpio_put(LED_BAR_2, 1);
    gpio_put(LED_BAR_3, 1);
    gpio_put(LED_BAR_4, 0);
}

else if(val == 5){
    gpio_put(LED_BAR_0, 1);
    gpio_put(LED_BAR_1, 1);
    gpio_put(LED_BAR_2, 1);
    gpio_put(LED_BAR_3, 1);
    gpio_put(LED_BAR_4, 1);
}


}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);
  
    gpio_set_irq_enabled_with_callback(
        BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);


        gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

        gpio_set_irq_enabled(
        SW_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    int valorBar = 0;

    bar_init();
    
    while (true) {


        if(btnStatus == 1){
            btnStatus = 0;
           

            if(swStatus == 1) valorBar++;
            else if (swStatus == 0) valorBar--;

            if(valorBar > 5)  valorBar = 5;
            else if(valorBar < 0) valorBar = 0;

            bar_display(valorBar);


            printf("sw: %d - valorBar: %d \n", swStatus, valorBar);
            
        }

    }
}
