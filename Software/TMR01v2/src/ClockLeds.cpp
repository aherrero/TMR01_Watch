#include "ClockLeds.h"

ClockLeds::ClockLeds(int *ledPins_)
{
    ledPins = ledPins_;
}

// Timing
int ClockLeds::DisplayTimeLed(int timeToDisplay, int enable)
{
    if(timeToDisplay >= 0 && timeToDisplay <= 11)
    {
        digitalWrite(ledPins[timeToDisplay], enable);
    }
    else if(timeToDisplay >= 12 && timeToDisplay <= 24)
    {
        // 24 hours format
        timeToDisplay = timeToDisplay - 12;
        digitalWrite(ledPins[timeToDisplay], enable);
    }
    else
    {
        return -1;
    }

    return 0;
}

int ClockLeds::DisplayTimeLedBlinking(int timeToDisplay, int timeDelayMs)
{
    int err = 0;

    err += DisplayTimeLed(timeToDisplay, 1);    // Display led ON
    delay(timeDelayMs);                                // Delay blinking on
    err += DisplayTimeLed(timeToDisplay, 0);    // Turn OFF THAT led (not ALL leds)
    delay(timeDelayMs);                                // Delay blinking off

    return err;
}

// Power OFF every led
void ClockLeds::PowerOffAllLeds()
{
    for(int i=0; i<12; i++)
    {
        digitalWrite(ledPins[i], 0);
    }
}

// Power ON every led
void ClockLeds::PowerOnAllLeds()
{
    for(int i=0; i<12; i++)
    {
        digitalWrite(ledPins[i], 1);
    }
}

// Power ON Sequence
void ClockLeds::SequenceLeds(int timeDelayMs)
{
    for(int i=0; i<12; i++)
    {
        digitalWrite(ledPins[i], 1);
        delay(timeDelayMs);
        digitalWrite(ledPins[i], 0);
    }
}
