// Note Frequencies
#define NOTE_C4  262  
#define NOTE_G4  392  
#define NOTE_A4  440  
#define NOTE_F4  349  
#define NOTE_E4  330  
#define NOTE_D4  294  
#define NOTE_C5  523  
#define NOTE_G5  784 

// Motor and Sensor Pin Definitions
#define motor_board_input_pin_IN2 6      // Connect to Arduino pin 6
#define motor_board_input_pin_IN1 9      // Connect to Arduino pin 9
#define motor_board_input_pin_IN4 3      // Connect to Arduino pin 3
#define motor_board_input_pin_IN3 5      // Connect to Arduino pin 5
#define IR1Pin A0                        // Connect to IR sensor 1 on the right
#define IR2Pin A1                        // Connect to IR sensor 2 on the left
#define TrigPin1 11                      // U/S1 Trig connected to pin 11
#define EchoPin1 10                      // U/S1 Echo connected to pin 10
#define HitBPin 2                        // Connect to the target sensor
#define PassiveBuzzerPin 4               // Connect the buzzer to D12

#include "RichShieldPassiveBuzzer.h"
PassiveBuzzer buz(PassiveBuzzerPin);

#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the I2C LCD with address 0x27, 16 columns, and 2 rows

int counterA = 1, counterB = 1; 
int IR1_Val = 1; 
int IR2_Val = 1; 
int Distance1, Distance2; 

unsigned long previousMessageTime = 0; // To track time for LCD messages
const unsigned long messageInterval = 3000; // 3 seconds interval for each message
int currentMessage = 0; // Track the current message to display

int buzzerIndex = 0;
unsigned long lastNoteTime = 0;

// New Melody and Note Durations
int melody[] = { 
  330, 392, 392, 440, 392, 330, 262, 294, 330, 330, 294, 262, 294, 196, 330, 392, 392, 440, 392, 330, 262, 294, 330, 330, 294, 294, 262, 349, 349, 349, 440, 0, 392, 392, 294, 330, 392, 392, 440, 392, 330, 262, 294, 330, 330, 294, 294, 262, 0, 0, 0, 0 
};
int noteDurations[] = { 
  288, 288, 384, 192, 288, 288, 384, 192, 288, 288, 288, 288, 576, 576, 288, 288, 384, 192, 288, 288, 384, 192, 288, 288, 288, 288, 1152, 576, 576, 288, 288, 576, 576, 576, 1152, 288, 288, 384, 192, 288, 288, 384, 192, 288, 288, 288, 288, 288, 288, 288, 288, 0 
};

// Function Definitions
void Turn_left(void);
void Turn_right(void);
void Go_straight(void);
void IRSense(void);
int getDistance(int EchoPin, int TrigPin);
void stop(void);
void bruhstop(void);
void messageonlcd(void);
void sing(void);

void setup() {
  pinMode(motor_board_input_pin_IN2, OUTPUT);
  pinMode(motor_board_input_pin_IN1, OUTPUT); 
  pinMode(motor_board_input_pin_IN4, OUTPUT);
  pinMode(motor_board_input_pin_IN3, OUTPUT);
  pinMode(TrigPin1, OUTPUT); 
  pinMode(EchoPin1, INPUT);  
  pinMode(HitBPin, INPUT_PULLUP); 
  pinMode(PassiveBuzzerPin, OUTPUT); // Set the buzzer pin as an output

  PCICR |= B00000100; //for target sensor
  PCMSK2 |= B00010100; //for target sensor 

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  lastNoteTime = millis();
}

void loop() {
  // Run the bruhstop logic
  bruhstop();

  // Run the messageonlcd logic in parallel
  messageonlcd();

  // Always run the sing function
  sing();
}

void messageonlcd() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMessageTime >= messageInterval) {
    previousMessageTime = currentMillis;
    lcd.clear();

    if (currentMessage == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Here is your");
      lcd.setCursor(0, 1);
      lcd.print("medication");
      currentMessage = 1; // Move to next message

    } else if (currentMessage == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Drink water");
      lcd.setCursor(0, 1);
      lcd.print("Stay healthy");
      currentMessage = 0; // Reset to first message
    }
  }
}

void Turn_left() {
  digitalWrite(motor_board_input_pin_IN2, HIGH);
  analogWrite(motor_board_input_pin_IN1, 200); //motor A left higher forward speed
  digitalWrite(motor_board_input_pin_IN3, LOW);
  analogWrite(motor_board_input_pin_IN4, 50); // motor B right lower reverse speed
  delay(100);
}

void Turn_right() {
  digitalWrite(motor_board_input_pin_IN2, LOW);
  analogWrite(motor_board_input_pin_IN1, 50); //motor A lower forward speed
  digitalWrite(motor_board_input_pin_IN3, HIGH);
  analogWrite(motor_board_input_pin_IN4, 200); // motor B higher reverse speed
  delay(100);
}

void Go_straight() {
  digitalWrite(motor_board_input_pin_IN2, HIGH);
  analogWrite(motor_board_input_pin_IN1, 190);
  digitalWrite(motor_board_input_pin_IN3, HIGH);
  analogWrite(motor_board_input_pin_IN4, 190);
}

void IRSense() {
  if (IR1_Val > 830 && IR2_Val <= 830) {
    Turn_left();
  } else if (IR2_Val > 830 && IR1_Val <= 830) {
    Turn_right();
  } else if (IR1_Val > 830 && IR2_Val > 830) {
    Go_straight();
  } else {
    Go_straight();
  }
}

int getDistance(int EchoPin, int TrigPin) {
  int distance;
  long pulseDuration; //variable needed by the ultrasound sensor code

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  pulseDuration = pulseIn(EchoPin, HIGH);
  distance = pulseDuration / 58;  // Convert to cm

  return distance;
}

void stop() {
  digitalWrite(motor_board_input_pin_IN2, HIGH);
  digitalWrite(motor_board_input_pin_IN1, HIGH);
  digitalWrite(motor_board_input_pin_IN3, HIGH);
  digitalWrite(motor_board_input_pin_IN4, HIGH);
  delay(50);
}

void bruhstop() {
  Distance1 = getDistance(EchoPin1, TrigPin1);
  IR1_Val = analogRead(IR1Pin);
  IR2_Val = analogRead(IR2Pin);

  if (Distance1 < 20) //to make the motor stop when ultrasonic senses something in front 
  {
    while (counterA == 0 && counterB == 0) //
    {
      stop();
    }
    counterA = 0;
    counterB = 0;
  } else {
    IRSense();
  }
  delay(50);
}

ISR(PCINT2_vect) {
  if (!digitalRead(HitBPin)) // if Hit_bPin is 0, Target B receives hit
    counterB++; // increase counterB by 1
}

void sing() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastNoteTime >= noteDurations[buzzerIndex]) {
    lastNoteTime = currentMillis;

    int noteDuration = noteDurations[buzzerIndex];
    buz.playTone(melody[buzzerIndex], noteDuration);

    // Move to the next note
    buzzerIndex++;
    if (buzzerIndex >= sizeof(melody) / sizeof(int)) {
      buzzerIndex = 0; // Loop back to the start
    }
  }
}