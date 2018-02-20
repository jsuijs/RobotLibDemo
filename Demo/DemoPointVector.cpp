//-----------------------------------------------------------------------------
// DemoPointVector.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use Points and Vectors.
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

#define DEMO_NAME DemoPointVector

//-------------
// OVERVIEW
//-------------
/*
The TPoint class and the TVector class are implemented as
different sides of the same coin: 
* The point class is the X,Y representation of a point in
  the coordinate system. 
* The vector class represents the Lenght, Angle from the 
  origin (0, 0) to a point in the coordinate system.
Both representations have their own strenghts: vectors allow 
easy rotation (e.g. relative to robot heading) and scaling 
(e.g. drive Length - 100mm), but adding two vectors requires
use of points (X,Y representation).

This pair of classes is designed to hide these details from
the user. Calculations (assing, add, subtract) with a mix of
points and vectors are supported.

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
   printf("DemoSetup for Points and Vectors.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for Points and Vectors.\n"); 
   
   // Create a vector and a point class, just
   // like you would create normal variable.
   TVector Vector;   
   TPoint  Point;
   
   //-------------------------------------------------------                                                         
   // Assume we are driving around in a obstacle free area.
   // At some point, we are provided with (500, 1000) as the 
   // coordinates of a soda can we need to retrieve
   //-------------------------------------------------------                                                         
   Point.X = 500;
   Point.Y = 1000;

   // Our current location:
   printf("Robot position - X: %d, Y: %d\n", Position.Pose.X, Position.Pose.Y);
                                                            
   //-------------------------------------------------------                                                         
   // Our gripper requires a distance of 150mm to the target 
   //-------------------------------------------------------                                                         
   
   // Get vector from Robot (current position) to Point. 
   // Note: The vector angle is now coordinate system relative,
   //       independant of the robot heading.
   Vector = Point - Position.Pose;   // Note: Pose behaves like a Point (actually, Pose is a superset of Point)
   
   // subtract 150 mm offset
   Vector.Length -= 150;
   
   // We can use the vector for UmDistanceHeading. If we want to
   // use UmXY, we need an absolute point.
   Point = Position.Pose + Vector;
   
   printf("Target point - X: %d, Y: %d\n", Point.X, Point.Y);
   
   // If you prefer the vector values:
   Vector = Point;
   printf("Target vector - length: %d, heading: %d (deg)\n",
         Vector.Length, Vector.Degrees());

   // And in (native) radials:                                  
   printf("Target vector - length: %d, heading: %f (rad)\n",
         Vector.Length, Vector.Angle);
}   

//-------------
// OTHER CODE 
//-------------
