/*
  Command.h
  GDC v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], September 2015
  License: LGPL
*/

#ifndef COMMAND_H
#define COMMAND_H

//Errors
#define OPR_TO_BIG 200
#define ARG_TO_BIG 201
#define INVALID_OPERATION 300

//Operations
#define OPR_INVALID -1
#define OPR_STOP 0
#define OPR_BREAK 1
#define OPR_CW 2
#define OPR_CCW 3
#define OPR_MANUAL 4

namespace eSeN
{
	namespace GDC
	{
		namespace Util
		{
			class Command
			{
			private:
				int error;
				int operation;
        unsigned int argument;
				char cmd[20];
				
				void setOperation(const char *);
				void setArgument(const char *);
				
			public:
				Command(const char *);
				virtual ~Command();
				void Execute(int &, int &);
			};

		}
	}

}

#endif // COMMAND_H
