/*
  config.h
  PMicroTB v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], July 2015
  License: LGPL
*/

#ifndef _CONFIG_H
#define _CONFIG_H

//Set U_HDL_WIREING define for wireing API
//#define U_HDL_WIREING

#include "uHDL.h"
#include "uHDL_Primitives.h"
#include "Command.h"

//Declare module list
#define NUM_OF_MODULES 2
extern uMODULES(modules, NUM_OF_MODULES)

#include "MainModule.h"

#endif
