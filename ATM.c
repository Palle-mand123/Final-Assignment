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
volatile static INT16U Pin;
volatile static INT16U WithdrawAmount;



enum ATM_states
{
    CREDIT_CARD_SALDO,
    PIN_CODE,
    CHECK_PIN,
    WITHDRAWAL_AMOUNTS,
    WITHDRAWAL_OPTIONS,
    PRINT

};


enum ATM_states ATM_state = CREDIT_CARD_SALDO;

void set_ATM_state(enum ATM_states new_state)
{
    ATM_state = new_state;
}

void send_amount(INT16U s)
{
    if(s !=0)
    {
    wr_ch_LCD(s / 10000 + '0');
    s = s % 10000;
    wr_ch_LCD(s / 1000 + '0');
    s = s % 1000;
    wr_ch_LCD(s / 100 + '0');
    s = s % 100;
    wr_ch_LCD(s / 10 + '0');
    s = s % 10;
    wr_ch_LCD(s + '0');
    }

}


void ATM_task(void *pvParameters)
{
    wr_str_LCD("Enter Saldo: ");
    static char key;
    static int step = 0;
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
           pos=0;
           home_LCD();
           vTaskDelay(10);
           wr_str_LCD("Enter pin: ");
           while(ATM_state==PIN_CODE)
           {
           if( uxQueueMessagesWaiting(xQueue_keypad))
                       {
                           if(xSemaphoreTake(xSemaphore_keypad,( TickType_t ) 10 ) == pdTRUE)
                           {
                               if (xQueueReceive(xQueue_keypad, &key, portMAX_DELAY))
                               {
                                   Set_cursor(0x40+0x80+pos);
                                   wr_ch_LCD(key);
                                   pos++;

                                   Pin = Pin * 10 +(key - '0');

                                   xSemaphoreGive(xSemaphore_keypad);

                               }
                           }
                       }
           if(check_first_switch()==0)
           {
               clr_LCD();
               set_ATM_state(CHECK_PIN);
               vTaskDelay(10);
           }
           }

            break;
        case CHECK_PIN:
            if((Pin % 8)==0)
                           {
                               set_ATM_state(WITHDRAWAL_AMOUNTS);


                           }
                           else
                           {
                               wr_str_LCD("Wrong Pin-Code");
                               vTaskDelay(1000);
                               clr_LCD();
                               vTaskDelay(10);
                               set_ATM_state(PIN_CODE);
                           }
            break;

        case WITHDRAWAL_AMOUNTS:
            step =0;
            home_LCD();
            vTaskDelay(10);
            wr_str_LCD("Withdraw:");
            vTaskDelay(10);
            Set_cursor(0x40+0x80+1);
            wr_str_LCD("500");
            vTaskDelay(10);
            Set_cursor(0x40+0x80+5);
            wr_str_LCD("200");
            vTaskDelay(10);
            Set_cursor(0x40+0x80+9);
            wr_str_LCD("100");
            vTaskDelay(10);
            Set_cursor(0x40+0x80+13);
            wr_str_LCD("50");
            while(ATM_state==WITHDRAWAL_AMOUNTS)
            {
                if(step==0)
                {
                    vTaskDelay(10);
                    Set_cursor(0x40+0x80);
                    wr_str_LCD(">");
                    vTaskDelay(10);
                    home_LCD();
                    if(check_second_switch()==0)
                    {
                       Set_cursor(0x40+0x80);
                       wr_str_LCD(" ");
                       step++;
                    }

                }

                if(step==1)
                {
                    vTaskDelay(10);
                    Set_cursor(0x40+0x80+4);
                    wr_str_LCD(">");
                    vTaskDelay(10);
                    home_LCD();

                        if(check_second_switch()==0)
                        {
                            Set_cursor(0x40+0x80+4);
                            wr_str_LCD(" ");
                            step++;
                        }

                    if(check_first_switch()==0)
                    {
                        Set_cursor(0x40+0x80+4);
                        wr_str_LCD(" ");
                        step--;
                    }
                }

                if(step==2)
                {
                    vTaskDelay(10);
                    Set_cursor(0x40+0x80+8);
                    wr_str_LCD(">");
                    vTaskDelay(10);
                    home_LCD();
                    if(check_second_switch()==0)
                    {
                         Set_cursor(0x40+0x80+8);
                         wr_str_LCD(" ");
                         step++;
                    }
                    if(check_first_switch()==0)
                                        {
                                            Set_cursor(0x40+0x80+8);
                                            wr_str_LCD(" ");
                                            step--;
                                        }
                 }

                if(step==3)
                {
                    vTaskDelay(10);
                    Set_cursor(0x40+0x80+12);
                    wr_str_LCD(">");
                    vTaskDelay(10);
                    home_LCD();

                    if(check_first_switch()==0)
                                        {
                                            Set_cursor(0x40+0x80+12);
                                            wr_str_LCD(" ");
                                            step--;
                                        }
                }
                vTaskDelay(10);
                Set_cursor(0x80+11);
                send_amount(WithdrawAmount);
                vTaskDelay(10);
                home_LCD();
            }
            break;
        case WITHDRAWAL_OPTIONS:
            break;
        case PRINT:
            break;

        }
    }
}
