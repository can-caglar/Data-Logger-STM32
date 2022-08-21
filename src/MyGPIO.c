#include "MyGPIO.h"
#include "MyRCC.h"
#include "MyBitStuff.h"
#include <user_stm32f407xx.h>

#ifdef TEST
#include <stdio.h>
#endif


// Library assumes clock for peripheral has been enabled already

// Private functions forward decl
static uint32_t pinInMask(const GPIO_Pin_Number_e bit, const GPIO_Pin_Mask_t mask);
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
static void configurePupd(io_register* reg, GPIO_Pin_Number_e pin, GPIO_PullUpDown_e mode);
static void configureAsOpenDrain(io_register* outmode, const GPIO_Pin_Number_e pin);
static void configureAsPushPull(io_register* outmode, const GPIO_Pin_Number_e pin);

// TODO static uint8 checkAltFunc(GPIO_ALTF_e altf);

enum // TODO, give this a typedef
{
    BITS_PER_PIN_MODER = 2,
    BITS_PER_PIN_PUPDR = 2,
    BITS_PER_PIN_AFR = 4,
};

// Public functions

Error_Code_e MyGPIO_Init(const MyGPIO* gpio)
{
    Error_Code_e err = ECODE_OK;
    GPIO_TypeDef* gpio_port = gpio->gpio_register;
    GPIO_Pin_Mask_t pin_mask = gpio->pin_mask;
    GPIO_Mode_e mode = gpio->mode;
    GPIO_OutputType_e outputType = gpio->output_type;
    GPIO_ALTF_e alt_func = gpio->alt_func;
    GPIO_PullUpDown_e pupd = gpio->pupd;

    // Check params
    if (noError(&err))
    {
        err = (alt_func >= MAX_ALT_FUNCTIONS) ? ECODE_BAD_PARAM : ECODE_OK;
    }
    // Start init...
    if (noError(&err))
    {
        // configuring each gpio pin "pin"
        for (GPIO_Pin_Number_e pin = pin_num_0; pin < MAX_GPIO_PINS; pin++)
        {
            if (pinInMask(pin, pin_mask))
            {
                // TODO, make into function
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
                switch(outputType)
                {
                    case GPIO_OPEN_DRAIN:
                        configureAsOpenDrain(&gpio_port->OTYPER, pin);
                        break;
                    case GPIO_PUSH_PULL:
                        configureAsPushPull(&gpio_port->OTYPER, pin);
                        break;
                }
                switch(pupd)
                {
                    case GPIO_PUPD_UP:
                        configurePupd(&gpio_port->PUPDR, pin, GPIO_PUPD_UP);
                        break;
                    case GPIO_PUPD_DOWN:
                        configurePupd(&gpio_port->PUPDR, pin, GPIO_PUPD_DOWN);
                        break;
                    case GPIO_PUPD_NONE:
                        configurePupd(&gpio_port->PUPDR, pin, GPIO_PUPD_NONE);
                        break;
                }
            }
        }
    }
    return err;
}

Error_Code_e MyGPIO_Write(GPIO_TypeDef* gpio_port, GPIO_Pin_Mask_t pin_mask, GPIO_State_e high)
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

uint32_t pinInMask(const GPIO_Pin_Number_e pin, const GPIO_Pin_Mask_t mask)
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

static void configureAsOpenDrain(io_register* outmode, const GPIO_Pin_Number_e pin)
{
    setBit(outmode, pin);
}

static void configureAsPushPull(io_register* outmode, const GPIO_Pin_Number_e pin)
{
    clearBit(outmode, pin);
}

static void configureAsAltFunction(io_register* moder, io_register* alt_func_reg, GPIO_ALTF_e alt_func, GPIO_Pin_Number_e pin)
{
    *moder |= GPIO_ALT << (pin * 2);
    if (pin < pin_num_8)
    {
        alt_func_reg[AFR_LOW] &= ~(GPIO_ALTF_MASK << (pin * BITS_PER_PIN_AFR));
        alt_func_reg[AFR_LOW] |= alt_func << (pin * BITS_PER_PIN_AFR);
    }
    else
    {
        pin -= pin_num_8;
        alt_func_reg[AFR_HIGH] &= ~(GPIO_ALTF_MASK << (pin * BITS_PER_PIN_AFR));
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

static void configurePupd(io_register* reg, GPIO_Pin_Number_e pin, GPIO_PullUpDown_e mode)
{
    *reg &= ~(GPIO_PUPD_MASK << getPositionForPinInRegister(pin, BITS_PER_PIN_PUPDR));
    *reg |= (mode << getPositionForPinInRegister(pin, BITS_PER_PIN_PUPDR));
}
