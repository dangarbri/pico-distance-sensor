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
#include "sensor.pio.h"
#include "distance_sensor.h"

/**
 * Instance of map that stores the memory address of the loaded
 * pio program
 */
std::map<PIO, uint> DistanceSensor::_PioOffsets;
DistanceSensor* DistanceSensor::_sensor_map[2][4] = {};

static void _SensorIrq() {
    // Check PIO0 interrupt sources
    for (uint i = 0; i < 4; i++) {
        // If the interrupt is from pio0 state machine i, then get the distance from
        // that PIO's state machine and write it to the distance sensor
        if (pio_interrupt_get(pio0, i)) {
            DistanceSensor::GetMappedSensor(0, i)->distance = pio_sm_get(pio0, i);
        };

        // If the interrupt is from pio1 state machine i, then get the distance from
        // that PIO's state machine and write it to the distance sensor
        if (pio_interrupt_get(pio1, i)) {
            DistanceSensor::GetMappedSensor(1, i)->distance = pio_sm_get(pio1, i);
        };
    }
}

DistanceSensor::DistanceSensor(uint trigger_gpio, PIO pio, uint sm) {
    // Initialize the program and set up the IRQ handler to save the
    // distance reading to this->distance.
    _InitializeSensorForGpio(trigger_gpio, pio, sm, _SensorIrq);
    DistanceSensor::_sensor_map[pio][sm] = this;
}

void DistanceSensor::_InitializeSensorForGpio(uint gpio, PIO pio, uint sm, irq_handler_t handler) {
    // Load the program and get the offset
    // Note this is safe to call even if the sensor was already loaded, it will not reload it.
    uint offset = _LoadSensorProgram(pio);
    // Initialize the program in the PIO
    distance_sensor_program_init(pio, sm, offset, gpio, handler);
}

uint DistanceSensor::_LoadSensorProgram(PIO pio) {
    // If the program hasn't already been loaded in memory, then load it.
    if (!DistanceSensor::_IsProgramLoaded(pio)) {
        uint offset = pio_add_program(pio, &distance_sensor_program);
        // Cache the offset so we don't need to reload it.
        DistanceSensor::_PioOffsets[pio] = offset;
    }

    // Return the offset in memory for the program
    return DistanceSensor::_PioOffsets[pio];
}

/**
 * Checks if _PioMemory has the pio offset saved in it
 */
bool DistanceSensor::_IsProgramLoaded(PIO pio) {
    // Check the map for the pio instance
    auto iter = DistanceSensor::_PioOffsets.find(pio);
    // If it's not the ".end" iterator, that means it was found.
    bool found = iter != DistanceSensor::_PioOffsets.end();
    return found;
}

DistanceSensor* DistanceSensor::GetMappedSensor(uint pio, uint sm) {
    return _sensor_map[pio][sm];
}

void TriggerRead() {
    pio_sm_put(pio, sm, 1);
}

