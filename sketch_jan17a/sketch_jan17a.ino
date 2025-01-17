#define motor_board_input_pin_IN2 6      //Connect to Arduino pin 6
#define motor_board_input_pin_IN1 9          //Connect to Arduino pin 9
#define motor_board_input_pin_IN4 3      //Connect to Arduino pin 3
#define motor_board_input_pin_IN3 5    //Connect to Arduino pin 5
#define IR1Pin A0 
#define IR2Pin A1
#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10
#define HitAPin 4
#define HitBPin 2
int counterA = 1 , counterB = 1;
int IR1_Val = 0; 
int IR2_Val = 0;
int Distance1, Distance2;
void Turn_left(void);
void Turn_right(void);
void Go_straight(void);
void IRSense(void);
int getDistance(int EchoPin, int TrigPin);
void stop(void);
void bruhstop(void);

void setup()
{
  pinMode(motor_board_input_pin_IN2, OUTPUT);
  pinMode(motor_board_input_pin_IN1, OUTPUT);
  pinMode(motor_board_input_pin_IN4, OUTPUT);
  pinMode(motor_board_input_pin_IN3, OUTPUT);
  pinMode(TrigPin1, OUTPUT); 
  pinMode(EchoPin1, INPUT);  
  pinMode(HitAPin, INPUT_PULLUP); 
  pinMode(HitBPin, INPUT_PULLUP); 
  PCICR |= B00000100; 
  PCMSK2 |= B00010100; 
}
 
void loop()
{
delay(1);
Distance1 = getDistance(EchoPin1,TrigPin1);
IR1_Val = analogRead(IR1Pin);
IR2_Val = analogRead(IR2Pin);
bruhstop();
}
 
void Turn_left()
{
  digitalWrite(motor_board_input_pin_IN2, HIGH);
  analogWrite(motor_board_input_pin_IN1, 150); //motor A higher forward speed
  digitalWrite(motor_board_input_pin_IN3, LOW);
  analogWrite(motor_board_input_pin_IN4, 0);// motor B lower reverse speed
  delay(80);
}
void Turn_Right()
{
  digitalWrite(motor_board_input_pin_IN2, LOW);
  analogWrite(motor_board_input_pin_IN1, 0); //motor A lower forward speed
  digitalWrite(motor_board_input_pin_IN3, HIGH);
  analogWrite(motor_board_input_pin_IN4, 150);// motor B higher reverse speed
  delay(80);
}

void Go_straight()
{
  digitalWrite(motor_board_input_pin_IN2, HIGH);
  analogWrite(motor_board_input_pin_IN1, 190);
  digitalWrite(motor_board_input_pin_IN3, HIGH);
  analogWrite(motor_board_input_pin_IN4, 190);
}

void IRSense()
{
  if (IR1_Val > 830 && IR2_Val <= 830) {
    Turn_left();
  } else if (IR2_Val > 830 && IR1_Val <= 830) {
    Turn_Right();
  } else if (IR1_Val > 830 && IR2_Val > 830) {
    Go_straight();  // Stop if both sensors detect obstacles
  } else {
    Go_straight();
  }
}

int getDistance(int EchoPin, int TrigPin)
{
  int distance;
  long pulseDuration; //variable needed by the ultrasound sensor code
  // 1. Produce a 10 us (micro-second) HIGH pulse to trigger the sensor...
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
 
  pulseDuration = pulseIn(EchoPin, HIGH);
  distance = pulseDuration / 58;  //  Convert to cm
 
  return distance;
}

void stop(){
  digitalWrite(motor_board_input_pin_IN2, HIGH);
  digitalWrite(motor_board_input_pin_IN1, HIGH);
  digitalWrite(motor_board_input_pin_IN3, HIGH);
  digitalWrite(motor_board_input_pin_IN4, HIGH);
  delay(50);
}

void bruhstop(){
  if (Distance1 < 20){
    for(;counterA==0 && counterB==0;){
      stop();
    }
    counterA=0;
    counterB=0;
  }
  else{
    IRSense();
  }
  delay(50);
}

ISR (PCINT2_vect){
if (!digitalRead(HitBPin)) // if Hit_bPin is 0, Target B receives hit
counterB ++; //increase counterB by 1
else if (!digitalRead(HitAPin)) // if Hit_aPin is 0, Target A receives hit
counterA ++; //increase counterA by 1
}