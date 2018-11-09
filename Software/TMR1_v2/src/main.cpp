#include <Arduino.h>

#include "MCP7940.h"
#include "LowPower.h"

// Constant
const int BUTTON_PIN = 2;    // Button For interruption
const uint8_t  SPRINTF_BUFFER_SIZE = 32;
const int SERIAL_ACTIVE = 1;

// Variables
MCP7940_Class MCP7940;
char inputBuffer[SPRINTF_BUFFER_SIZE];

static uint8_t secs;

// Function wakeup
void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup()
{
    // Led output
    pinMode(LED_BUILTIN, OUTPUT);

    // Serial output
    if(SERIAL_ACTIVE == 1)
        Serial.begin(9600);

    // RTC
    while (!MCP7940.begin())
    {
        if(SERIAL_ACTIVE == 1)
            Serial.println(F("Unable to find MCP7940M. Checking again in 3s."));
        delay(3000);
    }

    if(SERIAL_ACTIVE == 1)
        Serial.println(F("MCP7940 initialized."));

    while (!MCP7940.deviceStatus())
    {
        if(SERIAL_ACTIVE == 1)
            Serial.println(F("Oscillator is off, turning it on."));

        bool deviceStatus = MCP7940.deviceStart();
        if (!deviceStatus)
        {
            if(SERIAL_ACTIVE == 1)
                Serial.println(F("Oscillator did not start, trying again."));
            delay(1000);
        }
    }

    //MCP7940.adjust();   // Adjust time with the compilation time
    MCP7940.adjust(DateTime(2018,11,7,21,39,10));   //Set a specific time

    delay(2000);
}

void loop()
{

    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, HIGH);

    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0);

    // Do something here after wake up until next loop to sleep

    DateTime now = MCP7940.now();

    if (secs != now.second())
    {
        sprintf(inputBuffer,"%04d-%02d-%02d %02d:%02d:%02d", now.year(),
            now.month(), now.day(), now.hour(), now.minute(), now.second());

        if(SERIAL_ACTIVE == 1)
            Serial.println(inputBuffer);

        secs = now.second();

        // Toogle the led when reading time
        digitalWrite(LED_BUILTIN, HIGH);
        //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
        delay(4000);
        digitalWrite(LED_BUILTIN, LOW);
    }

    delay(2000);
}
