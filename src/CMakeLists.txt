add_executable(program
    main.cpp
)

pico_generate_pio_header(program ${CMAKE_CURRENT_LIST_DIR}/sensor.pio)

pico_enable_stdio_usb(program 1)
pico_enable_stdio_uart(program 0)

pico_add_extra_outputs(program)

target_link_libraries(program pico_stdlib hardware_pio hardware_irq)

