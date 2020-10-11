
#include <SoftwareSerial.h>
#include <Multiservo.h>

SoftwareSerial bluetooth(12, 13); // RX, TX

const byte MA_PWM = 10, MA_F = 8, MA_B = 7;
const byte MB_PWM = 9, MB_F = 6, MB_B = 5;
byte opcode = 0;
byte SPEED = 128;
Multiservo ServoS[2];

void setup() 
{
  
  pinMode(MA_PWM, OUTPUT);
  pinMode(MA_F, OUTPUT);
  pinMode(MA_B, OUTPUT);

  pinMode(MB_PWM, OUTPUT);
  pinMode(MB_F, OUTPUT);
  pinMode(MB_B, OUTPUT);

  ServoS[0].attach(0);
  ServoS[1].attach(1);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  bluetooth.print("Init completed");
}



void Move(bool dir)//0 - forward, 1 - backward
{
  analogWrite(MA_PWM, SPEED);
  analogWrite(MB_PWM, SPEED);
  
  digitalWrite(MA_F, dir);
  digitalWrite(MA_B, !dir);
  digitalWrite(MB_F, !dir);
  digitalWrite(MB_B, dir);
  
  delay(1000);
  
  digitalWrite(MA_F, 0);
  digitalWrite(MA_B, 0);
  digitalWrite(MB_F, 0);
  digitalWrite(MB_B, 0);
}

void Rotate(bool dir)//0 - right, 1 - left
{
  analogWrite(MA_PWM, SPEED);
  analogWrite(MB_PWM, SPEED);
  
  digitalWrite(MA_F, dir);
  digitalWrite(MA_B, !dir);
  digitalWrite(MB_F, dir);
  digitalWrite(MB_B, !dir);

  delay(1000);
  
  digitalWrite(MA_F, 0);
  digitalWrite(MA_B, 0);
  digitalWrite(MB_F, 0);
  digitalWrite(MB_B, 0);
}

void RunServo(int n)
{
  ServoS[n].write(abs(ServoS[n].read()-180));
}

void loop() 
{
 /* if(Serial.available() > 0)
  {
    opcode = Serial.read();
    Serial.print(int(opcode));
  
    switch (opcode - 48)
    {
      case 1:
        Move(0);
        break;
      case 2:
        Move(1);
        break;
    }
    opcode = 0;
  }*/
  if (bluetooth.available() >= 1)
  {
    opcode = bluetooth.read();
    //value = bluetooth.read();
    
    bluetooth.print("Got data");
    bluetooth.print(opcode);
    
    switch (opcode)
    {
      case 1:
        Move(1);
        break;
      case 2:
        Move(0);
        break;
      case 3:
        Rotate(1);
        break;
      case 4:
        Rotate(0);
        break;
      case 5:
        RunServo(0);
        break;
      case 6:
        RunServo(1);
        break;
    }
  }
  
  opcode = 0;
  //value = 0;
}
