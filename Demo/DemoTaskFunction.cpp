//---------/--------------------------------------------------------------------
// DemoTaskFunction.cpp 
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to create a Task function.
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
// along with RobotLib.  If not, see <http://www.gnu.org/licenses/>.
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

#define DEMO_NAME DemoTaskFunction

//-------------
// OVERVIEW
//-------------
/* 
This demo shows how you can create your own task function and have it
executed by RobotLib's task system. 
Task functions are (mostly) general function that are executed each milisecond
or at MAIN_TAKT interval.
*/

//-------------
// DECLARATIONS 
//-------------     
void DemoTask();
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for TaskFunction.\n");   

   // In most RobotLib applications, there are three
   // task list available:
   //MsTasks.Add(FP_FNAME(DemoTask));       // Executed everey ms
   MainTasks.Add(FP_FNAME(DemoTask));     // Executed every main interval
   //IdleTasks.Add(FP_FNAME(DemoTask));     // Executed whenever there is some spare time.s
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for TaskFunction.\n");
   printf("No specific command executed, see DemoTask output on terminal & see source code.\n");
}   

//-------------
// OTHER CODE 
//-------------

//-----------------------------------------------------------------------------
// DemoTask - print a message, every once in a while
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DemoTask()
{  static int c;
   
   c++;
   if (c>100) {
      printf("==> DemoTask trigger.\n");
      c = 0;
   }
}