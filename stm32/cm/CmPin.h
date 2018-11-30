/*
  The MIT License (MIT)

  Copyright (c) 2017 Lancaster University.

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/

#ifndef CODAL_CM_PIN_H
#define CODAL_CM_PIN_H

#include "Pin.h"
#include "CodalConfig.h"
#include "pinmap.h"
                                                        // Status Field flags...
#define IO_STATUS_DIGITAL_IN                0x01        // Pin is configured as a digital input, with no pull up.
#define IO_STATUS_DIGITAL_OUT               0x02        // Pin is configured as a digital output
#define IO_STATUS_ANALOG_IN                 0x04        // Pin is Analog in
#define IO_STATUS_ANALOG_OUT                0x08        // Pin is Analog out
#define IO_STATUS_TOUCH_IN                  0x10        // Pin is a makey-makey style touch sensor
#define IO_STATUS_EVENT_ON_EDGE             0x20        // Pin will generate events on pin change
#define IO_STATUS_EVENT_PULSE_ON_EDGE       0x40        // Pin will generate events on pin change

/**
  * Class definition for Pin.
  *
  * Commonly represents an I/O pin on the edge connector.
  */
namespace codal
{
    namespace _cm
    {
        class Pin : public codal::Pin
        {
            // The mbed object looking after this pin at any point in time (untyped due to dynamic behaviour).
            // TODO: void *pin;

            uint32_t rcc;   //  e.g. RCC_GPIOC
            uint32_t port;  //  e.g. GPIOC
            uint16_t pin;   //  e.g. GPIO13

            /**
              * Disconnect any attached mBed IO from this pin.
              *
              * Used only when pin changes mode (i.e. Input/Output/Analog/Digital)
              */
            void disconnect();

            /**
              * Performs a check to ensure that the current Pin is in control of a
              * DynamicPwm instance, and if it's not, allocates a new DynamicPwm instance.
              */
            int obtainAnalogChannel();

            /**
              * Interrupt handler for when an rise interrupt is triggered.
              */
            void onRise();

            /**
              * Interrupt handler for when an fall interrupt is triggered.
              */
            void onFall();

            /**
              * This member function manages the calculation of the timestamp of a pulse detected
              * on a pin whilst in IO_STATUS_EVENT_PULSE_ON_EDGE or IO_STATUS_EVENT_ON_EDGE modes.
              *
              * @param eventValue the event value to distribute onto the message bus.
              */
            void pulseWidthEvent(int eventValue);

            /**
              * This member function will construct an TimedInterruptIn instance, and configure
              * interrupts for rise and fall.
              *
              * @param eventType the specific mode used in interrupt context to determine how an
              *                  edge/rise is processed.
              *
              * @return DEVICE_OK on success
              */
            int enableRiseFallEvents(int eventType);

            /**
              * If this pin is in a mode where the pin is generating events, it will destruct
              * the current instance attached to this DevicePin instance.
              *
              * @return DEVICE_OK on success.
              */
            int disableEvents();

            public:

            /**
              * Constructor.
              * Create a DevicePin instance, generally used to represent a pin on the edge connector.
              *
              * @param id the unique EventModel id of this component.
              *
              * @param name the mbed PinName for this DevicePin instance.
              *
              * @param capability the capabilities this DevicePin instance should have.
              *                   (PIN_CAPABILITY_DIGITAL, PIN_CAPABILITY_ANALOG, PIN_CAPABILITY_AD, PIN_CAPABILITY_ALL)
              *
              * @code
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_ALL);
              * @endcode
              */
            Pin(
              int id,  //  e.g. DEVICE_ID_IO_PA0
              PinName name,   //  0 to 127
              uint32_t rcc,   //  e.g. RCC_GPIOC
              uint32_t port,  //  e.g. GPIOC
              uint16_t pin,   //  e.g. GPIO13
              PinCapability capability  //  e.g. PIN_CAPABILITY_DIGITAL
            );

            void setup(
              uint8_t mode,   //  e.g. GPIO_MODE_OUTPUT_2_MHZ
              uint8_t cnf     //  e.g. GPIO_CNF_OUTPUT_PUSHPULL
            );

