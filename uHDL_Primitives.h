/*
	uHDL_Primitives.h
	uHDL Arduino Ext v0.2 by Slavko Novak [slavko.novak.esen@gmail.com], January 2015
	License: LGPL
*/

#ifndef _U_HDL_PRIMITIVES_H
#define _U_HDL_PRIMITIVES_H

#include "uHDL.h"

#ifdef ARDUINO_TB
	#include "ArduinoTB.h"
	#include <iostream>
#else
	#include "Arduino.h"
#endif

using namespace uHDL;
using namespace std;

namespace uHDL
{
	//Not -->
	uMODULE(Not)
	{
		//Inputs
		u_input<bool> Input;
		
		//Outputs
		u_output<bool> Output;
		
		//Init Method
		uINIT(Not)
		{
			Input.Write(false);
			Output.Write(true);
		}
		
		//Sensitivity and always method
		uALWAYS(Input.Change(), Output.Write(!Input.Read());)
	};
	//<--Not
	
	//AND gate -->
	uMODULE(And)
	{
		//Inputs
		u_input<bool> Input1;
		u_input<bool> Input2;
		
		//Outputs
		u_output<bool> Output;

		//Init method
		uINIT(And)
		{
			Input1.Write(false);
			Input2.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(
				Input1.Change() || Input2.Change(),
				Output.Write(Input1.Read() && Input2.Read());
				)
	};
	//<-- AND gate
	
	//NAND gate -->
	uMODULE(NAnd)
	{
		//Inputs
		u_input<bool> Input1;
		u_input<bool> Input2;
		
		//Outputs
		u_output<bool> Output;

		//Init method
		uINIT(NAnd)
		{
			Input1.Write(false);
			Input2.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(
				Input1.Change() || Input2.Change(),
				Output.Write(!(Input1.Read() && Input2.Read()));
				)
	};
	//<-- NAND gate
	
	//OR gate -->
	uMODULE(Or)
	{
		//Inputs
		u_input<bool> Input1;
		u_input<bool> Input2;
		
		//Outputs
		u_output<bool> Output;

		//Init method
		uINIT(Or)
		{
			Input1.Write(false);
			Input2.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(
				Input1.Change() || Input2.Change(),
				Output.Write(Input1.Read() || Input2.Read());
				)
	};
	//<-- OR gate
	
	//NOR gate -->
	uMODULE(NOr)
	{
		//Inputs
		u_input<bool> Input1;
		u_input<bool> Input2;
		
		//Outputs
		u_output<bool> Output;

		//Init method
		uINIT(NOr)
		{
			Input1.Write(false);
			Input2.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(
				Input1.Change() || Input2.Change(),
				Output.Write(!(Input1.Read() || Input2.Read()));
				)
	};
	//<-- NOR gate
	
	//XOR gate -->
	uMODULE(XOr)
	{
		//Inputs
		u_input<bool> Input1;
		u_input<bool> Input2;
		
		//Outputs
		u_output<bool> Output;

		//Init method
		uINIT(XOr)
		{
			Input1.Write(false);
			Input2.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(
				Input1.Change() || Input2.Change(),
				Output.Write(Input1.Read() ^ Input2.Read());
				)
	};
	//<-- XOR gate

	//TFlipFlopPosEdg gate -->
	uMODULE(TFlipFlopPosEdg)
	{
		//Inputs
		u_input<bool> Clck;
		u_input<bool> Set;
		u_input<bool> Reset;
		
		//Outputs
		u_output<bool> Output;
		
		//Init method
		uINIT(TFlipFlopPosEdg)
		{
			Clck.Write(false);
			Set.Write(true);
			Reset.Write(true);
		}
		
		//Sensitivity and always method
		uALWAYS(
				Clck.PosEdge() || Set.Change() || Reset.Change(),
				if(Set.Read() ^ Reset.Read())
				{					
					if(false == Set.Read())
					{
						Output.Write(true);
					}
					else if(false == Reset.Read())
					{
						Output.Write(false);
					}
				}
				else
				{
					Output.Write(!Output.Read());		
				}
				)
	};
	//<-- TFlipFlopPosEdg gate

	
	//ClockTicks module -->
	uMODULE(ClockTicks)
	{
		//Inputs
		u_input<unsigned long> Delay_ticks;
		
		//Outputs
		u_output<bool> ClckOut;
		
		//Internals
		u_reg<unsigned long> tick;
		
		//Init method
		uINIT(ClockTicks)
		{
			Delay_ticks.Write(1);
			ClckOut.Write(false);
		}
		
		//Module method
		void ClockMethod()
		{	
			if(TICKS - tick.RW() >= Delay_ticks.Read())
			{
				ClckOut.Write(true);
				tick.Write(TICKS);
			}
			else ClckOut.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(true, ClockMethod();)
	};
	//<-- ClockMillis module
	
	//ClockMillis module -->
	uMODULE(ClockMillis)
	{
		//Inputs
		u_input<unsigned long> Delay_ms;
		
		//Outputs
		u_output<bool> ClckOut;
		
		//Internals
		u_reg<unsigned long> tick;
		
		//Init method
		uINIT(ClockMillis)
		{
			Delay_ms.Write(1);
			ClckOut.Write(false);
		}
		
		//Module method
		void ClockMethod()
		{	
			if(millis() - tick.RW() >= Delay_ms.Read())
			{
				ClckOut.Write(true);
				tick.Write(millis());
			}
			else ClckOut.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(true, ClockMethod();)
	};
	//<-- ClockMillis module
	
	//ClockMicros module -->
	uMODULE(ClockMicros)
	{
		//Inputs
		u_input<unsigned long> Delay_us;
		
		//Outputs
		u_output<bool> ClckOut;
		
		//Internals
		u_reg<unsigned long> tick;
		
		//Init method
		uINIT(ClockMicros)
		{
			Delay_us.Write(1);
			ClckOut.Write(false);
		}
		
		//Module method
		void ClockMethod()
		{	
			if(micros() - tick.RW() >= Delay_us.Read())
			{
				ClckOut.Write(true);
				tick.Write(micros());
			}
			else ClckOut.Write(false);
		}
		
		//Sensitivity and always method
		uALWAYS(true, ClockMethod();)
	};
	//<-- ClockMillis module
}

#endif
