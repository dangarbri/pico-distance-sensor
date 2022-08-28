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

#include "pico_stubs.h"
#include "distance_sensor.h"
#include <iostream>
#include <cassert>

// Create a derived test class for access to protected members
class TestHarness : public DistanceSensor {
public:
    TestHarness(uint trigger, PIO pio, uint sm) : DistanceSensor(trigger, pio, sm) {};

    void LoadProgram(PIO pio) {
        DistanceSensor::_LoadSensorProgram(pio);
    }

    uint GetOffset(PIO pio) {
        return DistanceSensor::_PioOffsets[pio];
    }
    bool IsLoaded(PIO pio) {
        return DistanceSensor::_IsProgramLoaded(pio);
    }
};

TestHarness test{15, 0, 0};

/**
 * Load programs into pico's memory and test that the
 * addresses are cached correctly
 */
void test_LoadPrograms() {
    // Load on pio0.
    printf("Testing that programs are loaded in memory.");
    // Verify that the program is loaded after calling load program
    assert(test.IsLoaded(0) == false);
    test.LoadProgram(0);
    assert(test.IsLoaded(0) == true);
    // Verify the program is loaded to the expected test address.
    assert(test.GetOffset(0) == 0);

    // Verify that the program is loaded after calling load program
    assert(test.IsLoaded(1) == false);
    test.LoadProgram(1);
    assert(test.IsLoaded(1) == true);
    // Verify the program is loaded to the expected test address.
    assert(test.GetOffset(1) == 1);
    puts(" Success");
}

/**
 * Tests that in the beginning, the programs are not loaded into pico memory
 */
void test_IsProgramLoaded_AtFirst() {
    // Assert that at the start of the program
    // each distance sensor is not loaded
    printf("Checking that programs are not loaded in memory: ");
    assert(test.IsLoaded(0) == false);
    assert(test.IsLoaded(1) == false);
    puts("Success.");
}


int main() {
    test_IsProgramLoaded_AtFirst();
    test_LoadPrograms();
    return 0;
}
