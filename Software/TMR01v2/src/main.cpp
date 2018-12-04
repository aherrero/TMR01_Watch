#include <Arduino.h>

// External libraries
#include "MCP7940.h"
#include "LowPower.h"

// Internal Libraries
#include "ClockLeds.h"

// Constant
const int BUTTON_PIN = 2;    // Button For interruption (PD2, INT0, atmega 32)

const int LED12_PIN = A3;   //PC3, atmega 26
const int LED1_PIN  = A2;   //PC2, atmega 25
const int LED2_PIN  = A1;   //PC1, atmega 24
const int LED3_PIN  = A0;   //PC0, atmega 23

const int LED4_PIN  = 13;   // Originally to A7 (D4_NA). But change to SCK -> PB5 (Arduino13). Atmega 17
const int LED5_PIN  = 12;   // Originally to A6 (D5_NA). But change to MISO -> PB4. Atmega 16

const int LED6_PIN  = 9;    //PB1, atmega 13

const int LED7_PIN  = 7;    //PD7, atmega 11
const int LED8_PIN  = 6;    //PD6, atmega 10
const int LED9_PIN  = 5;    //PD5, atmega 9
const int LED10_PIN = 4;    //PD4, atmega 2
const int LED11_PIN = 3;    //PD3, atmega 1

int ALL_LEDS_PIN[] = {LED12_PIN, LED1_PIN, LED2_PIN,
                        LED3_PIN, LED4_PIN, LED5_PIN,
                        LED6_PIN, LED7_PIN, LED8_PIN,
                        LED9_PIN, LED10_PIN, LED11_PIN};

const int WATCH_MODE_TIME = 0;
const int WATCH_MODE_SET_HOUR = 1;
const int WATCH_MODE_SET_MIN = 2;

// Variables
MCP7940_Class MCP7940;
ClockLeds clockLeds(ALL_LEDS_PIN);

int BORA_WATCH_MODE = WATCH_MODE_SET_HOUR;

DateTime time_now;

// Button detection long press
long longPressTime = 2000;

volatile int buttonPressed = 0;
unsigned long startMillis = 0;
unsigned long currentsMillis = 0;

const int SHORT_PRESSED = 1;
const int LONG_PRESSED = 2;

/*
* Return 1 if short pressed (Default < 1s)
* Return 2 if long pressed (Default > 1s) but still pressed
* Return 0 on other case
*/
int calculateLongShortButton()
{
    int resultLongPress = 0;
    unsigned long diffMillis = currentsMillis - startMillis;

    if(buttonPressed == 1 && diffMillis < longPressTime)
    {
        // Starting acum
        currentsMillis = millis();
    }
    else
    {
        if(diffMillis > 10)
        {
            if(diffMillis > longPressTime)
            {
                resultLongPress = LONG_PRESSED;
            }
            else
            {
                resultLongPress = SHORT_PRESSED;
            }
        }

        startMillis = millis();
        currentsMillis = millis();
    }

    return resultLongPress;
}


// callback
void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void pin_ISR()
{
    if(digitalRead(BUTTON_PIN) == HIGH)
        buttonPressed = 1;
    else
        buttonPressed = 0;
}

// Setup

void setup()
{
    // Leds output
    for(int i=0; i< sizeof(ALL_LEDS_PIN);i++)
        pinMode(ALL_LEDS_PIN[i], OUTPUT);

    // Button input
    pinMode(BUTTON_PIN, INPUT);

    // RTC
    while (!MCP7940.begin())
    {
        // Unable to find MCP7940M. Checking again in 3s.
        delay(3000);
    }

    // MCP7940 initialized

    while (!MCP7940.deviceStatus())
    {
        // Oscillator is off, turning it on
        bool deviceStatus = MCP7940.deviceStart();
        if (!deviceStatus)
        {
            // Oscillator did not start, trying again.
            delay(1000);
        }
    }

    //MCP7940.adjust();   // Adjust time with the compilation time
    MCP7940.adjust(DateTime(2000,1,1,0,0,0));   //Set a specific time
    delay(1000);

    // Good configuration
    clockLeds.SequenceLeds(20);

    // Attach an interrupt to the ISR vector (long or short press)
    attachInterrupt(0, pin_ISR, CHANGE);
}

