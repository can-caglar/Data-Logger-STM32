#include "MyGPIO.h"
#include "MyRCC.h"
#include <user_stm32f407xx.h>

#ifdef TEST
#include <stdio.h>
#endif

// Library assumes clock for peripheral has been enabled already

// Private functions forward decl
static uint32_t pinInMask(const GPIO_Pin_Number_e bit, const GPIO_Pin_Mask_e mask);
static io_register getPositionForPinInRegister(const GPIO_Pin_Number_e pin, const uint8_t num_bits);
static io_register moderMask(GPIO_Mode_e mode, GPIO_Pin_Number_e pin);
static void configureAsOutput(io_register* moder, const GPIO_Pin_Number_e pin);
static void configureAsInput(io_register* moder, const GPIO_Pin_Number_e pin);
uint8_t isConfiguredAs(GPIO_Mode_e whatMode, io_register moder, const GPIO_Pin_Number_e pin);
static inline void clearBit(io_register* reg, const uint8_t pin);
static inline void setBit(io_register* reg, const uint8_t pin);
static uint8_t bitIsHigh(io_register* reg, const uint8_t bit);

enum
{
    BITS_PER_PIN_MODER = 2,
};

// Public functions
void* MyGPIO_Init(GPIO_TypeDef* gpio_port, GPIO_Pin_Mask_e pin_mask, GPIO_Mode_e mode)
{
    // for all pins
    for (GPIO_Pin_Number_e pin = pin_num_0; pin < MAX_GPIO_PINS; pin++)
    {
        if (pinInMask(pin, pin_mask))
        {
            switch(mode)
            {
                case GPIO_OUTPUT:
                    configureAsOutput(&gpio_port->MODER, pin);
                    break;
                case GPIO_INPUT:
                    configureAsInput(&gpio_port->MODER, pin);
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}

Error_Code_e MyGPIO_Write(GPIO_TypeDef* gpio_port, GPIO_Pin_Mask_e pin_mask, GPIO_State_e high)
{
    Error_Code_e err = ECODE_OK;
    for (GPIO_Pin_Number_e pin = pin_num_0; pin < MAX_GPIO_PINS; pin++)
    {
        if (pinInMask(pin, pin_mask))
        {
            if (!isConfiguredAs(GPIO_OUTPUT, gpio_port->MODER, pin))
            {
                err = ECODE_NOT_OUTPUT;
            }
            else
            {
                if (high == GPIO_HIGH)
                {
                    setBit(&gpio_port->ODR, pin);
                }
                else
                {
                    clearBit(&gpio_port->ODR, pin);
                }
            }
        }
    }
    return err;
}

GPIO_State_e MyGPIO_Read(GPIO_TypeDef* gpio_reg, GPIO_Pin_Number_e pin)
{
    if (isConfiguredAs(GPIO_INPUT, gpio_reg->MODER, pin))
    {
        return (GPIO_State_e)(bitIsHigh(&gpio_reg->IDR, pin));
    }
    return GPIO_LOW;
}

/************************ Private functions **********************/

uint32_t pinInMask(const GPIO_Pin_Number_e pin, const GPIO_Pin_Mask_e mask)
{
    return (mask & (1UL << pin));
}

io_register getPositionForPinInRegister(const GPIO_Pin_Number_e pin, const uint8_t num_bits)
{
    return num_bits * pin;
}

void configureAsOutput(io_register* moder, const GPIO_Pin_Number_e pin)
{
    *moder |= (GPIO_OUTPUT << getPositionForPinInRegister(pin, BITS_PER_PIN_MODER));
}

static void configureAsInput(io_register* moder, const GPIO_Pin_Number_e pin)
{
    *moder &= ~(GPIO_MODE_MASK << getPositionForPinInRegister(pin, BITS_PER_PIN_MODER));
}

io_register moderMask(GPIO_Mode_e mode, GPIO_Pin_Number_e pin)
{
    return (mode << getPositionForPinInRegister(pin, BITS_PER_PIN_MODER));
}

uint8_t isConfiguredAs(GPIO_Mode_e whatMode, io_register moder, const GPIO_Pin_Number_e pin)
{
    io_register expected_moder = moderMask(whatMode, pin);
    io_register moder_mask = moderMask(GPIO_MODE_MASK, pin);
    moder &= moder_mask;
    return expected_moder == moder;
}

static void clearBit(io_register* reg, const uint8_t pin)
{
    *reg &= ~(0x1UL << pin);
}

static void setBit(io_register* reg, const uint8_t pin)
{
    *reg |= (0x1UL << pin);
}

static uint8_t bitIsHigh(io_register* reg, const uint8_t bit)
{
    return (GPIO_State_e)(*reg & (0x1UL << bit)) != 0;
}