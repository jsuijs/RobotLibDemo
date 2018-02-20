//-----------------------------------------------------------------------------
// DemoSharpSensor.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the Sharp distance sensor class.
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

#define DEMO_NAME DemoSharpSensor

//-------------
// DECLARATIONS 
//-------------

// For access from other source files, add prototypes of
// each sensor to project.h like
extern TSharpSensor   SharpL, SharpM, SharpR;   
// or
extern TSharpSensor   SharpL;
extern TSharpSensor   SharpM;
extern TSharpSensor   SharpR;

//-------------
// INSTANCES 
//-------------

// Define three Sharps's and link them to specific ADC read functions.
// (example for STM32F4)            
TSharpSensor      SharpL("SharpL", HalAdcRead, 11); // PC1
TSharpSensor      SharpM("SharpM", HalAdcRead, 12); // PC2
TSharpSensor      SharpR("SharpR", HalAdcRead, 14); // PC4

// Note: SharpSensor instances are automaticly added to the
// chain of SharpSensors, so they can be accessed by the
// sharp menu commands.

//-----------------------------------------------------------------------------
// DemoSetupHal - Hal specific setup for this demo (if required)
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DemoSetupHal()
{ 
   // Setup Analog-to-Digital-Convertor of the STM32F4
   // On other platforms, substitute this with equivalent code.

   // PC1 & 2 & 4 - configure pin for ADC ANALOG - Channel 11 & 12  & 14
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   
   GPIO_InitTypeDef GPIO_InitStruct; 
   GPIO_StructInit(&GPIO_InitStruct);              // clear struct (no intermittend bugs)
   GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;       // we want to configure this pins
   GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AN;     // analog mode
   GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL; // no pull up or pull down
   GPIO_Init(GPIOC,&GPIO_InitStruct);              // this passes the configuration to the Init function which takes care of the low level stuff
   
}

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for Sharp (STM32F4).\n");

   DemoSetupHal();
   
   // start ADC
   HalAdcInit();

   // setup Sharp Sensor defaults   
   SharpL.CalibrationA = 296000;  
   SharpL.CalibrationB = -100;  
   SharpM.CalibrationA = 355529;  
   SharpM.CalibrationB = -152;  
   SharpR.CalibrationA = 294000;  
   SharpR.CalibrationB = -90;  

   // Add sharp sensors to milisecond task list
   MsTasks.Add(FP_FNAME(SharpL));
   MsTasks.Add(FP_FNAME(SharpM));
   MsTasks.Add(FP_FNAME(SharpR));      
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Use sharp sub-commands for testing.\n\n");
   printf("Calibration procedure (for 100-800 mm sensor):\n");
   printf("1. Place object at 150mm\n");
   printf("2. Issue sharp cal command for sensor\n");
   printf("3. Place object at 400mm\n");
   printf("4. Issue sharp cal command again for sensor.\n");
   printf("5. Record A en B.\n");
   printf("6. Verify proper calibration with sharp print command\n");
   printf("7. Set A en B in sourcecode to make calibration permanent\n"); 
   printf("Remarks: feel free to swap step 1 and 3.\n\n");      

   // Use distance information of sensors:   
   printf("Sharp L: %d, M: %d, R: %d\n", SharpL.Distance, SharpM.Distance, SharpR.Distance);   
}                                

