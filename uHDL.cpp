/*
	uHDL.cpp
	uHDL v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], January 2015
	License: LGPL
*/

#include "uHDL.h"

namespace uHDL
{
	unsigned long TICKS = 0;
	
	void uMain(const int modc, uModule **modref)
	{
		for(int i = 0; i < modc; i++)
		{
			modref[i]->Always();
		}
		
		TICKS++;
	}
}
