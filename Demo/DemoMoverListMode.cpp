//-----------------------------------------------------------------------------
// DemoMoverListMode.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use Mover to handle a list of movements.
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

#define DEMO_NAME DemoMoverListMode

//-------------
// OVERVIEW
//-------------
/*    
The Mover object executes 'Universal movements' (um). It can be used to
execute a single UM at the time.

This demo shows how to use the Mover to handle a list of UM's.
Multiple 'universal movements' are supplied to the Mover at setup. 
This list of movements are executed sequentially by the Mover.

In this demo, we provide a list of movements to run the UMBTEST

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
   printf("DemoSetup for Mover in List mode.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for Mover.\n");   

   // A few vars to make the code more readable and easier to adopt:
   int CwCcw      = 1;     // CounterClockWise (1) or ClockWise (-1)
   int Distance   = 400;   // Length of one side of the square
   int Speed      = 300;   // Driving speed
            
   //--------------------------------------------------------------------------
   // Add steps of the movement.   
   // In this demo, we use a list of movements to run the UMBTEST 
   // Note: The first movement added will stop the current 
   //       movement / list of movements (if any). All
   //       subsequent movement in the same pass will 
   //       be added to the list of movements.  
   //--------------------------------------------------------------------------

   Mover.Reset();       // Reset discards any movements in the list
                        // It is only required if you need to make sure no
                        // movements are added to the list in this pass.
                        // *DO NOT* use Mover.Reset to stop robot movement.
                        // Use UmStop().
   
   // STEP: Drive 1th side of the square
   UmXY(Distance, 0, Speed, 0); // X, Y, Speed, EndSpeed - all in mm(/sec)

   // STEP: Turn to +90 of -90 degrees, depending on MissionHandler.ParamGet(0)
   UmRotate(90 * CwCcw, true); // Heading (in degrees)
   
   // STEP: Drive to X, Y or X, -Y, depending on MissionHandler.ParamGet(0)
   UmXY( Distance, 
         Distance * CwCcw, 
         Speed, 
         0); // X, Y, Speed, EndSpeed - all in mm(/sec)
            
   // STEP: Turn to 180 degrees
   UmRotate(180, true); // Heading (in degrees)

   // STEP: Drive to 0, Y or 0, -Y, depending on MissionHandler.ParamGet(0)
   UmXY( 0, 
         Distance * CwCcw, 
         Speed, 
         0); // X, Y, Speed, EndSpeed - all in mm(/sec)       

   // STEP: Turn to -90 or +90 degrees, depending on MissionHandler.ParamGet(0)
   UmRotate(-90 * CwCcw, true); // Heading (in degrees)
   
   // STEP: Drive to 0, 0
   UmXY(0, 0, Speed, 0); // X, Y, Speed, EndSpeed - all in mm(/sec)
   
   // STEP: Turn to 0 degrees
   UmRotate(0, true); // Heading (in graden)

   //--------------------------------------------------------------------------
   // All steps are in the list. If you want, you can print the overview. 
   //--------------------------------------------------------------------------
   Mover.List();  // list of movement to the consolse


   //--------------------------------------------------------------------------
   // At this point, the mover is about to start executing the list. This 
   // will take a while and we can't (blocking) wait for this. In a 
   // MissionStateMachine, we might want to wait until all movements are done
   // before taking the next action. Use 'if (Mover.IsDone()) {' for this.
   //--------------------------------------------------------------------------   
}   

//-------------
// OTHER CODE 
//-------------
