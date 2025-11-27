/*
 * ESP32-2432S028R Touch Test with CORRECT pins
 * Touch uses SEPARATE SPI bus from display!
 */

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();

// Touch controller pins - SEPARATE from display!
#define TOUCH_MOSI 32
#define TOUCH_MISO 39
#define TOUCH_CLK 25
#define TOUCH_CS 33
#define TOUCH_IRQ 36

// Create separate SPI bus for touch
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

// RGB LED pins
#define LED_R 4
#define LED_G 16
#define LED_B 17

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=== ESP32-2432S028R Touch Test ===");
  Serial.println("Using CORRECT separate SPI pins!");
  
  // Initialize RGB LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
  
  // Turn on backlight
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  
  // Initialize display (uses default VSPI: MOSI=13, MISO=12, CLK=14)
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);
  
  // Draw green boundary
  tft.drawRect(0, 0, 320, 240, TFT_GREEN);
  tft.drawRect(1, 1, 318, 238, TFT_GREEN);
  
  Serial.println("Display initialized");
  
  // Initialize SEPARATE SPI for touch
  touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  touch.begin(touchSPI);
  touch.setRotation(1);
  
  Serial.println("Touch initialized on separate SPI bus");
  Serial.printf("Touch pins: CLK=%d MISO=%d MOSI=%d CS=%d IRQ=%d\n", 
                TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS, TOUCH_IRQ);
  
  // Display success message
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.println("ESP32-2432S028R test");
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(10, 100);
  tft.println("Touch anywhere!");
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 130);
  tft.println("Using separate SPI pins");
  
  Serial.println("\nReady - touch the screen!");
}

void loop() {
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    
    // Map raw values - SWAP reversed: use X for screen X, Y for screen Y
    int x = map(p.x, 350, 3436, 0, 320);  // Using X for X
    int y = map(p.y, 541, 3566, 0, 240);  // Using Y for Y
    
    x = constrain(x, 0, 319);
    y = constrain(y, 0, 239);
    
    Serial.printf("Touch! Raw: X=%d Y=%d | Screen: X=%d Y=%d\n", p.x, p.y, x, y);
    
    // Flash green LED
    digitalWrite(LED_G, LOW);
    delay(50);
    digitalWrite(LED_G, HIGH);
    
    // Draw red dot
    tft.fillCircle(x, y, 5, TFT_RED);
    
    // Display coordinates in green
    tft.fillRect(10, 150, 300, 60, TFT_BLACK);
    tft.setCursor(10, 150);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.printf("X=%d Y=%d", x, y);
    
    tft.setTextSize(1);
    tft.setCursor(10, 175);
    tft.printf("Raw: X=%d Y=%d", p.x, p.y);
    
    delay(100);
  }
  
  delay(50);
}
