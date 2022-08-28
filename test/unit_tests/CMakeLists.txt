include(CTest)
include_directories(../../include .)

if(DEFINED ENV{HOST_CXX_COMPILER})
  set(CMAKE_CXX_COMPILER $ENV{HOST_CXX_COMPILER})
else()
  set(CMAKE_CXX_COMPILER "g++")
endif()

set(CMAKE_CXX_FLAGS "")
set(CMAKE_BUILD_TYPE Debug)

add_executable(test_sensor
    ../../src/distance_sensor.cpp
    test_distance_sensor.cpp
)
add_test(NAME test_sensor_code
         COMMAND test_sensor)

