//-----------------------------------------------------------------------------
// DemoSensorSimulator.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use SensorSimulator.
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
S
#define DEMO_NAME DemoSensorSimulator

//-------------
// OVERVIEW
//-------------
/*      
The SensorSimulator provides virtual sensor input. The simulator
is based on a 'environment' definition of lines. A sensor reports
the distance to the closed line, based on the robot pose.
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------                 

TSensorSimulator SimSharpL( 45, 120);  // look to front-left at 45 degrees, 120 mm from center
TSensorSimulator SimSharpM(  0, 120);  // look straight, 120 mm from robot center
TSensorSimulator SimSharpR(-45, 120);  // look to front-right at 45 degrees, 120mm from center

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for SensorSimulator.\n");    
   
   // Setup the 'environment'
   SimulatorTrack.Offset(-400, -400);  // Move track origin relative to robot 
                                       // position for NEW lines.
   SimulatorTrack.AddRoboRama();       // load the roborama track
   
   // Add 3 crosses of 70mm (similar to soda cans) 
   SimulatorTrack.AddCross(1200, 600, 70);
   SimulatorTrack.AddCross(1800, 600, 70);
   SimulatorTrack.AddCross(3000, 600, 70);
   
   // Dump track lines
   SimulatorTrack.Dump();
  
   // Dump track point at group 2 (can be used by RobotPresenter)
   SimulatorTrack.DumpPoints(2);      

   // Register simulated sensor tasks. The taks will update
   // the variable 'Distance'. Note that it is good enough 
   // to call it (only) from MainTaks, but it can be called
   // each ms too (as required for the sharp sensor).    
   MainTasks.Add(FP_FNAME(SimSharpL));
   MainTasks.Add(FP_FNAME(SimSharpM));
   MainTasks.Add(FP_FNAME(SimSharpR));
  
   // report sensor output when moving, through the presentation module
   Presentation.Mode = 1;
   Presentation.Add("g0", SimSharpL.Distance);
   Presentation.Add("g1", SimSharpM.Distance);
   Presentation.Add("g2", SimSharpR.Distance);
   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for SensorSimulator.\n"); 
   printf("Use um commands to let the robot drive and see\n");
   printf("the sensor data in RobotPresenter\n");    
   
   printf("Current value of simulated SharpL: %d mm\n", SimSharpL.Distance);
   printf("Current value of simulated SharpM: %d mm\n", SimSharpM.Distance);
   printf("Current value of simulated SharpR: %d mm\n", SimSharpR.Distance);
}   

//-------------
// OTHER CODE 
//-------------
