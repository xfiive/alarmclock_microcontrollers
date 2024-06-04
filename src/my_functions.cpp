#include <Arduino.h>
#include <EEPROM.h>

#include "rtc_wrapper.h"
#include "sensors.h"
#include "lcd_wrapper.h"
#include "config.h"

#include "my_functions.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define LIM_VALUE(a, b) a = MIN(a, b)

#define ADDR_HOUR 0
#define ADDR_MINUTE 1
#define ADDR_ENABLE 2

extern volatile byte btn_state;

static byte alarm_hour;
static byte alarm_minute;
static bool alarm_enable;

void load_alarm()
{
    alarm_hour = EEPROM.read(ADDR_HOUR);
    alarm_minute = EEPROM.read(ADDR_MINUTE);
    alarm_enable = EEPROM.read(ADDR_ENABLE) > 0;

    LIM_VALUE(alarm_hour, 23);
    LIM_VALUE(alarm_minute, 59);
}

void save_alarm()
{
    EEPROM.write(ADDR_HOUR, alarm_hour);
    EEPROM.write(ADDR_MINUTE, alarm_minute);
    EEPROM.write(ADDR_ENABLE, alarm_enable);
}

void set_alarm_hour(byte hour)
{
    LIM_VALUE(hour, 23);

    alarm_hour = hour;
}

byte get_alarm_hour()
{
    return alarm_hour;
}

void set_alarm_minute(byte minute)
{
    LIM_VALUE(minute, 59);

    alarm_minute = minute;
}

byte get_alarm_minute()
{
    return alarm_minute;
}

void set_alarm_enable(bool enable)
{
    alarm_enable = enable;
}

bool get_alarm_enable()
{
    return alarm_enable;
}

void lock_for_btn(byte button)
{
    while (btn_state & button)
    {
    }
}

void lock_until_any_btn()
{
    byte old_state = btn_state;

    while (btn_state == old_state)
    {
    }
}

void show_time()
{
    char row[17] = {0};

    snprintf(row, sizeof(row), TIME_FORMAT,
             get_hours(), get_minutes(), get_seconds());

    lcd_print_at(0, 4, row);
}

void show_date()
{
    char row[17] = {0};

    snprintf(row, sizeof(row), DATE_FORMAT,
             get_day(), get_month(), get_year());

    lcd_print_at(1, 3, row);
}

void show_env()
{
    char row[17] = {0};

    snprintf(row, sizeof(row), "T: %2.1fC H: %2d%", get_temperature(), get_humidity());

    lcd_print_at(1, 0, row);
}

