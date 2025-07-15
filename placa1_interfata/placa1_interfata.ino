#include <SoftwareSerial.h>
//SoftwareSerial: RX=A0, TX=A1
SoftwareSerial mySerial(A0, A1);
const int NUM_LEDS = 10;
const int ledPins[NUM_LEDS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int buttonPin = 13;

unsigned long previousMillis = 0;   // pentru controlul vitezei de parcurgere a LED-urilor
unsigned long interval = 1000;      // intervalul implicit (1 sec) intre LED-uri
int currentLedIndex = 0;            // LED-ul curent care se aprinde 
int score = 0;                      // scorul jucatorului
int gameMode = 1;                   // modul de joc curent (1..4)

void setup() {
  Serial.begin(9600);
  //configuram pinii LED ca OUTPUT
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  //configuram butonul cu rezistenta de pull-up interna
  pinMode(buttonPin, INPUT_PULLUP);

  // Initializam parametrii
  resetGameParameters();
}
void loop() {
  //verificam daca a venit mesaj pe Serial din interfata 
  checkSerialInput();
  runClassicGame();
}
//verificam daca avem date noi pe Serial
void checkSerialInput() {
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();  // citim un singur byte
    //transformam codul ASCII in numar real
    if (incomingByte >= '1' && incomingByte <= '4') {
      gameMode = incomingByte - '0';  // convertim char -> int
      resetGameParameters();
    }
  }
}
//gameMode
void resetGameParameters() {
  currentLedIndex = 0;
  score = 0;
  //trimitem scorul 0 placa 2
  sendScoreToSecondBoard(score);

  switch (gameMode) {
    case 1:
      interval = 1000;  
      break;
    case 2:
      interval = 700;   
      break;
    case 3:
      interval = 500;   
      break;
    case 4:
      interval = 200;   
      break;
  }
}
void runClassicGame() {
  unsigned long currentMillis = millis();

  //avansam la urmatorul LED la fiecare *interval*
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //stingem LED-ul vechi
    digitalWrite(ledPins[currentLedIndex], LOW);

    //trecem la urmatorul LED
    currentLedIndex++;
    if (currentLedIndex >= NUM_LEDS) {
      currentLedIndex = 0;
    }

    //aprindem LED-ul nou
    digitalWrite(ledPins[currentLedIndex], HIGH);
  }

  //verificam butonul doar daca LED-ul final este aprins
  if (currentLedIndex == (NUM_LEDS - 1)) {
    checkButtonPress();
  }
}

//apasarea butonului si actualizeaza scorul
void checkButtonPress() {
  //buton cu pull-up: apasat = LOW
  if (digitalRead(buttonPin) == LOW) {
    static unsigned long lastPressTime = 0;
    if (millis() - lastPressTime > 50) {  //debouncing
      lastPressTime = millis();
      score++;
      sendScoreToSecondBoard(score);
      victoryFlash();
    }
  }
}

//trimitem scorul la a doua placa (prin Serial)
void sendScoreToSecondBoard(int scor) {
  Serial.print("S");
  Serial.println(scor);
}

void victoryFlash() {
  static unsigned long flashStartTime = 0; // Momentul de start al flash-ului
  static bool flashActive = false;         // Stare pentru a verifica daca flash-ul este activ

  // Daca flash-ul nu este activ, il initializam
  if (!flashActive) {
    flashActive = true;                   // Activam flash-ul
    flashStartTime = millis();            // Salvam timpul curent
    // Aprindem toate LED-urile
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
  }
  // Verificam daca au trecut 500 ms
  if (millis() - flashStartTime >= 500) {
    // Stingem toate LED-urile
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    // Resetam starea flash-ului
    flashActive = false;
  }
}

