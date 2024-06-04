#include <Arduino.h>
#include "rtc_wrapper.h"
#include "sensors.h"
#include "lcd_wrapper.h"
#include "config.h"

#include "states.h"
#include "my_functions.h"

extern volatile byte btn_state;

enum state state_clock()
{
    static unsigned long show_info_timer = 0;
    static unsigned long show_time_timer = 0;
    static bool switch_flag = false;

    unsigned long current_time = millis();
    enum state return_state = CLOCK;

    if (current_time - show_info_timer > SENSORS_READ_INTERVAL)
    {
        lcd_clear();

        return_state = switch_flag ? SHOW_DATE : SHOW_ENV;

        switch_flag = !switch_flag;
        show_info_timer = current_time;

        show_time();
    }

    if (current_time - show_time_timer > 1000)
    {
        show_time();

        show_time_timer = current_time;

        if (is_time_to_alarm())
        {
            return ALARM;
        }
    }

    if (btn_state & BTN4_PRESSED)
    {
        return_state = ASK_ENABLE_ALARM;

        lock_until_any_btn();
    }
    else if (btn_state & BTN3_PRESSED)
    {
        return_state = EDIT_DATE;

        lock_until_any_btn();
    }
    else if (btn_state & BTN2_PRESSED)
    {
        return_state = EDIT_TIME;

        lock_until_any_btn();
    }

    return return_state;
}
enum state state_factory_reset()
{
    return ALARM;
}
enum state state_alarm()
{
    lcd_clear();

    lcd_print("ALARM!!!");

    digitalWrite(BUZZER_PIN, HIGH);

    lock_until_any_btn();
    lock_until_any_btn();

    digitalWrite(BUZZER_PIN, LOW);

    set_alarm_enable(false);

lcd_clear();

    return CLOCK;
}
enum state state_show_date()
{
    show_date();

    return CLOCK;
}
enum state state_show_env()
{
    show_env();

    return CLOCK;
}

enum state state_edit_date()
{
    edit_date();

    return CLOCK;
}

enum state state_edit_time()
{
    edit_time();

    return CLOCK;
}

enum state state_ask_enable_alarm()
{
    if (ask_edit_alarm())
    {
        return EDIT_ALARM;
    }

    return CLOCK;
}

enum state state_edit_alarm()
{
    edit_alarm();

    return CLOCK;
}
