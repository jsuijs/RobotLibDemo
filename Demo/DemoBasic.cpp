//-----------------------------------------------------------------------------
// DemoBasic.cpp
//
// Copyright (c) 2013-2019 Joep Suijs - All rights reserved.
//
// This demo shows how to use (activate) the Basic virtual machine.
//
// RobotLib tags: DEMO
//-----------------------------------------------------------------------------
// This file is part of RobotLib.
//
// RobotLib is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RobotLib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RobotLib. If not, see <http://www.gnu.org/licenses/>.
//
// See http://wiki.robotmc.be/index.php?title=RobotLib for more information.
//-----------------------------------------------------------------------------
// This is a DEMO file.
// A DEMO file can be included in a project by adding it to makefile.do.
// For details, see:
//    http://wiki.robotmc.be/index.php?title=RobotLib_Modules#RobotLib_Demo.27s
//-----------------------------------------------------------------------------
// tags_end
//-----------------------------------------------------------------------------
#include "RobotLib.h"

#define DEMO_NAME DemoBasic

//-------------
// OVERVIEW
//-------------
/*

This is the test program in BASIC:

      dim r
      Zero()
      One()
      Two()
      r = MyAdd(2, 5)
      print "result: " ; r

*/

//-------------
// DECLARATIONS
//-------------
void MsmStartMyMission(int Dummy, int Actie);
int  BasicDispatchDemo1(int Dummy, int Param1, int Param2);
int  BasicDispatchDemo2(int InDispatch);
static bool MSM_Sample(TMissionHandler *M, int &State, int NewState);

//-------------
// INSTANCES
//-------------

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for Basic.\n");

   //------------------------------------------------------------
   // Most of the work of setup is adding all required symbols.
   // Symbols are a way to access RobotLib code from basic.
   // Each symbol yields a (one) function basic, which can be
   // called with a specific number of integer params and returns
   // exactly one integer.
   //------------------------------------------------------------
   // Note: registry variables that are setup before BasicInit()
   //       is called are accessible from basic.
   //------------------------------------------------------------

#ifndef RLM_BASIC_DISABLE
   BasicInit();   // This one does most of the setup work.

   // Add your project specific functions like this:
   BasicSymbols.AddDispatchFunction("StartMyMission", "",   0, 1, (void *)MsmStartMyMission);

   // Use of params
   BasicSymbols.AddDispatchFunction("MyAdd", "", 0, 2, (void *)BasicDispatchDemo1);

   // Multiple symbols on same function:
   BasicSymbols.AddDispatchFunction("Zero",  "", 0, 0, (void *)BasicDispatchDemo2);
   BasicSymbols.AddDispatchFunction("One",   "", 1, 0, (void *)BasicDispatchDemo2);
   BasicSymbols.AddDispatchFunction("Two",   "", 2, 0, (void *)BasicDispatchDemo2);

   //--------------------------------------------------------------------------
   // Some optional classes can add their own symbols to the basic VM,
   // by a call to SetBasicSymbols.
   // Note:  the statement below is disabled since we
   //        don't have ArIo activated for this demo....
   //ArIo.SetBasicSymbols();

// todo: controle of alles goed gaat als basic init vooraan staat. of is achteraan
//       juist beter?
#endif

}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for Basic.\n");
}

//-------------
// OTHER CODE
//-------------

//-----------------------------------------------------------------------------
// MsmStartMyMission - Show how to start a mission from basic.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MsmStartMyMission(int Dummy, int Actie)
{
   Mission.ParamSet(0, Actie);   // Just to demo how to pass a param from basic,
                                 // to a MSM, the MSM below doesn't use it...
   Mission.Start(FP_FNAME(MSM_Sample));
}

//-----------------------------------------------------------------------------
// BasicDispatchDemo1 - Show use of parameters and return value
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int BasicDispatchDemo1(int Dummy, int Param1, int Param2)
{
   printf("BDB1 Param1: %d, Param2: %d\n", Param1, Param2);

   return Param1 + Param2; // return value of call in basic
}

//-----------------------------------------------------------------------------
// BasicDispatchDemo2 - Show use of dispatch number
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int BasicDispatchDemo2(int InDispatch)
{
   switch (InDispatch) {
      case 0 : printf("Keyword zero\n"); break;
      case 1 : printf("Keyword one\n"); break;
      case 2 : printf("Keyword two\n"); break;
   }
   return 0;
}

//-----------------------------------------------------------------------------
// MSM_Sample - Mission State Machine sample - MINIMAL VERSION
//-----------------------------------------------------------------------------
// See DemoMissionSample.cpp for more info on MSM's
//-----------------------------------------------------------------------------
static bool MSM_Sample(TMissionHandler *M, int &State, int NewState)
{
   printf("MSM_Sample done.\n");
   return true;   // always done
}
