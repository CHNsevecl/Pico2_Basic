#include <stdio.h>
#include "pico/stdlib.h"


int main()
{
    stdio_init_all();
    for (int i = 0; i < 49; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
    }

    while (true) {
        for (int i = 0; i < 49; i++) {
            gpio_put(i, 1);
        }
        sleep_ms(500);

        for (int i = 0; i < 49; i++) {
            gpio_put(i, 0);
        }
        sleep_ms(500);
    }
}
