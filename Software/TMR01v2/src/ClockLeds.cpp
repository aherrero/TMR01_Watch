#include "ClockLeds.h"

ClockLeds::ClockLeds(int *ledPins_)
{
    ledPins = ledPins_;
}

// Timing
int ClockLeds::DisplayHourLed(int timeToDisplay, int enable)
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

int ClockLeds::DisplayMinutesLed(int timeToDisplay, int enable)
{
    // int ALL_LEDS_PIN[] = {LED12_PIN, LED1_PIN, LED2_PIN,
    //                         LED3_PIN, LED4_PIN, LED5_PIN,
    //                         LED6_PIN, LED7_PIN, LED8_PIN,
    //                         LED9_PIN, LED10_PIN, LED11_PIN};

    if(timeToDisplay >= 0 && timeToDisplay < 5)
    {
        digitalWrite(ledPins[0], enable);
    }
    else if(timeToDisplay >= 5 && timeToDisplay < 10)
    {
        digitalWrite(ledPins[1], enable);
    }
    else if(timeToDisplay >= 10 && timeToDisplay < 15)
    {
        digitalWrite(ledPins[2], enable);
    }
    else if(timeToDisplay >= 15 && timeToDisplay < 20)
    {
        digitalWrite(ledPins[3], enable);
    }
    else if(timeToDisplay >= 20 && timeToDisplay < 25)
    {
        digitalWrite(ledPins[4], enable);
    }
    else if(timeToDisplay >= 25 && timeToDisplay < 30)
    {
        digitalWrite(ledPins[5], enable);
    }
    else if(timeToDisplay >= 30 && timeToDisplay < 35)
    {
        digitalWrite(ledPins[6], enable);
    }
    else if(timeToDisplay >= 35 && timeToDisplay < 40)
    {
        digitalWrite(ledPins[7], enable);
    }
    else if(timeToDisplay >= 40 && timeToDisplay < 45)
    {
        digitalWrite(ledPins[8], enable);
    }
    else if(timeToDisplay >= 45 && timeToDisplay < 50)
    {
        digitalWrite(ledPins[9], enable);
    }
    else if(timeToDisplay >= 50 && timeToDisplay < 55)
    {
        digitalWrite(ledPins[10], enable);
    }
    else if(timeToDisplay >= 55 && timeToDisplay < 60)
    {
        digitalWrite(ledPins[11], enable);
    }
    else
    {
        return -1;
    }

    return 0;
}

int ClockLeds::DisplayHourBlinking(int timeToDisplay, int timeDelayMs)
{
    int err = 0;

    err += DisplayHourLed(timeToDisplay, 1);    // Display led ON
    delay(timeDelayMs);                                // Delay blinking on
    err += DisplayHourLed(timeToDisplay, 0);    // Turn OFF THAT led (not ALL leds)
    delay(timeDelayMs);                                // Delay blinking off

    return err;
}

int ClockLeds::DisplayMinutesBlinking(int timeToDisplay, int timeDelayMs)
{
    int err = 0;

    err += DisplayMinutesLed(timeToDisplay, 1);    // Display led ON
    delay(timeDelayMs);                                // Delay blinking on
    err += DisplayMinutesLed(timeToDisplay, 0);    // Turn OFF THAT led (not ALL leds)
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