void edit_date()
{
    byte day = get_day();
    byte month = get_month();
    int year = get_year();

    enum Mode
    {
        SWITCH,
        EDIT
    };
    Mode currentMode = SWITCH;

    enum DatePart
    {
        DAY,
        MONTH,
        YEAR
    };
    DatePart currentPart = DAY;

    while (!(btn_state & THE_BTN_EXIT))
    {
        char dateStr[17] = {0};
        snprintf(dateStr, sizeof(dateStr), DATE_FORMAT, day, month, year);

        lcd_clear();

        lcd_print_at(0, 3, dateStr);

        char *str_mark = (currentMode == SWITCH) ? STR_SYMB_SWITCH : STR_SYMB_EDIT;

        if (currentPart == DAY)
        {
            lcd_print_at(1, 3, str_mark);
        }
        else if (currentPart == MONTH)
        {
            lcd_set_cursor(1, 3);
            lcd_print_at(1, 6, str_mark);
        }
        else if (currentPart == YEAR)
        {
            lcd_set_cursor(1, 6);
            lcd_print_at(1, 9, str_mark);
            lcd_print(str_mark);
        }

        lock_until_any_btn();

        if (btn_state & THE_BTN_SWITCH)
        {
            currentMode = (currentMode == SWITCH) ? EDIT : SWITCH;

            lock_for_btn(THE_BTN_SWITCH);
        }

        if (currentMode == SWITCH)
        {
            if (btn_state & THE_BTN_DOWN)
            {
                currentPart = (currentPart == DAY) ? YEAR : (currentPart - 1);

                lock_for_btn(THE_BTN_DOWN);
            }
            if (btn_state & THE_BTN_UP)
            {
                currentPart = (currentPart == YEAR) ? DAY : (currentPart + 1);

                lock_for_btn(THE_BTN_UP);
            }
        }
        else if (currentMode == EDIT)
        {
            if (btn_state & THE_BTN_DOWN)
            {
                switch (currentPart)
                {
                case DAY:
                    day = (day > 1) ? day - 1 : 31;
                    break;
                case MONTH:
                    month = (month > 1) ? month - 1 : 12;
                    break;
                case YEAR:
                    year = (year > 2000) ? year - 1 : 2099;
                    break;
                }

                lock_for_btn(THE_BTN_DOWN);
            }
            if (btn_state & THE_BTN_UP)
            {
                switch (currentPart)
                {
                case DAY:
                    day = (day < 31) ? day + 1 : 1;
                    break;
                case MONTH:
                    month = (month < 12) ? month + 1 : 1;
                    break;
                case YEAR:
                    year = (year < 2099) ? year + 1 : 2000;
                    break;
                }

                lock_for_btn(THE_BTN_UP);
            }
        }
    }

    set_date(day, month, year);

    lock_for_btn(THE_BTN_EXIT);

    lcd_clear();
}

void edit_time()
{
    byte hours = get_hours();
    byte minutes = get_minutes();
    // byte seconds = get_seconds();

    enum Mode
    {
        SWITCH,
        EDIT
    };
    Mode currentMode = SWITCH;

    enum TimePart
    {
        HOURS,
        MINUTES /*, SECONDS*/
    };
    TimePart currentPart = HOURS;

    while (!(btn_state & THE_BTN_EXIT))
    {
        char timeStr[17];
        snprintf(timeStr, sizeof(timeStr), TIME_FORMAT, hours, minutes, 0 /*, seconds*/);

        lcd_clear();

        lcd_print_at(0, 4, timeStr);

        char *str_mark = (currentMode == SWITCH) ? STR_SYMB_SWITCH : STR_SYMB_EDIT;

        if (currentPart == HOURS)
        {
            lcd_print_at(1, 4, str_mark);
        }
        else if (currentPart == MINUTES)
        {
            lcd_print_at(1, 7, str_mark);
        }
        // else if (currentPart == SECONDS) {
        //     lcd_print_at(1,10,str_mark);
        // }

        lock_until_any_btn();

        if (btn_state & THE_BTN_SWITCH)
        {
            currentMode = (currentMode == SWITCH) ? EDIT : SWITCH;
            lock_until_any_btn();
        }

        if (currentMode == SWITCH)
        {
            if (btn_state & THE_BTN_DOWN)
            {
                currentPart = (currentPart == HOURS) ? MINUTES : (currentPart - 1);
                lock_until_any_btn();
            }
            if (btn_state & THE_BTN_UP)
            {
                currentPart = (currentPart == MINUTES) ? HOURS : (currentPart + 1);
                lock_until_any_btn();
            }
        }
        else if (currentMode == EDIT)
        {
            if (btn_state & THE_BTN_DOWN)
            {
                switch (currentPart)
                {
                case HOURS:
                    hours = (hours > 0) ? hours - 1 : 23;
                    break;
                case MINUTES:
                    minutes = (minutes > 0) ? minutes - 1 : 59;
                    break;
                    // case SECONDS: seconds = (seconds > 0) ? seconds - 1 : 59; break;
                }
                lock_until_any_btn();
            }
            if (btn_state & THE_BTN_UP)
            {
                switch (currentPart)
                {
                case HOURS:
                    hours = (hours < 23) ? hours + 1 : 0;
                    break;
                case MINUTES:
                    minutes = (minutes < 59) ? minutes + 1 : 0;
                    break;
                    // case SECONDS: seconds = (seconds < 59) ? seconds + 1 : 0; break;
                }
                lock_until_any_btn();
            }
        }
    }

    set_time(hours, minutes, get_seconds());

    lock_for_btn(THE_BTN_EXIT);

    lcd_clear();
}

