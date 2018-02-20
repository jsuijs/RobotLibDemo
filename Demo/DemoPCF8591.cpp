//-----------------------------------------------------------------------------
// DemoPCF8591.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use PCF8591 i2c 4ch ADC 1 ch DAC.
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

#define DEMO_NAME DemoPCF8591

//-------------
// OVERVIEW
//-------------
/*   
   Basic implementation to init & PCF8591 i2c 
   This chip has four 8-bit ADC inputs and one 8-bit DAC output.
   
   No tasks, just calls i2c comms when functions are called.           
   
   The PCF8591 has 3 pins to select the i2c slave address.
   On the cheap modules from Ebay, all these pins are low.
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
   printf("DemoSetup for PCF8591 i2c 4ch ADC 1 ch DAC.\n");   
   PCF8591Init(PCF8591_BASE_I2C_ADDRESS);  
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  static int DacOut = 256;
   
   printf("Demo command for PCF8591 i2c 4ch ADC 1 ch DAC.\n");  

   // Increment DacOut on each call to show the output changes.   
   DacOut += 16;
   if (DacOut > 255) DacOut = 0;
   
   // Set DAC
   printf("Set PCF8591 DAC output to %d\n", DacOut);
   PCF8591Write(PCF8591_BASE_I2C_ADDRESS, DacOut);

   // Read each ADC channel
   printf("PCF8591 ADC0 : %d\n", PCF8591Read(PCF8591_BASE_I2C_ADDRESS, 0));
   printf("PCF8591 ADC1 : %d\n", PCF8591Read(PCF8591_BASE_I2C_ADDRESS, 1));
   printf("PCF8591 ADC2 : %d\n", PCF8591Read(PCF8591_BASE_I2C_ADDRESS, 2));
   printf("PCF8591 ADC3 : %d\n", PCF8591Read(PCF8591_BASE_I2C_ADDRESS, 3));
}   

//-------------
// OTHER CODE 
//-------------
