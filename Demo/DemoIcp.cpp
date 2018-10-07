//-----------------------------------------------------------------------------
// DemoIcp.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.
//
// This demo shows how to use ICP - Icp - Iterative Closest point..
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

#define DEMO_NAME DemoICP

//-------------
// OVERVIEW
//-------------
/*
Sorry, still a crude demo...

Steps of using ICP:
* (10) load a reference, a group of points that might be seen by the sensor.

* (2) make sure the robot (estimated) position and orientation are somewhere near the actuals.

* (11) get target poins from sensor (e.g. lidar).

* (20) use IcpAllign to get an estimate of the actual position. Since it is itterative, you can
  call it multiple times to get better results (if time permits).

* (21) Update position to pose (== ICP estimate)

Notes:
* IcpAllign (the core algorithm) works by calculating the offset of each sensor
  reading to its closest point of the reference. The position estimate is (sort of)
  average of all these offsets.
  Hence;
  - A few large outliers on a relatively low number of points will give quite some
    offset on the result. So take many points and suppress outliers if possible
  - This algorthm is not so good (probably: unable) to deal with large errors in
    robot angle, like 45 degrees or more in a area with right walls.
  - Nor will this algorithm detect you're in totally diferent place than you
    thought you were.
* New sensor readings are (in general) based on the robot position. So any
  readings taken before the position update are inaccurate (unless corrected).
  So in general, wait for a full lidar scan after a position update.


*/

//-------------
// DECLARATIONS
//-------------

//-------------
// INSTANCES
//-------------
#include <time.h>

#define ICP_MAX_POINTS 1500     // max # of points in list

int LastRev = -1;


clock_t start, end;
double cpu_time_used;


TPointList IcpReference(ICP_MAX_POINTS);  // Roboramabaan or alternative
TPointList IcpTarget(ICP_MAX_POINTS);     // Measurements


//-----------------------------------------------------------------------------
// DemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DemoSetup()
{
   printf("DemoSetup for Icp - Iterative Closest point..\n");
}

//-----------------------------------------------------------------------------
// CliCmd_Demo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_Demo(int ParamCounter, int *Params)
{  static TPose pose;

   printf("Demo command for Icp - Iterative Closest point.\n");

   if (ParamCounter != 1) {
      printf("use: demo <n>, where n is the sub command\n");
      printf("   Please look in the source code for valid values of n\n");
      printf("   (Sorry, no solid demo yet...)\n");
      return;
   }

   switch (Params[0]) {

      case 1 : {
         printf("Dump Reference\n");
         IcpReference.DumpAll(0);
         break;
      }

      case 2 : {
         printf("Update position to fixed location\n");
         Position.SetPose(300, 300, 0);
         Position.Print();
         break;
      }

      case 10 : {
         printf("Load IcpTarget (fixed setup)\n");
      	IcpTarget.Clear();
         for (int i=0;   i<360; i++) IcpTarget.Add(i*10, 30);
         printf("IcpTarget.Count: %d\n", IcpTarget.Count());
// Alternative: load roboramabaan (Roborama Track)
//      	if (IcpReference.Count() == 0) {
//         	IcpReference.Clear();
//            PlGetRoboramaBaanPoints(IcpReference);
//            printf("Reference.Count: %d\n", IcpReference.Count());
//            //Reference.Dump();
//         }
         break;
      }

      case 11 : {
         printf("Load IcpTarget from Lidar\n");
      	IcpTarget.Clear();
      	RpLidar.GetPoints(IcpTarget, -90, 90);

         printf("IcpTarget.Count: %d\n", IcpTarget.Count());
         break;
      }

      case 12 : {
         printf("Dump IcpTarget\n");
         printf("\n!** Color 0 0 255\n");
         IcpTarget.DumpAll(0);
         break;
      }

      case 20 : {
         printf("IcpAllign\n");
         StopWatchReset();
         IcpAlign(IcpReference, IcpTarget, pose, 1);

         printf("tijd: %d us\n", StopWatchGet());
         printf("pose %d %d %f\n", pose.X, pose.Y, pose.Angle);

         break;
      }

      case 21 : {
         printf("Update position to pose\n");
// old code, remove when new code below has been tested
//         Position.CorrectXY(Position.X + pose.X, Position.Y + pose.Y);
//         Position.CorrectAngleHires(Position.AngleHires() + (int)(256* RAD2GRAD(pose.Yaw)));

         // Calculate NewPose of robot (which is current position + pose calculated by ICP)
         TPose NewPose;
         NewPose.X      = pose.X + Position.Pose.X;
         NewPose.Y      = pose.Y + Position.Pose.Y;
         NewPose.Angle  = pose.Angle + Position.Pose.Angle;

         Position.SetPose(NewPose);   // SetPose(X, Y, Degrees) is not used, since it has a lower angular resolution...
         Position.Print();
         break;
      }

   }
}

//-------------
// OTHER CODE
//-------------
