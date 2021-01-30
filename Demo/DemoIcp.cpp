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
#include "project.h"

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

// prototype for my mapping function (see below)
static TPoint FindMyCLosestPoint(const TPoint &InPoint);

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for Icp - Iterative Closest point..\n");

   for (int x=0; x<1000; x+= 10) IcpReference.Add(x, 0);
   for (int y=0; y< 600; y+= 10) IcpReference.Add(0, y);
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void WEAK CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  static TPose pose;

   printf("Demo command for Icp - Iterative Closest point.\n");

   if (NrParams < 1) {
      printf("use: demo <n>, where n is the sub command\n");
      printf("   Please look in the source code for valid values of n\n");
      printf("   (Sorry, no solid demo yet...)\n");
      return;
   }

   switch (P[0].PInt) {

      case 1 : {
         printf("Publish Reference\n");
         IcpReference.Publish(1);
      }
      break;

      case 2 : {
         printf("Update position to fixed location\n");
         Position.SetPose(300, 300, 0);
         Position.Print();
      }
      break;

      case 10 : {
         printf("Load IcpTarget from Lidar\n");
      	IcpTarget.Clear();
      	MyLidar.GetPoints(IcpTarget, 180, 270);

         printf("IcpTarget.Count: %d\n", IcpTarget.Count());
      }
      break;

      case 11 : {
         printf("Publish IcpTarget\n");
         IcpTarget.Publish(2);
      }
      break;

      case 20 : {
         if (NrParams < 2) P[1].PInt = 9;
         printf("IcpAllign %d itterations\n", P[1].PInt);
         StopWatchReset();

         IcpAlign(FindMyCLosestPoint, IcpTarget, pose, P[1].PInt);   // ** the works **

         printf("tijd: %d us\n", StopWatchGet());
         printf("pose %d %d %f\n", pose.X, pose.Y, pose.Angle);
      }
      break;

      case 21 : {
         printf("Update position to pose\n");

         pose.X      += Position.X;
         pose.Y      += Position.Y;
         pose.Angle  += Position.Angle;

         Position.SetPose(pose);
         pose -= pose;  // clear pose
         Position.Print();
      }
      break;

      default : {
         printf("Error: invalid sub-command %d\n", P[0].PInt);
      }
      break;
   }
}

//-------------
// OTHER CODE
//-------------


//-----------------------------------------------------------------------------
// FindMyCLosestPoint - Stub function
//-----------------------------------------------------------------------------
// Create this function to map Icp search requests to your world-reference
//-----------------------------------------------------------------------------
static TPoint FindMyCLosestPoint(const TPoint &InPoint)
{
   // seach for InPoint in list of reference points
   return IcpReference.Search(InPoint);
}
