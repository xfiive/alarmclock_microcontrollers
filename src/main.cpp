#include <Arduino.h>
#include <TimerOne.h>

#include "rtc_wrapper.h"
#include "sensors.h"
#include "lcd_wrapper.h"
#include "states.h"
#include "config.h"
#include "my_functions.h"

/**
 * Holds combination of pressed buttons.
 */
volatile byte btn_state;

/**
 * ISR for button press.
 *
 * This function is executed, when button is pressed. The global variable
 * `btn_state` is then set according to the combination of pressed buttons.
 */
void on_button_press()
{
    btn_state = 0;

    if (digitalRead(BTN1_PIN) == HIGH)
    {
        btn_state |= BTN1_PRESSED;
    }

    if (digitalRead(BTN2_PIN) == HIGH)
    {
        btn_state |= BTN2_PRESSED;
    }

    if (digitalRead(BTN3_PIN) == HIGH)
    {
        btn_state |= BTN3_PRESSED;
    }

    if (digitalRead(BTN4_PIN) == HIGH)
    {
        btn_state |= BTN4_PRESSED;
    }
}

void setup()
{
    // needed from original main() function
    init();

    Serial.begin(9600);
    // while(!Serial.available()){
    //     delay(100);
    // }

    pinMode(BTN1_PIN, INPUT);
    pinMode(BTN2_PIN, INPUT);
    pinMode(BTN3_PIN, INPUT);
    pinMode(BTN4_PIN, INPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    // attachInterrupt(digitalPinToInterrupt(2), on_button_press, RISING);

    // TimerOne
    Timer1.initialize(200000); // 200 ms
    Timer1.attachInterrupt(on_button_press);

    // init alarm clock modules
    lcd_init();
    clock_init();
    sensors_init();

    // load alarm from MEMORY 😘😘😘
    load_alarm();
}

int main()
{
    setup();

    // init
    enum state state = CLOCK;

    // main loop
    for (;;)
    {
        // in loop()
        switch (state)
        {
        case ALARM:
            state = state_alarm();
            break;

        case CLOCK:
            state = state_clock();
            break;

        case SHOW_ENV:
            state = state_show_env();
            break;

        case FACTORY_RESET:
            state = state_factory_reset();
            break;

        case SHOW_DATE:
            state = state_show_date();
            break;

        case EDIT_DATE:
            state = state_edit_date();
            break;

        case EDIT_TIME:
            state = state_edit_time();
            break;

        case ASK_ENABLE_ALARM:
            state = state_ask_enable_alarm();
            break;

        case EDIT_ALARM:
            state = state_edit_alarm();
            break;

        default:
            state = CLOCK;
            break;
        }
    }
}
