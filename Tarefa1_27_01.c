#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

// Definições de pinos
#define LED_RED_PIN 13
#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define MATRIX_PIN 7
#define NUM_PIXELS 25

// Variáveis globais
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
volatile uint8_t current_number = 0;
absolute_time_t last_button_a_time;
absolute_time_t last_button_b_time;

// Protótipos de funções
void gpio_irq_handler(uint gpio, uint32_t events);
void debounce_button(uint gpio, absolute_time_t *last_time, volatile bool *button_pressed);
void display_number(uint8_t number, PIO pio, uint sm);
void piscar_led_vermelho();

uint32_t rgb_color(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

void piscar_led_vermelho() {
    static absolute_time_t last_toggle_time = 0;
    static bool led_state = false;

    if (absolute_time_diff_us(last_toggle_time, get_absolute_time()) >= 200000) {
        led_state = !led_state;
        gpio_put(LED_RED_PIN, led_state);
        last_toggle_time = get_absolute_time();
    }
}

void gpio_irq_handler(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A_PIN) {
        debounce_button(BUTTON_A_PIN, &last_button_a_time, &button_a_pressed);
    } else if (gpio == BUTTON_B_PIN) {
        debounce_button(BUTTON_B_PIN, &last_button_b_time, &button_b_pressed);
    }
}

void debounce_button(uint gpio, absolute_time_t *last_time, volatile bool *button_pressed) {
    absolute_time_t current_time = get_absolute_time();
    if (absolute_time_diff_us(*last_time, current_time) >= 200000) {
        *button_pressed = true;
        *last_time = current_time;
    }
}

void display_number(uint8_t number, PIO pio, uint sm) {
    const uint32_t timer_frames[10][25] = {
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0}
    };

    for (int i = 0; i < NUM_PIXELS; i++) {
        uint32_t color = (timer_frames[number][i] == 1) ? rgb_color(0.3, 0.1, 0) : rgb_color(0, 0, 0);
        pio_sm_put_blocking(pio, sm, color);
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, MATRIX_PIN);

    while (true) {
        piscar_led_vermelho();
        if (button_a_pressed) {
            button_a_pressed = false;
            current_number = (current_number + 1) % 10;
            display_number(current_number, pio, sm);
        }
        if (button_b_pressed) {
            button_b_pressed = false;
            current_number = (current_number == 0) ? 9 : current_number - 1;
            display_number(current_number, pio, sm);
        }
        sleep_ms(100);
    }
}
