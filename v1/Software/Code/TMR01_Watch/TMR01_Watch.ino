/*electronoobs digital watch code. Read all comments
Website: http://www.electronoobs.com/eng_arduino_tut40.php
Video: https://www.youtube.com/watch?v=nHZllsSoZp4
*/
#include <avr/wdt.h>            // library for default watchdog functions
#include <avr/interrupt.h>      // library for interrupts handling
#include <avr/sleep.h>          // library for sleep
#include <avr/power.h>          // library for power control

int analog_treshold = 400;
int mode_change_delay = 1000;
bool button_pressed = false;
unsigned int pressed_button_counter = 0;
unsigned int actual_button_counter = 0;
unsigned int push_time = 0;

unsigned int time_seconds = 0;
unsigned int time_minutes = 0;
unsigned int time_hours = 0;

unsigned int time_counter = 0;
unsigned previous_time_counter = 0;

int mode = 0;
int flash_delay = 300;
bool mode_was_changed = false;

void blinkAll(int timeBlinkMs)
{
    for(int i=2; i<=13;i++)
    {
        digitalWrite(i, HIGH);
        delay(timeBlinkMs);
        digitalWrite(i, LOW);
    }
}

void setup() {
  //Serial.begin(9600);
  
  PCICR |= B00000111;    //enable port C o have interrupts                                                 
  PCMSK1 |= B00000001;  //Set pin D11 (increase button) trigger an interrupt on state change.
  
  power_adc_disable(); // ADC converter
  power_spi_disable(); // SPI
  power_usart0_disable();// Serial (USART) 
  //power_timer0_disable();// Timer 0
  power_timer1_disable();// Timer 1
  power_timer2_disable();// Timer 2
  power_twi_disable(); // TWI (I2C)
  
  
  DDRD = B11111100;   //Set pins D2-D7 as outputs
  DDRB = B00111111;   //Set pins B0-B5 as outputs
  pinMode(A0,INPUT);

  blinkAll(25);
  blinkAll(25);

}

