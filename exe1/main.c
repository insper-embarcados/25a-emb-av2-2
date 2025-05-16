#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"


const int pins[5] = {2,3,4,5,6};
const int btn = 22;
const int swit = 28;
volatile int swit_crementa = 0;
volatile int btn_verde = 0;


void btn_callback(uint gpio, uint32_t events) {
    if (gpio == swit){ 
        if(events == 0x4){
            swit_crementa = 1;
        }
        else if (events == 0x8){
            swit_crementa = 0;
        }
    }
    if(gpio == btn && events == 0x4){
        btn_verde = 1;
    }

  }


// finalizado inicializacao
void bar_init(){
    for(int i = 0; i<5; i++){
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_OUT);
        gpio_put(pins[i], 0);
    }
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_pull_up(btn);
    gpio_init(swit);
    gpio_set_dir(swit, GPIO_IN);
    gpio_pull_up(swit);
}

// acende d=ou desliga leds
void bar_display(int val){
    for(int i = 0; i< val; i++){
        gpio_put(pins[i], 1);
    }

    for(int j = 5; j> val; j-- ){
        gpio_put(pins[j-1], 0);
    }

}

int main() {
    stdio_init_all();
    bar_init();
    gpio_set_irq_enabled_with_callback(btn, 
        GPIO_IRQ_EDGE_FALL,
        true,
        &btn_callback);
    gpio_set_irq_enabled(swit, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    int counter = 0;
    while (true) {
        if(btn_verde){
            if(swit_crementa ){
                if ((counter + 1) <=5)counter++;
                printf("switch incrementando\n");
                printf("counter is %d", counter);
                bar_display(counter);

                
            } else if (!swit_crementa){
                if ((counter - 1) >=0)counter--;
                printf("switch decrementando\n");
                printf("counter is %d", counter);
                bar_display(counter);
              

            }
            btn_verde = 0;
        }
    }

    
}
