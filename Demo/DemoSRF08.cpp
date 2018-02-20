//-----------------------------------------------------------------------------
// DemoSRF08.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use SRF08 i2c ultrasonic sensor.
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

#define DEMO_NAME DemoSRF08
               
//-------------
// OVERVIEW
//-------------
/*               
   based on C:\MyRobot\tbot\versie1\ultrasonic.c
   Specs of sensor: http://www.robot-electronics.co.uk/htm/srf08tech.html 
   
   * multiple instances can be created, one for each sensor.
   * there is no sync between instances (yet), so you
     probably want to enable only one at the time.

*/

//-------------
// DECLARATIONS 
//-------------
// For access from other source files, add prototypes to project.h like
extern TSRF08   SRF08;   
 
//-------------
// INSTANCES 
//-------------    

TSRF08 SRF08(SRF08_DEFAULT_I2C_ADDRESS);  // SRF08 instance with i2c slave address as parameter 
                                          // (Address range 0xE0..0xFE, see sensor doc's for details).

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for SRF08 i2c ultrasonic sensor.\n");
   //--------------------------------------------
   // Add sensor to task list.
   // The task has it's own timing, so you could
   // use the Milisecond task or the idle task.
   //--------------------------------------------
   MsTasks.Add(FP_FNAME(SRF08));

   // Optional disable on startup:             
   // SRF08.Enable = false; // default is true,

   // Optional change ping interval             
   //SRF08.Interval = 90; // default is 70ms,   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for SRF08 i2c ultrasonic sensor.\n");
   
   if (SRF08.Distance < 990) {
      printf("SRF08 enable: %d, distance: %d mm, light: %d\n", SRF08.Enable, SRF08.Distance, SRF08.LDR);
   } else {
      printf("SRF08 enable: %d, no valid distance, light: %d.\n", SRF08.Enable, SRF08.LDR);
   }
}   

//-------------
// OTHER CODE 
//-------------

