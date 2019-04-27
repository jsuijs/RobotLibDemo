//-----------------------------------------------------------------------------
// TestPid.cpp
//
// Copyright (c) 2013-2019 Joep Suijs - All rights reserved.
//
// Test of the PID controller.
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

#define DEMO_NAME TestPIDController

//#define SIM_REVERSE   // reverse PID and SIM behavoir (higher input -> lower output)


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
void PidTestTakt();
int  Simulator(int);

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
   Pid1.SetSampleTime(1);                    // Sample time in milliseconds.
   Pid1.SetTunings(2.0, 0.5, 2.0);           // Set Kp, Ki & Kd
   Pid1.Name      = "DemoPid";
   Pid1.Input.SetSrc(MyInput);               // Tell the PID where the input and setpoint vars are.
   Pid1.Setpoint.SetSrc(MySetpoint);

   Pid1.SetOutputLimits(-2550, 2550);
#ifdef SIM_REVERSE
   Pid1.Reverse = true;
#endif
   MsTasks.Add(FP_FNAME(Pid1));

   MainTasks.Add(FP_FNAME(PidTestTakt));

   MySetpoint = 1800;

   // temporary
   CliAddCommands(CliPid, "Pid");

// The presentation module has been depricated.
// One day, I'll check if this demo requires an alternative.
   //Presentation.Mode = 2;  // continues
   //Presentation.Add("g0", MySetpoint);
   //Presentation.Add("g1", MyInput);
   //Presentation.Add("g2", Pid1.Output);

}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for PID controller.\n");

   if (NrParams == 0) {
      // no params -> print output
      printf("Pid %s %d -> %d\n", Pid1.Name, MyInput, Pid1.Output);



      return;
   }
   if (NrParams == 1) {
      printf("Setpoint: %d\n", P[0].PInt);
      MySetpoint = P[0].PInt;
   }
}

//-------------
// OTHER CODE
//-------------

//-----------------------------------------------------------------------------
// PidTestTakt -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PidTestTakt()
{  static int Interval;

   if (PidAutoTuner.IsDone()) {
      //we're done, set the tuning parameters
      PidAutoTuner.Preview();
      PidAutoTuner.Commit();
   }

   //send-receive with processing if it's time
   static unsigned long serialTime;
   if(millis()>serialTime) {
      serialTime = millis() + 500;

      printf("%d setpoint: %d, input: %d, output: %d ", millis(), MySetpoint, MyInput, Pid1.Output);
      if (PidAutoTuner.IsIdle()) {
         printf("Kp: %0.3f, Ki: %0.3f, Kd: %0.3f\n", Pid1.GetKp(), Pid1.GetKi(), Pid1.GetKd());
      } else {
         printf("tuning mode\n");
      }
   }


   // feed output via simulator to input
   MyInput = Simulator(Pid1.Output);


   if (Interval-- < 0) {
      Interval = 10;
      //printf("Pid %s %d -> %d\n", Pid1.Name, MyInput, Pid1.Output);
   }
}


#include <stdlib.h>
// Autotune results Ku: 3.239798, Pu: 5.200000

//-----------------------------------------------------------------------------
// Simulator - to test PID, transfers PID output to an input value.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int Simulator(int SimInput)
{  static bool First = true;
   static int PrevSimOutput;

   const float KpModel=1.5, Tau_p=100, outputStart=5;
   const int DelayLineLenght = 29;
   static int DelayLine[DelayLineLenght+1];

   if (First) {
      First = false;
      // simulator setup
      for(int i=0;i<DelayLineLenght+1;i++) {
        DelayLine[i]=outputStart;
      }
   }

#ifdef SIM_REVERSE
   DelayLine[DelayLineLenght] = -SimInput;
#else
   DelayLine[DelayLineLenght] = SimInput;
#endif

   //cycle the dead time
   for(int i=0;i<DelayLineLenght;i++) {
      DelayLine[i] = DelayLine[i+1];
   }

   //compute output of process -> input for PID
   int SimOutput = (KpModel / Tau_p) *(DelayLine[0]-outputStart) + PrevSimOutput*(1-1/Tau_p) + (rand() % 200) / 100.0 - 1;
   PrevSimOutput = SimOutput;
   //printf("Sim %d %d\n", SimInput, SimOutput);
   return SimOutput;

}