#include <Arduino.h>

#include "lcd_wrapper.h"
#include "config.h"

#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_init()
{
    lcd.init();      // Initialize the LCD
    lcd.backlight(); // Turn on the backlight by default
}

void lcd_clear()
{
    lcd.clear(); // Clear the display
}

void lcd_set_cursor(int y, int x)
{
    lcd.setCursor(x, y); // Set the cursor to specified position (column, row)
}

void lcd_print(char *text)
{
    lcd.print(text); // Print text at the current cursor position
}

void lcd_print_at(int y, int x, char *text)
{
    lcd.setCursor(x, y); // Set the cursor to specified position (column, row)
    lcd.print(text);     // Print text at the specified position
}

void lcd_backlight(bool state)
{
    if (state)
    {
        lcd.backlight(); // Turn on the backlight
    }
    else
    {
        lcd.noBacklight(); // Turn off the backlight
    }
}