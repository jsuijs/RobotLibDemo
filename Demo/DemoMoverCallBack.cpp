//-----------------------------------------------------------------------------
// DemoMoverCallBack.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use Mover CallBack function.
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

#define DEMO_NAME DemoMoverCallBack

//-------------
// OVERVIEW
//-------------
/*    
The Mover object executes 'Universal movements' (um). It takes one or multiple
um* commands and sequentally executes them.

This demo shows how to use call-back funtions for specific movements. These
functions are registered to a specific movement and are called at MAIN_TAKT_INTERVAL
as long as the um is active.                     
Typical use of call-back funtions:
* track sensor readings to (early) end current movement (and start
  the next one, when in list mode).
* track sensor readings to end the list of movements.
* enhance behavior of movement by calling its update function.

The same call-back function can be registered to multiple movements.

*/

//-------------
// DECLARATIONS 
//-------------
void PrintStars(bool FirstCall);    // prototype of 1st call-back routine, code is at bottom of this file.
void DriveHeading(bool FirstCall);  // prototype of 2nd call-back routine, code is at bottom of this file.

// A constants to make the code more readable and easier to adopt:
#define DEMO_DISTANCE 1000   // Length of test track
#define DEMO_SPEED     100   // Driving speed
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for Mover CallBack function.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for Mover CallBack function.\n");   

   if (NrParams != 1) {
      printf("This demo requires one parameter:\n");
      printf("  0 - demo CallBack function (one movement).\n");
      printf("  1 - demo CallBack function (multiple movements).\n");
      return;
   }

   switch (P[0].PInt) {
      case 0: {   // demo with one movement        
         // Basic movement, enhanced with call-back
         UmSpeedLR(DEMO_SPEED, DEMO_SPEED);          // Start movement (SpeedLR)
         Mover.SetCallBack(FP_FNAME(DriveHeading));  // add call-back to last registered movement.
         break;
      }
      case 1: {   // demo with list of movements
         
         // Note: see DemoMoverList for more details on using list mode.
         
         // Setup multiple movements  
         Mover.Reset();       // Reset discards any movements in the list
                              // It is only required if you need to make sure no
                              // movements are added to the list in this pass.
                              // *DO NOT* use Mover.Reset to stop robot movement.
                              // Use UmStop().
         
         //----------
         // fill list
         //----------

         // First step is a 2 second pause. We add the 'PrintStar' call-back
         // routine, which prints a star at each call
         UmStop(2000);                             // 2 seconds pause         
         Mover.SetCallBack(FP_FNAME(PrintStars));  // add call-back to last registered movement.

         // Second step is a basic movement (SpeedLR), enhanced by a call-back routine
         UmSpeedLR(DEMO_SPEED, DEMO_SPEED);
         Mover.SetCallBack(FP_FNAME(DriveHeading));  // add call-back to last registered movement.
         
         // Third (and last) step is an universal movement without callback
         UmRotate(180, 0);  // Turn around 
         // (no callback here)
         
         // print list         
         Mover.List();  // list of previous added movement to the consolse
         break;
      }
      default : {
         printf("Error: invalid parameter (%d)\n", P[0].PInt);
         break;
      }       
   }
}   

//-------------
// OTHER CODE 
//-------------

//----------------------------------------------------------------------------- 
// PrintStars - A simple call-back routine, print a star at every call
//----------------------------------------------------------------------------- 
// The call-back routine is called at MAIN_TAKT interval, when the mover
// is executed. 
//----------------------------------------------------------------------------- 
void PrintStars(bool FirstCall)
{     
   printf("*");
}
 
//----------------------------------------------------------------------------- 
// DriveHeading - A call-back routine to show the use of UPDATE and ISDONE
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
void DriveHeading(bool FirstCall)
{   
   // This function is called from a SpeedLR movement, so we are allowed
   // to call SpeedLRUpdate(). 
   
   // Demo1: drive a heading of 0 degrees at the first half of the track. 
   // The second half of the track, we drive at 30 degrees.

   // Determine required heading   
   int HeadingSetpoint; // in degrees
   if (Position.OdoT_mm < (DEMO_DISTANCE/2)) {
      HeadingSetpoint = 0;
   } else {
      HeadingSetpoint = 30;
   }
   
   // SpeedHeading to SpeedLR logic (simplified):    
   int Correctie = Position.Degrees - HeadingSetpoint;   
   int SetSpeedL = DEMO_SPEED + Correctie;
   int SetSpeedR = DEMO_SPEED - Correctie;

   // Call Update() of active movement
   UmSpeedLRUpdate(false, SetSpeedL, SetSpeedR);           
  

   // Demo2: look at the Odometer (sensor) to see if we finished the
   // track. If so, signal the movement is done. This will trigger
   // the next movement. When the last movement is done, it signals 
   // the mover is done.
   
   if (Position.OdoT_mm > DEMO_DISTANCE) { 
      printf("StepDone\n");
      Mover.StepDone(); // movement done
   }      
}

