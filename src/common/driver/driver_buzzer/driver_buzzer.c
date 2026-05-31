// DRIVER_BUZZER
// MAY 30, 2026

// Header Includes
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "driver_buzzer.h"
#include "bsp.h"

// Defines

// Local Variables

// Local Functions

// External Functions

// DRIVER_BUZZER_Init: Initialise The Buzzer On The Given GPIO Pin.
//   pin    : GPIO Pin The Buzzer Is Connected To
//   freq   : Output PWM Frequency
//   duty   : Duty Percentage
//   returns: PWM Slice Value (0 On Error)
uint8_t DRIVER_BUZZER_Init(uint32_t freq, float duty)
{
    uint32_t slice;
    uint32_t channel;
    uint32_t wrap;

    gpio_set_function(BSP_BUZZER_GPIO, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(BSP_BUZZER_GPIO);
    channel = pwm_gpio_to_channel(BSP_BUZZER_GPIO);

    wrap =  (1000000 / freq) - 1;

    // Set Clock Divider. To Support Low Frequencies Needed For Buzzer
    // Divides Clock To 1 Mhz
    pwm_set_clkdiv(slice, 150.0f);

    // Set Frequency
    pwm_set_wrap(slice, wrap);

    // Set Duty Cycle
    pwm_set_chan_level(slice, channel, (wrap + 1) * duty / 100.0);

    return slice;
}

// DRIVER_BUZZER_On: Turn The Buzzer On
//   slice : PWM Slice
//   returns: True On Success, False Otherwise
bool DRIVER_BUZZER_On(uint8_t slice)
{
    pwm_set_enabled(slice, true);

    return true;
}

// DRIVER_BUZZER_Off: Turn The Buzzer Off.
//   slice : PWM Slice
//   returns: True On Success, False Otherwise
bool DRIVER_BUZZER_Off(uint8_t slice)
{
    pwm_set_enabled(slice, false);

    return true;
}
