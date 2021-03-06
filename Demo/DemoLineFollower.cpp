//-----------------------------------------------------------------------------
// DemoLineFollower.cpp
//
// Copyright (c) 2013-2020 Joep Suijs - All rights reserved.
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

#ifndef ADC_MAX127
// Not MAX127 => ADS1015
TADS1015 Ads1015_0(ADS_SDA_I2C_ADDRESS);
TADS1015 Ads1015_1(ADS_GND_I2C_ADDRESS);
#endif

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for LineFollower.\n");

   //----------------------------------------------------------------
   // Setup the linesensor - see DemoLineSensorQTR8A.cpp for details
   // Linesensor is required to demonstrate
   // use of the LineFollower.
#ifdef ADC_MAX127
   //-----------------------------------------------
   // SETUP FOR MAX127
   //-----------------------------------------------
   // -> one AdcReadFunction for all sensors
   // -> A different AdcChannel nr for each sensor
   for (int i=0; i<8; i++) {
      LineSensor.AdcIn[i].SetSrc(Max127Read, i);
   }
#else
   //-----------------------------------------------
   // SETUP FOR ADS1015
   //-----------------------------------------------
   // -> link to 8 source integers
   //-----------------------------------------------
   LineSensor.Input[0].SetSrc(Ads1015_0, 0);
   LineSensor.Input[1].SetSrc(Ads1015_0, 1);
   LineSensor.Input[2].SetSrc(Ads1015_0, 2);
   LineSensor.Input[3].SetSrc(Ads1015_0, 3);
   LineSensor.Input[4].SetSrc(Ads1015_1, 0);
   LineSensor.Input[5].SetSrc(Ads1015_1, 1);
   LineSensor.Input[6].SetSrc(Ads1015_1, 2);
   LineSensor.Input[7].SetSrc(Ads1015_1, 3);
#endif
   MainTasks.Add(FP_FNAME(LineSensor));
   if (LineSensor.NeedDefaults()) {
      LineSensor.SetCalibration(220,580);
      LineSensor.LightDarkSwap = false;
      LineSensor.LeftRightSwap = false;
   }
   LineSensor.On();
   // LineSensor setup done - see DemoLineSensorQTR8A.cpp for details
   //----------------------------------------------------------------

   // ----------------------
   // Setup the LineFollower
   // ----------------------

   // Set the source of the LineFollower and the value
   // when the line is at the center of the sensor
   LineFollower.SensorInput.SetSrc(LineSensor);
   LineFollower.SensorOffset = 0;
   LineSensor.SetCalibration(220,580);  // put your own calibration values here

   // Set the speed used to follow the line and the
   // settings of the P-controller
   LineFollower.SpeedSetpoint = 200;
   LineFollower.PID.SetTunings(0.2,0,0);  // Kp, Ki & Kp

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
   LineFollower.RegainCorrect     = 30;   // +/- degrees/sec
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

         // Let's follow the line, parameter is mode:
         //  0 Follow line     - use when you're on the line
         //  1 Find line left  - Start 'RegainLine' mode to the left
         //  2 Find line right - Start 'RegainLine' mode to the right
         UmLineFollower(0);
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
