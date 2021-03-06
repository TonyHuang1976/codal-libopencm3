//  Based on https://github.com/nedseb/codal-stm32-iot-node/blob/master/model/STM32IotNode.cpp
/*
    The MIT License (MIT)

    Copyright (c) 2016 Lancaster University, UK.

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
#include <logger.h>
#include "STM32BluePill.h"
#include "Timer.h"

using namespace codal;

static STM32BluePill *device_instance = NULL;

/**
  * Constructor.
  *
  * Create a representation of a GenuinoZero device, which includes member variables
  * that represent various device drivers used to control aspects of the micro:bit.
  */
STM32BluePill::STM32BluePill() :
    timer(),
    messageBus(),
    io(),
    i2c1(io.sda1, io.scl1),   //  Port I2C1 (I2C2 not supported yet)
    spi1(io.mosi1, io.miso1, io.sck1, io.nss1),  //  Port SPI1
    spi2(io.mosi2, io.miso2, io.sck2, io.nss2),  //  Port SPI2
    usart2(io.tx2, io.rx2) {  //  Port USART2 (USART1, USART3 not supported yet)
    //  Clear our status
    status = 0;
    device_instance = this;
    debug_println("bluepill construct"); debug_flush(); ////
}

void STM32BluePill::enableDebug() { target_enable_debug(); }
void STM32BluePill::disableDebug() { target_disable_debug(); }

/**
  * Post constructor initialisation method.
  *
  * This call will initialised the scheduler, memory allocator and Bluetooth stack.
  *
  * This is required as the Bluetooth stack can't be brought up in a
  * static context i.e. in a constructor.
  *
  * @code
  * uBit.init();
  * @endcode
  *
  * @note This method must be called before user code utilises any functionality
  *       contained within the GenuinoZero class.
  */
int STM32BluePill::init()
{
    if (status & DEVICE_INITIALIZED)
        return DEVICE_NOT_SUPPORTED;
    status |= DEVICE_INITIALIZED;

    debug_println("bluepill init scheduler"); debug_flush(); ////

    //  Codal initialisation... Bring up fiber scheduler.
    scheduler_init(messageBus);
    debug_println("bluepill init components"); debug_flush(); ////

    //  Init the timer first, all other components depend on the timer.
    timer.init();

    //  Initialise all the Codal components.
    for(int i = 0; i < DEVICE_COMPONENT_COUNT; i++)
    {
        if(CodalComponent::components[i])
            CodalComponent::components[i]->init();
    }
    status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;
    debug_println("bluepill init ok"); debug_flush(); ////
    return DEVICE_OK;
}

/**
  * A periodic callback invoked by the fiber scheduler idle thread.
  * We use this for any low priority, background housekeeping.
  *
  */
void STM32BluePill::idleCallback() {
    //  TODO: Poll USB.
    codal_dmesg_flush();
}

void STM32BluePill::periodicCallback() {
    //  TODO
}

