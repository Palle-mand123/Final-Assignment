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
    static int previousState = 1;
    int currentState;

    currentState = (GPIO_PORTF_DATA_R & 0x10) ? 1 : 0;

    if (previousState == 0 && currentState == 1) {
        previousState = currentState;
        return 0;
    } else {
        previousState = currentState;
        return 1;
    }
}

int check_second_switch()
{
    static int previousState = 1;
    int currentState;

    currentState = (GPIO_PORTF_DATA_R & 0x01) ? 1 : 0;

    if (previousState == 0 && currentState == 1) {
        previousState = currentState;
        return 0;
    } else {
        previousState = currentState;
        return 1;
    }
}

