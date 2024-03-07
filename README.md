Printed Circuit Board designed with Kicad to control an electrical lock.
It has a microcontroller (ESP32-C3) capable of bluetooth, WiFi and USB, a connector for a RFID reader board based on NXP MFRC522 chip, a servomotor connector and RGB LED.

To program the board using Arduino IDE, you need to add the ESP32 support https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
Connect your board and select the "Adafruit QT Py ESP32-C3" board, and the port that should appear. The first time or if the port does not appear, you need to put manually your board in download mode : push the boot button, reset the board, release the push button.

You should be able to flash an example to the board.

For the LED, you can use the Adafruit NeoPixel library, test it with RGBWstrandtest example configured with the LED_PIN set to 2 and LED_COUNT set to 1

For the RFID board, the RST_PIN is 7, SS_PIN is 10, and SPI.begin(4, 5, 6, 10).
