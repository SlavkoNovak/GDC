/*
  Command.cpp
  GDC v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], September 2015
  License: LGPL
*/

#include "Command.h"
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

namespace eSeN
{
	namespace GDC
	{
		namespace Util
		{
			Command::Command(const char *commandStr) : error(0), operation(0), argument(0)
			{
				memset(this->cmd, 0, 20);
				strncpy(this->cmd, commandStr, 19);				
			}

			Command::~Command()
			{
			}
			
			void Command::Execute(int &OP, int &argument)
			{
				char opr[10]; //operator
				char arg[6]; //operend object
				
				memset(opr, '\0', 10);
				memset(arg, '\0', 6);

				char *pcmd = this->cmd;
				
				//opr
				for(int i = 0; i < 10; i++)
				{
					if(9 == i) { this->error = OPR_TO_BIG; break; }
					if(' ' != *pcmd && '\0' != *pcmd) opr[i] = *pcmd++; else break;
				}
				pcmd++;
				if(0 == this->error) this->setOperation(opr);
				
				//arg
				if(0 == this->error)
				{
					for(int i = 0; i < 6; i++)
					{
						if(5 == i) { this->error = ARG_TO_BIG; break; }
						if(' ' != *pcmd && '\0' != *pcmd) arg[i] = *pcmd++; else break;
					}
					pcmd++;
				}
				if(0 == this->error) this->setArgument(arg);
								
				if(0 == error)
				{
          OP = this->operation;
          argument = this->argument;
				}
				else
				{
          char tmp[10];
				  Serial.print("ERROR: ");
				  Serial.println(itoa(this->error, tmp, 10));
          OP = -1;
          argument = 0;
				}
			}
			
			void Command::setOperation(const char *opr)
			{
				char oprUpper[10];
				char *pOprUpper = oprUpper;

				while(*opr) *pOprUpper++ = toupper(*opr++);
				*pOprUpper = '\0';
				
				if(strstr(oprUpper, "STOP"))
				{
					this->operation = OPR_STOP;
				}
				else if(!strcmp(oprUpper, "BREAK"))
				{
					this->operation = OPR_BREAK;
				}
				else if(!strcmp(oprUpper, "CW"))
				{
					this->operation = OPR_CW;
				}
				else if(!strcmp(oprUpper, "CCW"))
				{
					this->operation = OPR_CCW;
				}
        else if(strstr(oprUpper, "MANUAL"))
        {
          this->operation = OPR_MANUAL;
        }
				else
				{
					this->operation = OPR_INVALID;
					this->error = INVALID_OPERATION;
				}
			}
			
			void Command::setArgument(const char *arg)
			{
				this->argument = atoi(arg);
			}
		}
	}
}

