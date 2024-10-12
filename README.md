# Pico Simple PWM Library

The Pico Simple PWM Library provides an easy-to-use interface for controlling PWM signals on a specified GPIO pin. This library is designed for use with Raspberry Pi Pico boards.

## Features

- Set the GPIO pin for the PWM signal.
- Configure the frequency of the PWM signal.
- Adjust the duty cycle of the PWM signal.
- Enable or disable the PWM signal.

## Installation

### Git

Add the Pico Simple PWM Library as a submodule in your project.

```sh
git submodule add https://github.com/julienfdev/pico-simple-pwm.git lib/pico-simple-pwm
```
### Cmake

Alter your `CMakeLists.txt` file to include the Pico Simple PWM Library.

```cmake
# After the `pico_sdk_init` call
add_subdirectory(lib/pico-simple-pwm)

# Add the library to your project
target_link_libraries(your_project pico_simple_pwm)

# Add the standard include files to the build
target_include_directories(simple_pwm_dev PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib/pico-simple-pwm # Add this line
)
```

## Usage

### Initialization

To initialize the Simple PWM, create an instance of the `SimplePwm` class with the desired GPIO pin and frequency.

```cpp
#include "simple_pwm.h"

int main() {
    SimplePwm pwm(15, 1000); // GPIO 15, 1000 Hz frequency
    return 0;
}
```

### Setting the GPIO Pin

To set the GPIO pin for the PWM signal, use the `setGpio` method.

```cpp
pwm.setGpio(15);
```

### Setting the Frequency

To set the frequency of the PWM signal, use the `setFrequency` method.

```cpp
pwm.setFrequency(1000); // 1000 Hz
```

### Setting the Duty Cycle

To set the duty cycle of the PWM signal, use the `on` method.

```cpp
pwm.on(50); // 50% duty cycle
```

### Disabling the PWM Signal

To disable the PWM signal, use the `off` method.

```cpp
pwm.off();
```

## Known Limitations

### Accuracy

The library is generally pretty reliable for frequencies away from the frequency limits. However, the accuracy of the PWM signal may vary depending on the frequency and the duty cycle due to rounding errors and resolution limitations around the extremes values.

### Slices crosstalk

The library does not take into account other instances of the `SimplePwm` class that may be already using the same slice. This can lead to undefined behavior if two instances of the `SimplePwm` class are using the same slice.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
