#include <Servo.h> 
#include <Time.h>
/*This code is for dark line following mode, meaning it lights up when it sees dark*/

//--------------Autonomous vehicle runs on dark line following mode where Dark is a boolean value of 1------------------//

//-------------Motor Initializers-------------//
Servo myservoRight;
Servo myservoLeft;
Servo myservoShoot; 
Servo myservoReload; 
//------------------------------------------//

//---------Sensor Initializers------------//
int linPinRight = 13; // right most
int linPinRenter = 12; //right of center
int linPinCenter = 11; //center
int linPinLenter = 10; //left of center
int linPinLeft = 9;// Left most
int bounceRight = 7; //right bump
int bounceLeft = 6; //left bump

int linSensorRight;//variables for use in the functions themselves
int linSensorLeft;
int linSensorCenter;
int linSensorLenter;
int linSensorRenter;
int bumpRight;
int bumpLeft;
///--------------------------------------//



unsigned long starts, finished; // Initializes time variables used for findBump method.
int end_time = 2500;

void setup() {
  
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  
//-----------Set pins as input--------------//
  pinMode(linPinRight, INPUT); 
  pinMode(linPinLeft, INPUT); 
  pinMode(linPinCenter, INPUT);
  pinMode(linPinRenter, INPUT);
  pinMode(linPinLenter, INPUT);
  pinMode(bounceRight, INPUT_PULLUP);
  pinMode(bounceLeft, INPUT_PULLUP);
//-----------------------------------------//
  
//-----------------Assign motors to pins--------------//  
  myservoLeft.attach(2); 
  myservoRight.attach(3); 
  myservoShoot.attach(4); 
  myservoReload.attach(5);
//-----------------------------------------------------//

}

void loop() 
{

///--------------Check Line Sensors-----------///
   linSensorRight = digitalRead(linPinRight); 
   linSensorLeft = digitalRead(linPinLeft); 
   linSensorCenter = digitalRead(linPinCenter);
   linSensorLenter = digitalRead(linPinLenter);
   linSensorRenter = digitalRead(linPinRenter);
///------------------------------------------///

 

  myservoShoot.write(81);///Stops the launcher motor from moving.
  myservoReload.write(92);//Stops the reloader motor from moving.
  

  findLaunch(linSensorRight, linSensorLeft, linSensorCenter, linSensorLenter, linSensorRenter);//checks if its at a launch location, if it is, it proceeds to align itself and launch projectile.
  navigate(linSensorRight, linSensorLeft, linSensorCenter, linSensorLenter, linSensorRenter);//navigates the vehicle if it's not at a launch location or after launching the projectile.

 
}


///////-----------------------------Functions--------------------------------------////////


/*

The navigate funtion is responsible for telling the wheels the speed at which to move as well as the direction.
The speed and direction of the wheels depend on which line sensors are on. Different combinations of
which line sensors are on indicate the intensity of the curve. The outside motor spins progressively faster than the 
inside motor with increasing intensity or steepness of the curve.

Calls the motorSpeed funtion(defined below) which tells the motors the speed and direction to move as well as for how long(delay time).

*/

