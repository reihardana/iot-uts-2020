#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 1);
#define triggerPin 16 //D0
#define echoPin 15 //D8
#define SS_PIN 2  //D4
#define RST_PIN 0 //D3
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.home();
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200); // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop()
{
  lcd.home();
  lcd.print("MASUKKAN COIN!");
  Serial.println("Waiting card...");
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    long duration, jarak;
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    jarak = (duration/2) / 29.1;
    Serial.print(jarak);
    Serial.println(" cm");
    if (jarak < 5){
      lcd.clear();
      lcd.print("TERIMA KASIH");
      Serial.print("TERIMA KASIH");
      delay(3000);
    }
    delay(10);
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    delay(10);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  String key;
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    key.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    key.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message: ");
  key.toUpperCase();
  if (key.substring(1) == "F9 22 8C A3"){
    lcd.clear();
    lcd.print("AKSES SUKSES");
    delay(3000);
  }else{
    lcd.clear();
    lcd.print("AKSES DITOLAK");
    delay(3000);
  }
}
