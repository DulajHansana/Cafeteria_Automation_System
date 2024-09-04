#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <SPI.h>
#include <MFRC522.h>

// hx711 pin assign

const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;

// rfid reader pin assign

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER 8

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd i2c address
MFRC522 mfrc522(SS_PIN, RST_PIN);   // mrfc instance

void setup() {
  Serial.begin(9600);
  
  // load cell

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  lcd.init();                      // initialize lcd 
  lcd.backlight();
  scale.set_scale(2280.f);         // calibration
  scale.tare();                    // reset scale to 0
  
  // rfid

  SPI.begin();                     // iinit spi bus
  mfrc522.PCD_Init();              // init mrfc
  pinMode(BUZZER, OUTPUT);
}

void loop() {

  // weight measurement

  lcd.clear();
  long weight = scale.get_units(5); // get average of 5 readings

  if (weight < 50) {
    Serial.println("Out of Stock");
    lcd.setCursor(0,0);
    lcd.print("Out of Stock");
  } else {
    
    lcd.setCursor(0,0);
    lcd.print("Stock in Oder ");
  }

  // rfid card reading

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String readUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      readUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      readUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    readUID.toUpperCase();

    //serial monitor print for testing

    Serial.print("Card UID: ");
    Serial.println(readUID);

    //buzzer beep

    digitalWrite(BUZZER, HIGH);
    delay(100); 
    digitalWrite(BUZZER, LOW);

    //payment display

    Serial.println("Payment is Successful");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Payment Success");
    delay(2000);                          // display message 2s before next loop iteration
  }

  delay(500); // main loop delay
}
