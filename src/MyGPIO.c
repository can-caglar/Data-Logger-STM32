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
static void configureAsAltFunction(io_register* moder, io_register* alt_func_reg, GPIO_ALTF_e alt_func, GPIO_Pin_Number_e pin);
uint8_t isConfiguredAs(GPIO_Mode_e whatMode, io_register moder, const GPIO_Pin_Number_e pin);
static inline void clearBit(io_register* reg, const uint8_t pin);
static inline void setBit(io_register* reg, const uint8_t pin);
static uint8_t bitIsHigh(io_register* reg, const uint8_t bit);
static inline uint8_t noError(const Error_Code_e* err);

// TODO static uint8 checkAltFunc(GPIO_ALTF_e altf);

enum
{
    BITS_PER_PIN_MODER = 2,
    BITS_PER_PIN_AFR = 4,
};

// Public functions
Error_Code_e MyGPIO_Init(const MyGPIO* gpio)
{
    Error_Code_e err = ECODE_OK;
    GPIO_TypeDef* gpio_port = gpio->gpio_register;
    GPIO_Pin_Mask_e pin_mask = gpio->pin_mask;
    GPIO_Mode_e mode = gpio->mode;
    GPIO_ALTF_e alt_func = gpio->alt_func;

    // Check params
    if (noError(&err))
    {
        err = (alt_func >= GPIO_MAX_ALT_FUNCTIONS) ? ECODE_BAD_PARAM : ECODE_OK;
    }
    if (noError(&err))
    {
        err = (pin_mask & LAST_16_BITS) ? ECODE_BAD_PARAM : ECODE_OK;
    }

    // Start init...
    if (noError(&err))
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
                    case GPIO_ALT:
                        configureAsAltFunction(&gpio_port->MODER, gpio_port->AFR, alt_func, pin);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return err;
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

static void configureAsAltFunction(io_register* moder, io_register* alt_func_reg, GPIO_ALTF_e alt_func, GPIO_Pin_Number_e pin)
{
    *moder |= GPIO_ALT << (pin * 2);
    if (pin < pin_num_8)
    {
        alt_func_reg[AFR_LOW] |= alt_func << (pin * BITS_PER_PIN_AFR);
    }
    else
    {
        alt_func_reg[AFR_HIGH] |= alt_func << (pin * BITS_PER_PIN_AFR);
    }
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

static inline uint8_t noError(const Error_Code_e* err)
{
    return *err == ECODE_OK;
}
