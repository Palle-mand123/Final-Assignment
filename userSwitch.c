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


int check_first_switch()
{
    int K;
    // Read the current state of pin PF4
    if ((0b00010000) & (GPIO_PORTF_DATA_R))
    {
        K = 1;
        return K;

    }
    else
    {
        K = 0;
        return K;
    }
}

