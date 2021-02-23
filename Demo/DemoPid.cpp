//-----------------------------------------------------------------------------
// DemoPid.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.
//
// This demo shows how to use the PID controller.
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

#define DEMO_NAME DemoPIDController

//-------------
// OVERVIEW
//-------------
/*
   A PID controller is control loop feedback mechanism for
   (more or less) linear processes.

   The core of this PID controller is Brett Beauregard excellent
   PID libarary for arduino.

   This core routines are integrated into robotlib through the
   takt class and command line interface.
*/

//-------------
// DECLARATIONS
//-------------


//-------------
// INSTANCES
//-------------
TPID Pid1;

static int MyInput, MySetpoint;

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for PID controller.\n");

   // Setup PID controller
   Pid1.Name      = "DemoPid";
   Pid1.SetSampleTime(MAIN_TAKT_INTERVAL);         // Sample time in milliseconds (MAIN_TAKT_TIME is default).
   if (Pid1.NeedDefaults()) {
      Pid1.SetTunings(2.0, 0.1, 0.1);                 // Set Kp, Ki & Kd
   }

   Pid1.Input.SetSrc(MyInput);                     // Tell the PID where to find the input (see SourceConnect for more details and options).
   Pid1.Setpoint.SetSrc(MySetpoint);               // Tell the PID where to find the setpoint (see SourceConnect for more details and options).
   Pid1.SetOutputLimits(2, 4095);                  // set output range. Note: value 2 (or 1, not 0) makes it more obvious that the limit is hit.

   Pid1.SetTunerType(PID_TYPE_SOME_OVERSHOOT_PID, false); // default type, tells the autotuner how to transform Ku & Pu to Ki, Kp and Kd

   MainTasks.Add(FP_FNAME(Pid1), 5);               // Add to TaktList with order 5 (after sensors, which in general have default order 0).

   MySetpoint = 1024;
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for PID controller.\n");

   // Note: we have no process to control and no feedback, so running
   // the PID controller is quite useless.
   // 'Demo' TestPid shows a working PID controller with a simulated process.

   // Link output back to input
   MyInput = (MyInput + Pid1.Output) / 2;

   if (NrParams == 0) {
      // no params -> print output
      printf("Pid %s %d -> %d\n", Pid1.Name, MyInput, Pid1.Output);
      return;
   }
   if (NrParams == 1) {
      // one param -> setpoint
      MySetpoint = P[0].PInt;
      printf("Pid %s setpoint: %d\n", Pid1.Name, MySetpoint);
      return;
   }
}

//-------------
// OTHER CODE
//-------------
