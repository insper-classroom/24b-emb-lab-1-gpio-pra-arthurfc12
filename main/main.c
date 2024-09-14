#include "hardware/gpio.h"
#include <stdio.h>
#include "pico/stdlib.h"

// GPIO numbers for the button and the LED
const int BTN_PIN = 2;
const int LED_PIN = 15;

// GPIO numbers for the stepper motor connected to ULN2003 driver
const int MOTOR_PIN1 = 16;
const int MOTOR_PIN2 = 17;
const int MOTOR_PIN3 = 18;
const int MOTOR_PIN4 = 19;

// Sequence for the 28BYJ-48 stepper motor
const int step_sequence[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
};

void step_motor(int step) {
    gpio_put(MOTOR_PIN1, step_sequence[step][0]);
    gpio_put(MOTOR_PIN2, step_sequence[step][1]);
    gpio_put(MOTOR_PIN3, step_sequence[step][2]);
    gpio_put(MOTOR_PIN4, step_sequence[step][3]);
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    // Initialize motor pins
    gpio_init(MOTOR_PIN1);
    gpio_set_dir(MOTOR_PIN1, GPIO_OUT);
    gpio_init(MOTOR_PIN2);
    gpio_set_dir(MOTOR_PIN2, GPIO_OUT);
    gpio_init(MOTOR_PIN3);
    gpio_set_dir(MOTOR_PIN3, GPIO_OUT);
    gpio_init(MOTOR_PIN4);
    gpio_set_dir(MOTOR_PIN4, GPIO_OUT);

    int last_button_state = 1;
    while (true) {
        int button_state = gpio_get(BTN_PIN);
        if (button_state == 0 && last_button_state == 1) { // Button press detected
            gpio_put(LED_PIN, 1);  // Turn on LED
            // Rotate stepper motor 360 degrees
            for (int i = 0; i < 512; i++) { // 512 steps for a full revolution (4096 steps per revolution divided by 8 steps per sequence)
                for (int step = 0; step < 8; step++) {
                    step_motor(step);
                    sleep_ms(2);  // Adjust speed of the stepper motor
                }
            }
            gpio_put(LED_PIN, 0);  // Turn off LED
        }
        last_button_state = button_state;
        sleep_ms(10); // Debounce delay
    }
}
