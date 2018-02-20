//-----------------------------------------------------------------------------
// DemoVL53L0x.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use VL53L0x ToF ranging sensor.
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

#define DEMO_NAME DemoVL53L0x
               
//-------------
// OVERVIEW
//-------------
/*               

*/

//-------------
// DECLARATIONS 
//-------------
// For access from other source files, add prototypes to project.h like
extern TVL53L0x   VL53L0x;    // Note: this prototype is also in VL53L0x.h, so you
                              //       can omit it. However, you need this prototype
                              //       for any sensor with a different name. 
 
//-------------
// INSTANCES 
//-------------    

TVL53L0x    VL53L0x("MyVl53l0x", VL53L0X_DEFAULT_I2C_ADDRESS);     // VL53L0x instance with i2c slave address as parameter 
                        
//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for VL53L0x ToF ranging sensor.\n");     
   
   // Init sensor. 
   // If the I2C address is not the default one, this
   // call also updates the sensor address.  
   VL53L0x.Init();   
   
   //--------------------------------------------
   // Add sensor to task list.
   // The sensor is checked every cycle via i2c, so the
   // main tasklist is recommended. For less delay, you 
   // could choose the milisecond task list. But this
   // only makes sense if you also use the data
   // more than once per maintakt interval.
   //--------------------------------------------
   MainTasks.Add(FP_FNAME(VL53L0x));


   //--------------------------------------------
   // Since any VL53L0x has the default address
   // on startup, you might want to use an
   // i2c mux like PCA8544A to connect multiple
   // sensor. SetI2cMux let you specify a i2c
   // slave address and a single databyte that 
   // is sent to that address, every time before
   // the VL53L0x is accessed. I2c address 0 
   // (defautl) disable this feature.
   //--------------------------------------------
   //VL53L0x.SetI2cMux(0xE0, 4+0);  // PCA9544A i2c mux enable + select CH0 


   // Optional disable on startup:             
   //VL53L0x.Enable = false; // default is true,

   // Optional add output to Registry
   Registry.Add("VL53L0x", VL53L0x.Distance); 
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for VL53L0x laser ranging sensor.\n");
   
   printf("VL53L0x distance: %d mm\n", VL53L0x.Distance);   
}   

//-------------
// OTHER CODE 
//-------------
