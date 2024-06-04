/**
 * List of available states.
 */
enum state
{
    ALARM,
    CLOCK,
    FACTORY_RESET,
    SHOW_DATE,
    SHOW_ENV,
    EDIT_DATE,
    EDIT_TIME,
    EDIT_ALARM,
    ASK_ENABLE_ALARM
};

/**
 * Individual implementation of states.
 */

enum state state_clock();
enum state state_factory_reset();
enum state state_alarm();
enum state state_show_date();
enum state state_show_env();
enum state state_edit_date();
enum state state_edit_time();
enum state state_ask_enable_alarm();
enum state state_edit_alarm();