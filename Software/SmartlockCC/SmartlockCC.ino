/* Example of code for the ESP32-C3 board with RFID, Servo and RGBW LED */

#define DEBUG
/* Servo */
#include <ServoStep.h>
#include "SmartlockCC.h"

ServoStep servo = ServoStep(8);

/* LED */
#include <Adafruit_NeoPixel.h>
#define LED_PIN 2
// How many NeoPixels are attached to the board?
#define LED_COUNT 1
// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 20  // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

/* RFID */
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 7  // Configurable, see typical pin layout above
#define SS_PIN 10  // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
#ifdef DEBUG
  Serial.begin(115200);  // Initialize serial communications with the PC
  while (!Serial)
    ;  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
#endif
  // Servo
  servo.servoSet(15);  // Some servos don't like 0°
  // LED
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  // RFID
  SPI.begin(4, 5, 6, 10);  //SCK, MISO, MOSI, SS);			// Init SPI bus
  mfrc522.PCD_Init();      // Init MFRC522
  delay(4);                // Optional delay. Some board do need more time after init to be ready, see Readme
#ifdef DEBUG
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
#endif
}

void loop() {
  uint64_t serial_card_num=0;
  bool granted=false;
  String name="";
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    strip.setPixelColor(0, strip.Color(255, 0, 0, 0));
    servo.servoSet(15);
    strip.show();
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
#ifdef DEBUG
  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
#endif
  for (byte i = 0; i < mfrc522.uid.size; i++) {
      /*if(mfrc522.uid.uidByte[i] < 0x10) {
        Serial.print(F(" 0"));
      } else {
        Serial.print(F(" "));
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }*/
    serial_card_num |= mfrc522.uid.uidByte[i];
    serial_card_num <<= 8;
  }

  for(int j=0; j<tablesize; j++) {
    if(serial_card_num == table[j].UID) {
      name=table[j].name;
      granted=true;
      break;
    }
  }
  if(granted) {
    Serial.print("Accès authorisé pour ");
    Serial.println(name);
    servo.servoSet(180);
    strip.setPixelColor(0, strip.Color(0, 255, 0, 0));
    strip.show();
    delay(2000);
  } else {
    Serial.print("Accès refusé pour UID : 0x");
    Serial.println(serial_card_num, HEX);
    delay(1000);
  }

}
