//-----------------------------------------------------------------------------
// DemoAx12.cpp
//
// Copyright (c) 2013-2021 Joep Suijs - All rights reserved.
//
// This demo shows how to use the Ax-12 servo gateway.
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

#define DEMO_NAME DemoAx12

//-------------
// OVERVIEW
//-------------
/*
   The Ax12 library provides access to a bus with AX12 servo's
   via an Arduino i2c to Ax12 convertor. This gateway eliminates
   the need for the specific hardware and dedicated uart required
   for this bus. The gateway is based on this hardware:
   http://robottini.altervista.org/dynamixel-ax-12a-and-arduino-how-to-use-the-serial-port

   An Ax12 bus provides access to up to 253 intelligent Ax12 servo's.
   Each servo has its own ID and 50 status- and control registers. Some
   of these registers can be accessed directly through methodes of the AX12
   class. All registers can be accessed through the read/write byte/word methodes.

   In addition to this, an interface is provided to link a TServo object to
   a specific Ax12 servo.
*/

//-------------
// DECLARATIONS
//-------------
extern TAx12 Ax12;

//-------------
// INSTANCES
//-------------

// The AX12 gateway
TAx12 Ax12(AX12_GW_DEFAULT_I2C_ADDRESS);

// Optional, create servo instances
TServo   Ax12ServoA("Ax12-A", Ax12_ServoSet, 8);   // Ax12 servo A has ID 8
TServo   Ax12ServoB("Ax12-B", Ax12_ServoSet, 9);   // Ax12 servo B has ID 9

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for Ax12.\n");

   Debug.SetLevel(RLM_AX12, 3);

   //--------------
   // Setup servo's
   //--------------

   // Note: the Ax12 servo provide many control options,
   // which are available trough the TAx12 class.

   Ax12ServoA.A    = 67;      // / Map 0..300 degrees to native range of Ax12
   Ax12ServoA.B    = 500;     // \ (see example below for more details)

   Ax12ServoA.Degrees(90);    // Initial angle

   // Ax12ServoA.Step = 40;   // ** Do use Step of the servo class, use Ax12.SetSpeed() **
   Ax12.SetSpeed(2,200);      // Set turn speed of Ax12 #2 to 200 (= 200 / 1024 * 114 rpm or 134 deg/sec)
                              // Value 0 turns speed control off (max speed)

   Ax12ServoA.Enable(true);

   // 2nd Servo,
   Ax12ServoB.A    = -88;     // / A and B map degrees to raw where raw 500 is 0 degrees and
   Ax12ServoB.B    = 2390;    // \ raw 2500 is 300 degrees (1023 in Ax12 domain)
   Ax12ServoB.Degrees(90);
   Ax12ServoB.Enable(true);

}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for Ax12.\n");
}

//-------------
// OTHER CODE
//-------------
