//-----------------------------------------------------------------------------
// DemoLidarLite.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use LidarLite laser ranging sensor.
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

#define DEMO_NAME DemoLidarLite
               
//-------------
// OVERVIEW
//-------------
/*               
   * multiple instances can be created, one for each sensor.
*/

//-------------
// DECLARATIONS 
//-------------
// For access from other source files, add prototypes to project.h like
extern TLidarLite   LidarLite;   
 
//-------------
// INSTANCES 
//-------------    

TLidarLite LidarLite(LIDAR_LITE_DEFAULT_I2C_ADDRESS);     // LidarLite instance with i2c slave address as parameter 
                        
//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for LidarLite laser ranging sensor.\n");
   //--------------------------------------------
   // Add sensor to task list.
   // The task has it's own timing, so you could
   // use the Milisecond task or the idle task. 
   // (see Cycle time below)
   //--------------------------------------------
   MsTasks.Add(FP_FNAME(LidarLite));
   //IdleTasks.Add(FP_FNAME(LidarLite));

   // Optional disable on startup:             
   // LidarLite.Enable = false; // default is true,

   // Optional add output to Registry
   //Registry.Add("LidarLite", LidarLite.Distance); 
   //Registry.Add("LLite Q", LidarLite.SignalStrength); 
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for LidarLite laser ranging sensor.\n");
   
   printf("LidarLite distance: %d mm, SigStrength: %d\n", LidarLite.Distance, LidarLite.SignalStrength);
   
   // Cycle time / FixedDelay is an indication of the sensor speed.
   // Note: cycle time excludes state machine call time. About 3-4 calls 
   //       (state transistions) are required for a reading, which is 
   //       about 3-4 ms when MsTakt is used. 
   //       IdleTasks yield a more accurate result of this value, but 
   //       also yield more i2c traffic.
   printf("FYI: cycle time: %d ms\n", LidarLite.FixedDelay);
   
}   

//-------------
// OTHER CODE 
//-------------
