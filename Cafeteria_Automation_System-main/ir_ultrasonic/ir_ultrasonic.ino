#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// lcd initialization

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ir sensor pins 

const int entrySensorPin = 2; 
const int exitSensorPin = 3; 
int count = 0; // counter for entries and exits

// 1st ultrasonic sensor pins

#define TRIG_PIN_1 9
#define ECHO_PIN_1 10
#define RED_LED_PIN_1 13
#define GREEN_LED_PIN_1 12

// 2nd ultrasonic sensor pins

#define TRIG_PIN_2 4
#define ECHO_PIN_2 5
#define RED_LED_PIN_2 6
#define GREEN_LED_PIN_2 7

void setup() {
  Serial.begin(9600);

  // iR sensor stup

  pinMode(entrySensorPin, INPUT_PULLUP);
  pinMode(exitSensorPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.print("Welcome!");
  delay(2000);
  lcd.clear();
  Serial.println("System Initialized");

  // ultrasonic sensor setup

  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(RED_LED_PIN_1, OUTPUT);
  pinMode(GREEN_LED_PIN_1, OUTPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(RED_LED_PIN_2, OUTPUT);
  pinMode(GREEN_LED_PIN_2, OUTPUT);
}

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // mesure the distsnce 
  return distance;
}

void loop() {

  // ir sensor function

  if (digitalRead(entrySensorPin) == LOW) {
    count++;
    while (digitalRead(entrySensorPin) == LOW);
    updateDisplayAndSerial();
  }
  if (digitalRead(exitSensorPin) == LOW) {
    count--;
    count = max(0, count);
    while (digitalRead(exitSensorPin) == LOW);
    updateDisplayAndSerial();
  }

  // ultrasonic sensor function

  long distance1 = measureDistance(TRIG_PIN_1, ECHO_PIN_1);
  Serial.print("Distance 1: ");
  Serial.println(distance1);
  if(distance1 < 10) {
    digitalWrite(RED_LED_PIN_1, HIGH);
    digitalWrite(GREEN_LED_PIN_1, LOW);
  } else {
    digitalWrite(RED_LED_PIN_1, LOW);
    digitalWrite(GREEN_LED_PIN_1, HIGH);
  }
  long distance2 = measureDistance(TRIG_PIN_2, ECHO_PIN_2);
  Serial.print("Distance 2: ");
  Serial.println(distance2);
  if(distance2 < 10) {
    digitalWrite(RED_LED_PIN_2, HIGH);
    digitalWrite(GREEN_LED_PIN_2, LOW);
  } else {
    digitalWrite(RED_LED_PIN_2, LOW);
    digitalWrite(GREEN_LED_PIN_2, HIGH);
  }

  delay(100); // delay for stability
}

void updateDisplayAndSerial() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Count: ");
  lcd.print(count);
  //Serial.print("Count: ");
  //Serial.println(count);
}
