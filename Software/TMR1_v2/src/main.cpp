#include <Arduino.h>

#include "MCP7940.h"
#include "LowPower.h"

// Constant
const int buttonPin = 2;    // Button Digital Pin
const uint8_t  SPRINTF_BUFFER_SIZE =     32;                                  // Buffer size for sprintf()        //

const int serial_active = 1;


// Variables
// int buttonState = 0;         // variable for reading the pushbutton status
MCP7940_Class MCP7940;                                                        // Create an instance of the MCP7940//
char          inputBuffer[SPRINTF_BUFFER_SIZE];                               // Buffer for sprintf()/sscanf()    //

static uint8_t secs;

// Function wakeup
void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    // pinMode(buttonPin, INPUT);

    if(serial_active == 1)
      Serial.begin(9600);                                                 // Start serial port at Baud rate   //

    // Serial.print(F("\nStarting SetAndCalibrate program\n"));                    // Show program information         //
    // Serial.print(F("- Compiled with c++ version "));                            //                                  //
    // Serial.print(F(__VERSION__));                                               // Show compiler information        //
    // Serial.print(F("\n- On "));                                                 //                                  //
    // Serial.print(F(__DATE__));                                                  //                                  //
    // Serial.print(F(" at "));                                                    //                                  //
    // Serial.print(F(__TIME__));                                                  //                                  //
    // Serial.print(F("\n"));                                                      //                                  //
    while (!MCP7940.begin()) {                                                  // Initialize RTC communications    //
      //Serial.println(F("Unable to find MCP7940M. Checking again in 3s."));      // Show error text                  //
      delay(3000);                                                              // wait a second                    //
    } // of loop until device is located
    if(serial_active == 1)
      Serial.println(F("MCP7940 initialized."));                                  //                                  //
    while (!MCP7940.deviceStatus()) {
      if(serial_active == 1)
        Serial.println(F("Oscillator is off, turning it on."));                   //                                  //
      bool deviceStatus = MCP7940.deviceStart();                                // Start oscillator and return state//
      if (!deviceStatus) {
        if(serial_active == 1)
          Serial.println(F("Oscillator did not start, trying again."));           // Show error and                   //
        delay(1000);                                                            // wait for a second                //
      } // of if-then oscillator didn't start                                   //                                  //
    } // of while the oscillator is off                                         //                                  //
    MCP7940.adjust();                                                           // Set to library compile Date/Time //

    MCP7940.adjust(DateTime(2018,11,6,22,49,15));

    //Serial.println(F("Enter the following serial commands:"));                  //                                  //
    //Serial.println(F("SETDATE yyyy-mm-dd hh:mm:ss"));                           //                                  //
    //Serial.println(F("CALDATE yyyy-mm-dd hh:mm:ss"));                           //                                  //
    pinMode(LED_BUILTIN,OUTPUT);                                                    // Declare built-in LED as output   //

    delay(2000);
}                                               //                                  //

void loop()
{
    // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    // delay(1000);                       // wait for a second
    // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    // delay(1000);                       // wait for a second

    // // read the state of the pushbutton value:
    // buttonState = digitalRead(buttonPin);
    //
    // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    // if (buttonState == HIGH)
    // {
    //   // turn LED on:
    //   digitalWrite(LED_BUILTIN, HIGH);
    // }
    // else
    // {
    //   // turn LED off:
    //   digitalWrite(LED_BUILTIN, LOW);
    // }

    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, HIGH);


    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0);

    // Do something here
//    // Example: Read sensor, data logging, data transmission.
//     digitalWrite(LED_BUILTIN, HIGH);
//     delay(3000);
//     digitalWrite(LED_BUILTIN, LOW);
//     delay(3000);

                                              // store the seconds value          //
    DateTime now = MCP7940.now();                                               // get the current time             //
    if (secs != now.second()) {                                                 // Output if seconds have changed   //
      sprintf(inputBuffer,"%04d-%02d-%02d %02d:%02d:%02d", now.year(),          // Use sprintf() to pretty print    //
              now.month(), now.day(), now.hour(), now.minute(), now.second());  // date/time with leading zeros     //
      if(serial_active == 1)
        Serial.println(inputBuffer);                                              // Display the current date/time    //
      secs = now.second();                                                      // Set the counter variable         //
      for(int i=0; i<3; i++)
      {
        digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));                              // Toggle the LED                   //
        delay(500);
      }
      digitalWrite(LED_BUILTIN, LOW);

    } // of if the seconds have changed                                         //                                  //
    delay(2000);

}
