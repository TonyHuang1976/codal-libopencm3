//  Based on https://github.com/nedseb/codal-stm32-iot-node/blob/master/model/STM32IotNodeIO.h
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

#ifndef STM32BLUEPILL_IO_H
#define STM32BLUEPILL_IO_H

#include "CodalConfig.h"
#include "CmPin.h"
#include "STM32BluePillPin.h"

// Codal Component IDs for each pin.  DEVICE_ID_IO_P0 to DEVICE_ID_IO_P0 + 127 are reserved for I/O Pin IDs.
// The can be user defined, but uniquely identify a pin when using the eventing APIs
// From https://docs.google.com/spreadsheets/d/1yLciHFyPfhRfwEcG3wfqHDgRFa5OoQYTk63bUmTboa8/edit#gid=0

//  Codal Component IDs are autogenerated. Do not update here
#define ID_PIN_PA0         (DEVICE_ID_IO_P0 + 0)
#define ID_PIN_PA1         (DEVICE_ID_IO_P0 + 1)
#define ID_PIN_PA2         (DEVICE_ID_IO_P0 + 2)
#define ID_PIN_PA3         (DEVICE_ID_IO_P0 + 3)
#define ID_PIN_PA4         (DEVICE_ID_IO_P0 + 4)
#define ID_PIN_PA5         (DEVICE_ID_IO_P0 + 5)
#define ID_PIN_PA6         (DEVICE_ID_IO_P0 + 6)
#define ID_PIN_PA7         (DEVICE_ID_IO_P0 + 7)
#define ID_PIN_PA8         (DEVICE_ID_IO_P0 + 8)
#define ID_PIN_PA9         (DEVICE_ID_IO_P0 + 9)
#define ID_PIN_PA10        (DEVICE_ID_IO_P0 + 10)
#define ID_PIN_PA11        (DEVICE_ID_IO_P0 + 11)
#define ID_PIN_PA12        (DEVICE_ID_IO_P0 + 12)
#define ID_PIN_PA13        (DEVICE_ID_IO_P0 + 13)
#define ID_PIN_PA14        (DEVICE_ID_IO_P0 + 14)
#define ID_PIN_PA15        (DEVICE_ID_IO_P0 + 15)
#define ID_PIN_PB0         (DEVICE_ID_IO_P0 + 16)
#define ID_PIN_PB1         (DEVICE_ID_IO_P0 + 17)
#define ID_PIN_PB2         (DEVICE_ID_IO_P0 + 18)
#define ID_PIN_PB3         (DEVICE_ID_IO_P0 + 19)
#define ID_PIN_PB4         (DEVICE_ID_IO_P0 + 20)
#define ID_PIN_PB5         (DEVICE_ID_IO_P0 + 21)
#define ID_PIN_PB6         (DEVICE_ID_IO_P0 + 22)
#define ID_PIN_PB7         (DEVICE_ID_IO_P0 + 23)
#define ID_PIN_PB8         (DEVICE_ID_IO_P0 + 24)
#define ID_PIN_PB9         (DEVICE_ID_IO_P0 + 25)
#define ID_PIN_PB10        (DEVICE_ID_IO_P0 + 26)
#define ID_PIN_PB12        (DEVICE_ID_IO_P0 + 27)
#define ID_PIN_PB13        (DEVICE_ID_IO_P0 + 28)
#define ID_PIN_PB14        (DEVICE_ID_IO_P0 + 29)
#define ID_PIN_PB15        (DEVICE_ID_IO_P0 + 30)
//#define ID_PIN_PC0         (DEVICE_ID_IO_P0 + 31)
//#define ID_PIN_PC1         (DEVICE_ID_IO_P0 + 32)
//#define ID_PIN_PC2         (DEVICE_ID_IO_P0 + 33)
//#define ID_PIN_PC3         (DEVICE_ID_IO_P0 + 34)
//#define ID_PIN_PC4         (DEVICE_ID_IO_P0 + 35)
//#define ID_PIN_PC5         (DEVICE_ID_IO_P0 + 36)
//#define ID_PIN_PC6         (DEVICE_ID_IO_P0 + 37)
//#define ID_PIN_PC7         (DEVICE_ID_IO_P0 + 38)
//#define ID_PIN_PC8         (DEVICE_ID_IO_P0 + 39)
//#define ID_PIN_PC9         (DEVICE_ID_IO_P0 + 40)
//#define ID_PIN_PC10        (DEVICE_ID_IO_P0 + 41)
//#define ID_PIN_PC11        (DEVICE_ID_IO_P0 + 42)
//#define ID_PIN_PC12        (DEVICE_ID_IO_P0 + 43)
#define ID_PIN_PC13        (DEVICE_ID_IO_P0 + 44)
#define ID_PIN_PC14        (DEVICE_ID_IO_P0 + 45)
#define ID_PIN_PC15        (DEVICE_ID_IO_P0 + 46)
//#define ID_PIN_PD2         (DEVICE_ID_IO_P0 + 47)
//#define ID_PIN_PH0         (DEVICE_ID_IO_P0 + 48)
//#define ID_PIN_PH1         (DEVICE_ID_IO_P0 + 49)
#define ID_PIN_ADC0        (DEVICE_ID_IO_P0 + 50)
#define ID_PIN_ADC1        (DEVICE_ID_IO_P0 + 51)
#define ID_PIN_ADC2        (DEVICE_ID_IO_P0 + 52)
#define ID_PIN_ADC3        (DEVICE_ID_IO_P0 + 53)
#define ID_PIN_ADC4        (DEVICE_ID_IO_P0 + 54)
#define ID_PIN_ADC5        (DEVICE_ID_IO_P0 + 55)
#define ID_PIN_ADC6        (DEVICE_ID_IO_P0 + 56)
#define ID_PIN_ADC7        (DEVICE_ID_IO_P0 + 57)
#define ID_PIN_ADC8        (DEVICE_ID_IO_P0 + 58)
#define ID_PIN_ADC9        (DEVICE_ID_IO_P0 + 59)
#define ID_PIN_ADCTEMP     (DEVICE_ID_IO_P0 + 60)
#define ID_PIN_ADCVREF     (DEVICE_ID_IO_P0 + 61)
#define ID_PIN_ADCVBAT     (DEVICE_ID_IO_P0 + 62)
#define ID_PIN_TEMPERATURE (DEVICE_ID_IO_P0 + 63)
#define ID_PIN_VREF        (DEVICE_ID_IO_P0 + 64)
#define ID_PIN_VBAT        (DEVICE_ID_IO_P0 + 65)
#define ID_PIN_LED         (DEVICE_ID_IO_P0 + 66)
#define ID_PIN_SCL         (DEVICE_ID_IO_P0 + 67)
#define ID_PIN_SDA         (DEVICE_ID_IO_P0 + 68)
#define ID_PIN_SCL1        (DEVICE_ID_IO_P0 + 69)
#define ID_PIN_SDA1        (DEVICE_ID_IO_P0 + 70)
#define ID_PIN_SCL2        (DEVICE_ID_IO_P0 + 71)
#define ID_PIN_SDA2        (DEVICE_ID_IO_P0 + 72)
#define ID_PIN_NSS         (DEVICE_ID_IO_P0 + 73)
#define ID_PIN_SCK         (DEVICE_ID_IO_P0 + 74)
#define ID_PIN_MISO        (DEVICE_ID_IO_P0 + 75)
#define ID_PIN_MOSI        (DEVICE_ID_IO_P0 + 76)
#define ID_PIN_NSS1        (DEVICE_ID_IO_P0 + 77)
#define ID_PIN_SCK1        (DEVICE_ID_IO_P0 + 78)
#define ID_PIN_MISO1       (DEVICE_ID_IO_P0 + 79)
#define ID_PIN_MOSI1       (DEVICE_ID_IO_P0 + 80)
#define ID_PIN_NSS2        (DEVICE_ID_IO_P0 + 81)
#define ID_PIN_SCK2        (DEVICE_ID_IO_P0 + 82)
#define ID_PIN_MISO2       (DEVICE_ID_IO_P0 + 83)
#define ID_PIN_MOSI2       (DEVICE_ID_IO_P0 + 84)
#define ID_PIN_TX          (DEVICE_ID_IO_P0 + 85)
#define ID_PIN_RX          (DEVICE_ID_IO_P0 + 86)
#define ID_PIN_TX1         (DEVICE_ID_IO_P0 + 87)
#define ID_PIN_RX1         (DEVICE_ID_IO_P0 + 88)
#define ID_PIN_TX2         (DEVICE_ID_IO_P0 + 89)
#define ID_PIN_RX2         (DEVICE_ID_IO_P0 + 90)
#define ID_PIN_TX3         (DEVICE_ID_IO_P0 + 91)
#define ID_PIN_RX3         (DEVICE_ID_IO_P0 + 92)
#define ID_PIN_JTMS        (DEVICE_ID_IO_P0 + 93)
#define ID_PIN_JTCK        (DEVICE_ID_IO_P0 + 94)
#define ID_PIN_JTDI        (DEVICE_ID_IO_P0 + 95)
#define ID_PIN_JTDO        (DEVICE_ID_IO_P0 + 96)
#define ID_PIN_JTRST       (DEVICE_ID_IO_P0 + 97)
#define ID_PIN_SWDIO       (DEVICE_ID_IO_P0 + 98)
#define ID_PIN_SWCLK       (DEVICE_ID_IO_P0 + 99)
#define ID_PIN_BOOT0       (DEVICE_ID_IO_P0 + 100)
#define ID_PIN_BOOT1       (DEVICE_ID_IO_P0 + 101)
#define ID_PIN_WKUP        (DEVICE_ID_IO_P0 + 102)
#define ID_PIN_NRST        (DEVICE_ID_IO_P0 + 103)

