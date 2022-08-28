#pragma once
/**
 * Stubs for testing
 */
#include "pico_stubs.h"
typedef struct {
    const uint16_t * instructions;
    uint8_t length;
    int8_t origin;
} pio_program_t;

// Using arbitrary values to simulate a test program
// for testing
pio_program_t distance_sensor_program = {
    (uint16_t*)0xDEAD, 0x10, 0x10
};

/**
 * Simulates adding a program to the PIO
 * Using an ID means that each time a program is loaded, it will
 * have a different result which can be used to verify that it is
 * distinct from a separate program that was loaded
 * @return ID representing the offset into memory for a given program
 */
uint pio_add_program(PIO pio, const pio_program_t*	program) {
    static uint simulated_offset = 0;
    return simulated_offset++;
};
