
#include <SoftwareSerial.h>
#include <Multiservo.h>

SoftwareSerial bluetooth(12, 13); // RX, TX

const int LED_PIN = 11;
const byte MA_PWM = 10, MA_F = 8, MA_B = 7;
const byte MB_PWM = 9, MB_F = 6, MB_B = 5;
byte opcode = 0;
byte SPEED = 64;
int SERVO_STEP = 15, pos1 = 180, pos2 = 130, Floor = 2;
const int SPEEDING_TIME = 1000;
const int SERVO_PIN_1 = 0, SERVO_PIN_2 = 1;
Multiservo ServoS[2];

void setup() 
{
  pinMode(MA_PWM, OUTPUT);
  pinMode(MA_F, OUTPUT);
  pinMode(MA_B, OUTPUT);

  pinMode(MB_PWM, OUTPUT);
  pinMode(MB_F, OUTPUT);
  pinMode(MB_B, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, 0);
  
  ServoS[0].attach(SERVO_PIN_1);
  ServoS[1].attach(SERVO_PIN_2);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  ServoS[0].write(pos1);
  delay(100);
  ServoS[1].write(pos2);
  bluetooth.flush();
  Serial.flush();
  bluetooth.println("Init completed");
  Serial.println("Init completed");
}

int func(unsigned long timer)
{
  if(timer > SPEEDING_TIME) return 255;
  else if(timer < 250) return 2*SPEED;
  else return (((double)(255 - SPEED)*timer/SPEEDING_TIME + SPEED));
  //delay(10);
}

void Move(bool dir)//0 - forward, 1 - backward
{
  digitalWrite(MA_F, dir);
  digitalWrite(MA_B, !dir);
  digitalWrite(MB_F, !dir);
  digitalWrite(MB_B, dir);

  unsigned long start_time = millis();
  
  while(bluetooth.available() < 1)
  { 
    unsigned long current_time = millis() - start_time;
    int cur_spd = func(current_time);
    analogWrite(MA_PWM, cur_spd);
    analogWrite(MB_PWM, cur_spd);
    //bluetooth.print(cur_spd);
    //bluetooth.print(current_time);
    delay(5);
  }

    delay(50);
    digitalWrite(MA_F, 0);
    digitalWrite(MA_B, 0);
    digitalWrite(MB_F, 0);
    digitalWrite(MB_B, 0);
}

void Slowing()
{
  byte cur_spd = 255;
  while(bluetooth.available() < 1)
  {
    if(cur_spd > SPEED) 
    {
      cur_spd -= 1;
      analogWrite(MA_PWM, cur_spd);
      analogWrite(MB_PWM, cur_spd);
      delay(250);
    }
    else
    {
      analogWrite(MA_PWM, SPEED);
      analogWrite(MB_PWM, SPEED);
      break;
    }
  }
}


void Rotate(bool dir)//0 - right, 1 - left
{
  analogWrite(MA_PWM, 2*SPEED);
  analogWrite(MB_PWM, 2*SPEED); 
  
  digitalWrite(MA_F, dir);
  digitalWrite(MA_B, !dir);
  digitalWrite(MB_F, dir);
  digitalWrite(MB_B, !dir);

  while(bluetooth.available() < 1) delay(100);
  
  digitalWrite(MA_F, 0);
  digitalWrite(MA_B, 0);
  digitalWrite(MB_F, 0);
  digitalWrite(MB_B, 0);
}

void ServoMove(int n, bool dir)
{
  //Serial.print(ServoS[0].read());
  //Serial.print("   ");
  //Serial.println(ServoS[1].read());
  
  if(n == 0)
  {
    if(dir) ServoS[n].write(50);
    else ServoS[n].write(pos1);
  }
  if(n == 1)
  {
    if(dir)ServoS[n].write(pos2); 
    else ServoS[n].write(180);   
  }
  delay(25);
  
  //Serial.print(ServoS[0].read());
  //Serial.print("   ");
  //Serial.println(ServoS[1].read());
}

void Short()
{
  digitalWrite(LED_PIN, 1);
  delay(75);
  digitalWrite(LED_PIN, 0);
  delay(200);
}

void Long()
{
  digitalWrite(LED_PIN, 1);
  delay(300);
  digitalWrite(LED_PIN, 0);
  delay(200);
}

void Funny()
{
  Long();
  Short();
  
  Short();
  Short();

  Long();
  Short();
  Long();

  Short();
  Short();

  Long();

  Short();
  Long();
}

void loop() 
{
  if (bluetooth.available() >= 1)
  {
    opcode = bluetooth.read();
    //Serial.flush();
    //bluetooth.flush();
    //value = bluetooth.read();
    
    //bluetooth.print("Got data");
    //bluetooth.print(opcode);
    //Serial.println(opcode);
    
    switch (opcode)
    {
      case 1:
        Move(0);
        break;
        
      case 2:
        Move(1);
        break;
        
      case 3:
        Rotate(0);
        break;
        
      case 4:
        Rotate(1);
        break;
        
      case 5:
        Slowing();
        break;

      case 6:
        ServoMove(0, 1);
        break;
      
      case 7:
        ServoMove(0, 0);
        break;

      case 8:
        ServoMove(1, 0);
        break;

      case 9:
        ServoMove(1, 1);
        break;
      
      case 10:
        Funny();
        break;

      case 11:
        //Stop Servos, Stop Rotate
        break;

      case 12:
        ServoS[0].write(105);
        break;
    }
  }
  
  opcode = 0;
  delay(5);
  //value = 0;
}
