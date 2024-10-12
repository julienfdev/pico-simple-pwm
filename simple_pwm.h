#ifndef SIMPLE_PWM_LIBRARY_H
#define SIMPLE_PWM_LIBRARY_H
#include <hardware/pwm.h>


class SimplePwm {
    using slice = int;

public:

    SimplePwm(int gpio, int frequency);

    [[nodiscard]] int gpio() const { return m_gpio; }

    /**
     * \brief Sets the GPIO pin for the PWM signal.
     *
     * This function sets the GPIO pin for the PWM signal.
     * The PWM signal will be output on the specified GPIO pin.
     *
     * \param gpio The GPIO pin.
     *
     * \throws std::runtime_error if the GPIO pin is out of the valid range (0-29).
     */
    void setGpio(int gpio);

    [[nodiscard]] int frequency() const { return m_frequency; }

    /**
     * \brief Sets the frequency of the PWM signal.
     *
     * This function sets the frequency of the PWM signal to the specified value.
     * The frequency determines the number of times the signal changes from high to low per second.
     * Frequency will be clamped between the minimum and maximum values supported by the hardware.
     *
     * \param frequency The desired frequency in Hz.
     *
     */
    void setFrequency(int frequency);

    /**
     * \brief Sets the duty cycle of the PWM signal.
     *
     * This function sets the duty cycle of the PWM signal to the specified percentage.
     * The duty cycle determines the proportion of time the signal is high within one period.
     *
     * \param duty_cycle The desired duty cycle as a percentage (0-100%).
     *                   - 0%: The signal is always low.
     *                   - 100%: The signal is always high.
     *
     * \throws std::runtime_error if the duty cycle is out of the valid range (0-100%).
     */
    void on(int duty_cycle);

    /**
     * \brief Disables the PWM signal.
     *
     * This function turns off the PWM signal by disabling the PWM slice.
     * It stops the PWM output on the specified GPIO pin.
    */
    void off();

private:
    bool m_enabled {false};
    int m_gpio;
    int m_slice {};
    uint m_channel {};
    int m_frequency;
    int m_wrap { 2 };
    float m_clkdiv { 1 } ;
    int m_duty_cycle { 50 };
};
#endif //SIMPLE_PWM_LIBRARY_H