void loop()
{
    int resultLongPress = 0;

    switch (BORA_WATCH_MODE)
    {
        case WATCH_MODE_TIME:
            // Disable external pin interrupt for counting time
            detachInterrupt(0);
            delay(10);

            // Allow wake up pin to trigger interrupt on low.
            attachInterrupt(0, wakeUp, HIGH);

            // Enter power down state with ADC and BOD module disabled.
            // Wake up when wake up pin is low.
            LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

            // Disable external pin interrupt on wake up pin.
            detachInterrupt(0);
            delay(10);

            // Do something here after wake up until next loop to sleep

            // Show the time
            // get time from RTC
            time_now = MCP7940.now();
            //  now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()

            // Display
            // if hour = minutes
            // only the minutes blinking
            clockLeds.DisplayHourLed(time_now.hour());
            for(int i = 0; i < 3; i++)
            {
                // blinking 1s, 3 times
                clockLeds.DisplayHourBlinking(time_now.minute(), 500);

                // If the button continue pressed (after showing time at least once (1s after)
                if(digitalRead(BUTTON_PIN) == HIGH)
                {
                    // Set the hour
                    BORA_WATCH_MODE = WATCH_MODE_SET_HOUR;
                    clockLeds.SequenceLeds(40);

                    calculateLongShortButton(); //Restart variables once unpressed
                    // Attach an interrupt to the ISR vector
                    attachInterrupt(0, pin_ISR, CHANGE);

                    break;  //Break the loop for showing time
                }
            }
        break;

        case WATCH_MODE_SET_HOUR:
            // Clean Leds
            clockLeds.PowerOffAllLeds();
            delay(50);

            // Get the hour
            time_now = MCP7940.now();
            //Normally minutes blink each 1 sec. IN setting time, the hours blink each 500ms
            clockLeds.DisplayHourBlinking(time_now.hour(), 250);

            // Read button status and long/short press
            resultLongPress = calculateLongShortButton();
            if(resultLongPress == SHORT_PRESSED)
            {
                DateTime newtime(time_now.year(),time_now.month(),time_now.day(),
                                time_now.hour() + 1,time_now.minute(),time_now.second());

                MCP7940.adjust(newtime);   //Set a specific time
            }
            else if(resultLongPress == LONG_PRESSED)
            {
                // Set the minutes
                BORA_WATCH_MODE = WATCH_MODE_SET_MIN;
                clockLeds.SequenceLeds(40);
                delay(100);
            }

            calculateLongShortButton(); //Restart variables once unpressed

        break;

        case WATCH_MODE_SET_MIN:
            // Clean Leds
            clockLeds.PowerOffAllLeds();
            delay(50);

            // Get the hour
            time_now = MCP7940.now();
            //Normally minutes blink each 1 sec. IN setting time, the minutes blink each 500ms
            clockLeds.DisplayMinutesBlinking(time_now.minute(), 125);

            // Read button status and long/short press
            resultLongPress = calculateLongShortButton();
            if(resultLongPress == SHORT_PRESSED)
            {
                // +1 if we want to set the minutes one per once
                // +5 if we set each 5 minutes and display each 5 minutes
                DateTime newtime(time_now.year(),time_now.month(),time_now.day(),
                                time_now.hour(),time_now.minute() + 1,time_now.second());

                MCP7940.adjust(newtime);   //Set a specific time
            }
            else if(resultLongPress == LONG_PRESSED)
            {
                // Come back show time
                BORA_WATCH_MODE = WATCH_MODE_TIME;
                clockLeds.SequenceLeds(20);
                clockLeds.SequenceLeds(20);
                delay(100);
            }

            calculateLongShortButton(); //Restart variables once unpressed

        break;
    }   //end switch
}   //end loop
