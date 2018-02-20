//-----------------------------------------------------------------------------
// DemoLineFollower.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use LineFollower.
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

#define DEMO_NAME DemoLineFollower

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------

// We need the input from a line sensor. For this demo,
// the Qrt8A is used.
TLineSensorQtr8a LineSensor;

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for LineFollower.\n");


   // Setup the linesensor, required to demonstrate the
   // use of the LineFollower. See the linesensor demo
   // for details on the setup
   CliAddCommands(CliLineSensor, "LineSensor");   
   for (int i=0; i<8; i++) {       
      LineSensor.AdcIn[i].SetSrc(Max127Read, i);
   }
   MainTasks.Add(FP_FNAME(LineSensor));
   LineSensor.On();
   LineSensor.Reverse = false;  
   // LineSensor setup done.


   // ----------------------
   // Setup the LineFollower
   // ----------------------
   
   // Set the source of the LineFollower and the value 
   // when the line is at the center of the sensor
   LineFollower.SensorInput.SetSrc(LineSensor, SC_LS_VALUE_INTERPOLLED);
   LineFollower.SensorOffset = 0; 
   LineSensor.SetCalibration(220,580);  // put your own calibration values here
   
   // Set the speed used to follow the line and the
   // settings of the P-controller
   LineFollower.SpeedSetpoint = 200;
   LineFollower.PID.SetTunings(0.01,0,0);  // Kp, Ki & Kp

   // There is a grace periode when the line is lost.
   // During this period, the robot drives straight,
   // assuming the line will be found again. Use this
   // to cope with small interruptions of the line
   // or sensor glitches.
   LineFollower.NoLineGraceTicks  = 4;       
   LineFollower.GraceCorrect      = 0;  // drive straigth during grace period

   // When the line is lost (and grace period has passed)
   // the robot will try to find the line again. It will turn
   // left or right, depending on the last line value.
   LineFollower.RegainLineTicks   = 200;  // Try to find the line for this many ticks
   LineFollower.RegainCorrect     = 10;   // +/- degrees, relative to current heading
   
   // Set Mode:
   //  0 Follow line     - use when you're on the line
   //  1 Find line left  - Start 'RegainLine' mode to the left
   //  2 Find line right - Start 'RegainLine' mode to the right   
   LineFollower.Mode = 0; 
}                                             

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  int Action = 0;
     
   printf("Demo command for LineFollower - ");
 

   if (NrParams == 1) Action = P[0].PInt; // use 1th param as action (action 0 = default)
 
   // You could change the setup of the line follower here,
   // depending on the specific requirements for this
   // movement. 
   // For the demo, we use the settings from DefaultDemoSetup()

   switch (Action) {
      case 0 : {
         printf("Follow line.\n");
   
         // Let's follow the line:
         UmLineFollower();       
         break;
      }

      case 1 : {
         printf("Calibrate LineSensor.\n");
   
         // This MSM calibrates the linesensor by a turn to left and right.
         // Make sure the robot is on the line when this MSM is started!
         Mission.Start(FP_FNAME(MSM_LineSensorCalibrate));
         break;
      }

      case 2 : {
         printf("Calibrate LineSensor & follow line.\n");
         
         // This MSM first calibrates the linesensor using MSM_LineSensorCalibrate
         // Next, it follows the line by a call to UmLineFollower.         
         Mission.Start(FP_FNAME(MSM_Cal_LineFollow));
         break;
      }   
   }
}   

//-------------
// OTHER CODE 
//-------------