void navigate(int linSensorRight, int linSensorLeft, int linSensorCenter, int linSensorLenter, int linSensorRenter)
{

 
  if(linSensorCenter == 1 && linSensorRenter == 1 &&  linSensorLenter == 0 && linSensorRight == 0 && linSensorLeft == 0){ //mild right curve
  
    motorSpeed(110,80,50);//parameters are in this order: left wheel speed, right wheel speed, delay time.
   
  }
  
  if(linSensorCenter == 0 && linSensorRenter == 1 && linSensorLenter == 0 && linSensorRight == 0 && linSensorLeft == 0){ //medium right curve
  
    motorSpeed(125,84,50);//for right curve, left wheel moves faster than right wheel, (90-180) where 90 is stop and 180 is fastest speed in the proper direction for the left wheel.  
    
  }

  if(linSensorRenter == 1 && linSensorRight == 1 && linSensorLenter == 0 && linSensorLeft == 0 && linSensorCenter == 0){ //steep right curve
      
    motorSpeed(150,88,50); 
   
  }
  
  if(linSensorRight == 1 && linSensorRenter == 0 && linSensorLenter == 0 && linSensorLeft == 0 && linSensorCenter == 0){ //steepest right curve
  
    motorSpeed(160,90,50);  
    
  }
  
  if(linSensorCenter == 1 && linSensorLenter == 1 && linSensorLeft == 0 && linSensorRenter == 0 && linSensorRight == 0){ //mild left curve
  
    motorSpeed(100,70,50);//for left curve, right wheel moves faster than right wheel, (90-0) where 90 is stop and 0 is fastest speed in the proper direction for the right wheel.
  
  }
  
   if(linSensorCenter == 0 && linSensorLenter == 1 && linSensorLeft == 0 && linSensorRenter == 0 && linSensorRight == 0){ //medium left curve
   
   motorSpeed(96,65,50);
   
  }
    
  if(linSensorLeft == 1 && linSensorLenter == 1 && linSensorCenter == 0 && linSensorRenter == 0 && linSensorRight == 0){ //steep left curve
    
    motorSpeed(92,50,50);
    
  }
  
  if(linSensorLeft == 1 && linSensorRenter == 0 && linSensorLenter == 0 && linSensorRight == 0 && linSensorCenter == 0)//steepest left curve
  { 
    
    motorSpeed(90,40,50);
    
  }
 
  if (linSensorRight == 0 && linSensorLeft == 0 && linSensorCenter == 1 && linSensorLenter == 0 && linSensorRenter == 0)//only center line sensor detects black, tells the car to travel straight.
  { 
  
    motorSpeed(105,75,50);//Both wheels rotate at the same speed.
  
  } 
  
 

  if(linSensorRight == 0 && linSensorLeft == 0 && linSensorCenter == 0 && linSensorLenter == 0 && linSensorRenter == 0)// sensors detect no black line, tries to find line again by moving backwards.
  {
    motorSpeed(75,105,50);//Both wheels rotate at opposite their usual directions and the same relative speed to find the black line.
  }
 
}


/*

navigateSlow is responsible for telling the vehicle to follow the line slowly, in order to find one of the bumps.
It is only called after the vehicle detects it's at a launch location and it only uses the left of center and right of center line
sensors as the input.

*/
void navigateSlow(int linSensorLenter, int linSensorRenter)
{
  if( linSensorRenter == 1 ){ //mild right curve
  
    motorSpeed(101,82,50);//left wheel moves faster than right.
   
  }

  if(linSensorLenter == 1){//mild left curve

    motorSpeed(100,78,50);//right wheel moves faster than the left.
  }

  if(linSensorLenter == 0 && linSensorRenter == 0)//perceives it is travelling straight.
  {
    motorSpeed(98,82,50); //Both motors travel at the same speed. Slower straight line speed as in navigate.
  }

  
}


/*
the findLaunch method is responsible for checking if the vehicle is at the launch location.
It uses the method isAtLaunch check if it is at a launch location and calls the method findBump
to find the bump if isAtLaunch detects it is at a launch location.
*/

void findLaunch(int linSensorRight, int linSensorLeft, int linSensorCenter, int linSensorLenter, int linSensorRenter)
{


  if(isAtLaunch(linSensorRight, linSensorLeft, linSensorCenter, linSensorLenter, linSensorRenter))
  {
   motorSpeed(90,90,2000);
   findBump();
  }


}


/*

isAtLaunch checks if 5 or 4 of the line sensors detect a black line. This indicates that the vehicle is at a launch location.
It returns a boolean value of true if 4 or 5 line sensors are on and false if otherwise.

*/
boolean isAtLaunch(int linSensorRight, int linSensorLeft, int linSensorCenter, int linSensorLenter, int linSensorRenter)
{
  
  boolean output;
  
  if(linSensorRight == 1 && linSensorLeft == 1 && linSensorCenter == 1 && linSensorLenter == 1 && linSensorRenter == 1)//all line sensors are on
  {
    output = true;

  }

  else if(linSensorRight == 0 && linSensorLeft == 1 && linSensorCenter == 1 && linSensorLenter == 1 && linSensorRenter == 1)//the left four line sensors are on
  {
    output = true;
    
  }

  else if(linSensorRight == 1 && linSensorLeft == 0 && linSensorCenter == 1 && linSensorLenter == 1 && linSensorRenter == 1)//the right four line sensors are on
  {
    output = true;
   
  }

  else
  {
    output = false;
   
  }

  return output;
}


