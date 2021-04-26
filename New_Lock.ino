#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleStream.h>

#define BLYNK_MAX_SENDBYTES 1200
#define SS_PIN 10
#define RST_PIN 9
#define BLYNK_PRINT DebugSerial
#define BUTTON 8

// lock
#define lock 5
#define sound 4

// led
#define led_red 2
#define led_green 3

// counting wrong access
int wrongCount;
Servo myservo;
int pos = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial DebugSerial(6, 7); // RX, TX
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

//Blynk authentication
char auth[] = "06D4Upg7JhbhsVyGl26_4-T-uiCU-Umd";
void setup()
{
    DebugSerial.begin(9600);
    Serial.begin(9600); // Initiate a serial communication
    Blynk.begin(Serial, auth);

    pinMode(lock, OUTPUT);

    SPI.begin();        // Initiate  SPI bus
    mfrc522.PCD_Init(); // Initiate MFRC522

    delay(1000);

    pinMode(led_green, OUTPUT);
    pinMode(led_red, OUTPUT);
    myservo.attach(6);
    lcd.init();
    lcd.clear();
    lcd.backlight(); // Make sure backlight is on
    lcd.setCursor(1, 0);
    lcd.print("SYSTEM IS ON");
    delay(1000);
}

void loop()
{
    Blynk.run();
    // while();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Input key card");
    digitalWrite(led_red, 0);
    digitalWrite(led_green, 0);

    String content = "";
    if (mfrc522.PICC_IsNewCardPresent() | mfrc522.PICC_ReadCardSerial())
    {
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
    }

    content.toUpperCase();
    if (digitalRead(BUTTON) == HIGH | content.substring(1) == "27 6A F5 2C") //change here the UID of the card/cards that you want to give access
    {
        Blynk.notify("Unlocked");
        digitalWrite(lock, HIGH);
        // Print a message on both lines of the LCD.
        lcd.clear();
        lcd.setCursor(2, 0); //Set cursor to character 2 on line 0
        lcd.print("Welcome Home");

        for (int i = 0; i < 3; i++)
        {
            tone(sound, 500);
            digitalWrite(led_red, 0);
            digitalWrite(led_green, 1);
            delay(50);
            noTone(sound);
            digitalWrite(led_green, 0);
            delay(50);
        }

        // Serial.println();
        for (pos = 0; pos <= 180; pos += 1)
            myservo.write(pos);

        delay(4000);
        digitalWrite(lock, LOW);
        lcd.clear();
        wrongCount = 0;
    }
    else if (!(content.substring(1) == "27 6A F5 2C") | !(content.substring(1) == ""))
    {
        Blynk.notify("Wrong Card Enter!!!");
        lcd.clear();
        lcd.setCursor(0, 0); //Move cursor to character 2 on line 1
        lcd.print("Card Deny");
        lcd.setCursor(0, 1);
        lcd.print("Please try again");

        for (int i = 0; i < 3; i++)
        {
            tone(sound, 500);
            digitalWrite(led_red, 1);
            digitalWrite(led_green, 0);
            delay(300);
            noTone(sound);
            digitalWrite(led_red, 0);
            delay(300);
        }

        wrongCount++;
        if (wrongCount >= 5)
        {
            lcd.clear();

            char num[5];
            itoa(wrongCount, num, 10);

            String body = String("Someone is trying to access to your house ") + num + " times of wrong authentication).";

            Blynk.email("Subject: Future Door", body);
            Blynk.notify("Someone is trying to access to your house!!!");
            lcd.setCursor(4, 0); //Move cursor to character 2 on line 1
            lcd.print("Calling");
            lcd.setCursor(5, 1); //Move cursor to character 2 on line 1
            lcd.print("Police");
            for (int i = 0; i < 3; i++)
            {

                lcd.setCursor(11, 1);
                lcd.print("     ");
                delay(600);
                lcd.setCursor(11, 1);
                lcd.print(".    ");
                delay(600);
                lcd.setCursor(11, 1);
                lcd.print("..   ");
                delay(600);
                lcd.setCursor(11, 1);
                lcd.print("...  ");
                delay(600);
            }
            delay(2000);
            return;

        }
        for (pos = 180; pos >= 5; pos -= 1)
            myservo.write(pos);

        delay(2000);
        lcd.clear();
    }
}
