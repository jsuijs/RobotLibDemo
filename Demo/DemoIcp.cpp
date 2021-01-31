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
* load a reference, a list of points or list of lines that might be seen by the sensor.

* make sure the robot (estimated) position and orientation are somewhere near the actuals.

* get target poins from sensor (e.g. lidar).

* use IcpAllign to get an estimate of the actual position. Since it is itterative, you can
  call it multiple times to get better results (if time permits).

* Update position to pose (== ICP estimate)

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
//#define USE_REF_POINTS        // see comments in DefaultDemoSetup
#define ICP_MAX_POINTS 1500   // max # of points in list

// prototype for my mapping function (see below)
static TPoint FindMyCLosestPoint(const TPoint &InPoint);

//-------------
// INSTANCES
//-------------

#ifdef USE_REF_POINTS
   TPointList IcpReferencePoints(ICP_MAX_POINTS);  // Roboramabaan or alternative
#else
   TLineSegmentList IcpReferenceLines(10);
#endif

TPointList IcpTarget(ICP_MAX_POINTS);     // Measurements

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for Icp - Iterative Closest point..\n");

#ifdef USE_REF_POINTS
   // points referene is the 'old' way. Usefull when the world map is not based on (known) lines.
   // note that 160 points example is about 6x slower than 2-line setup...
   for (int x=0; x<1000; x+= 10) IcpReferencePoints.Add(x, 0);
   for (int y=0; y< 600; y+= 10) IcpReferencePoints.Add(0, y);
#else
   // world reference based on lines
   IcpReferenceLines.Add(0, 0, 3600,    0);
   IcpReferenceLines.Add(0, 0,    0, 1200);
#endif

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
#ifdef USE_REF_POINTS
         IcpReferencePoints.Publish(1);
#else
         IcpReferenceLines.Publish();
#endif
      }
      break;

      case 10 : {
         printf("Load IcpTarget from Lidar\n");
      	IcpTarget.Clear();
      	MyLidar.GetPoints(IcpTarget, 135, 315);

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
#ifdef USE_REF_POINTS
   // seach for InPoint in list of reference points
   return IcpReferencePoints.Search(InPoint);
#else
   return IcpReferenceLines.ClosestPoint(InPoint);
#endif
}
