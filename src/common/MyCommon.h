#ifndef MY_COMMON_H_INCLUDED
#define MY_COMMON_H_INCLUDED

#include <stdint.h>

#define FIRST_16_BITS  0x0000FFFF
#define LAST_16_BITS   0xFFFF0000
#define ALL_32_BITS    0xFFFFFFFF

// Error codes
typedef enum
{
    ECODE_OK,
    ECODE_NOT_OUTPUT,
    ECODE_BAD_PARAM,
    ECODE_NOT_READY,
} Error_Code_e;

// GPIO ports and pins
typedef enum
{
    GPIO_PORT_A_e,
    GPIO_PORT_B_e,
    GPIO_PORT_C_e,
    GPIO_PORT_D_e,
    GPIO_PORT_E_e,
    GPIO_PORT_F_e,
    GPIO_PORT_G_e,
    GPIO_PORT_H_e,
    GPIO_PORT_I_e,
    GPIO_PORT_COUNT_e,
} GPIO_Port_e;

typedef uint16_t GPIO_Pin_Mask_t;

extern const GPIO_Pin_Mask_t pin0_mask;
extern const GPIO_Pin_Mask_t pin1_mask;
extern const GPIO_Pin_Mask_t pin2_mask;
extern const GPIO_Pin_Mask_t pin3_mask;
extern const GPIO_Pin_Mask_t pin4_mask;
extern const GPIO_Pin_Mask_t pin5_mask;
extern const GPIO_Pin_Mask_t pin6_mask;
extern const GPIO_Pin_Mask_t pin7_mask;
extern const GPIO_Pin_Mask_t pin8_mask;
extern const GPIO_Pin_Mask_t pin9_mask;
extern const GPIO_Pin_Mask_t pin10_mask;
extern const GPIO_Pin_Mask_t pin11_mask;
extern const GPIO_Pin_Mask_t pin12_mask;
extern const GPIO_Pin_Mask_t pin13_mask;
extern const GPIO_Pin_Mask_t pin14_mask;
extern const GPIO_Pin_Mask_t pin15_mask;
extern const GPIO_Pin_Mask_t pins_all_mask;

typedef enum
{
    pin_num_0,
    pin_num_1,
    pin_num_2,
    pin_num_3,
    pin_num_4,
    pin_num_5,
    pin_num_6,
    pin_num_7,
    pin_num_8,
    pin_num_9,
    pin_num_10,
    pin_num_11,
    pin_num_12,
    pin_num_13,
    pin_num_14,
    pin_num_15,
    MAX_GPIO_PINS
} GPIO_Pin_Number_e;

// TODO, this information happens to be in two places
// see MY_USART
typedef enum
{
    USART1_Mask = (0x1U << 14),
} USART_Mask_e;

typedef enum
{
    GPIO_ALTF_0 = 0,
    GPIO_ALTF_1,
    GPIO_ALTF_2,
    GPIO_ALTF_3,
    GPIO_ALTF_4,
    GPIO_ALTF_5,
    GPIO_ALTF_6,
    GPIO_ALTF_7,
    GPIO_ALTF_8,
    GPIO_ALTF_9,
    GPIO_ALTF_10,
    GPIO_ALTF_11,
    GPIO_ALTF_12,
    GPIO_ALTF_13,
    GPIO_ALTF_14,
    GPIO_ALTF_15,
    GPIO_ALTF_MASK = GPIO_ALTF_15,
    MAX_ALT_FUNCTIONS,
} GPIO_ALTF_e;

#define AFR_LOW 0
#define AFR_HIGH 1

typedef volatile uint32_t io_register;

// USART
#define MY_USART        USART1  
#define MY_USART_GPIO   GPIOA
#define MY_USART_GPIO_RCC_POS  17
#define MY_USART_UART_RCC_POS  14
#define MY_USART_TX     pin2_mask 
#define MY_USART_RX     pin3_mask 
#define MY_USART_ALT    GPIO_ALTF_1

// LED
#define LED_GPIO_PORT   GPIOB
#define LED_GPIO_PIN    GPIO_PIN_3
#define LED_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE

#define LED_ERROR_PORT GPIOA
#define LED_ERROR_PIN  GPIO_PIN_11
#define LED_ERROR_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE

// Button
#define BUTTON_PIN GPIO_PIN_0
#define BUTTON_PORT GPIOB
#define BUTTON_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE

// DIP switch
#define GPIO_PORT_DIP   GPIOB
#define CLK_ENABLE_DIP  __HAL_RCC_GPIOB_CLK_ENABLE
#define GPIO_PIN_DIP_1  GPIO_PIN_5
#define GPIO_PIN_DIP_2  GPIO_PIN_1
#define GPIO_PIN_DIP_3  GPIO_PIN_0
#define GPIO_PIN_DIP_4  GPIO_PIN_4

#endif
