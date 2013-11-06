#include <Servo.h>
#include <EEPROM.h>

Servo one;
Servo two;
Servo three;
Servo four;
Servo five;
Servo claw;

// Calibration values
int minDegOne, minDegTwo, minDegThree, minDegFour, minDegFive, minDegClaw;
int maxDegOne, maxDegTwo, maxDegThree, maxDegFour, maxDegFive, maxDegClaw;
int minFeedOne, minFeedTwo, minFeedThree, minFeedFour, minFeedFive, minFeedClaw;
int maxFeedOne, maxFeedTwo, maxFeedThree, maxFeedFour, maxFeedFive, maxFeedClaw;
int posOne, posTwo, posThree, posFour, posFive;
int tolerance = 2; // max feedback measurement error
int addr = 0;
int motion = 0;
int temp = 0;

void setup()
{
  Serial.begin(9600);
  claw.attach(7);
  one.attach(8);
  two.attach(9);
  three.attach(10);
  four.attach(11);
  five.attach(12);
  pinMode(13, OUTPUT);  // LED
  pinMode(7, INPUT);    // Button
  delay(100);
  
  // One center to left
  for (int i = 90; i > 29; i--)
  {
    one.write(i);
    //Serial.println(i);
    delay(10);
  }
  minDegOne = 30;
  minFeedOne = analogRead(1);
  //Serial.println(analogRead(2));
  delay(500);
  // One left to right
  for (int i = 30; i < 151; i++)
  {
    one.write(i);
    delay(20);
  }
  maxDegOne = 150;
  maxFeedOne = analogRead(1);
  delay(500);
  // One right to center
  for (int i = 150; i > 89; i--)
  {
    one.write(i);
    delay(20);
  }
  delay(500);
  
  // Two up to forward
  for (int i = 90; i > 29; i--)
  {
    two.write(i);
    delay(10);
  }
  minDegTwo = 30;
  minFeedTwo = analogRead(2);
  delay(500);
  // Two forward to backward
  for (int i = 30; i < 151; i++)
  {
    two.write(i);
    delay(20);
  }
  maxDegTwo = 150;
  maxFeedTwo = analogRead(2);
  delay(500);
  // Two backward to up
  for (int i = 150; i > 89; i--)
  {
    two.write(i);
    delay(20);
  }
  delay(500);
  // Three up to forward
  for (int i = 90; i > 29; i--)
  {
    three.write(i);
    delay(10);
  }
  minDegThree = 30;
  minFeedThree = analogRead(3);
  delay(500);
  // Three forward to backward
  for (int i = 30; i < 151; i++)
  {
    three.write(i);
    delay(20);
  }
  maxDegThree = 150;
  maxFeedThree = analogRead(3);
  delay(500);
  // Three backward to up
  for (int i = 150; i > 89; i--)
  {
    three.write(i);
    delay(20);
  }
  delay(500);  
  // Four up to forward
  for (int i = 90; i > 29; i--)
  {
    four.write(i);
    delay(10);
  }
  minDegFour = 30;
  minFeedFour = analogRead(4);
  delay(500);
  // Four forward to backward
  for (int i = 30; i < 151; i++)
  {
    four.write(i);
    delay(20);
  }
  maxDegFour = 150;
  maxFeedFour = analogRead(4);
  delay(500);
  // Four backward to up
  for (int i = 150; i > 89; i--)
  {
    four.write(i);
    delay(20);
  }
  delay(500);
  // Five up to forward
  for (int i = 90; i > 29; i--)
  {
    five.write(i);
    delay(10);
  }
  minDegFive = 30;
  minFeedFive = analogRead(5);
  delay(500);
  // Five forward to backward
  for (int i = 30; i < 151; i++)
  {
    five.write(i);
    delay(20);
  }
  maxDegFive = 150;
  maxFeedFive = analogRead(5);
  delay(500);
  // Five backward to up
  for (int i = 150; i > 89; i--)
  {
    five.write(i);
    delay(20);
  }
  delay(500);
  
  // Center all servos
  one.write(90);
  two.write(90);
  three.write(90);
  four.write(90);
  five.write(90);
  delay(1000);
  one.detach();
  two.detach();
  three.detach();
  four.detach();
  five.detach();
}

