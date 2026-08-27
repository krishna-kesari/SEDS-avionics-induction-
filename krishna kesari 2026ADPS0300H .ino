// C++ code

#include <LiquidCrystal.h>

// LCD pin connections: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pin assignments
const int buttonPin = 8;     // Push button
const int ledPin = 12;       // LED
const int buzzerPin = 11;    // Buzzer
const int trigPin = 9;       // Ultrasonic TRIG
const int echoPin = 10;      // Ultrasonic ECHO
const int lightPin = A0;     // Light sensor (LDR)

// State machine states
enum State { OPEN_SEA, ANCHOR_DROPPED, STORM, CHARYBDIS, WRECKED };
State currentState = OPEN_SEA;

// Timing
unsigned long stateStartTime = 0;
const unsigned long dangerDuration = 5000; // 5 seconds

// Anchor toggle
bool anchorDropped = false;

// Hysteresis thresholds for light sensor
const int stormEnterThreshold = 400; // enter storm if below this
const int stormExitThreshold  = 500; // exit storm if above this



// Startup grace period
unsigned long startupTime = 0;
const unsigned long startupDelay = 5000; // 5 seconds grace period

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Odysseus Watch");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);

  // Mark startup time
  startupTime = millis();
}

void loop() {
  // --- Read sensors ---
  int lightValue = analogRead(lightPin);
  long distance = readUltrasonic();

  // Debugging output
  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print("  Distance: ");
  Serial.println(distance);

  // --- Handle button (anchor toggle) ---
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    anchorDropped = !anchorDropped;
    if (anchorDropped) {
      currentState = ANCHOR_DROPPED;
    } else {
      currentState = OPEN_SEA;
    }
    stateStartTime = millis();
  }
  lastButtonState = buttonState;

  // --- State transitions with startup grace period ---
  if (!anchorDropped && currentState != WRECKED) {
    if (millis() - startupTime < startupDelay) {
      // Force OPEN SEA during grace period
      currentState = OPEN_SEA;
    } else {
      if (currentState == OPEN_SEA) {
        if (lightValue < stormEnterThreshold) {
          currentState = STORM;
          stateStartTime = millis();
        } else if (distance < 100) {
          currentState = CHARYBDIS;
          stateStartTime = millis();
        }
      } else if (currentState == STORM) {
        if (lightValue > stormExitThreshold) {
          currentState = OPEN_SEA;
        }
      } else if (currentState == CHARYBDIS) {
        if (distance >= 100) {
          currentState = OPEN_SEA;
        }
      }
    }
  }

  // --- State actions ---
  switch (currentState) {
    case OPEN_SEA:
      lcd.setCursor(0, 0);
      lcd.print("State: OPEN SEA   ");
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      break;

    case ANCHOR_DROPPED:
      lcd.setCursor(0, 0);
      lcd.print("State: ANCHOR     ");
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      break;

    case STORM:
      lcd.setCursor(0, 0);
      lcd.print("State: STORM      ");
      digitalWrite(ledPin, (millis() / 500) % 2); // Blink LED
      digitalWrite(buzzerPin, LOW);
      if (millis() - stateStartTime > dangerDuration) {
        currentState = WRECKED;
      }
      break;

    case CHARYBDIS:
      lcd.setCursor(0, 0);
      lcd.print("State: CHARYBDIS  ");
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, HIGH);
      if (millis() - stateStartTime > dangerDuration) {
        currentState = WRECKED;
      }
      break;

    case WRECKED:
      lcd.setCursor(0, 0);
      lcd.print("State: WRECKED    ");
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      break;
  }
}

// --- Helper function for ultrasonic sensor ---
long readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // cm
  return distance;
}
