/*
  MainModule.h
  GDC v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], September 2015
  License: LGPL
*/

#ifndef _MAIN_MODULE_H
#define _MAIN_MODULE_H

#include "config.h"
	
using namespace uHDL;
using namespace eSeN::GDC::Util;

//Main module -->
uMODULE(GDC)
{
  //Inputs
  u_input<bool> Reset;
  u_input<unsigned int> PotInput;
  u_input<bool> Clk;
  
  //Outputs
  u_output<bool> OutChange;
  u_output<bool> OutHi_SW1;
  u_output<bool> OutHi_SW2;
  u_output<unsigned char> OutPWM1;
  u_output<unsigned char> OutPWM2;

    
  //Signals

  //Internal registers
  u_reg<int> analogDirection;
  u_reg<bool> automatic;
  
  //Submodules
  
  //Init method
  uINIT(GDC)
  {
    Clk.Write(LOW);
    OutChange.Write(LOW);
    OutHi_SW1.Write(HIGH);
    OutHi_SW2.Write(HIGH);
    OutPWM1.Write(0);
    OutPWM2.Write(0);
    automatic.Write(LOW);
    Reset.Write(HIGH);
    
    modules.Add(this);
  }
  
  //Module method
  void GDCMethod()
  {
    if(Reset.NegEdge())
    {
      OutChange.Write(LOW);
      OutHi_SW1.Write(HIGH);
      OutHi_SW2.Write(HIGH);
      OutPWM1.Write(0);
      OutPWM2.Write(0);
      automatic.Write(LOW);
      
      Serial.begin(9600);
    }
    else Reset.Write(HIGH); //Pullup

    if(Serial.available())
    {
      String str = Serial.readStringUntil('\n');

      if(0 < str.length())
      {
        Command cmd(str.c_str());

        int op;
        int arg;
        cmd.Execute(op, arg);
        switch(op)
        {
          case OPR_STOP:
            OutHi_SW1.Write(HIGH);
            OutHi_SW2.Write(HIGH);
            OutPWM1.Write(0);
            OutPWM2.Write(0);        
            break;
             
          case OPR_BREAK:
            OutHi_SW1.Write(HIGH);
            OutHi_SW2.Write(HIGH);
            OutPWM1.Write(255);
            OutPWM2.Write(arg);          
            break;

          case OPR_CW:
            OutHi_SW1.Write(LOW);
            OutHi_SW2.Write(HIGH);
            OutPWM1.Write(0);
            OutPWM2.Write(arg);
            break;

          case OPR_CCW:
            OutHi_SW1.Write(HIGH);
            OutHi_SW2.Write(LOW);
            OutPWM1.Write(arg);
            OutPWM2.Write(0);
            break;
             
          case OPR_MANUAL:
            automatic.Write(LOW);
            break;
            
          default:
            return;
        }

        if(-1 != op)
        {
          if(OPR_MANUAL != op) automatic.Write(HIGH);
          Serial.println("OK");
        }
      }
    }

    if(PotInput.Change() && !automatic.Read())
    {
      analogDirection.RW() = ((PotInput.Read() + 1) >> 1) - 256;
      analogDirection.RW() = analogDirection.RW() < -255 ? -255 : analogDirection.RW();
      analogDirection.RW() = analogDirection.RW() > 255 ? 255 : analogDirection.RW();

      OutChange.Write(HIGH);
      
      if(16 > analogDirection.RW() && -16 < analogDirection.RW())  //STOP
      {
        OutHi_SW1.Write(HIGH);
        OutHi_SW2.Write(HIGH);
        OutPWM1.Write(0);
        OutPWM2.Write(0);          
      }
      else if(0 < analogDirection.RW())  //CW
      {
        OutHi_SW1.Write(LOW);
        OutHi_SW2.Write(HIGH);
        OutPWM1.Write(0);
        OutPWM2.Write(analogDirection.RW());
      }
      else  //CCW
      {
        OutHi_SW1.Write(HIGH);
        OutHi_SW2.Write(LOW);
        OutPWM1.Write(~analogDirection.RW() + 1);
        OutPWM2.Write(0);          
      }
    }
    else OutChange.Write(LOW);
  }
  
  //Sensitivity and always method
  uALWAYS(Clk.PosEdge(), GDCMethod();)
};
//<-- Main module

#endif