/*
The function findBump is responsible for aligning the vehicle for optimum launch position.
It constantly checks which bump sensors are pressed in order to tell which motor to move.
*/

void findBump()
{

  //---------Check Bump Sensors---------//
  bumpLeft = digitalRead(bounceLeft);
  bumpRight  = digitalRead(bounceRight);
  //----------------------------------//
  

  while(bumpLeft == 1 && bumpRight == 1) //stops the loop when one of the bump sensors are pressed
  {
   //--------------Check Line Sensors ---------------//
   linSensorLenter = digitalRead(linPinLenter);

   linSensorRenter = digitalRead(linPinRenter);
   //-----------------------------------------//
  
   
    navigateSlow(linSensorLenter, linSensorRenter);//calls the method to navigate the vehicle to one of the bumps
    
    //-----------Check Bump Sensors------------//
    bumpLeft = digitalRead(bounceLeft);
    bumpRight = digitalRead(bounceRight);
    //----------------------------------------//
    
    
    myservoShoot.write(81);///Stops the launcher motor from moving.
    myservoReload.write(92);///Stops the reloader motor from moving.
    
    delay(50);
        
  }


  myservoShoot.write(81);///Stops the launcher motor from moving.
  myservoReload.write(92);///Stops the reloader motor from moving.
  motorSpeed(90,90,1000);//tells the vehicle to stop for one second.
  

  starts = millis();//Records the time at the start of the second while loop.
 
  while(bumpLeft == 1 || bumpRight == 1) //stops the loop when both of them are pressed.
  {

    finished = millis();//Records the time during the while loop
    
    
    //------------------Responsible to switch direction of motor at the case where the bump sensor is in front of the bump-----------------//
    if((finished - starts) > end_time)
    {

      if((bumpLeft == 1) && (bumpRight == 0))//if bump the left bump sensor remains unpressed the left motor rotates in opposite of usual direction to find the bump.
    {
      motorSpeed(82,90,50);
     
    }

    else if((bumpRight == 1) && (bumpLeft == 0))//if the right bump sensor remains unpressed, the right motor rotates in opposite of usual direction to find the bump.
    {
      
      motorSpeed(90,98,50);
    }
      
    }
    //--------------------------------------------------------------------------------------------------------------------------------------//
    
    
    
    else if((bumpLeft == 1) && (bumpRight == 0))//if the left bump sensor is unpressed, left motor rotates to find the left bump.
    {
      motorSpeed(98,90,50);
     
    }

    

    else if((bumpLeft == 0) && (bumpRight == 1))//if the right bump sensor is unpressed, the right motor rotates to find the right bump.
    {
      
      motorSpeed(90,82,50);
    }

    
    //----------------Check Bump Sensors------------///
    bumpLeft = digitalRead(bounceLeft);
    bumpRight = digitalRead(bounceRight);
    //----------------------------------------------//
    
    myservoShoot.write(81);///Stops the launcher motor from moving.
    myservoReload.write(92);///Stops the reloader motor from moving.
    
    delay(20);
  }

 motorSpeed(90,90,1000);//vehicle stops for 1 second.
 
 fireLauncher();//the vehicle then loads the launcher and launches the projectile.
 
 myservoShoot.write(81);///Stops the launcher motor from moving.
 myservoReload.write(92);///Stops the reloader motor from moving.
 delay(50);

}


/*
motorSpeed is responsible for specifying the direction and speed of the driving motors.

*/

void motorSpeed(int left, int right, int time)
{
  
  myservoRight.write(right);
  myservoLeft.write(left);
  delay(time);
  
}


/*
fireLauncher is responsible for reloading the launcher and launching the projectile.

*/
void fireLauncher()
{

  myservoReload.write(88);//rotates the reload motor to load one of the balls into the launcher
  delay(9100);//the delay time is 9.1 seconds, which is approximately 360 degrees of rotation for that speed.
  
 
  myservoReload.write(92);//tells the reload motor to stop.
  
  motorSpeed(90, 80, 800);//tells the vehicle to rotate in place for 0.8s in order to compensate for misalignment due to the bump sensors..
  motorSpeed(90,90,900);//tells the vehicle to stop for 0.9s
 
  myservoShoot.write(90);//rotates the launch motor for 1.4 seconds in order to drive the hammer to launch the projectile to the target.                  
  delay(1400); 
  
}





