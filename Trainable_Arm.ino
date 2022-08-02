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

  for(int i=0; i<5; i++)
    servos[i].srv.attach(8+i);

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
  for(int i=0; i<5; i++)
    servos[i].srv.detach();
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

      for(int i = 0; i<5; i++){
        int pos = map(analogRead(1), servos[i].minFeed, servos[i].maxFeed, servos[i].minDeg, servos[i].maxDeg);
        pos = constrain(pos, 0, 180);
        EEPROM.write(addr, pos);
        addr++;
      }

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
    for(int i = 0; i<5; i++)
      servos[i].srv.attach(8+i);

    delay(1000);

    // Center servos
    for(int i = 0; i<5; i++)
      servos[i].srv.write(90);

    delay(1000);

    // Start playback
    addr = 0;
    while (1)
    {
      for(int i = 0; i<5; i++){
        servos[i].pos = EEPROM.read(addr);
        servos[i].pos1 = EEPROM.read(addr+5);
        addr++;
      }
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
      
      for(int j = 0; j<5; j++){
        int direct = servos[j].pos1 > servos[j].pos ? 1 : -1;
        // Move servo to saved position
        for (int i = servos[j].pos; i != servos[j].pos1; i += direct)
        {
          servos[j].srv.write(i);
          delay(5);
        }
      }
    }

    recorded = false;
    addr = 0;

    delay(500);

    // Center all servos
    for(int i = 0; i<5; i++)
      servos[i].srv.write(90);

    delay(1000);

    // Detach them to save power and allow human manipulation
    for(int i = 0; i<5; i++)
      servos[i].srv.detach();
      
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
