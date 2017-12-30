//-----------------------------------------------------------------------------
// DemoSensorSimLidar.cpp
//
// Copyright (c) 2013-2017 Joep Suijs - All rights reserved.        
//
// This demo shows how to use SensorSimLidar.
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

#define DEMO_NAME DemoSensorSimLidar

//-------------
// OVERVIEW
//-------------
/*      
The SensorSimLidar provides virtual sensor input. 
See DemoSensorSimulator for more details on the simulator. The focus of
this demo is on the use of the LIDAR type of sensor.

Note: make sure "#define USE_RPLIDAR_SIM" is somewhere at the top of Prj_RobotLib_conf.h
is in the
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------                 
TRpLidarSim RpLidar; 

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for SensorSimLidar.\n");    

   // load the roborama track  + 3 crosses (as soda cans)
   SimulatorTrack.AddRoboRama();     
   SimulatorTrack.AddCross(1200, 600, 70);
   SimulatorTrack.AddCross(1800, 600, 70);
   SimulatorTrack.AddCross(3000, 600, 70);
     
   // Dump track point at group 2 (can be used by RobotPresenter)
//   SimulatorTrack.DumpPoints(2);      

   // Register simulated sensor task. 
   MainTasks.Add(FP_FNAME(RpLidar));
 
   CliAddCommands(CliLidar, "RpLidarSim");      
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for SensorSimLidar.\n"); 
   printf("Use lidar commands to see output\n");   
}   

//-------------
// OTHER CODE 
//-------------
