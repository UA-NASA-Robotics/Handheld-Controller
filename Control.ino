#define SPACEE_INTERFACE
//control.ino
//
//Version 19
//Created by Zac Kilburn with guidence from Igor Vinograd :)
//3/7/14


//Software Packages Include
#include <FastTransfer.h>
#include <Timers.h>
#include "SoftwareSerial.h"
//#include <digitalWriteFast.h>
//#include "Adafruit_Thermal.h"
#include <avr/pgmspace.h>

#define GETTING_COMMS   0
#define AUTONOMOUS      1
#define MACRO           2
#define NORMAL          3
int GlobalState;
//Internal File Includes
//#include "LED.h"
#include "Methods.h"
#include "Definitions.h"
#include "Variables.h"
//#include "Printer.h"
#include "Comms.h"
#include "PinMap.h"
#include "Debug.h"

Timers batteryPresentWarningPage(2500);
Timers batteryWarningTimer(500);
bool batteryPowered = false;
bool flash=true;
bool bootup = true;
//#define GETTING_COMMS   0
//#define AUTONOMOUS      1
//#define MACRO           2
//#define NORMAL          3


void setup()
{
//  printerPrint();
//  while(1) ;
  //setupLED();
  initializeCommunicaton();
  initializePins();
  delay(3000);
 
  
 digitalWrite(LED1,HIGH);
 delay(500);
 digitalWrite(LED1,LOW);
 delay(500);
 digitalWrite(LED1,HIGH);
 
}
Timers blinkTimer(500);
void loop() {
  controlsUpdate();
  //macroUpdate();
  updateCommunication();
  
  
}






