// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <Wire.h> 
// Liquid Crystal Library can be found here: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <LiquidCrystal_I2C.h>
#include <String.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Set a date of the new year, in this case Jan 1st 2022 at 12:00 AM
DateTime newYearDate = DateTime(2022, 1, 1, 0, 0, 0);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;        // Will store last time LCD was updated

// constants won't change:
const long updateInterval = 1000;           // interval at which to update LCD (milliseconds)

// Change from displaying the date and time to displaying the countdown every five seconds:
const int updatesPerCycle = 5;
int updatesThisCycle = 0;
bool printCurrentDate = true;

void setup () {
    Serial.begin(9600);

    // Check if the RTC is connected
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }
    // Initialize the LCD, print the start message
    lcd.begin();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Countdown to the");
    lcd.setCursor(0, 1);
    lcd.print("    New Year    ");
}

void loop () {
    // Update currentMillis
    unsigned long currentMillis = millis();

    // Retrieve the current time
    DateTime now = rtc.now();

    // Check if updateInterval has elapsed since previousMillis
    if (currentMillis - previousMillis >= updateInterval) {
        // save the last time you updated the LCD
        previousMillis = currentMillis;

        // Handle switching from displaying the date and time to displaying the countdown:
        if (updatesThisCycle < updatesPerCycle) {
            updatesThisCycle++;
        }
        else {
            printCurrentDate = !printCurrentDate;
            updatesThisCycle = 0;
        }

        if (printCurrentDate) {
            // Print current time
            Serial.print(now.year(), DEC);
            Serial.print('/');
            Serial.print(now.month(), DEC);
            Serial.print('/');
            Serial.print(now.day(), DEC);
            Serial.print(" (");
            Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
            Serial.print(") ");
            Serial.print(now.hour(), DEC);
            Serial.print(':');
            Serial.print(now.minute(), DEC);
            Serial.print(':');
            Serial.print(now.second(), DEC);
            Serial.println();
    
            String dateStr = "Date: " + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
            String timeStr = "Time:   " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
            lcd.setCursor(0, 0);
            lcd.print(dateStr);
            lcd.setCursor(0, 1);
            lcd.print(timeStr);
    
            // Print temperature
            Serial.print("Temperature: ");
            Serial.print(rtc.getTemperature());
            Serial.println(" C");
        }
        else {
            // Print remainingTime to New Year
            TimeSpan remainingTime = newYearDate - now;
            // TODO test whether this handles dates in the past (negative TimeIntervals)  
            if (remainingTime.totalseconds() <= 0) {
                Serial.println("Happy New Year!");
                Serial.println();
                // Trigger your fancy output here
            }
            else {
                Serial.print("Time until New Year's: ");
                // Years and months are unused in this case
                //    Serial.print(remainingTime.year(), DEC);
                //    Serial.print('/');
                //    Serial.print(remainingTime.month(), DEC);
                //    Serial.print('/');
                Serial.print(remainingTime.days(), DEC);
                Serial.print("d:");
                Serial.print(remainingTime.hours(), DEC);
                Serial.print("h:");
                Serial.print(remainingTime.minutes(), DEC);
                Serial.print("m:");
                Serial.print(remainingTime.seconds(), DEC);
                Serial.println("s");
                Serial.println();
                
                String timeStr = String(remainingTime.days()) + "d:" + String(remainingTime.hours()) + "h:" + String(remainingTime.minutes()) + "m:" + String(remainingTime.seconds()) + "s";
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("New Year Starts:");
                lcd.setCursor(0, 1);
                lcd.print(timeStr);
            }
        }
    }
}
