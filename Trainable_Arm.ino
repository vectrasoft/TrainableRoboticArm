#include <Servo.h>
#include <EEPROM.h>

struct superServo{
    Servo srv;
    int minDeg;
    int maxDeg;
    int minFeed;
    int maxFeed;
    int pos;
    int pos1;
} servos[5];

int addr = 0;
boolean recorded = false;
int val;

void setup(){
  Serial.begin(9600);

  servos[0].srv.attach(8);
  servos[1].srv.attach(9);
  servos[2].srv.attach(10);
  servos[3].srv.attach(11);
  servos[4].srv.attach(12);

  pinMode(13, OUTPUT);  // LED
  pinMode(6, INPUT);    // Replay Button
  pinMode(7, INPUT);    // Train Button

  delay(100);

  for(int j = 0; j<5; j++){
    // Turn servo left and check values
    for (int i = 90; i--; delay(10))
      servos[j].srv.write(i);
      
    servos[j].minDeg = 0;
    servos[j].minFeed = analogRead(j+1);
    delay(200);

    // Turn servo right and check values
    for (int i = 30; i++ < 181; delay(10))
      servos[j].srv.write(i);

    servos[j].maxDeg = 180;
    servos[j].maxFeed = analogRead(j+1);
    delay(200);

    // Turn to middle value
    for (int i = 180; i-- > 90; delay(10))
      servos[j].srv.write(i);

    delay(200);
  }

  delay(1300);

  // Detach to save power and allow human manipulation
  servos[0].srv.detach();
  servos[1].srv.detach();
  servos[2].srv.detach();
  servos[3].srv.detach();
  servos[4].srv.detach();
  /*
  // Display minimums and maximums for analog feedback
  // Uncomment for debugging
  Serial.print("servos[0].srv Min: ");
  Serial.println(servos[0].minFeed);
  Serial.print("servos[0].srv Max: ");
  Serial.println(servos[0].maxFeed);
  Serial.print("servos[1].srv Min: ");
  Serial.println(servos[1].minFeed);
  Serial.print("servos[1].srv Max: ");
  Serial.println(servos[1].maxFeed);
  Serial.print("servos[2].srv Min: ");
  Serial.println(servos[2].minFeed);
  Serial.print("servos[2].srv Max: ");
  Serial.println(servos[2].maxFeed);
  Serial.print("servos[3].srv Min: ");
  Serial.println(servos[3].minFeed);
  Serial.print("servos[3].srv Max: ");
  Serial.println(servos[3].maxFeed);
  Serial.print("servos[4].srv Min: ");
  Serial.println(servos[4].minFeed);
  Serial.print("servos[4].srv Max: ");
  Serial.println(servos[4].maxFeed);
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
      int servos[0].pos = map(analogRead(1), servos[0].minFeed, servos[0].maxFeed, servos[0].minDeg, servos[0].maxDeg);
      servos[0].pos = constrain(servos[0].pos, 0, 180);
      EEPROM.write(addr, servos[0].pos);
      addr++;
      int servos[1].pos = map(analogRead(2), servos[1].minFeed, servos[1].maxFeed, servos[1].minDeg, servos[1].maxDeg);
      servos[1].pos = constrain(servos[1].pos, 0, 180);
      EEPROM.write(addr, servos[1].pos);
      addr++;
      int servos[2].pos = map(analogRead(3), servos[2].minFeed, servos[2].maxFeed, servos[2].minDeg, servos[2].maxDeg);
      servos[2].pos = constrain(servos[2].pos, 0, 180);
      EEPROM.write(addr, servos[2].pos);
      addr++;
      int servos[3].pos = map(analogRead(4), servos[3].minFeed, servos[3].maxFeed, servos[3].minDeg, servos[3].maxDeg);
      servos[3].pos = constrain(servos[3].pos, 0, 180);
      EEPROM.write(addr, servos[3].pos);
      addr++;
      int servos[4].pos = map(analogRead(5), servos[4].minFeed, servos[4].maxFeed, servos[4].minDeg, servos[4].maxDeg);
      servos[4].pos = constrain(servos[4].pos, 0, 180);
      EEPROM.write(addr, servos[4].pos);
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
      Serial.print(servos[0].pos);
      Serial.print("\t");
      Serial.print(servos[1].pos);
      Serial.print("\t");
      Serial.print(servos[2].pos);
      Serial.print("\t");
      Serial.print(servos[3].pos);
      Serial.print("\t");
      Serial.print(servos[4].pos);
      Serial.println();
      */
    }
    EEPROM.write(addr, 255);
  }
  if (recorded || digitalRead(6))
  {
    digitalWrite(13, LOW);
    // Power up servos
    servos[0].srv.attach(8);
    servos[1].srv.attach(9);
    servos[2].srv.attach(10);
    servos[3].srv.attach(11);
    servos[4].srv.attach(12);
    delay(1000);
    // Center servos
    servos[0].srv.write(90);
    servos[1].srv.write(90);
    servos[2].srv.write(90);
    servos[3].srv.write(90);
    servos[4].srv.write(90);
    delay(1000);
    // Start playback
    addr = 0;
    while (1)
    {
      servos[0].pos = EEPROM.read(addr);
      servos[0].pos1 = EEPROM.read(addr+5);
      addr++;
      servos[1].pos = EEPROM.read(addr);
      servos[1].pos1 = EEPROM.read(addr+5);
      addr++;
      servos[2].pos = EEPROM.read(addr);
      servos[2].pos1 = EEPROM.read(addr+5);
      addr++;
      servos[3].pos = EEPROM.read(addr);
      servos[3].pos1 = EEPROM.read(addr+5);
      addr++;
      servos[4].pos = EEPROM.read(addr);
      servos[4].pos1 = EEPROM.read(addr+5);
      addr++;
      /*
      // Display positions being written to the servos
      // Uncomment for debugging
      Serial.print("servos[0].srv: ");
      Serial.print(servos[0].pos);
      Serial.print("\t\t\t");
      Serial.println(servos[0].pos1);
      Serial.print("servos[1].srv: ");
      Serial.print(servos[1].pos);
      Serial.print("\t\t");
      Serial.println(servos[1].pos1);
      Serial.print("servos[2].srv: ");
      Serial.print(servos[2].pos);
      Serial.print("\t\t");
      Serial.println(servos[2].pos1);
      Serial.print("servos[3].srv: ");
      Serial.print(servos[3].pos);
      Serial.print("\t\t");
      Serial.println(servos[3].pos1);
      Serial.print("servos[4].srv: ");
      Serial.print(servos[4].pos);
      Serial.print("\t\t");
      Serial.println(servos[4].pos1);
      Serial.println();
      */
      
      // Check for the end of the recorded commands, if so then break out of the infinite loop
      if ((servos[0].pos == 255) || (servos[0].pos1 == 255) || (servos[1].pos == 255) || (servos[1].pos1 == 255) || (servos[2].pos == 255) || (servos[2].pos1 == 255) || (servos[3].pos == 255) || (servos[3].pos1 == 255) || (servos[4].pos == 255) || (servos[4].pos1 == 255))
      {
        break;
      }
      
      // Step from servos[0].srv recording to the next for each servo
      if ((servos[0].pos1 - servos[0].pos) > 0)
      {
        for (int i = servos[0].pos; i < servos[0].pos1; i++)
        {
          servos[0].srv.write(i);
          delay(5);
        }
      }   
      else if ((servos[0].pos1 - servos[0].pos) < 0)
      {
        for (int i = servos[0].pos; i > servos[0].pos1; i--)
        {
          servos[0].srv.write(i);
          delay(5);
        }
      }
      if ((servos[1].pos1 - servos[1].pos) > 0)
      {
        for (int i = servos[1].pos; i < servos[1].pos1; i++)
        {
          servos[1].srv.write(i);
          delay(5);
        }
      }   
      else if ((servos[1].pos1 - servos[1].pos) < 0)
      {
        for (int i = servos[1].pos; i > servos[1].pos1; i--)
        {
          servos[1].srv.write(i);
          delay(5);
        }
      }
      if ((servos[2].pos1 - servos[2].pos) > 0)
      {
        for (int i = servos[2].pos; i < servos[2].pos1; i++)
        {
          servos[2].srv.write(i);
          delay(5);
        }
      }   
      else if ((servos[2].pos1 - servos[2].pos) < 0)
      {
        for (int i = servos[2].pos; i > servos[2].pos1; i--)
        {
          servos[2].srv.write(i);
          delay(5);
        }
      }
      if ((servos[3].pos1 - servos[3].pos) > 0)
      {
        for (int i = servos[3].pos; i < servos[3].pos1; i++)
        {
          servos[3].srv.write(i);
          delay(5);
        }
      }   
      else if ((servos[3].pos1 - servos[3].pos) < 0)
      {
        for (int i = servos[3].pos; i > servos[3].pos1; i--)
        {
          servos[3].srv.write(i);
          delay(5);
        }
      }
      if ((servos[4].pos1 - servos[4].pos) > 0)
      {
        for (int i = servos[4].pos; i < servos[4].pos1; i++)
        {
          servos[4].srv.write(i);
          delay(5);
        }
      }   
      else if ((servos[4].pos1 - servos[4].pos) < 0)
      {
        for (int i = servos[4].pos; i > servos[4].pos1; i--)
        {
          servos[4].srv.write(i);
          delay(5);
        }
      }
    }
    recorded = false;
    addr = 0;
    delay(500);
    // Center all servos
    servos[0].srv.write(90);
    servos[1].srv.write(90);
    servos[2].srv.write(90);
    servos[3].srv.write(90);
    servos[4].srv.write(90);
    delay(1000);
    // Detach them to save power and allow human manipulation
    servos[0].srv.detach();
    servos[1].srv.detach();
    servos[2].srv.detach();
    servos[3].srv.detach();
    servos[4].srv.detach();
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

