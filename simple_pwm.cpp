#include "simple_pwm.h"
#include "pico/stdlib.h"
#include <cmath>
#include <cstdio>
#include "hardware/pwm.h"
#include <algorithm>

struct GpioPwmInfo {
    int gpio{};
    int channel{};
    int slice{};
};

static GpioPwmInfo gpioInfo[30]{
    {0, PWM_CHAN_A, 0},
    {1, PWM_CHAN_B, 0},
    {2, PWM_CHAN_A, 1},
    {3, PWM_CHAN_B, 1},
    {4, PWM_CHAN_A, 2},
    {5, PWM_CHAN_B, 2},
    {6, PWM_CHAN_A, 3},
    {7, PWM_CHAN_B, 3},
    {8, PWM_CHAN_A, 4},
    {9, PWM_CHAN_B, 4},
    {10, PWM_CHAN_A, 5},
    {11, PWM_CHAN_B, 5},
    {12, PWM_CHAN_A, 6},
    {13, PWM_CHAN_B, 6},
    {14, PWM_CHAN_A, 7},
    {15, PWM_CHAN_B, 7},
    {16, PWM_CHAN_A, 0},
    {17, PWM_CHAN_B, 0},
    {18, PWM_CHAN_A, 1},
    {19, PWM_CHAN_B, 1},
    {20, PWM_CHAN_A, 2},
    {21, PWM_CHAN_B, 2},
    {22, PWM_CHAN_A, 3},
    {23, PWM_CHAN_B, 3},
    {24, PWM_CHAN_A, 4},
    {25, PWM_CHAN_B, 4},
    {26, PWM_CHAN_A, 5},
    {27, PWM_CHAN_B, 5},
    {28, PWM_CHAN_A, 6},
    {29, PWM_CHAN_B, 6}
};

static constexpr float f_sys{125'000'000.0f};

SimplePwm::SimplePwm(const int gpio, const int frequency): m_gpio{gpio}, m_frequency{frequency} {
    if (gpio < 0 || gpio > 29) {
        panic("Invalid GPIO pin");
    }

    setFrequency(m_frequency);
    setGpio(m_gpio);

#ifdef LIB_DEBUG
    printf("GPIO: %i, Slice: %i, Channel: %i, Target Frequency: %i, Actual Frequency: %f, Wrap: %i, Clkdiv: %f\n",
           m_gpio, m_slice, m_channel, m_frequency, f_sys / (m_clkdiv * static_cast<float>(m_wrap)), m_wrap, m_clkdiv);
#endif
}

void SimplePwm::on(const int duty_cycle) {
    m_duty_cycle = duty_cycle;
    m_enabled = true;

    if (duty_cycle < 0 || duty_cycle > 100) {
        panic("Invalid duty cycle");
    }
    pwm_set_chan_level(m_slice, m_channel,
                       static_cast<int>(std::round(
                           static_cast<float>(m_wrap) * static_cast<float>(m_duty_cycle) / 100.0f)));
    pwm_set_enabled(m_slice, m_enabled);
}

void SimplePwm::off() {
    m_enabled = false;

    pwm_set_enabled(m_slice, m_enabled);
    gpio_put(m_gpio, m_enabled);
}

void SimplePwm::setGpio(const int gpio) {
    if (gpio < 0 || gpio > 29) {
        panic("Invalid GPIO pin");
    }

    // Force disabling current PWM signal
    pwm_set_enabled(m_slice, false);

    m_gpio = gpio;

    // Find the slice and channel
    // TODO - Check if the GPIO is already in use by another PWM slice
    m_slice = gpioInfo[gpio].slice;
    m_channel = gpioInfo[gpio].channel == PWM_CHAN_A ? PWM_CHAN_A : PWM_CHAN_B;
    // Let's deinit and reinit the PWM slice to avoid conflicts
    gpio_deinit(m_gpio);
    gpio_init(m_gpio);
    gpio_set_function(m_gpio, GPIO_FUNC_PWM);
    pwm_set_clkdiv(m_slice, m_clkdiv);
    pwm_set_wrap(m_slice, m_wrap);
    pwm_set_chan_level(m_slice, m_channel,
                       static_cast<int>(std::round(
                           static_cast<float>(m_wrap) * static_cast<float>(m_duty_cycle) / 100.0f)));

    // Setting back the enabled state of the PWM signal
    pwm_set_enabled(m_slice, m_enabled);
    // when a pwm signal is disabled, we assume that what we want is a low signal
    if (!m_enabled) {
        gpio_put(m_gpio, m_enabled);
    }
}

void SimplePwm::setFrequency(const int frequency) {
    m_frequency = frequency;

    // Initial calculation of clkdiv using the maximum wrap value, we'll do this step again once we've calculated the wrap value
    float tmp_clkdiv = f_sys / (static_cast<float>(frequency) * 65535.0f);
    tmp_clkdiv = std::clamp(tmp_clkdiv, 1.0f, 255.0f);
    const float tmp_wrap = f_sys / (static_cast<float>(frequency) * tmp_clkdiv);
    m_wrap = static_cast<int>(std::round(tmp_wrap));
    m_wrap = std::clamp(m_wrap, 2, 65535);
    // capped between 2 and 65535 to allow for at least a 50% duty cycle at the lowest frequency
    // Recalculate clkdiv based on the new wrap value
    m_clkdiv = f_sys / (static_cast<float>(frequency) * static_cast<float>(m_wrap));
    m_clkdiv = std::clamp(m_clkdiv, 1.0f, 255.0f);

    pwm_set_clkdiv(m_slice, m_clkdiv);
    pwm_set_wrap(m_slice, m_wrap);
    pwm_set_chan_level(m_slice, m_channel,
                       static_cast<int>(std::round(
                           static_cast<float>(m_wrap) * static_cast<float>(m_duty_cycle) / 100.0f)));

    pwm_set_enabled(m_slice, m_enabled);
    // when a pwm signal is disabled, we assume that what we want is a low signal
    if (!m_enabled) {
        gpio_put(m_gpio, m_enabled);
    }
}
