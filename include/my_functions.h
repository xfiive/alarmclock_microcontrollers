#ifndef _MY_FUNK
#define _MY_FUNK

#include "config.h"

#define DATE_FORMAT "%02d/%02d/%04d"
#define TIME_FORMAT "%02d:%02d:%02d"

#define STR_SYMB_SWITCH "##"
#define STR_SYMB_EDIT "^^"

#define THE_BTN_EXIT /*      */ BTN4_PRESSED
#define THE_BTN_SWITCH /*    */ BTN3_PRESSED
#define THE_BTN_UP /*        */ BTN2_PRESSED
#define THE_BTN_DOWN /*      */ BTN1_PRESSED

void show_time();

void show_date();

void show_env();

void lock_for_btn(byte button);

void lock_until_any_btn();

void edit_date();

void edit_time();

bool ask_edit_alarm();

void edit_alarm();

void load_alarm();

void save_alarm();

void set_alarm_hour(byte hour);

byte get_alarm_hour();

void set_alarm_minute(byte minute);

byte get_alarm_minute();

void set_alarm_enable(bool enable);

bool get_alarm_enable();

bool is_time_to_alarm();

#endif