#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

//pinii LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//SoftwareSerial: RX=A0, TX=A1
SoftwareSerial mySerial2(A0, A1);

//buffer pentru datele primite
String incomingData = "";

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LedReflexTest");
  mySerial2.begin(9600);
}

void loop() {
  //Citim caracterele din serial, non-blocant
  while (mySerial2.available() > 0) {
    char c = mySerial2.read();
    if (c == '\n' || c == '\r') {
      //avem un sfarsit de linie
      if (incomingData.length() > 0) {
        parseMessage(incomingData);
        incomingData = "";
      }
    } else {
      //construim sirul
      incomingData += c;
    }
  }
}

// Functie de parse
void parseMessage(const String &msg) {
  // Cautam "SCORE:"
  if (msg.startsWith("SCORE:")) {
    //extragem valoarea
    String val = msg.substring(6);
    int scoreValue = val.toInt();
    //afisam pe LCD
    lcd.clear();
    lcd.print("Scor curent: ");
    lcd.setCursor(0,1);
    lcd.print(scoreValue);
  }
}