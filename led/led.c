
#include "hardware/pwm.h"
#include "pico/stdlib.h"

int main() {
    const uint LED_PIN = 25;
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);

    pwm_set_wrap(slice_num, 1000); // PWM-Periode
    pwm_set_enabled(slice_num, true);

    while (true) {
        for (int duty = 0; duty <= 1000; duty += 5) {
            pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_PIN), duty);
            sleep_ms(1);
        }
        for (int duty = 1000; duty >= 0; duty -= 5) {
            pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_PIN), duty);
            sleep_ms(1);
        }
    }
}
