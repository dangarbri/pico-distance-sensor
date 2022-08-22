add_executable(test_program
    main.cpp
)

pico_enable_stdio_usb(test_program 1)
pico_enable_stdio_uart(test_program 0)

pico_add_extra_outputs(test_program)

target_link_libraries(test_program pico_stdlib)

