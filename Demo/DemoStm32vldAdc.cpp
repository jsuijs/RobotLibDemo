//-----------------------------------------------------------------------------
// DemoStm32vldAdc.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use ADC on the STM32 Value line discovery board.
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

#define DEMO_NAME DemoStm32vldAdc

//-------------
// OVERVIEW
//-------------
/*
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
   printf("DemoSetup for ADC on STM32 Value Line Discoveryboard.\n");
   HalAdcInit();
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for ADC on STM32 Value Line Discoveryboard.\n");   
      
   printf("ADC input 00 (pa0): %d\n", HalAdcRead(ADC_Channel_0));
   printf("ADC input 01 (pa1): %d\n", HalAdcRead(ADC_Channel_1));
   printf("ADC input 02 (pa2): %d\n", HalAdcRead(ADC_Channel_2));
   printf("ADC input 03 (pa3): %d\n", HalAdcRead(ADC_Channel_3));
   printf("ADC input 04 (pa4): %d\n", HalAdcRead(ADC_Channel_4));
   printf("ADC input 05 (pa5): %d\n", HalAdcRead(ADC_Channel_5));
   printf("ADC input 06 (pa6): %d\n", HalAdcRead(ADC_Channel_6));
   printf("ADC input 07 (pa7): %d\n", HalAdcRead(ADC_Channel_7));   
   printf("ADC input 08 (pb0): %d\n", HalAdcRead(ADC_Channel_8));
   printf("ADC input 09 (pb1): %d\n", HalAdcRead(ADC_Channel_9));
   printf("ADC input 10 (pc0): %d\n", HalAdcRead(ADC_Channel_10));
   printf("ADC input 11 (pc1): %d\n", HalAdcRead(ADC_Channel_11));
   printf("ADC input 12 (pc2): %d\n", HalAdcRead(ADC_Channel_12));
   printf("ADC input 13 (pc3): %d\n", HalAdcRead(ADC_Channel_13));
   printf("ADC input 14 (pc4): %d\n", HalAdcRead(ADC_Channel_14));
   printf("ADC input 15 (pc5): %d\n", HalAdcRead(ADC_Channel_15));
}   

//-------------
// OTHER CODE 
//-------------
