#include "config.h"

#define LED 13
#define POT 14
#define PWM1 3
#define PWM2 5
#define HI_SW1 18
#define HI_SW2 19

uMODULES(modules, NUM_OF_MODULES)

//Main module instance
GDC modGDC;
ClockMillis clck;

void setup()
{
  modules.Add(&clck);
  
  //Set inputs
  pinMode(POT, INPUT);

  //Set outputs
  pinMode(LED, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(HI_SW1, OUTPUT);
  pinMode(HI_SW2, OUTPUT);  

  //Initialize input ports
  clck.Delay_ms.Write(10); //Clock at 100 Hz
  modGDC.PotInput.Write(analogRead(POT));
  modGDC.Reset.Write(LOW); //Reset the main module
}

void loop()
{
  //Read inputs
  modGDC.PotInput.Write(analogRead(POT));
  modGDC.Clk.Write(clck.ClckOut.Read());
  
  //Do uHDL engine clocking
  uHDL::uMain(modules);

  //Write outputs
  digitalWrite(LED, modGDC.OutChange.Read());
  digitalWrite(HI_SW1, modGDC.OutHi_SW1.Read());
  digitalWrite(HI_SW2, modGDC.OutHi_SW2.Read());
  analogWrite(PWM1, modGDC.OutPWM1.Read());
  analogWrite(PWM2, modGDC.OutPWM2.Read());
}

