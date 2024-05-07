#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "lcd.h"
#include "glob_def.h"
#include "tmodel.h"
#include "semphr.h"
#include "portmacro.h"


extern SemaphoreHandle_t xSemaphore_button;
extern QueueHandle_t xQueue_button;

INT8U button_one()
{
    return( !(GPIO_PORTF_DATA_R & 0x10) );
}

INT8U button_two()
{
    return( !(GPIO_PORTF_DATA_R & 0x01) );
}

void button_task(void *pvParameters)
{
    const TickType_t xDelay = 250 / portTICK_PERIOD_MS;
    INT8U press;

    while(1)
    {
        if (button_one()==1)
        {
            press = 0x01;
            vTaskDelay(xDelay);
            if(uxQueueSpacesAvailable(xQueue_button))
            {
                if(xSemaphoreTake(xSemaphore_button, portMAX_DELAY))
                {
                    if(xQueueSend(xQueue_button, &press, portMAX_DELAY))
                    {
                        xSemaphoreGive(xSemaphore_button);

                    }
                }
            }
        }

        if (button_two()==1)
        {
            vTaskDelay(200 / portTICK_PERIOD_MS);
            if(button_two()==1)
            {
                press = 0x03;
                if(uxQueueSpacesAvailable(xQueue_button))
                {
                    if(xSemaphoreTake(xSemaphore_button, portMAX_DELAY))
                    {
                        if(xQueueSend(xQueue_button, &press, portMAX_DELAY))
                        {
                            xSemaphoreGive(xSemaphore_button);

                        }
                    }
                }
                vTaskDelay(20);
            }
            else
            {
                press = 0x02;
                if(uxQueueSpacesAvailable(xQueue_button))
                {
                    if(xSemaphoreTake(xSemaphore_button, portMAX_DELAY))
                    {
                        if(xQueueSend(xQueue_button, &press, portMAX_DELAY))
                        {
                            xSemaphoreGive(xSemaphore_button);

                        }
                    }
                }
            }
            }
        }
    }