void loop()
{
  temp = digitalRead(7);
  delay(100);
  if (temp)
  {
    motion = 1;
    digitalWrite(13, HIGH);
    delay(2000);
    while (!digitalRead(7))
    {
      delay(100);
      int posOne = map(analogRead(1), minFeedOne, maxFeedOne, minDegOne, maxDegOne);
      EEPROM.write(addr, posOne);
      addr++;
      int posTwo = map(analogRead(2), minFeedTwo, maxFeedTwo, minDegTwo, maxDegTwo);
      EEPROM.write(addr, posTwo);
      addr++;
      int posThree = map(analogRead(3), minFeedThree, maxFeedThree, minDegThree, maxDegThree);
      EEPROM.write(addr, posThree);
      addr++;
      int posFour = map(analogRead(4), minFeedFour, maxFeedFour, minDegFour, maxDegFour);
      EEPROM.write(addr, posFour);
      addr++;
      int posFive = map(analogRead(5), minFeedFive, maxFeedFive, minDegFive, maxDegFive);
      EEPROM.write(addr, posFive);
      addr++;
      if (addr == 512)
      {
        EEPROM.write(addr, 255);
        break;
      }
      delay(100);
    }
    EEPROM.write(addr, 255);
  }
  if (motion || digitalRead(6))
  {
    digitalWrite(13, LOW);
    Serial.println("Playing back motion!");
    one.attach(8);
    two.attach(9);
    three.attach(10);
    four.attach(11);
    five.attach(12);
    delay(1000);
    one.write(90);
    two.write(90);
    three.write(90);
    four.write(90);
    five.write(90);
    delay(1000);
    // Start playback
    addr = 0;
    while (1)
    {
      temp = EEPROM.read(addr);
      addr++;
      if (temp == 255)
      {
        Serial.println("End of recording!");
        break;
      }
      posOne = temp;
      posTwo = EEPROM.read(addr);
      addr++;
      posThree = EEPROM.read(addr);
      addr++;
      posFour = EEPROM.read(addr);
      addr++;
      posFive = EEPROM.read(addr);
      addr++;
      one.write(posOne);
      two.write(posTwo);
      three.write(posThree);
      four.write(posFour);
      five.write(posFive);
      delay(200);
    }
    motion = 0;
    addr = 0;
    delay(500);
    // Center all servos
    one.write(90);
    two.write(90);
    three.write(90);
    four.write(90);
    five.write(90);
    delay(500);
    // Detach them to save power
    one.detach();
    two.detach();
    three.detach();
    four.detach();
    five.detach();
  }
/*
// Original Code from October 31st 2013
  digitalWrite(13, HIGH);
  one.detach();
  two.detach();
  three.detach();
  four.detach();
  five.detach();
  delay(5000);
  digitalWrite(13, LOW);
  one.attach(8);
  two.attach(9);
  three.attach(10);
  four.attach(11);
  five.attach(12);
  int posOne = map(analogRead(1), minFeedOne, maxFeedOne, minDegOne, maxDegOne);
  int posTwo = map(analogRead(2), minFeedTwo, maxFeedTwo, minDegTwo, maxDegTwo);
  int posThree = map(analogRead(3), minFeedThree, maxFeedThree, minDegThree, maxDegThree);
  int posFour = map(analogRead(4), minFeedFour, maxFeedFour, minDegFour, maxDegFour);
  int posFive = map(analogRead(5), minFeedFive, maxFeedFive, minDegFive, maxDegFive);
  Serial.print("pos1: ");
  Serial.println(posOne);
  Serial.print("pos2: ");
  Serial.println(posTwo);
  Serial.print("pos3: ");
  Serial.println(posThree);
  Serial.print("pos4: ");
  Serial.println(posFour);
  Serial.print("pos5: ");
  Serial.println(posFive);
  Serial.print("\n\n");
  
  delay(1000);
  one.write(90);
  two.write(90);
  three.write(90);
  four.write(90);
  five.write(90);
  delay(1000);
  one.write(posOne);
  two.write(posTwo);
  three.write(posThree);
  four.write(posFour);
  five.write(posFive);
  delay(4000);
*/

}

