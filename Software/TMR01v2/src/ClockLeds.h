#ifndef ClockLeds_h
#define ClockLeds_h

#include "Arduino.h"

class ClockLeds
{
public:
    ClockLeds(int *ledPins_);

    int DisplayTimeLed(int timeToDisplay, int enable = 1);
    int DisplayTimeLedBlinking(int timeToDisplay, int timeDelayMs);

    void PowerOnAllLeds();
    void PowerOffAllLeds();
    void SequenceLeds(int timeDelayMs = 40);

private:
    int *ledPins;

};

#endif
