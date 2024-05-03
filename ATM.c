#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "tmodel.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "lcd.h"
#include "keypad.h"

extern SemaphoreHandle_t xSemaphore_ATM;
extern QueueHandle_t xQueue_ATM;
extern SemaphoreHandle_t xSemaphore_keypad;
extern QueueHandle_t xQueue_keypad;

enum ATM_states
{
    CREDIT_CARD,
    WITHDRAWAL,
    PIN_CODE,
    WITHDRAWAL_AMOUNTS,
    WITHDRAWAL_OPTIONS,
    PRINT,

};

void Set_ATM_state(enum ATM_states new_state)
{
    ATM_state = new_state;
}

enum ATM_states ATM_state = CREDIT_CARD;

void ATM_task(void *pvParameters)
{
    static char key;
    while(1)
    {
        switch(ATM_state)
        {
        case CREDIT_CARD:
            wr_ch_LCD('Enter Saldo: ');
            if( uxQueueMessagesWaiting(xQueue_keypad))
            {
                if(xSemaphoreTake(xSemaphore_keypad,( TickType_t ) 10 ) == pdTRUE)
                {
                    if (xQueueReceive(xQueue_keypad, &key, portMAX_DELAY))
                    {

                        xSemaphoreGive(xSemaphore_keypad);
                    }
                }
            }

        case WITHDRAWAL:
        case PIN_CODE:
        case WITHDRAWAL_AMOUNTS:
        case WITHDRAWAL_OPTIONS:
        case PRINT:

        }
    }
}
