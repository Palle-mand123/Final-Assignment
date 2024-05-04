#include <stdint.h>
#include "FreeRTOS.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "lcd.h"

#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "systick_frt.h"
#include "rotary.h"
#include "keypad.h"
#include "ATM.h"
#include "userSwitch.h"

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3
#define QUEUE_LEN 128

QueueHandle_t xQueue_lcd;
SemaphoreHandle_t xSemaphore_lcd;

QueueHandle_t xQueue_keypad;
SemaphoreHandle_t xSemaphore_keypad;

QueueHandle_t xQueue_ATM;
SemaphoreHandle_t xSemaphore_ATM;


static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  init_gpio();
  //init_rotary();
  //init_led();
  xQueue_lcd = xQueueCreate(QUEUE_LEN, sizeof(INT8U));
  xQueue_keypad = xQueueCreate(QUEUE_LEN, sizeof(INT8U));
  xQueue_ATM = xQueueCreate(QUEUE_LEN, sizeof(INT16U));

  xSemaphore_lcd = xSemaphoreCreateMutex();
  xSemaphore_keypad = xSemaphoreCreateMutex();
  xSemaphore_ATM = xSemaphoreCreateMutex();

}

int main(void) {
    // Initialize hardware peripherals (e.g., GPIO) here
    setupHardware();
    // Initialize the LCD module
    xTaskCreate(rotary_task, "rotary", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, NULL);
    xTaskCreate(key_task, "keyboard", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, NULL);
    xTaskCreate(lcd_task, "LCDTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, NULL); // Add LCD task
    xTaskCreate(ATM_task, "ATM", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, NULL);
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();



    return 0;
}