            /**
              * Configures this IO pin as a digital output (if necessary) and sets the pin to 'value'.
              *
              * @param value 0 (LO) or 1 (HI)
              *
              * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if value is out of range, or DEVICE_NOT_SUPPORTED
              *         if the given pin does not have digital capability.
              *
              * @code
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_BOTH);
              * P0.setDigitalValue(1); // P0 is now HI
              * @endcode
              */
            int setDigitalValue(int value);

            /**
              * Configures this IO pin as a digital input (if necessary) and tests its current value.
              *
              *
              * @return 1 if this input is high, 0 if input is LO, or DEVICE_NOT_SUPPORTED
              *         if the given pin does not have digital capability.
              *
              * @code
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_BOTH);
              * P0.getDigitalValue(); // P0 is either 0 or 1;
              * @endcode
              */
            int getDigitalValue();

            /**
              * Configures this IO pin as a digital input with the specified internal pull-up/pull-down configuraiton (if necessary) and tests its current value.
              *
              * @param pull one of the mbed pull configurations: PullUp, PullDown, PullNone
              *
              * @return 1 if this input is high, 0 if input is LO, or DEVICE_NOT_SUPPORTED
              *         if the given pin does not have digital capability.
              *
              * @code
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_BOTH);
              * P0.getDigitalValue(PullUp); // P0 is either 0 or 1;
              * @endcode
              */
            int getDigitalValue(PullMode pull);

            /**
              * Configures this IO pin as an analog/pwm output, and change the output value to the given level.
              *
              * @param value the level to set on the output pin, in the range 0 - 1024
              *
              * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if value is out of range, or DEVICE_NOT_SUPPORTED
              *         if the given pin does not have analog capability.
              */
            int setAnalogValue(int value);

            /**
              * Configures this IO pin as an analog/pwm output (if necessary) and configures the period to be 20ms,
              * with a duty cycle between 500 us and 2500 us.
              *
              * A value of 180 sets the duty cycle to be 2500us, and a value of 0 sets the duty cycle to be 500us by default.
              *
              * This range can be modified to fine tune, and also tolerate different servos.
              *
              * @param value the level to set on the output pin, in the range 0 - 180.
              *
              * @param range which gives the span of possible values the i.e. the lower and upper bounds (center +/- range/2). Defaults to DEVICE_PIN_DEFAULT_SERVO_RANGE.
              *
              * @param center the center point from which to calculate the lower and upper bounds. Defaults to DEVICE_PIN_DEFAULT_SERVO_CENTER
              *
              * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if value is out of range, or DEVICE_NOT_SUPPORTED
              *         if the given pin does not have analog capability.
              */
            int setServoValue(int value, int range = DEVICE_PIN_DEFAULT_SERVO_RANGE, int center = DEVICE_PIN_DEFAULT_SERVO_CENTER);

            /**
              * Configures this IO pin as an analogue input (if necessary), and samples the Pin for its analog value.
              *
              * @return the current analogue level on the pin, in the range 0 - 1024, or
              *         DEVICE_NOT_SUPPORTED if the given pin does not have analog capability.
              *
              * @code
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_BOTH);
              * P0.getAnalogValue(); // P0 is a value in the range of 0 - 1024
              * @endcode
              */
            int getAnalogValue();

            /**
              * Determines if this IO pin is currently configured as an input.
              *
              * @return 1 if pin is an analog or digital input, 0 otherwise.
              */
            int isInput();

            /**
              * Determines if this IO pin is currently configured as an output.
              *
              * @return 1 if pin is an analog or digital output, 0 otherwise.
              */
            int isOutput();

            /**
              * Determines if this IO pin is currently configured for digital use.
              *
              * @return 1 if pin is digital, 0 otherwise.
              */
            int isDigital();

            /**
              * Determines if this IO pin is currently configured for analog use.
              *
              * @return 1 if pin is analog, 0 otherwise.
              */
            int isAnalog();

            /**
              * Configures this IO pin as a "makey makey" style touch sensor (if necessary)
              * and tests its current debounced state.
              *
              * Users can also subscribe to DeviceButton events generated from this pin.
              *
              * @return 1 if pin is touched, 0 if not, or DEVICE_NOT_SUPPORTED if this pin does not support touch capability.
              *
              * @code
              * DeviceMessageBus bus;
              *
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_ALL);
              * if(P0.isTouched())
              * {
              *     //do something!
              * }
              *
              * // subscribe to events generated by this pin!
              * bus.listen(DEVICE_ID_IO_P0, DEVICE_BUTTON_EVT_CLICK, someFunction);
              * @endcode
              */
            int isTouched();

