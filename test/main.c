/**
 * MIT License
 * 
 * Copyright (c) 2022 Daniel Garcia-Briseno
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include "pico.h"
#include "pico/stdlib.h"
#include "sensor.pio.h"
#include "hardware/pio.h"

#define SENSE_START_IRQ 0b0001
#define SENSE_END_IRQ   0b0010
#define PIO_SM_2_IRQ 0b0100
#define PIO_SM_3_IRQ 0b1000

volatile uint64_t sense_start_time = 0;
volatile uint64_t sense_end_time = 0;

void print_debug_info() {
    printf("FSTAT: 0x%08X\n", pio0_hw->fstat);
    printf("FDEBUG: 0x%08X\n", pio0_hw->fdebug);
}

void sense_irq() {
    uint32_t irq = pio0_hw->irq;
    printf("Working with irq: 0x%08X\n", irq);
    if (irq & SENSE_START_IRQ) {
        sense_start_time = time_us_64();
    }
    if (irq & SENSE_END_IRQ) {
        sense_end_time = time_us_64();
    }

    hw_clear_bits(&pio0_hw->irq, irq);
}

void begin_sense() {
    pio_sm_put_blocking(pio0, 0, 1);
}

int main() {
    stdio_init_all();
    sleep_ms(5000);
    puts("Testing the HC-SR04 distance sensor with PIO");
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &distance_sensor_program);
    distance_sensor_program_init(pio, sm, offset, 0, sense_irq);

    uint count = 0;
    while (true) {
        printf("Sense count %d: \n", count++);
        begin_sense();
        // Wait for ISR to notify us that the sense is done.
        while (sense_end_time == 0) {
            sleep_ms(50);
        }
        uint64_t elapsed_time = sense_end_time - sense_start_time;
        uint64_t distance = (elapsed_time) / 58;
        printf("Start time: %lld, End Time: %lld\n", sense_start_time, sense_end_time);
        printf("Sensed %lldcm\n", distance);

        // Reset sense time for the next loop
        sense_end_time = 0;
        sleep_ms(1000);
    }
}

