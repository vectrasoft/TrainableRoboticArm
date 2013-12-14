#include <Servo.h>
#include <EEPROM.h>

// Servo declarations
Servo one;
Servo two;
Servo three;
Servo four;
Servo five;
// Calibration values
int minDegOne, minDegTwo, minDegThree, minDegFour, minDegFive;
int maxDegOne, maxDegTwo, maxDegThree, maxDegFour, maxDegFive;
int minFeedOne, minFeedTwo, minFeedThree, minFeedFour, minFeedFive;
int maxFeedOne, maxFeedTwo, maxFeedThree, maxFeedFour, maxFeedFive;
int posOne, posTwo, posThree, posFour, posFive;
int posOne1, posTwo1, posThree1, posFour1, posFive1;
int addr = 0;
boolean recorded = false;
int val;

void setup()
{
  Serial.begin(9600);
  one.attach(8);
  two.attach(9);
  three.attach(10);
  four.attach(11);
  five.attach(12);
  pinMode(13, OUTPUT);  // LED
  pinMode(6, INPUT);    // Replay Button
  pinMode(7, INPUT);    // Train Button
  delay(100);
  // One center to left
  for (int i = 90; i > -1; i--)
  {
    one.write(i);
    delay(10);
  }
  minDegOne = 0;
  minFeedOne = analogRead(1);
  delay(200);
  // One left to right
  for (int i = 30; i < 181; i++)
  {
    one.write(i);
    delay(10);
  }
  maxDegOne = 180;
  maxFeedOne = analogRead(1);
  delay(200);
  // One right to center
  for (int i = 180; i > 89; i--)
  {
    one.write(i);
    delay(10);
  }
  delay(200);
  // Two up to forward
  for (int i = 90; i > -1; i--)
  {
    two.write(i);
    delay(10);
  }
  minDegTwo = 0;
  minFeedTwo = analogRead(2);
  delay(200);
  // Two forward to backward
  for (int i = 0; i < 181; i++)
  {
    two.write(i);
    delay(10);
  }
  maxDegTwo = 180;
  maxFeedTwo = analogRead(2);
  delay(200);
  // Two backward to up
  for (int i = 180; i > 89; i--)
  {
    two.write(i);
    delay(10);
  }
  delay(200);
  // Three up to forward
  for (int i = 90; i > -1; i--)
  {
    three.write(i);
    delay(10);
  }
  minDegThree = 0;
  minFeedThree = analogRead(3);
  delay(200);
  // Three forward to backward
  for (int i = 0; i < 181; i++)
  {
    three.write(i);
    delay(10);
  }
  maxDegThree = 180;
  maxFeedThree = analogRead(3);
  delay(200);
  // Three backward to up
  for (int i = 180; i > 89; i--)
  {
    three.write(i);
    delay(10);
  }
  delay(200);  
  // Four up to forward
  for (int i = 90; i > -1; i--)
  {
    four.write(i);
    delay(10);
  }
  minDegFour = 0;
  minFeedFour = analogRead(4);
  delay(200);
  // Four forward to backward
  for (int i = 0; i < 181; i++)
  {
    four.write(i);
    delay(10);
  }
  maxDegFour = 180;
  maxFeedFour = analogRead(4);
  delay(200);
  // Four backward to up
  for (int i = 180; i > 89; i--)
  {
    four.write(i);
    delay(10);
  }
  delay(200);
  // Five up to forward
  for (int i = 90; i > -1; i--)
  {
    five.write(i);
    delay(10);
  }
  minDegFive = 0;
  minFeedFive = analogRead(5);
  delay(200);
  // Five forward to backward
  for (int i = 0; i < 181; i++)
  {
    five.write(i);
    delay(10);
  }
  maxDegFive = 180;
  maxFeedFive = analogRead(5);
  delay(200);
  // Five backward to up
  for (int i = 180; i > 89; i--)
  {
    five.write(i);
    delay(10);
  }
  delay(500);
  // Center all servos
  one.write(90);
  two.write(90);
  three.write(90);
  four.write(90);
  five.write(90);
  delay(1000);
  // Detach to save power and allow human manipulation
  one.detach();
  two.detach();
  three.detach();
  four.detach();
  five.detach();
  /*
  // Display minimums and maximums for analog feedback
  // Uncomment for debugging
  Serial.print("One Min: ");
  Serial.println(minFeedOne);
  Serial.print("One Max: ");
  Serial.println(maxFeedOne);
  Serial.print("Two Min: ");
  Serial.println(minFeedTwo);
  Serial.print("Two Max: ");
  Serial.println(maxFeedTwo);
  Serial.print("Three Min: ");
  Serial.println(minFeedThree);
  Serial.print("Three Max: ");
  Serial.println(maxFeedThree);
  Serial.print("Four Min: ");
  Serial.println(minFeedFour);
  Serial.print("Four Max: ");
  Serial.println(maxFeedFour);
  Serial.print("Five Min: ");
  Serial.println(minFeedFive);
  Serial.print("Five Max: ");
  Serial.println(maxFeedFive);
  Serial.println();
  */
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }
}

