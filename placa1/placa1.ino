#include <SoftwareSerial.h>
//SoftwareSerial: RX=A0, TX=A1
SoftwareSerial mySerial(A0, A1);

const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int NUM_LEDS = 9;

const int lastLedPin = 11;   
const int buttonPin  = 12;    //buton INPUT_PULLUP

unsigned long LED_INTERVAL = 1000UL;  //timp inițial 1 secunda
const unsigned long MIN_LED_INTERVAL = 300UL;  //timp intre LED-uri

int currentLedIndex = 0;
unsigned long previousLedChange = 0;
unsigned long lastLedStartTime = 0;
bool lastLedActive = false;
bool wasButtonPressedDuringLastLed = false;

int score = 0;
bool lastButtonState = HIGH;

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  pinMode(lastLedPin, OUTPUT);
  digitalWrite(lastLedPin, LOW);

  pinMode(buttonPin, INPUT_PULLUP);

  mySerial.begin(9600);

  previousLedChange = millis();
  currentLedIndex = 0;
  digitalWrite(ledPins[currentLedIndex], HIGH); //aprindem primul LED
}

void loop() {
  unsigned long currentMillis = millis();

  bool buttonState = digitalRead(buttonPin);
  bool buttonPressed = (lastButtonState == HIGH && buttonState == LOW);
  lastButtonState = buttonState;

  if (currentMillis - previousLedChange >= LED_INTERVAL && !lastLedActive) {
    // Stingem LED-ul curent
    digitalWrite(ledPins[currentLedIndex], LOW);

    // Incrementam indexul LED-ului
    currentLedIndex++;
    if (currentLedIndex >= NUM_LEDS) {
      currentLedIndex = 0;

      // Activam ultimul LED
      digitalWrite(lastLedPin, HIGH);
      lastLedStartTime = millis();
      lastLedActive = true;
      wasButtonPressedDuringLastLed = false;
    } else {
      //aprindem urmatorul LED
      digitalWrite(ledPins[currentLedIndex], HIGH);
    }

    previousLedChange = currentMillis;
  }

  if (lastLedActive) {
    if (buttonState == LOW) {
      wasButtonPressedDuringLastLed = true;
    }

    if (currentMillis - lastLedStartTime >= LED_INTERVAL) {
      // Stingem ultimul LED
      digitalWrite(lastLedPin, LOW);
      lastLedActive = false;

      if (wasButtonPressedDuringLastLed) {
        score++;

        if (score % 3 == 0) {
          //efectul de victorie
          unsigned long effectStart = millis();
          while (millis() - effectStart < 1000) {
            for (int i = 0; i < NUM_LEDS; i++) {
              digitalWrite(ledPins[i], HIGH);
            }
            digitalWrite(lastLedPin, HIGH);
          }
          for (int i = 0; i < NUM_LEDS; i++) {
            digitalWrite(ledPins[i], LOW);
          }
          digitalWrite(lastLedPin, LOW);

          if (LED_INTERVAL > MIN_LED_INTERVAL) {
            LED_INTERVAL -= 300;
          }
        }
      } else {
        //resetam scorul la zero
        score = 0;
        LED_INTERVAL = 1000UL;
      }

      mySerial.print("SCORE:");
      mySerial.println(score);
    }
  }
}
