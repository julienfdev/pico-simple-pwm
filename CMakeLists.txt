cmake_minimum_required(VERSION 3.13)
project(pico_simple_pwm)

set(CMAKE_CXX_STANDARD 17)

# Add the source file for the library
add_library(pico_simple_pwm STATIC simple_pwm.cpp)

# Link the necessary libraries from the Pico SDK
target_link_libraries(pico_simple_pwm
        pico_stdlib     # Standard Pico libraries
        hardware_pwm    # The hardware_pwm library
)
# Add the standard include files
target_include_directories(pico_simple_pwm PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)