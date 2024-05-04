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
#include "userSwitch.h"

extern SemaphoreHandle_t xSemaphore_ATM;
extern QueueHandle_t xQueue_ATM;
extern SemaphoreHandle_t xSemaphore_keypad;
extern QueueHandle_t xQueue_keypad;

volatile static INT32U Saldo = 0;



enum ATM_states
{
    CREDIT_CARD_SALDO,
    PIN_CODE,
    WITHDRAWAL_AMOUNTS,
    WITHDRAWAL_OPTIONS,
    PRINT

};


enum ATM_states ATM_state = CREDIT_CARD_SALDO;

void set_ATM_state(enum ATM_states new_state)
{
    ATM_state = new_state;
}



void ATM_task(void *pvParameters)
{
    wr_str_LCD("Enter Saldo: ");
    static char key;
    static INT8U pos = 0;
    while(1)
    {
        switch(ATM_state)
        {
        case CREDIT_CARD_SALDO:


            if( uxQueueMessagesWaiting(xQueue_keypad))
            {
                if(xSemaphoreTake(xSemaphore_keypad,( TickType_t ) 10 ) == pdTRUE)
                {
                    if (xQueueReceive(xQueue_keypad, &key, portMAX_DELAY))
                    {
                        Set_cursor(0x40+0x80+pos);
                        wr_ch_LCD(key);
                        pos++;

                        Saldo = Saldo * 10 +(key - '0');

                        xSemaphoreGive(xSemaphore_keypad);

                    }
                }
            }
            if(check_first_switch()==0)
            {
                set_ATM_state(PIN_CODE);
                clr_LCD();
            }

            break;



        case PIN_CODE:
           home_LCD();
           vTaskDelay(10);
           wr_str_LCD("Enter pin: ");

            break;

        case WITHDRAWAL_AMOUNTS:
            break;
        case WITHDRAWAL_OPTIONS:
            break;
        case PRINT:
            break;

        }
    }
}
