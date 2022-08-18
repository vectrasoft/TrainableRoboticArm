#include <Servo.h>
#include <EEPROM.h>

#define _servo_fun(A) for(int i=0; i<5; i++) servos[i].srv.(A)

#define servo_on _servo_fun(attach(8+i))
#define servo_off _servo_fun(detach(8+i))
#define servo_center _servo_fun(write(90))

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
int val;

void setup(){
  Serial.begin(9600);

  servo_on;

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
  servo_off;

  /* Display minimums and maximums for analog feedback
  // Uncomment for debugging
  for(int i=0; i<5; i++){
    Serial.print("servos[i].srv Min: ");
    Serial.println(servos[i].minFeed);
    Serial.print("servos[i].srv Max: ");
    Serial.println(servos[i].maxFeed);
  }
  Serial.println();
  */
  blink();
}

void loop(){
  // enter in recording mode
  if (digitalRead(7)){
    digitalWrite(13, HIGH);
    delay(1000);

    while (!digitalRead(7)){
      delay(50);

      for(int i = 0; i<5; i++){
        int pos = map(analogRead(i+1), servos[i].minFeed, servos[i].maxFeed, servos[i].minDeg, servos[i].maxDeg);
        pos = constrain(pos, 0, 180);
        EEPROM.write(addr, pos);
        addr++;
      }

      if (addr == 510) break;

      delay(50);

      /* Display recorded values
      // Uncomment for debugging
      for(int i=0; i<5; i++){
        Serial.print(servos[i].pos);
        Serial.print("\t");
      }
      */
    }

    EEPROM.write(addr, 255);
    addr = 0;

    servo_on;
    delay(1000);

    servo_center;
    delay(1000);

    servo_off;
  }

  // enter in executing mode
  if (digitalRead(6)){
    digitalWrite(13, LOW);
    servo_on;

    // Start playback
    while (1){
      for(int i = 0; i<5; i++){
        servos[i].pos = EEPROM.read(addr);
        servos[i].pos1 = EEPROM.read(addr+5);
        addr++;
      }

      /* Display positions being written to the servos
      // Uncomment for debugging
      for(int i=0; i<5; i++){
        Serial.print("servos[i].srv: ");
        Serial.print(servos[i].pos);
        Serial.print("\t\t");
        Serial.println(servos[i].pos1);
      }
      Serial.println();
      */
      
      // Check for the end of the recorded commands, if so then break out of the infinite loop
      if ((servos[0].pos == 255))
        break;
      
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

    addr = 0;

    delay(500);
    servo_center;

    delay(1000);
    servo_off;

    // Flash the LED to let user know replay is completed
    blink();
  }

  delay(100);
}

void blink(){
  for (int i = 0; i < 3; i++){
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }
}
