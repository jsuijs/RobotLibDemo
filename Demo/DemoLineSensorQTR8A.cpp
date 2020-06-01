//-----------------------------------------------------------------------------
// DemoLineSensorQTR8A.cpp
//
// Copyright (c) 2013-2020 Joep Suijs - All rights reserved.
//
// This demo shows how to use the LineSensor.
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

#define DEMO_NAME DemoLineSensor

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS
//-------------
extern TLineSensorQtr8a LineSensor;

//-------------
// INSTANCES
//-------------
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
   printf("DemoSetup for LineSensor.\n");

   //-----------------------------------------------
   // Configure ADC inputs for linesensor
   //-----------------------------------------------

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

   // Add linesensor to task list, to have it called in the main loop.
   // (the sensor reads all eight channels in one pass)
   MainTasks.Add(FP_FNAME(LineSensor));

   // Calibrate mode =>
   //    continues update of min and max value.
   //    Sensor is active and can be used in calibration mode.
   //    Off() or On() ends calibration.
   //LineSensor.Calibrate();

   // Try to load configuration file from NonVolatile storage
   // if load fails, set defaults
   if (LineSensor.NeedDefaults()) {
      LineSensor.SetCalibration(220,580);  // put your own calibration values here
      // LightDark depends on sensor and background.
      // LeftRightSwap depend on input configuration, sensor orientation etc.
      // Use 'ls calmsm' for automatic calibration
      LineSensor.LightDarkSwap = false;
      LineSensor.LeftRightSwap = false;
   }

   // Sensor on (not off, not calibration)
   LineSensor.On();
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  int Value;

   printf("Demo command for LineSensor.\n");

   // Dump all data (equivalent to CLI command 'linesensor dump')
   LineSensor.Dump();
   printf("\n**************\n\n");
   printf("position of line center : %d\n",     LineSensor.LineValue );

   // Note: ScRead returns true on succes.
   if (LineSensor.ScRead(SC_LS_LINE_LEFT, Value))     printf("position of left line   : %d\n", Value);
   if (LineSensor.ScRead(SC_LS_LINE_RIGHT, Value))    printf("position of right line  : %d\n", Value);
   if (LineSensor.ScRead(SC_LS_LINEBITS, Value))      printf("one bit for each sensor : 0x%x\n", Value);
   if (LineSensor.ScRead(SC_ARRAY0+3, Value))         printf("raw sensor 3            : %d\n", Value);
   if (LineSensor.ScRead(SC_ARRAY1+3, Value))         printf("Normalised sensor 3     : %d\n", Value);   // calibrated value

   // This would clear all readings and calibration data:
   //LineSensor.Clear();

   // This would move your robot for calibration:
   //Mission.Start(FP_FNAME(MSM_LineSensorCalibrate));
}

//-------------
// OTHER CODE
//-------------
