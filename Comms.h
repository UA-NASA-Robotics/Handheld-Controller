//comms.h
FastTransfer robot;
int roboMessage[20];

//COMM ADDRESSES
#define CONTROL_ADDRESS            5
#define NAVIGATION_ADDRESS         4
#define PIC_ADDRESS                1
#define MOTOR_ADDRESS              6
#define LED_ADDRESS                2
#define POWER_ADDRESS	           3


//CONTROL RECEIVE STUFF
#define LAST_BOARD_ADDRESS_RECEIVE   0
//NAVIGATION
#define MACRO_COMMAND_RECEIVE        1
#define MACRO_SUB_COMMAND_RECEIVE    2
#define MACRO_COMPLETE               3
#define GYRO_ANGLE                   4
#define MOTORL                       5
#define MOTORR                       6
//POWER
#define VOLTS                        7
#define MILLIAMPS                    8
#define MILLIAMPHOUR                 9
#define WATTHOURS_POWER_RECEIVE      10
//PIC (MAYBE NAV)
#define ENCODERSPEEDL                11
#define ENCODERSPEEDR                12
#define ENCODER_R_H_PIC_RECEIVE                 13
#define ENCODER_R_L_PIC_RECEIVE                 15
#define ENCODER_L_H_PIC_RECEIVE                 14
#define ENCODER_L_L_PIC_RECEIVE                 16
//MOTOR
#define BUCKET_ANGLE                 15
//Wii Camera Stuff
#define WII_CAMERA_LEFT_ANGLE        16
#define WII_CAMERA_RIGHT_ANGLE       17
#define WII_CAMERA_LEFT_LOCKED       18
#define WII_CAMERA_RIGHT_LOCKED      19
#define WII_CAMERA_LEFT_BEACON       20 
#define WII_CAMERA_RIGHT_BEACON      21

#define LIDAR_HEADING                 22  
#define LIDAR_DISPLACEMENT            23
#define LIDAR_ANGLE                   24
#define LIDAR_DISTANCE                25


//CONTROL SEND - ROBOT

//MOTOR
#define LEFTMOTORSPEED              1
#define RIGHTMOTORSPEED             2
#define ACTUATORSPEED               3
#define CONVEYORSPEED               4
//NAVIGATION
#ifdef SPACEE_INTERFACE
  #define MACRO_COMMAND_SEND          5
  #define MACRO_SUB_COMMAND           6
#else
  #define MACRO_COMMAND_SEND          1
  #define MACRO_SUB_COMMAND           2
#endif
#define MACRO_STOP                  3

//LED

#define LED_STATE_OVERRIDE          1
#define COLOR_PICKER                2
#define LED_UPDATE_FLAG             3
#define BOX_LOCATION                4

//PIC
#define ROBOT_MOVING                 1
#define WII_SUBSYSTEM_MODE           2    //MODES INCLUDE: TRIG=0, LEFT_BEACON_ANGLES=1, RIGHT_BEACON_ANGLES=2, CAMERA_OVERRIDE_BEACON_ANGLES=3, CONTINUOUS_AQUISITION=4
#define WII_LEFT_CAMERA_MODE         3    //MODES 3 and 4 only used for WII_SUBSYSTEM_MODE 3   
#define WII_RIGHT_CAMERA_MODE        4            //0=LEFT_BEACON 1=RIGHT_BEACON
#define SERVO_OVERRIDE_LEFT          5
#define SERVO_OVERRIDE_RIGHT         6
//WIIMODES
#define WIIMODE_VERTICAL_BEACON      1
#define WIIMODE_HORIZONTAL_BEACON    2
#define WIIMODE_CONTINUOUS           4
#define WIIMODE_SERVO_OVERRIDE       5

void initializeCommunicaton()
{

  Serial.begin(38400);

  robot.begin(Details(roboMessage), CONTROL_ADDRESS, false, &Serial);
}

union jointhem{
    int32_t joined;
    struct {      
      uint16_t low;
      uint16_t high;
    }endian;
}_16_to_32;


Timers minimumResponseTimer(50);
Timers checkComms(5);
Timers resendTimer(200);
inline void updateCommunication()
{
  
   
  //So if you have heard from the comm after sending -or- if the resend timer timed out and you havent already sent before a safety timeout condition
  if ( minimumResponseTimer.timerDone())
  {
    digitalWrite(LED1, !digitalRead(LED1));

       if(rightMotorSpeed > 0)
       {
          digitalWrite(LED2, !digitalRead(LED2));
       }
    sendDataMotor(leftMotorSpeed, rightMotorSpeed);
    minimumResponseTimer.resetTimer();     
    resendTimer.resetTimer();

  }

}



void sendDataNavigation(int _macro_command, int _macro_sub_command)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(MACRO_SUB_COMMAND, _macro_sub_command);
  robot.ToSend(MACRO_COMMAND_SEND, _macro_command);
  robot.sendData(NAVIGATION_ADDRESS);
}

void sendDataNavigation(int _macro_command)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(MACRO_COMMAND_SEND, _macro_command);
  robot.sendData(NAVIGATION_ADDRESS);
}

void sendDataMotor(int leftMotor, int rightMotor)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(LEFTMOTORSPEED  , leftMotor);
  robot.ToSend(RIGHTMOTORSPEED , rightMotor);
  robot.sendData(MOTOR_ADDRESS);
}

int prevSub, prevMacro;

//
//void sendDataMotor(int leftMotor, int rightMotor, int actuator, int conveyor)
//{
//  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
//  robot.ToSend(LEFTMOTORSPEED  , leftMotor);
//  robot.ToSend(RIGHTMOTORSPEED , rightMotor);
//  robot.sendData(MOTOR_ADDRESS);
//}
//
//void sendDataMotor(int actuator)
//{
//  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
//  robot.ToSend(ACTUATORSPEED   , actuator);
//  robot.sendData(MOTOR_ADDRESS);
//}
//
//void sendLEDdata(int stateOverride, int colorPicker)
//{
//  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
//  robot.ToSend(LED_STATE_OVERRIDE, stateOverride);
//  robot.ToSend(COLOR_PICKER, colorPicker);
//  robot.ToSend(LED_UPDATE_FLAG, 1);
//  robot.sendData(LED_ADDRESS);
//}

