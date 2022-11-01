#include "button.h"

static void * p_user_c = NULL; /* Pointer for safe user context */
static button_callback_t p_user_callback = NULL;

static volatile uint16_t counter = 0;
static volatile uint8_t click_counter = 0;
static volatile bool event_flag = false;
static button_ces_t ces;

enum button_ms_t
{
  BUTTON_MS_FIRST_CLICK,
  BUTTON_MS_WAIT_PAUSE,
  BUTTON_MS_NEXT_CLICK,
} bms = BUTTON_MS_FIRST_CLICK;  /* Button mashine state */

static bool button_check(void)
{
  if(HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN) == GPIO_PIN_SET)
  {
    return true;
  }
  else return false;
}

static void button_set_wait_first_click(void)
{
  counter = 0;
  click_counter = 0;
  bms = BUTTON_MS_FIRST_CLICK;
}

static void button_ms_first_handler(void)
{
  if (button_check() == true)
  {
    counter++;
  }
  else 
  {
    if (counter > BUTTON_TIME_ONE_CLICK_MIN_MS)
    {
      if (counter < BUTTON_TIME_ONE_CLICK_MAX_MS)
      {
        bms = BUTTON_MS_WAIT_PAUSE;
        counter = 0;
        click_counter = 1;
      }
      else if (counter > BUTTON_TIME_ONE_CLICK_MAX_MS)
      {
        counter = 0;
        
        if (counter >= BUTTON_TIME_LONG_PRESS_MIN_MS && counter <= BUTTON_TIME_LONG_PRESS_MAX_MS)
        {
          ces = BUTTON_CES_LONG_CLICK;
          event_flag = true;
        }        
      }
    }
    else 
    {
      counter = 0;
    }
  }
}

static void button_ms_pause_handler(void)
{
  if (button_check() == false)
  {
    counter++;
    if (counter >= BUTTON_TIME_CLICK_PAUSE_MAX_MS)
    {
      counter = 0;
      bms = BUTTON_MS_FIRST_CLICK;
      
      
      switch(click_counter)
      {
        case 1:
          ces = BUTTON_CES_ONE_CLICK;
          event_flag = true;
          break;

        case 2:
          ces = BUTTON_CES_DOUBLE_CLICK;
          event_flag = true;
          break;

        case 3:
          ces = BUTTON_CES_THREE_CLICK;
          event_flag = true;

        default:
          button_set_wait_first_click();
          break;
      }
      click_counter = 0;
    }
  }
  else 
  {
    if (counter >= BUTTON_TIME_CLICK_PAUSE_MIN_MS && 
        counter <= BUTTON_TIME_CLICK_PAUSE_MAX_MS)
    {
      if(click_counter > BUTTON_MAX_CLICK_NUMBER)
      {
        button_set_wait_first_click();
      }
      else 
      {
        counter = 0;
        bms = BUTTON_MS_NEXT_CLICK;
        click_counter++;
      }
    }
    else 
    {
      button_set_wait_first_click();
    }
  }  
   
}

void button_ms_next_click_handler(void)
{
  if (button_check() == true)
  {
    counter++;
  }
  else 
  {
    if (counter >= BUTTON_TIME_CLICK_PAUSE_MIN_MS &&
        counter <= BUTTON_TIME_CLICK_PAUSE_MAX_MS)
    {
      counter = 0;
      bms = BUTTON_MS_WAIT_PAUSE;
    }
    else 
    {
      button_set_wait_first_click();
    }  
  }
}

void button_init(button_callback_t p_callback, void * p_context)
{
  p_user_c = p_context;
  p_user_callback = p_callback;

  counter = 0;
  click_counter = 0;
  event_flag = false;
  bms = BUTTON_MS_FIRST_CLICK;
}

void button_process(void)
{
  // Check button event flag
  if (event_flag == true)
  {
    if(p_user_callback != NULL)
    {
      p_user_callback(ces, p_user_c);
    }
    event_flag = false;
  }
}

void button_timer_process(void)
{
  if (event_flag == false)
  {

    switch(bms)
    {
      case BUTTON_MS_FIRST_CLICK:
          button_ms_first_handler();    
          break;
      case BUTTON_MS_WAIT_PAUSE:
          button_ms_pause_handler();
          break;
      case BUTTON_MS_NEXT_CLICK:
          button_ms_next_click_handler();
          break;

      default:
          button_set_wait_first_click();
          break;
    }
  }
}
