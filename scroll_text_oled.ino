#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1   
#define SCROLL_SPEED  0.001  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BluetoothSerial SerialBT;
String receivedText = "Waiting for Msg...";  
int textSize = 3;  

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32-OLED");  
    Serial.println("Bluetooth Started!");

    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }

    display.clearDisplay();
    display.setTextSize(textSize);  
    display.setTextColor(WHITE);
    display.setTextWrap(false);  
    display.display();
}

void loop() {
    if (SerialBT.available()) {
        receivedText = "";
        while (SerialBT.available()) {
            char c = SerialBT.read();
            receivedText += c;
        }

        Serial.println("Received: " + receivedText);
    }

    scrollText(receivedText);  // Scroll continuously
}

// Function to scroll text smoothly in a single line
void scrollText(String text) {
    int charWidth = 6 * textSize;  // Each char ~6px wide * scale
    int textWidth = text.length() * charWidth;
    int startX = SCREEN_WIDTH;

    while (true) {
        for (int x = startX; x > -textWidth; x -= 2) {
            display.clearDisplay();
            display.setTextSize(textSize);  // Reapply size here
            display.setCursor(x, 20);  // Adjust vertical position
            display.println(text);
            display.display();
            delay(5);  // Delay for scroll speed

            if (SerialBT.available()) return;  // Stop scrolling if new text arrives
        }
    }
}