bool ask_edit_alarm()
{
    bool result = false;

    lcd_clear();
    lcd_print_at(0, 0, "Enable Alarm?");

    lcd_print_at(1, 0, (alarm_enable ? "No " : "Yes"));

    lcd_print(":1 Cls:2 Ed:3");

    lock_until_any_btn();

    if (btn_state & BTN2_PRESSED)
    {
        result = true;
    }
    else if (btn_state & BTN4_PRESSED)
    {
        alarm_enable = !alarm_enable;

        save_alarm();
    }

    lock_until_any_btn();

    lcd_clear();

    return result;
}

void edit_alarm()
{
    byte hours = alarm_hour;
    byte minutes = alarm_minute;

    enum Mode
    {
        SWITCH,
        EDIT
    };
    Mode currentMode = SWITCH;

    enum TimePart
    {
        HOURS,
        MINUTES
    };
    TimePart currentPart = HOURS;

    while (!(btn_state & THE_BTN_EXIT))
    {
        char timeStr[17];
        snprintf(timeStr, sizeof(timeStr), TIME_FORMAT, hours, minutes, 0);

        lcd_clear();

        lcd_print_at(0, 4, timeStr);

        char *str_mark = (currentMode == SWITCH) ? STR_SYMB_SWITCH : STR_SYMB_EDIT;

        if (currentPart == HOURS)
        {
            lcd_print_at(1, 4, str_mark);
        }
        else if (currentPart == MINUTES)
        {
            lcd_print_at(1, 7, str_mark);
        }

        lock_until_any_btn();

        if (btn_state & THE_BTN_SWITCH)
        {
            currentMode = (currentMode == SWITCH) ? EDIT : SWITCH;
            lock_until_any_btn();
        }

        if (currentMode == SWITCH)
        {
            if (btn_state & THE_BTN_DOWN)
            {
                currentPart = (currentPart == HOURS) ? MINUTES : (currentPart - 1);
                lock_until_any_btn();
            }
            if (btn_state & THE_BTN_UP)
            {
                currentPart = (currentPart == MINUTES) ? HOURS : (currentPart + 1);
                lock_until_any_btn();
            }
        }
        else if (currentMode == EDIT)
        {
            if (btn_state & THE_BTN_DOWN)
            {
                switch (currentPart)
                {
                case HOURS:
                    hours = (hours > 0) ? hours - 1 : 23;
                    break;
                case MINUTES:
                    minutes = (minutes > 0) ? minutes - 1 : 59;
                    break;
                }
                lock_until_any_btn();
            }
            if (btn_state & THE_BTN_UP)
            {
                switch (currentPart)
                {
                case HOURS:
                    hours = (hours < 23) ? hours + 1 : 0;
                    break;
                case MINUTES:
                    minutes = (minutes < 59) ? minutes + 1 : 0;
                    break;
                }
                lock_until_any_btn();
            }
        }
    }

    alarm_minute = minutes;
    alarm_hour = hours;

    save_alarm();

    lock_for_btn(THE_BTN_EXIT);

    lcd_clear();
}

bool is_time_to_alarm()
{
    Serial.print(get_hours());
    Serial.print(":");
    Serial.print(get_minutes());
    Serial.print(" -> ");
    Serial.print(alarm_hour);
    Serial.print(":");
    Serial.print(alarm_minute);
    Serial.print(" -> ");
    Serial.print(alarm_enable);
    Serial.print(" -> ");
    Serial.println(alarm_enable && get_hours() == alarm_hour && get_minutes() == alarm_minute);

    return alarm_enable && get_hours() == alarm_hour && get_minutes() == alarm_minute;
}