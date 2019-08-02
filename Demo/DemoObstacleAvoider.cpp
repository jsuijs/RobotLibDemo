//-----------------------------------------------------------------------------
// DemoObstacleAvoider.cpp
//
// Copyright (c) 2013-2019 Joep Suijs - All rights reserved.
//
// This demo shows how to use ObstacleAvoider.
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

#define DEMO_NAME DemoObstacleAvoider

// note: need to find a better way to access required sensors for setup....
extern TVL53L0x VL53L0xL, VL53L0xM, VL53L0xR;
extern int VlArrayLL, VlArrayLM, VlArrayLF, VlArrayRF, VlArrayRM, VlArrayRR;

//-------------
// OVERVIEW
//-------------
/*
The obstacle avoider is implemented as a class and called via universal movement
UmObstacleAvoider().

This Demo shows how to setup the Avoider.
*/

//-------------
// DECLARATIONS
//-------------

//-------------
// INSTANCES
//-------------

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for ObstacleAvoider.\n");

   // Link 3 sensors to class.
   ObstacleAvoider.Sensor[0].Input.SetSrc(VlArrayLM);    // left (~ 45 degrees)
   ObstacleAvoider.Sensor[1].Input.SetSrc(VlArrayLF);    // forward
   ObstacleAvoider.Sensor[2].Input.SetSrc(VlArrayRM);    // right (~ -45 degrees)
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for ObstacleAvoider.\n");
   printf("   Call ObstacleAvoider via 'um avoid'\n");
}

//-------------
// OTHER CODE
//-------------
