#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "pinmap.h"
#include "pin_device.h"

void pin_clear(
    uint32_t port,  //  e.g. GPIOC
    uint16_t pin    //  e.g. GPIO13
) {
	//  Clear GPIO pin.
	gpio_clear(port, pin);
}

void pin_set(
    uint32_t port,  //  e.g. GPIOC
    uint16_t pin    //  e.g. GPIO13
) {
	//  Set GPIO pin.
	gpio_set(port, pin);
}

void pin_toggle(
    uint32_t port,  //  e.g. GPIOC
    uint16_t pin    //  e.g. GPIO13
) {
	//  Toggle GPIO pin.
	gpio_toggle(port, pin);
}

#ifdef NOTUSED

    #define error MBED_ERROR

    extern GPIO_TypeDef *Set_GPIO_Clock(uint32_t port_idx);

    const uint32_t ll_pin_defines[16] = {
    #ifdef TODO
        LL_GPIO_PIN_0,
        LL_GPIO_PIN_1,
        LL_GPIO_PIN_2,
        LL_GPIO_PIN_3,
        LL_GPIO_PIN_4,
        LL_GPIO_PIN_5,
        LL_GPIO_PIN_6,
        LL_GPIO_PIN_7,
        LL_GPIO_PIN_8,
        LL_GPIO_PIN_9,
        LL_GPIO_PIN_10,
        LL_GPIO_PIN_11,
        LL_GPIO_PIN_12,
        LL_GPIO_PIN_13,
        LL_GPIO_PIN_14,
        LL_GPIO_PIN_15
    #endif  //  TODO
    };

    typedef enum {
        PortA = 0,
        PortB = 1,
        PortC = 2,
        PortD = 3,
        PortE = 4,
        PortF = 5,
        PortG = 6,
        PortH = 7,
        PortI = 8,
        PortJ = 9,
        PortK = 10
    } PortName;

    // Enable GPIO clock and return GPIO base address
    GPIO_TypeDef *Set_GPIO_Clock(uint32_t port_idx) {
        return NULL;  ////TODO
    #ifdef TODO
        uint32_t gpio_add = 0;
        switch (port_idx) {
            case PortA:
                gpio_add = GPIOA_BASE;
                __HAL_RCC_GPIOA_CLK_ENABLE();
                break;
            case PortB:
                gpio_add = GPIOB_BASE;
                __HAL_RCC_GPIOB_CLK_ENABLE();
                break;
    #if defined(GPIOC_BASE)
            case PortC:
                gpio_add = GPIOC_BASE;
                __HAL_RCC_GPIOC_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOD_BASE
            case PortD:
                gpio_add = GPIOD_BASE;
                __HAL_RCC_GPIOD_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOE_BASE
            case PortE:
                gpio_add = GPIOE_BASE;
                __HAL_RCC_GPIOE_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOF_BASE
            case PortF:
                gpio_add = GPIOF_BASE;
                __HAL_RCC_GPIOF_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOG_BASE
            case PortG:
    #if defined TARGET_STM32L4
                __HAL_RCC_PWR_CLK_ENABLE();
                HAL_PWREx_EnableVddIO2();
    #endif
                gpio_add = GPIOG_BASE;
                __HAL_RCC_GPIOG_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOH_BASE
            case PortH:
                gpio_add = GPIOH_BASE;
                __HAL_RCC_GPIOH_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOI_BASE
            case PortI:
                gpio_add = GPIOI_BASE;
                __HAL_RCC_GPIOI_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOJ_BASE
            case PortJ:
                gpio_add = GPIOJ_BASE;
                __HAL_RCC_GPIOJ_CLK_ENABLE();
                break;
    #endif
    #if defined GPIOK_BASE
            case PortK:
                gpio_add = GPIOK_BASE;
                __HAL_RCC_GPIOK_CLK_ENABLE();
                break;
    #endif
            default:
                error("Pinmap error: wrong port number.");
                break;
        }
        return (GPIO_TypeDef *) gpio_add;
    #endif  //  TODO
    }

    /**
     * Configure pin (mode, speed, output type and pull-up/pull-down)
     */
    void pin_function(PinName pin, int data)
    {
    #ifdef TODO
        MBED_ASSERT(pin != (PinName)NC);

        // Get the pin informations
        uint32_t mode  = STM_PIN_FUNCTION(data);
        uint32_t afnum = STM_PIN_AFNUM(data);
        uint32_t port = STM_PORT(pin);
        uint32_t ll_pin  = ll_pin_defines[STM_PIN(pin)];
        uint32_t ll_mode = 0;

        // Enable GPIO clock
        GPIO_TypeDef *gpio = Set_GPIO_Clock(port);

        /*  Set default speed to high.
        *  For most families there are dedicated registers so it is
        *  not so important, register can be set at any time.
        *  But for families like F1, speed only applies to output.
        */
    #if defined (TARGET_STM32F1)
    if (mode == STM_PIN_OUTPUT) {
    #endif
        LL_GPIO_SetPinSpeed(gpio, ll_pin, LL_GPIO_SPEED_FREQ_HIGH);
    #if defined (TARGET_STM32F1)
    }
    #endif

        switch (mode) {
            case STM_PIN_INPUT:
                ll_mode = LL_GPIO_MODE_INPUT;
                break;
            case STM_PIN_OUTPUT:
                ll_mode = LL_GPIO_MODE_OUTPUT;
                break;
            case STM_PIN_ALTERNATE:
                ll_mode = LL_GPIO_MODE_ALTERNATE;
                // In case of ALT function, also set he afnum
            stm_pin_SetAFPin(gpio, pin, afnum);
                break;
            case STM_PIN_ANALOG:
                ll_mode = LL_GPIO_MODE_ANALOG;
                break;
            default:
                MBED_ASSERT(0);
                break;
        }
        LL_GPIO_SetPinMode(gpio, ll_pin, ll_mode);

    #if defined(GPIO_ASCR_ASC0)
        /*  For families where Analog Control ASC0 register is present */
        if (STM_PIN_ANALOG_CONTROL(data)) {
            LL_GPIO_EnablePinAnalogControl(gpio, ll_pin);
        } else {
            LL_GPIO_DisablePinAnalogControl(gpio, ll_pin);
        }
    #endif

        /*  For now by default use Speed HIGH for output or alt modes */
        if ((mode == STM_PIN_OUTPUT) ||(mode == STM_PIN_ALTERNATE)) {
        if (STM_PIN_OD(data)) {
                LL_GPIO_SetPinOutputType(gpio, ll_pin, LL_GPIO_OUTPUT_OPENDRAIN);
        } else {
                LL_GPIO_SetPinOutputType(gpio, ll_pin, LL_GPIO_OUTPUT_PUSHPULL);
        }
        }

        stm_pin_PullConfig(gpio, ll_pin, STM_PIN_PUPD(data));

        stm_pin_DisconnectDebug(pin);
    #endif  //  TODO
    }

    /**
     * Configure pin pull-up/pull-down
     */
    void pin_mode(PinName pin, PinMode mode)
    {
    #ifdef TODO
        MBED_ASSERT(pin != (PinName)NC);

        uint32_t port_index = STM_PORT(pin);
        uint32_t ll_pin  = ll_pin_defines[STM_PIN(pin)];
        // Enable GPIO clock
        GPIO_TypeDef *gpio = Set_GPIO_Clock(port_index);
        uint32_t function = LL_GPIO_GetPinMode(gpio, ll_pin);

        if ((function == LL_GPIO_MODE_OUTPUT) || (function == LL_GPIO_MODE_ALTERNATE))
        {
            if ((mode == OpenDrainNoPull) || (mode == OpenDrainPullUp) || (mode == OpenDrainPullDown)) {
                LL_GPIO_SetPinOutputType(gpio, ll_pin, LL_GPIO_OUTPUT_OPENDRAIN);
            } else {
                LL_GPIO_SetPinOutputType(gpio, ll_pin, LL_GPIO_OUTPUT_PUSHPULL);
            }
        }

        if ((mode == OpenDrainPullUp) || (mode == PullUp)) {
            stm_pin_PullConfig(gpio, ll_pin, GPIO_PULLUP);
        } else if ((mode == OpenDrainPullDown) || (mode == PullDown)) {
            stm_pin_PullConfig(gpio, ll_pin, GPIO_PULLDOWN);
        } else {
            stm_pin_PullConfig(gpio, ll_pin, GPIO_NOPULL);
        }
    #endif  //  TODO
    }

#endif  //  NOTUSED
