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

#include <CodalConfig.h>

#ifndef DEVICE_DYNAMIC_PWM_H
#define DEVICE_DYNAMIC_PWM_H

#define DEVICE_DEFAULT_PWM_PERIOD 20000

////TODO: From https://os.mbed.com/users/screamer/code/mbed/file/667d61c9177b/PinNames.h/
enum PinName {
 
    // LPC11U Pin Names
   P0_0 = 0
  , P0_1 = 1
  , P0_2 = 2
  , P0_3 = 3
  , P0_4 = 4
  , P0_5 = 5
  , P0_6 = 6
  , P0_7 = 7
  , P0_8 = 8
  , P0_9 = 9
  , P0_10 = 10
  , P0_11 = 11
  , P0_12 = 12
  , P0_13 = 13
  , P0_14 = 14
  , P0_15 = 15
  , P0_16 = 16
  , P0_17 = 17
  , P0_18 = 18
  , P0_19 = 19
  , P0_20 = 20
  , P0_21 = 21
  , P0_22 = 22
  , P0_23 = 23
  , P0_24 = 24
  , P0_25 = 25
  , P0_26 = 26
  , P0_30 = 30

  // Not connected
  , NC = (int)0xFFFFFFFF
};
typedef enum PinName PinName;

////TODO: From https://os.mbed.com/users/screamer/code/mbed/file/aff670d0d510/PwmOut.h/
class PwmOut {
  public: 
    /** Create a PwmOut connected to the specified pin
     *
     *  @param pin PwmOut pin to connect to
     */
    PwmOut(PinName pin, const char *name = NULL);
  protected: 
    ////PWMName _pwm;
};

/**
  * Class definition for DynamicPwm.
  *
  * This class addresses a few issues found in the underlying libraries.
  * This provides the ability for a neat, clean swap between PWM channels.
  */
class DynamicPwm : public PwmOut
{
    private:
    uint32_t period;
    float lastValue;

    public:

    /**
      * An internal constructor used when allocating a new DynamicPwm instance.
      *
      * @param pin the name of the pin for the pwm to target
      *
      * @param persistance the level of persistence for this pin PWM_PERSISTENCE_PERSISTENT (can not be replaced until freed, should only be used for system services really.)
      *                    or PWM_PERSISTENCE_TRANSIENT (can be replaced at any point if a channel is required.)
      */
    DynamicPwm(PinName pin);

    /**
      * Frees this DynamicPwm instance for reuse.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate();
      * pwm->release();
      * @endcode
      */
    void release();

    /**
      * A lightweight wrapper around the super class' write in order to capture the value
      *
      * @param value the duty cycle percentage in floating point format.
      *
      * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if value is out of range
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate();
      * pwm->write(0.5);
      * @endcode
      */
    int write(float value);

    /**
      * Retreives the PinName associated with this DynamicPwm instance.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      *
      * // returns the PinName n.
      * pwm->getPinName();
      * @endcode
      *
      * @note This should be used to check that the DynamicPwm instance has not
      *       been reallocated for use in another part of a program.
      */
    PinName getPinName();

    /**
      * Retreives the last value that has been written to this DynamicPwm instance.
      * in the range 0 - 1023 inclusive.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->write(0.5);
      *
      * // will return 512.
      * pwm->getValue();
      * @endcode
      */
    int getValue();

    /**
      * Retreives the current period in use by the entire PWM module in microseconds.
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->getPeriod();
      * @endcode
      */
    uint32_t getPeriodUs();

    /**
      * Retreives the current period in use by the entire PWM module in milliseconds.
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->setPeriodUs(20000);
      *
      * // will return 20000
      * pwm->getPeriod();
      * @endcode
      */
    uint32_t getPeriod();

    /**
      * Sets the period used by the WHOLE PWM module.
      *
      * @param period the desired period in microseconds.
      *
      * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if period is out of range
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      *
      * // period now is 20ms
      * pwm->setPeriodUs(20000);
      * @endcode
      *
      * @note Any changes to the period will AFFECT ALL CHANNELS.
      */
    int setPeriodUs(uint32_t period);

    /**
      * Sets the period used by the WHOLE PWM module. Any changes to the period will AFFECT ALL CHANNELS.
      *
      * @param period the desired period in milliseconds.
      *
      * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER if period is out of range
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      *
      * // period now is 20ms
      * pwm->setPeriod(20);
      * @endcode
      */
      int setPeriod(uint32_t period);
};

#endif