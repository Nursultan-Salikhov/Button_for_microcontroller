
#ifndef BUTTON_H
#define BUTTON_H



/* Includes */

#include "stm32f0xx_hal.h"

#include "stdbool.h"

/* Defines */
#define BUTTON_GPIO_PORT                  GPIOA
#define BUTTON_GPIO_PIN                   GPIO_PIN_0

#define BUTTON_TIME_ONE_CLICK_MIN_MS      50    /* Minimum time for one click in ms */
#define BUTTON_TIME_ONE_CLICK_MAX_MS      350   /* Maximum time for one click in ms */
#define BUTTON_TIME_CLICK_PAUSE_MIN_MS    50    /* Minimum pause time between click in ms */
#define BUTTON_TIME_CLICK_PAUSE_MAX_MS    400   /* Maximum pause time between click in ms */
#define BUTTON_TIME_LONG_PRESS_MIN_MS     5000  /* Minimum time for long press */
#define BUTTON_TIME_LONG_PRESS_MAX_MS     10000 /* Maximum time for long press */

#define BUTTON_MAX_CLICK_NUMBER           3     /* Maximum number of click */

/**
* @brief  Click event status type.
*/
typedef enum 
{
  BUTTON_CES_ONE_CLICK,
  BUTTON_CES_DOUBLE_CLICK,
  BUTTON_CES_THREE_CLICK,
  BUTTON_CES_LONG_CLICK
} button_ces_t;

/** 
* @brief    Button callback type.
*/
typedef void (* button_callback_t)(button_ces_t status, void * p_context);

/**
* @brief    Init function.
* @param[in] p_callback  Pointer to callback.
* @param[in] p_context   Pointer to user context for callback.
*/ 
void button_init(button_callback_t p_callback, void * p_context);

/**
* @brief    Run function for button process, 
*           must be placed in a supercycle or in a stream.
*/
void button_process(void);

/** 
* @brief    Function to timer process, it is necessary
*           to put this function in the timer called once per millisecond
*/
void button_timer_process(void);




#endif /* BUTTON_H */