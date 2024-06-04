#include <Arduino.h>

#include "rtc_wrapper.h"
#include "config.h"

#include <RtcDS1302.h>

static ThreeWire myWire(RTC_DAT_PIN, RTC_CLK_PIN, RTC_RST_PIN);
static RtcDS1302<ThreeWire> Rtc(myWire);

void clock_init()
{
    Rtc.Begin();
    if (!Rtc.GetIsRunning())
    {
        Rtc.SetIsRunning(true);
    }
}

void set_date(const byte day, const byte month, const int year)
{
    RtcDateTime current = Rtc.GetDateTime();
    RtcDateTime newDateTime(year, month, day, current.Hour(), current.Minute(), current.Second());
    Rtc.SetDateTime(newDateTime);
}

void set_time(const byte hours, const byte minutes, const byte seconds)
{
    RtcDateTime current = Rtc.GetDateTime();
    RtcDateTime newDateTime(current.Year(), current.Month(), current.Day(), hours, minutes, seconds);
    Rtc.SetDateTime(newDateTime);
}

void set_datetime(const byte day, const byte month, const int year, const byte hours, const byte minutes, const byte seconds)
{
    RtcDateTime newDateTime(year, month, day, hours, minutes, seconds);
    Rtc.SetDateTime(newDateTime);
}

byte get_day()
{
    return Rtc.GetDateTime().Day();
}

byte get_month()
{
    return Rtc.GetDateTime().Month();
}

int get_year()
{
    return Rtc.GetDateTime().Year();
}

byte get_hours()
{
    return Rtc.GetDateTime().Hour();
}

byte get_minutes()
{
    return Rtc.GetDateTime().Minute();
}

byte get_seconds()
{
    return Rtc.GetDateTime().Second();
}

dt now()
{
    RtcDateTime current = Rtc.GetDateTime();
    dt currentDateTime;
    currentDateTime.day = current.Day();
    currentDateTime.month = current.Month();
    currentDateTime.year = current.Year();
    currentDateTime.hours = current.Hour();
    currentDateTime.minutes = current.Minute();
    currentDateTime.seconds = current.Second();
    return currentDateTime;
}