            /**
              * Configures this IO pin as an analog/pwm output if it isn't already, configures the period to be 20ms,
              * and sets the pulse width, based on the value it is given.
              *
              * @param pulseWidth the desired pulse width in microseconds.
              *
              * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if value is out of range, or DEVICE_NOT_SUPPORTED
              *         if the given pin does not have analog capability.
              */
            int setServoPulseUs(int pulseWidth);

            /**
              * Configures the PWM period of the analog output to the given value.
              *
              * @param period The new period for the analog output in milliseconds.
              *
              * @return DEVICE_OK on success, or DEVICE_NOT_SUPPORTED if the
              *         given pin is not configured as an analog output.
              */
            int setAnalogPeriod(int period);

            /**
              * Configures the PWM period of the analog output to the given value.
              *
              * @param period The new period for the analog output in microseconds.
              *
              * @return DEVICE_OK on success, or DEVICE_NOT_SUPPORTED if the
              *         given pin is not configured as an analog output.
              */
            int setAnalogPeriodUs(int period);

            /**
              * Obtains the PWM period of the analog output in microseconds.
              *
              * @return the period on success, or DEVICE_NOT_SUPPORTED if the
              *         given pin is not configured as an analog output.
              */
            uint32_t getAnalogPeriodUs();

            /**
              * Obtains the PWM period of the analog output in milliseconds.
              *
              * @return the period on success, or DEVICE_NOT_SUPPORTED if the
              *         given pin is not configured as an analog output.
              */
            int getAnalogPeriod();

            /**
              * Configures the pull of this pin.
              *
              * @param pull one of the mbed pull configurations: PullMode::Up, PullMode::Down, PullMode::None
              *
              * @return DEVICE_NOT_SUPPORTED if the current pin configuration is anything other
              *         than a digital input, otherwise DEVICE_OK.
              */
            int setPull(PullMode pull);

            /**
              * Configures the events generated by this DevicePin instance.
              *
              * DEVICE_PIN_EVENT_ON_EDGE - Configures this pin to a digital input, and generates events whenever a rise/fall is detected on this pin. (DEVICE_PIN_EVT_RISE, DEVICE_PIN_EVT_FALL)
              * DEVICE_PIN_EVENT_ON_PULSE - Configures this pin to a digital input, and generates events where the timestamp is the duration that this pin was either HI or LO. (DEVICE_PIN_EVT_PULSE_HI, DEVICE_PIN_EVT_PULSE_LO)
              * DEVICE_PIN_EVENT_ON_TOUCH - Configures this pin as a makey makey style touch sensor, in the form of a DeviceButton. Normal button events will be generated using the ID of this pin.
              * DEVICE_PIN_EVENT_NONE - Disables events for this pin.
              *
              * @param eventType One of: DEVICE_PIN_EVENT_ON_EDGE, DEVICE_PIN_EVENT_ON_PULSE, DEVICE_PIN_EVENT_ON_TOUCH, DEVICE_PIN_EVENT_NONE
              *
              * @code
              * DeviceMessageBus bus;
              *
              * DevicePin P0(DEVICE_ID_IO_P0, DEVICE_PIN_P0, PIN_CAPABILITY_BOTH);
              * P0.eventOn(DEVICE_PIN_EVENT_ON_PULSE);
              *
              * void onPulse(Event evt)
              * {
              *     int duration = evt.timestamp;
              * }
              *
              * bus.listen(DEVICE_ID_IO_P0, DEVICE_PIN_EVT_PULSE_HI, onPulse, MESSAGE_BUS_LISTENER_IMMEDIATE)
              * @endcode
              *
              * @return DEVICE_OK on success, or DEVICE_INVALID_PARAMETER if the given eventype does not match
              *
              * @note In the DEVICE_PIN_EVENT_ON_PULSE mode, the smallest pulse that was reliably detected was 85us, around 5khz. If more precision is required,
              *       please use the InterruptIn class supplied by ARM mbed.
              */
            int eventOn(int eventType);
        };
    }
}

#endif  //  CODAL_CM_PIN_H
