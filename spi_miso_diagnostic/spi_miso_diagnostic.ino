/*
 * SPI MISO Diagnostic for ESP32-2432S028R
 * This checks if we can read ANY data from the touch controller
 */

#include <SPI.h>

#define TOUCH_CS 33
#define TOUCH_IRQ 36
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== SPI MISO Diagnostic ===\n");
  
  pinMode(TOUCH_CS, OUTPUT);
  pinMode(TOUCH_IRQ, INPUT);
  digitalWrite(TOUCH_CS, HIGH);
  
  // Initialize SPI
  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI);
  
  Serial.println("SPI initialized");
  Serial.printf("SCLK=%d, MISO=%d, MOSI=%d, CS=%d, IRQ=%d\n\n", 
                TFT_SCLK, TFT_MISO, TFT_MOSI, TOUCH_CS, TOUCH_IRQ);
  
  Serial.println("Waiting for touch (IRQ goes LOW)...\n");
}

void loop() {
  int irqState = digitalRead(TOUCH_IRQ);
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();
    Serial.printf("IRQ pin %d state: %s\n", TOUCH_IRQ, irqState == LOW ? "TOUCHED (LOW)" : "not touched (HIGH)");
  }
  
  // Only try to read when touched
  if (irqState == LOW) {
    Serial.println("\n>>> Touch detected! Attempting SPI read...");
    
    // Try to read X position
    digitalWrite(TOUCH_CS, LOW);
    delayMicroseconds(10);
    
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    
    uint8_t cmd = 0xD0; // X position command
    SPI.transfer(cmd);
    Serial.printf("Sent command: 0x%02X\n", cmd);
    
    delayMicroseconds(10);
    
    uint8_t b1 = SPI.transfer(0x00);
    uint8_t b2 = SPI.transfer(0x00);
    
    Serial.printf("Received bytes: 0x%02X 0x%02X\n", b1, b2);
    
    uint16_t raw = ((b1 << 8) | b2) >> 3;
    Serial.printf("Raw value: %d\n", raw);
    
    SPI.endTransaction();
    
    digitalWrite(TOUCH_CS, HIGH);
    delayMicroseconds(100);
    
    // Try Y position
    digitalWrite(TOUCH_CS, LOW);
    delayMicroseconds(10);
    
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    
    cmd = 0x90; // Y position command
    SPI.transfer(cmd);
    Serial.printf("Sent command: 0x%02X\n", cmd);
    
    delayMicroseconds(10);
    
    b1 = SPI.transfer(0x00);
    b2 = SPI.transfer(0x00);
    
    Serial.printf("Received bytes: 0x%02X 0x%02X\n", b1, b2);
    
    raw = ((b1 << 8) | b2) >> 3;
    Serial.printf("Raw value: %d\n\n", raw);
    
    SPI.endTransaction();
    
    digitalWrite(TOUCH_CS, HIGH);
    
    delay(500); // Don't spam
  }
  
  delay(50);
}
