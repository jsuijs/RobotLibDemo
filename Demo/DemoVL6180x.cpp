//-----------------------------------------------------------------------------
// DemoVL6180x.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use VL6180x ToF ranging sensor.
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

#define DEMO_NAME DemoVL6180x
               
//-------------
// OVERVIEW
//-------------
/*               

*/

//-------------
// DECLARATIONS 
//-------------
// For access from other source files, add prototypes to project.h like
extern TVL6180x   VL6180x;    // Note: this prototype is also in VL6180x.h, so you
                              //       can omit it. However, you need this prototype
                              //       for any sensor with a different name. 
 
//-------------
// INSTANCES 
//-------------    

TVL6180x    VL6180x("MyVl6180x", VL6180X_DEFAULT_I2C_ADDRESS);     // VL6180x instance with i2c slave address as parameter 
                        
//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for VL6180x ToF ranging sensor.\n");     

   // Init sensor. 
   // If the I2C address is not the default one, this
   // call also updates the sensor address.  
   VL6180x.Init();      
   
   // Set scaling to 3 (max range 200 * 3 = 600mm, result (Distance) remains in mm).   
   VL6180x.SetScaling(3);

   //--------------------------------------------
   // Add sensor to task list.
   // The sensor is checked every cycle via i2c, so the
   // main tasklist is recommended. For less delay, you 
   // could choose the milisecond task list...
   //--------------------------------------------
   MainTasks.Add(FP_FNAME(VL6180x));

   // Optional disable on startup:             
   //VL6180x.Enable = false; // default is true,

   // Optional add output to Registry
   Registry.Add("VL6180x", VL6180x.Distance); 
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for VL6180x laser ranging sensor.\n");
   
   printf("VL6180x distance: %d mm\n", VL6180x.Distance);   
}   

//-------------
// OTHER CODE 
//-------------