namespace codal
{
    /**
     * Represents a collection of all I/O pins exposed by the device.
     */
    class STM32BluePillIO
    {
    public:
        STM32BluePillIO();

        //  Codal Pins are autogenerated. Do not update here
        _cm::Pin pa0;
        _cm::Pin pa1;
        _cm::Pin pa2;
        _cm::Pin pa3;
        _cm::Pin pa4;
        _cm::Pin pa5;
        _cm::Pin pa6;
        _cm::Pin pa7;
        _cm::Pin pa8;
        _cm::Pin pa9;
        _cm::Pin pa10;
        _cm::Pin pa11;
        _cm::Pin pa12;
        _cm::Pin pa13;
        _cm::Pin pa14;
        _cm::Pin pa15;
        _cm::Pin pb0;
        _cm::Pin pb1;
        _cm::Pin pb2;
        _cm::Pin pb3;
        _cm::Pin pb4;
        _cm::Pin pb5;
        _cm::Pin pb6;
        _cm::Pin pb7;
        _cm::Pin pb8;
        _cm::Pin pb9;
        _cm::Pin pb10;
        _cm::Pin pb12;
        _cm::Pin pb13;
        _cm::Pin pb14;
        _cm::Pin pb15;
        //_cm::Pin pc0;
        //_cm::Pin pc1;
        //_cm::Pin pc2;
        //_cm::Pin pc3;
        //_cm::Pin pc4;
        //_cm::Pin pc5;
        //_cm::Pin pc6;
        //_cm::Pin pc7;
        //_cm::Pin pc8;
        //_cm::Pin pc9;
        //_cm::Pin pc10;
        //_cm::Pin pc11;
        //_cm::Pin pc12;
        _cm::Pin pc13;
        _cm::Pin pc14;
        _cm::Pin pc15;
        //_cm::Pin pd2;
        //_cm::Pin ph0;
        //_cm::Pin ph1;
        _cm::Pin adc0;
        _cm::Pin adc1;
        _cm::Pin adc2;
        _cm::Pin adc3;
        _cm::Pin adc4;
        _cm::Pin adc5;
        _cm::Pin adc6;
        _cm::Pin adc7;
        _cm::Pin adc8;
        _cm::Pin adc9;
        _cm::Pin adctemp;
        _cm::Pin adcvref;
        _cm::Pin adcvbat;
        _cm::Pin temperature;
        _cm::Pin vref;
        _cm::Pin vbat;
        _cm::Pin led;
        _cm::Pin scl;
        _cm::Pin sda;
        _cm::Pin scl1;
        _cm::Pin sda1;
        _cm::Pin scl2;
        _cm::Pin sda2;
        _cm::Pin nss;
        _cm::Pin sck;
        _cm::Pin miso;
        _cm::Pin mosi;
        _cm::Pin nss1;
        _cm::Pin sck1;
        _cm::Pin miso1;
        _cm::Pin mosi1;
        _cm::Pin nss2;
        _cm::Pin sck2;
        _cm::Pin miso2;
        _cm::Pin mosi2;
        _cm::Pin tx;
        _cm::Pin rx;
        _cm::Pin tx1;
        _cm::Pin rx1;
        _cm::Pin tx2;
        _cm::Pin rx2;
        _cm::Pin tx3;
        _cm::Pin rx3;
        _cm::Pin jtms;
        _cm::Pin jtck;
        _cm::Pin jtdi;
        _cm::Pin jtdo;
        _cm::Pin jtrst;
        _cm::Pin swdio;
        _cm::Pin swclk;
        _cm::Pin boot0;
        _cm::Pin boot1;
        _cm::Pin wkup;
        _cm::Pin nrst;
    };
}

