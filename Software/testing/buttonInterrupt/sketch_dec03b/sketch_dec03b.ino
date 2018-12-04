const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
volatile int buttonState = 0;         // variable for reading the pushbutton status

volatile int pressed = 0;
unsigned long startMillis = 0;
unsigned long currentsMillis = 0;
unsigned long diffMillis = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(0, pin_ISR, CHANGE);
}

void loop() {
  // Nothing here!
  diffMillis = currentsMillis - startMillis;
  if(pressed == 1 && diffMillis < 3000)
  {
    currentsMillis = millis();
  }
  else
  {
    //diffMillis = currentsMillis - startMillis;
    if(diffMillis > 10)
    {
      digitalWrite(ledPin, HIGH);
      delay(diffMillis);
      digitalWrite(ledPin, LOW);
    }
    
    startMillis = millis();
    currentsMillis = millis();
  }
}

void pin_ISR() {
  buttonState = digitalRead(buttonPin);
  //digitalWrite(ledPin, buttonState);
  if(buttonState == HIGH)
    pressed = 1;
  else
    pressed = 0;
}
