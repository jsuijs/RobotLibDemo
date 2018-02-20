//-----------------------------------------------------------------------------
// DemoBH1750.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the BH1750 i2c light senso.
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

#define DEMO_NAME DemoBH1750

//-------------
// OVERVIEW
//-------------
/*   
   Basic implementation to init & read one of two BH1750 light sensors.
   No tasks, just calls i2c comms when functions are called.           
   
   The sensor has an pin called ADDR, which select the i2c slave address.
   This pin is default low; pull it high for the second BH1750 on the
   I2C bus.
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
   printf("DemoSetup for BH1750 i2c light sensor.\n");   
   
   printf("Try to init 2 sensors; one might give an error.\n");   
   BH1750Init(0);   // init sensor with address line low (open)
   BH1750Init(1);   // init sensor with address line high
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  int Light;
   
   printf("Demo command for BH1750 i2c light sensor.\n");  
   printf("Try to read 2 sensors; one might give an error.\n");   

   Light = BH1750Read(0);   // read sensor with address line low (default)
   printf("Light0: %d lux\n", Light);

   Light = BH1750Read(1);   // read sensor with address line high
   printf("Light1: %d lux\n", Light);
}   

//-------------
// OTHER CODE 
//-------------