#endif  //  STM32BLUEPILL_IO_H

#ifdef NOTUSED
#define ID_PIN_P0           (DEVICE_ID_IO_P0 + 0)
#define ID_PIN_P1           (DEVICE_ID_IO_P0 + 1)
#define ID_PIN_P2           (DEVICE_ID_IO_P0 + 2)
#define ID_PIN_P3           (DEVICE_ID_IO_P0 + 3)
#define ID_PIN_P4           (DEVICE_ID_IO_P0 + 4)
#define ID_PIN_P5           (DEVICE_ID_IO_P0 + 5)
#define ID_PIN_P6           (DEVICE_ID_IO_P0 + 6)
#define ID_PIN_P7           (DEVICE_ID_IO_P0 + 7)
#define ID_PIN_LED_GREEN    (DEVICE_ID_IO_P0 + 8)
#define ID_PIN_SCL          (DEVICE_ID_IO_P0 + 9)
#define ID_PIN_SDA          (DEVICE_ID_IO_P0 + 10)
#define ID_PIN_P11          (DEVICE_ID_IO_P0 + 11)
#define ID_PIN_P12          (DEVICE_ID_IO_P0 + 12)
#define ID_PIN_P13          (DEVICE_ID_IO_P0 + 13)
#define ID_PIN_P14          (DEVICE_ID_IO_P0 + 14)
#define ID_PIN_LED_RED      (DEVICE_ID_IO_P0 + 15)
#define ID_PIN_LED_BLUE     (DEVICE_ID_IO_P0 + 16)
#define ID_PIN_BUZZER       (DEVICE_ID_IO_P0 + 17)
#define ID_PIN_P18          (DEVICE_ID_IO_P0 + 18)
#define ID_PIN_P19          (DEVICE_ID_IO_P0 + 19)
#define ID_PIN_P20          (DEVICE_ID_IO_P0 + 20)
#define ID_PIN_P21          (DEVICE_ID_IO_P0 + 21)
#define ID_PIN_P22          (DEVICE_ID_IO_P0 + 22)
#define ID_PIN_P23          (DEVICE_ID_IO_P0 + 23)
#define ID_PIN_P24          (DEVICE_ID_IO_P0 + 24)
#define ID_PIN_SERVO_ONE    (DEVICE_ID_IO_P0 + 25)
#define ID_PIN_SERVO_TWO    (DEVICE_ID_IO_P0 + 26)
#define ID_PIN_INT          (DEVICE_ID_IO_P0 + 27)
#define ID_PIN_BUTTON_UP    (DEVICE_ID_IO_P0 + 28)
#define ID_PIN_BUTTON_DOWN  (DEVICE_ID_IO_P0 + 29)
#define ID_PIN_BUTTON_LEFT  (DEVICE_ID_IO_P0 + 30)
#define ID_PIN_BUTTON_RIGHT (DEVICE_ID_IO_P0 + 31)
#define ID_PIN_TEMP_SENSOR  (DEVICE_ID_IO_P0 + 32)
#define ID_PIN_LIGHT_SENSOR (DEVICE_ID_IO_P0 + 33)
#endif  //  NOTUSED
