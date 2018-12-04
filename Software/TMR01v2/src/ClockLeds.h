#ifndef ClockLeds_h
#define ClockLeds_h

#include "Arduino.h"

class ClockLeds
{
public:
    ClockLeds(int *ledPins_);

    int DisplayHourLed(int timeToDisplay, int enablepin = 1);
    int DisplayHourBlinking(int timeToDisplay, int timeDelayMs);

    int DisplayMinutesLed(int timeToDisplay, int enablepin = 1);
    int DisplayMinutesBlinking(int timeToDisplay, int timeDelayMs);

    void PowerOnAllLeds();
    void PowerOffAllLeds();
    void SequenceLeds(int timeDelayMs = 40);

private:
    int *ledPins;

};

#endif