void loop()
{
  delay(100);
  if (digitalRead(7))
  {
    recorded = true;
    digitalWrite(13, HIGH);
    delay(1000);
    while (!digitalRead(7))
    {
      delay(50);
      int posOne = map(analogRead(1), minFeedOne, maxFeedOne, minDegOne, maxDegOne);
      posOne = constrain(posOne, 0, 180);
      EEPROM.write(addr, posOne);
      addr++;
      int posTwo = map(analogRead(2), minFeedTwo, maxFeedTwo, minDegTwo, maxDegTwo);
      posTwo = constrain(posTwo, 0, 180);
      EEPROM.write(addr, posTwo);
      addr++;
      int posThree = map(analogRead(3), minFeedThree, maxFeedThree, minDegThree, maxDegThree);
      posThree = constrain(posThree, 0, 180);
      EEPROM.write(addr, posThree);
      addr++;
      int posFour = map(analogRead(4), minFeedFour, maxFeedFour, minDegFour, maxDegFour);
      posFour = constrain(posFour, 0, 180);
      EEPROM.write(addr, posFour);
      addr++;
      int posFive = map(analogRead(5), minFeedFive, maxFeedFive, minDegFive, maxDegFive);
      posFive = constrain(posFive, 0, 180);
      EEPROM.write(addr, posFive);
      addr++;
      if (addr == 512)
      {
        EEPROM.write(addr, 255);
        break;
      }
      delay(50);
      /*
      // Display recorded values
      // Uncomment for debugging
      Serial.print(posOne);
      Serial.print("\t");
      Serial.print(posTwo);
      Serial.print("\t");
      Serial.print(posThree);
      Serial.print("\t");
      Serial.print(posFour);
      Serial.print("\t");
      Serial.print(posFive);
      Serial.println();
      */
    }
    EEPROM.write(addr, 255);
  }
  if (recorded || digitalRead(6))
  {
    digitalWrite(13, LOW);
    // Power up servos
    one.attach(8);
    two.attach(9);
    three.attach(10);
    four.attach(11);
    five.attach(12);
    delay(1000);
    // Center servos
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
      posOne = EEPROM.read(addr);
      posOne1 = EEPROM.read(addr+5);
      addr++;
      posTwo = EEPROM.read(addr);
      posTwo1 = EEPROM.read(addr+5);
      addr++;
      posThree = EEPROM.read(addr);
      posThree1 = EEPROM.read(addr+5);
      addr++;
      posFour = EEPROM.read(addr);
      posFour1 = EEPROM.read(addr+5);
      addr++;
      posFive = EEPROM.read(addr);
      posFive1 = EEPROM.read(addr+5);
      addr++;
      /*
      // Display positions being written to the servos
      // Uncomment for debugging
      Serial.print("One: ");
      Serial.print(posOne);
      Serial.print("\t\t\t");
      Serial.println(posOne1);
      Serial.print("Two: ");
      Serial.print(posTwo);
      Serial.print("\t\t");
      Serial.println(posTwo1);
      Serial.print("Three: ");
      Serial.print(posThree);
      Serial.print("\t\t");
      Serial.println(posThree1);
      Serial.print("Four: ");
      Serial.print(posFour);
      Serial.print("\t\t");
      Serial.println(posFour1);
      Serial.print("Five: ");
      Serial.print(posFive);
      Serial.print("\t\t");
      Serial.println(posFive1);
      Serial.println();
      */
      
      // Check for the end of the recorded commands, if so then break out of the infinite loop
      if ((posOne == 255) || (posOne1 == 255) || (posTwo == 255) || (posTwo1 == 255) || (posThree == 255) || (posThree1 == 255) || (posFour == 255) || (posFour1 == 255) || (posFive == 255) || (posFive1 == 255))
      {
        break;
      }
      
      // Step from one recording to the next for each servo
      if ((posOne1 - posOne) > 0)
      {
        for (int i = posOne; i < posOne1; i++)
        {
          one.write(i);
          delay(5);
        }
      }   
      else if ((posOne1 - posOne) < 0)
      {
        for (int i = posOne; i > posOne1; i--)
        {
          one.write(i);
          delay(5);
        }
      }
      if ((posTwo1 - posTwo) > 0)
      {
        for (int i = posTwo; i < posTwo1; i++)
        {
          two.write(i);
          delay(5);
        }
      }   
      else if ((posTwo1 - posTwo) < 0)
      {
        for (int i = posTwo; i > posTwo1; i--)
        {
          two.write(i);
          delay(5);
        }
      }
      if ((posThree1 - posThree) > 0)
      {
        for (int i = posThree; i < posThree1; i++)
        {
          three.write(i);
          delay(5);
        }
      }   
      else if ((posThree1 - posThree) < 0)
      {
        for (int i = posThree; i > posThree1; i--)
        {
          three.write(i);
          delay(5);
        }
      }
      if ((posFour1 - posFour) > 0)
      {
        for (int i = posFour; i < posFour1; i++)
        {
          four.write(i);
          delay(5);
        }
      }   
      else if ((posFour1 - posFour) < 0)
      {
        for (int i = posFour; i > posFour1; i--)
        {
          four.write(i);
          delay(5);
        }
      }
      if ((posFive1 - posFive) > 0)
      {
        for (int i = posFive; i < posFive1; i++)
        {
          five.write(i);
          delay(5);
        }
      }   
      else if ((posFive1 - posFive) < 0)
      {
        for (int i = posFive; i > posFive1; i--)
        {
          five.write(i);
          delay(5);
        }
      }
    }
    recorded = false;
    addr = 0;
    delay(500);
    // Center all servos
    one.write(90);
    two.write(90);
    three.write(90);
    four.write(90);
    five.write(90);
    delay(1000);
    // Detach them to save power and allow human manipulation
    one.detach();
    two.detach();
    three.detach();
    four.detach();
    five.detach();
    // Flash the LED to let user know replay is completed
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
  }
}

