
#define DEADZONE_HIGH_X   710
#define DEADZONE_LOW_X    430
#define X_AXIS_ZERO       581     //A2

#define X_AXIS_MAX        840
#define X_AXIS_MIN        120
#define X_RANGE           X_AXIS_MAX-X_AXIS_MIN

#define DEADZONE_HIGH_Y   550
#define DEADZONE_LOW_Y    510

#define Y_AXIS_ZERO       530     //A3

#define Y_AXIS_MAX        830
#define Y_AXIS_MIN        150
#define X_RANGE           Y_AXIS_MAX-Y_AXIS_MIN



//pinmap.h
//---------------------------------------------------------------------
//initialize all the inputs
void initializePins()
{
  
 
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  //joystick pins
  pinMode(JOYSTICK_1_Y, INPUT);
  pinMode(JOYSTICK_1_X, INPUT);
  pinMode(JOYSTICK_2_Y, INPUT);
  pinMode(JOYSTICK_2_X, INPUT);

  //pot pins
  pinMode(POT_1, INPUT);
  pinMode(POT_2, INPUT);
/*
  //set the digital input pins
  pinMode(ACTUATOR_UP1, INPUT);
  pinMode(ACTUATOR_DOWN1, INPUT);
  #ifdef ACTUATOR_UP2
    pinMode(ACTUATOR_UP2, INPUT);
  #else
    pinMode(MACRO_BUTTON_ENABLE1, INPUT);
  #endif
  #ifdef ACTUATOR_DOWN2
     pinMode(ACTUATOR_DOWN2, INPUT);
  #else
     pinMode(MACRO_BUTTON_ENABLE2, INPUT);
  #endif
  pinMode(ESTOP, INPUT);
  pinMode(MACRO_BUTTON, INPUT);

  pinMode(AUTO_LIGHT, OUTPUT);

  digitalWrite(33, LOW);
  digitalWrite(36, LOW);
  digitalWrite(13, HIGH);
  */
}
//---------------------------------------------------------------------
//update the scaling and joysticks
inline void updateJoysticks()
{
  if (drivingProfile == 1)
  {
    //---------------------------------------------------------------------
    //Profile 1
    //---------------------------------------------------------------------
    //Standard Tank Drive
    int leftActualY = analogRead(JOYSTICK_1_Y);
    int rightActualY = analogRead(JOYSTICK_2_Y);


    //DEADZONE
    if (DEADZONE_HIGH_X > rightActualY && rightActualY > DEADZONE_LOW_X)
    {
      rightActualY = 512;
    }

    if (DEADZONE_HIGH_Y > leftActualY && leftActualY > DEADZONE_LOW_Y)
    {
      leftActualY = 512;
    }

    int leftScale = 1023 - analogRead(POT_1);
    int rightScale = 1023 - analogRead(POT_2);


    //eliminates zeroes from the scales
    if (leftScale < 2)
    {
      leftScale = 2;
    }
    if (rightScale < 2)
    {
      rightScale = 2;
    }

    static int pastleftScale = 0;
    static int pastrightScale = 0;
    static int leftLimit = 0;
    static int rightLimit = 0;

    if (abs(leftScale - pastleftScale) > 7)
    {
      pastleftScale = leftScale;
      leftLimit = map(leftScale, 0, 1023, 0, 90);
    }

    if (abs(rightScale - pastrightScale) > 7)
    {
      pastrightScale = rightScale;
      rightLimit = map(rightScale, 0, 1023, 0, 90);
    }

    leftMotorSpeed = map(leftActualY, 0, 1023, -leftLimit, leftLimit);
    
    rightMotorSpeed = map(rightActualY, 0, 1023, -rightLimit, rightLimit);
   
  }
  else if (drivingProfile == 2)
  {
    //---------------------------------------------------------------------
    //Profile 2
    //---------------------------------------------------------------------
    //Single Joystick Operation
    rightActualY = 1023-analogRead(JOYSTICK_2_Y);
    rightActualX = 1023-analogRead(JOYSTICK_2_X);  
    
    rightActualY= map(rightActualY, Y_AXIS_MIN, Y_AXIS_MAX, -500, 500);
    rightActualX = map(rightActualX, X_AXIS_MIN, X_AXIS_MAX, -500, 500);
    
//    Serial.print("x: ");
//    Serial.println(rightActualX);
//    Serial.print("y: ");
//    Serial.println(rightActualY);

    //DEADZONE
    if (DEADZONE_HIGH_Y > rightActualY && rightActualY > DEADZONE_LOW_Y)
    {
      rightActualY = Y_AXIS_ZERO;
    }
    if (DEADZONE_HIGH_X > rightActualX && rightActualX > DEADZONE_LOW_X)
    {
      rightActualX = X_AXIS_ZERO;
    }

    //--------------SCALING FROM SLIDE POTS----------------
    rightScale = analogRead(POT_2);
    
//    Serial.print("RS: ");
//    Serial.println(rightScale);

    //eliminates zeroes from the scales

    if (rightScale < 2)
    {
      rightScale = 2;
    }

    //static int pastleftScale = 0;
    pastrightScale = 0;
    //static int leftLimit = 0;
    rightLimit = 0;

    if (abs(rightScale - pastrightScale) > 7)
    {
      pastrightScale = rightScale;
      rightLimit = map(rightScale, 0, 1030, 0, 100);
    }   
    //--------------END SCALING FROM SLIDE POTS----------------
    
    
    //NEW DRIVING PROFILE     
//    
//    rightActualY = rightActualY - 512; 
//    rightActualX = rightActualX - 512; 
    
    //OUTPUT to variables
    leftMotorSpeed = map(constrain(rightActualY+(rightActualX*0.5),-512,512), -512, 512, -rightLimit, rightLimit);
    rightMotorSpeed = map(constrain(rightActualY-(rightActualX*0.5),-512,512), -512, 512, -rightLimit, rightLimit);
    
//    Serial.print("lm: ");
//    Serial.println(leftMotorSpeed);
//    Serial.print("rm: ");
//    Serial.println(rightMotorSpeed);
//    Serial.println();
  }
  else if (drivingProfile == 3)
  {
    //---------------------------------------------------------------------
    //Profile 3
    //---------------------------------------------------------------------
    //Dual Joystick (combined track R / differential control L
  }

}
/*
//---------------------------------------------------------------------
//UPDATE THE ACTUATOR BUTTONS
static inline void actuatorUpdate()
{
  conveyorButton= !digitalRead(JOYSTICK_BUTTON);
  
  if (digitalRead(ACTUATOR_UP1) == LOW||digitalRead(ACTUATOR_UP2) == LOW)
  {
    
    if (overrideEngaged) {
      actuatorSpeed = 252;
    }
    else {
      actuatorSpeed = 254;
    }
  }
  else if (digitalRead(ACTUATOR_DOWN1) == LOW||digitalRead(ACTUATOR_DOWN2) == LOW)
  {
    if (overrideEngaged) {
      actuatorSpeed = 251;
    }
    else {
      actuatorSpeed = 253;
    }
  }
  else
  {
    actuatorSpeed = 255;
  }
  //  Serial.print("Actuator Speed:  ");
  //  Serial.print(actuatorSpeed);
  //  Serial.println("");
  //  Serial.print("Actuator Down:  ");
  //  Serial.print(digitalReadFast(ACTUATOR_DOWN));
  //  Serial.println("");
  //  Serial.print("Actuator Up:  ");
  //  Serial.print(digitalReadFast(ACTUATOR_UP));
  //  Serial.println("");
  //  Serial.print("MacroKill:  ");
  //  Serial.print(digitalReadFast(MACRO_BUTTON) );
  //  Serial.println("");
  //  Serial.print("ESTOP:  ");
  //  Serial.print(digitalReadFast(ESTOP) );
  //  Serial.println("");
  
}


//---------------------------------------------------------------------
//update the estop button
static inline void estopUpdate()
{
  //stop the f***ing robot fast
  if (digitalRead(ESTOP) == LOW)
  {
    leftMotorSpeed = 0;
    rightMotorSpeed = 0;
    actuatorSpeed = 255;

    //if in autonomous mode
    if (internalMacroKeeper != 0)
    {
       killMacro();
    }
  }
}
*/
//---------------------------------------------------------------------
//pinMaps update function (uses internal methods--called externally)
inline void controlsUpdate()
{
  updateJoysticks();
  //actuatorUpdate();
  //estopUpdate();
}
















