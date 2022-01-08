

#include <Wire.h>
#include <NXShield.h>
#include <Time.h>

//#define GLOBAL_DELAY 50

int sensorPower = 11;
int sensor1 = 10;
int sensor2 = 9;
int m1Speed = 5;
int delta_time;
int time1;
int time2;

//time_t now();

NXShield nxshield;
//_______________________sensor detection_________________________________________
void setup()
{
  Serial.begin(115200);
  nxshield.init(SH_SoftwareI2C);
  nxshield.bank_a.motorReset();
  nxshield.bank_b.motorReset();

  Serial.println ("Watch the LED on NXShield ...");
  pinMode(sensorPower, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  digitalWrite(sensorPower, HIGH); 
  nxshield.ledSetRGB(0,0,1);
}
//_________________________motor detection________________________________________________
void loop()
{

  if (!digitalRead(sensor1))
  {
    nxshield.ledSetRGB(1,0,0);
    Serial.println ("Sensor 1");
    
    time_t time1 = now();
  }
  if (!digitalRead(sensor2)) 
  {
    nxshield.ledSetRGB(0,1,0);
    Serial.println ("Sensor 2");
    time_t time2 = now();
    int m1Speed2=50;
    delta_time = time2 - time1;
    int distance=10;
    float speeed = distance/delta_time;
    int speedlimit=50;
    Serial.println(delta_time);
    if (delta_time > 1)
    {
      {
      nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, 5);
      delay(1000);
      }
    }
    else if (delta_time > 10)
    {
    nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, 100);
    Serial.println(delta_time);
    }
  }
    delay(10000);

    Serial.println(delta_time);
    nxshield.bank_b.motorStop(SH_Motor_1, SH_Next_Action_Brake ); 
    nxshield.bank_a.motorStop(SH_Motor_1, SH_Next_Action_Brake );
    //nxshield.bank_a.motorStop(SH_Motor_1, SH_Next_Action_Brake );
    //nxshield.bank_b.motorStop(SH_Motor_1, SH_Next_Action_Brake );