void loop() {
  time_counter = millis();
  //we count 1000ms for one second, than each 60 sec we have a minute
  //and each 60 minute we have one hour and reset at 12h
  if(time_counter - previous_time_counter >= 1000)
  {  
    previous_time_counter = previous_time_counter + 1000;
    time_seconds = time_seconds + 1;
    if(time_seconds > 59)
    {
      time_seconds = 0;
      time_minutes = time_minutes + 1;
    }

    if(time_minutes > 59)
    {
      time_minutes = 0;
      time_hours = time_hours + 1;
    }
    if(time_hours > 11)
    {
      time_hours = 0;
    }
  }
  //Uncomment this for debug only. Remember to uncoment Serial.begin as well above!
  
  //Serial.print("Hour: "); Serial.print(time_hours);Serial.print("    ");
  //Serial.print("Minute: "); Serial.print(time_minutes);Serial.print("    ");
  //Serial.print("Second: "); Serial.println(time_seconds);
  
  
  if(analogRead(A0) > analog_treshold  && !button_pressed )
  {
    //The button was pressed
    button_pressed = true;
    pressed_button_counter = millis();    
  }//end of analog above treshold


  if(analogRead(A0) < analog_treshold  && button_pressed)
  {
    //Button was released
    button_pressed = false;   
    
    if(mode == 0 && !mode_was_changed)
    {
      //Mode 0 is to show time
      //Mode 1 is set hour
      //mode 2 is set minute, then we go to mode 0 again
      if(time_hours == 0)
      {
        PORTD &= B00000011;     //pins D2 - D7 to low
        PORTB &= B11000000;     //pins D8 - D13 to low
        PORTD |= B00000100;     //Pin D2 to high
      }
      //Do the same for each hour
      if(time_hours == 1)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00001000;
      }
      if(time_hours == 2)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00010000;
      }
      if(time_hours == 3)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00100000;
      }
      if(time_hours == 4)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B01000000;
      }
      if(time_hours == 5)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B10000000;
      }
      if(time_hours == 6)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000001;
      }
      if(time_hours == 7)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000010;
      }
      if(time_hours == 8)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000100;
      }
      if(time_hours == 9)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00001000;
      }
      if(time_hours == 10)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00010000;
      }
      if(time_hours == 11)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00100000;
      }
      
      //////////////////////////////////////////////////////////
      //////////////////////////////////////////end of HOURS////

      //This function will flash the minute LED 4 times
      //We do the same for each minute for ranges 0-5, 5-10, etc
      if(0 <= time_minutes && time_minutes < 5)
      {
        for(int i=0; i<4; i++)
        {
          PORTD |= B00000100;
          delay(flash_delay);
          PORTD &= B11111011;
          delay(flash_delay);
        }
      }//end of minutes 1-5

      if(5 <= time_minutes && time_minutes< 10)
      {
        for(int i=0; i<4; i++)
        {
          PORTD |= B00001000;
          delay(flash_delay);
          PORTD &= B11110111;
          delay(flash_delay);
        }
      }//end of minutes 5-10

      if(10 <= time_minutes && time_minutes< 15)
      {
        for(int i=0; i<4; i++)
        {
          PORTD |= B00010000;
          delay(flash_delay);
          PORTD &= B11101111;
          delay(flash_delay);
        }
      }//end of minutes 10-15

      if(15 <= time_minutes && time_minutes< 20)
      {
        for(int i=0; i<4; i++)
        {
          PORTD |= B00100000;
          delay(flash_delay);
          PORTD &= B11011111;
          delay(flash_delay);
        }
      }//end of minutes 15-20

      if(20 <= time_minutes && time_minutes< 25)
      {
        for(int i=0; i<4; i++)
        {
          PORTD |= B01000000;
          delay(flash_delay);
          PORTD &= B10111111;
          delay(flash_delay);
        }
      }//end of minutes 20-25

      if(25 <= time_minutes && time_minutes< 30)
      {
        for(int i=0; i<4; i++)
        {
          PORTD |= B10000000;
          delay(flash_delay);
          PORTD &= B01111111;
          delay(flash_delay);
        }
      }//end of minutes 25-30

      if(30 <= time_minutes && time_minutes< 35)
      {
        for(int i=0; i<4; i++)
        {
          PORTB |= B00000001;
          delay(flash_delay);
          PORTB &= B11111110;
          delay(flash_delay);
        }
      }//end of minutes 30-35

      if(35 <= time_minutes && time_minutes< 40)
      {
        for(int i=0; i<4; i++)
        {
          PORTB |= B00000010;
          delay(flash_delay);
          PORTB &= B11111101;
          delay(flash_delay);
        }
      }//end of minutes 35-40

      if(40 <= time_minutes && time_minutes< 45)
      {
        for(int i=0; i<4; i++)
        {
          PORTB |= B00000100;
          delay(flash_delay);
          PORTB &= B11111011;
          delay(flash_delay);
        }
      }//end of minutes 40-45

      if(45 <= time_minutes && time_minutes< 50)
      {
        for(int i=0; i<4; i++)
        {
          PORTB |= B00001000;
          delay(flash_delay);
          PORTB &= B11110111;
          delay(flash_delay);
        }
      }//end of minutes 45-50

      if(50 <= time_minutes && time_minutes< 55)
      {
        for(int i=0; i<4; i++)
        {
          PORTB |= B00010000;
          delay(flash_delay);
          PORTB &= B11101111;
          delay(flash_delay);
        }
      }//end of minutes 50-55

      if(55 <= time_minutes && time_minutes < 60)
      {
        for(int i=0; i<4; i++)
        {
          PORTB |= B00100000;
          delay(flash_delay);
          PORTB &= B11011111;
          delay(flash_delay);
        }
      }//end of minutes 50-55

      
      //////////////////////////////////////////////////////////
      ////////////////////////////////////////end of MINUTES////
      
      PORTD &= B00000011;
      PORTB &= B11000000;
      //After the time was shown, we turn off all LEDs to save power
      
    }//end of mode 0 (time show)


    //We start mode 1 and set the hour
    if(mode == 1 && !mode_was_changed)
    { 
      //After long press, each time we press button we increase the hour by 1
      time_hours = time_hours+1;
      if(time_hours == 0)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00000100;      
      }
      if(time_hours == 1)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00001000;      
      }
      if(time_hours == 2)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00010000;      
      }
      if(time_hours == 3)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00100000;      
      }
      if(time_hours == 4)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B01000000;      
      }
      if(time_hours == 5)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B10000000;      
      }
      if(time_hours == 6)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000001;      
      }
      if(time_hours == 7)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000010;      
      }
      if(time_hours == 8)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000100;      
      }
      if(time_hours == 9)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00001000;       
      }
      if(time_hours == 10)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00010000;      
      }
      if(time_hours == 11)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00100000;       
      }
      if(time_hours == 12)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00000100;       
      }
    }//end of mode 1 (hour set)

    
    //Do the same for minute set
    if(mode == 2 && !mode_was_changed)
    {
      time_minutes = time_minutes+5;      
      if(0 <= time_minutes && time_minutes < 5)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00000100;      
      }
      if(5 <= time_minutes && time_minutes < 10)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00001000;      
      }
      if(10 <= time_minutes && time_minutes < 15)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00010000;      
      }
      if(15 <= time_minutes && time_minutes < 20)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00100000;      
      }
      if(20 <= time_minutes && time_minutes < 25)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B01000000;      
      }
      if(25 <= time_minutes && time_minutes < 30)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B10000000;      
      }
      if(30 <= time_minutes && time_minutes < 35)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000001;      
      }
      if(35 <= time_minutes && time_minutes < 40)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000010;      
      }
      if(40 <= time_minutes && time_minutes < 45)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000100;      
      }
      if(45 <= time_minutes && time_minutes < 50)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00001000;      
      }
      if(50 <= time_minutes && time_minutes < 55)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00010000;      
      }
      if(55 <= time_minutes && time_minutes < 60)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00100000;      
      }
      if(60 <= time_minutes && time_minutes < 65)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00000100;   
        time_minutes = time_minutes+1;    
      }
    }//end of mode 2 (minute set)
        

    
    mode_was_changed = false;
    power_adc_disable(); // ADC converter disabled to save power

  }//end of analog below treshold



  
  //Here we count time in order to see if the button was pressed or long pressed for mode change
  if(button_pressed)
  {
  actual_button_counter = millis();
  push_time = actual_button_counter - pressed_button_counter;
  }

  //Pressed for more than 1second, set hour
  if(push_time > mode_change_delay && button_pressed && mode == 0)
  {
    mode = 1;
    if(time_hours == 0)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTD |= B00000100;      
    }
    if(time_hours == 1)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTD |= B00001000;      
    }
    if(time_hours == 2)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTD |= B00010000;      
    }
    if(time_hours == 3)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTD |= B00100000;      
    }
    if(time_hours == 4)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTD |= B01000000;      
    }
    if(time_hours == 5)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTD |= B10000000;      
    }
    if(time_hours == 6)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTB |= B00000001;      
    }
    if(time_hours == 7)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTB |= B00000010;      
    }
    if(time_hours == 8)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTB |= B00000100;      
    }
    if(time_hours == 9)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTB |= B00001000;       
    }
    if(time_hours == 10)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTB |= B00010000;      
    }
    if(time_hours == 11)
    {
      PORTD &= B00000011;
      PORTB &= B11000000;
      PORTB |= B00100000;       
    }
    
    button_pressed = false;
    mode_was_changed = true;
  }
  
  //Now we set minute
  if(push_time > mode_change_delay && button_pressed && mode == 1)
  {
    mode = 2;
    PORTD &= B00000011;
    PORTB &= B11000000;
    button_pressed = false;
    mode_was_changed = true;
    if(0 <= time_minutes && time_minutes < 5)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00000100;      
      }
      if(5 <= time_minutes && time_minutes < 10)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00001000;      
      }
      if(10 <= time_minutes && time_minutes < 15)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00010000;      
      }
      if(15 <= time_minutes && time_minutes < 20)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00100000;      
      }
      if(20 <= time_minutes && time_minutes < 25)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B01000000;      
      }
      if(25 <= time_minutes && time_minutes < 30)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B10000000;      
      }
      if(30 <= time_minutes && time_minutes < 35)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000001;      
      }
      if(35 <= time_minutes && time_minutes < 40)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000010;      
      }
      if(40 <= time_minutes && time_minutes < 45)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00000100;      
      }
      if(45 <= time_minutes && time_minutes < 50)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00001000;      
      }
      if(50 <= time_minutes && time_minutes < 55)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00010000;      
      }
      if(55 <= time_minutes && time_minutes < 60)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTB |= B00100000;      
      }
      if(60 <= time_minutes && time_minutes < 65)
      {
        PORTD &= B00000011;
        PORTB &= B11000000;
        PORTD |= B00000100;      
      }
  }
  
  //exit set mode and turn all LEDs off and get back to mode 0
  if(push_time > mode_change_delay && button_pressed && mode == 2)
  {
    blinkAll(25);
    mode = 0;
    PORTD &= B00000011;
    PORTB &= B11000000;
    button_pressed = false;
    mode_was_changed = true;
  }
  
 
  

}//END OF VOID LOOP



ISR(PCINT1_vect)
{
power_adc_enable(); // turn back on the ADC converter in order to be able to detech the button push
}
