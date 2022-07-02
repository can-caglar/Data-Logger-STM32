#include "MyGPIO.h"
#include "MyRCC.h"
#include <stm32f407xx.h>

#ifdef TEST
#include <stdio.h>
#endif

// Library assumes clock for peripheral has been enabled already

// Private functions forward decl
static uint8_t pinInMask(const uint8_t bit, const GPIO_Pin_e mask);
static io_register getPositionForPinInRegister(const uint8_t pin, const uint8_t num_bits);
static io_register moderMask(GPIO_Mode_e mode, uint8_t pin);
static void configureAsOutput(io_register* moder, const uint8_t pin);
static uint8_t isConfiguredAsOutput(io_register moder, const uint8_t pin);

enum
{
    BITS_PER_PIN_MODER = 2,
};

// Public functions
void* MyGPIO_Init(GPIO_TypeDef* gpio_port, GPIO_Pin_e pin_mask, GPIO_Mode_e mode)
{
    // for all pins
    for (uint8_t pin = 0; pin < MAX_GPIO_PINS; pin++)
    {
        if (pinInMask(pin, pin_mask))
        {
            configureAsOutput(&gpio_port->MODER, pin);
        }
    }
    return 0;
}

Error_Code_e MyGPIO_Write(GPIO_TypeDef* gpio_port, GPIO_Pin_e pin_mask, int high)
{
    Error_Code_e err = ECODE_OK;
    if (high)
    {
        gpio_port->ODR |= pin_mask;
    }
    else
    {
        gpio_port->ODR &= ~pin_mask;
    }
    // TODO: refactor below.
    for (uint8_t pin = 0; pin < MAX_GPIO_PINS; pin++)
    {
        if (pinInMask(pin, pin_mask))
        {
            if (!isConfiguredAsOutput(gpio_port->MODER, pin))
            {
                err = ECODE_NOT_INITIALIZED;
            }
        }
    }
    return err;
}

/************************ Private functions **********************/

uint8_t pinInMask(const uint8_t pin, const GPIO_Pin_e mask)
{
    return (mask & (1UL << pin));
}

io_register getPositionForPinInRegister(const uint8_t pin, const uint8_t num_bits)
{
    return num_bits * pin;
}

void configureAsOutput(io_register* moder, const uint8_t pin)
{
    *moder |= (GPIO_OUTPUT << getPositionForPinInRegister(pin, BITS_PER_PIN_MODER));
}

io_register moderMask(GPIO_Mode_e mode, uint8_t pin)
{
    mode &= 0x3;
    return (mode << getPositionForPinInRegister(pin, BITS_PER_PIN_MODER));
}

uint8_t isConfiguredAsOutput(io_register moder, const uint8_t pin)
{
    io_register expected_moder = moderMask(GPIO_OUTPUT, pin);
    io_register moder_mask = moderMask(0x3, pin);
    moder &= moder_mask;
    return expected_moder == moder;
}
