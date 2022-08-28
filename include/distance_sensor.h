#pragma once

#include <map>

/**
 * Interface for using an HC-SR04 distance sensor. Has
 * not been tested with other sensors
 */
class DistanceSensor {
public:
    /**
     * Initializes the sensor.
     * @note Due to the PIO implementation, the echo pin must be
     *       connected to trigger_gpio + 1. If you connect the
     *       trigger pin to GPIO 5, then GPIO 6 must be the echo pin.
     * @param[in] trigger_gpio GPIO connected to the trigger pin
     * @param[in] pio PIO block to use, Either pio0 or pio1
     * @param[in] sm The state machine to use, 0 to 3
     */
    DistanceSensor(uint trigger_gpio, PIO pio, uint sm);

protected:
    /**
     * Offset into PIO memory for the sensor program
     * Cached here so we don't waste space in PIO memory by reloading it.
     * @TODO might consume less space to implement our own cache instead of
     *       using std::map, not sure how big that is.
     */
    static std::map<PIO, uint> _PioOffsets;

    /**
     * Checks if the sensor program is already loaded in the PIO
     * @note This checks the _is_loaded_in_pio mapping
     * @param[in] pio PIO memory to check
     */
    static bool _IsProgramLoaded(PIO pio);

    /**
     * Loads the PIO program into memory if it's not already loaded
     * @param[in] pio PIO block to use, Either pio0 or pio1
     * @returns Program offset
     */
    static uint _LoadSensorProgram(PIO pio);
};
