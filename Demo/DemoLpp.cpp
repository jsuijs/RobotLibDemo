//-----------------------------------------------------------------------------
// DemoLpp.cpp
//
// Copyright (c) 2013-2019 Joep Suijs - All rights reserved.
//
// This demo shows how to use Lpp.
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

#define DEMO_NAME DemoLpp

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
TLpp Lpp;

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for Lpp (LidarPreProcessor).\n");

   MainTasks.Add(FP_FNAME(Lpp)); // add to taks-list


   Lpp.SetAngleOffset(180);      // Align lidar with robotlib coordinate system
   Lpp.SetReverse(1);

   Lpp.ArraySetup(-90, 20, 9);   // Setup array with 9 segments of 20 degrees
   Lpp.SensorSetup(0, -60, 120);  // Setup Sensor 0 to detect forward objects
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for Lpp.\n");

   UmRotate(Position.Degrees + Lpp.Sensor[0].Angle/32, 1, 100);
}

//-------------
// OTHER CODE
//-------------
