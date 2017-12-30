//-----------------------------------------------------------------------------
// DemoLineSensorQTR8A_MAX128.cpp
//
// Copyright (c) 2013-2017 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the LineSensor.
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

#define DEMO_NAME DemoLineSensor

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS 
//-------------
extern TLineSensorQtr8a LineSensor;
 
//-------------
// INSTANCES 
//-------------
TLineSensorQtr8a LineSensor;
   
//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for LineSensor.\n"); 
   
   // register linesensor commans to command-line
   CliAddCommands(CliLineSensor, "LineSensor");   
     
   // Configure ADC inputs for linesensor   
   // -> one AdcReadFunction for all sensors
   // -> A different AdcChannel nr for each sensor
   for (int i=0; i<8; i++) {       
      LineSensor.AdcIn[i].SetSrc(Max127Read, i);
   }
  
   // Add linesensor to task list, to have it called in the main loop.
   // (the sensor reads all eight channels in one pass) 
   MainTasks.Add(FP_FNAME(LineSensor));
   
   // Calibrate mode => 
   //    continues update of min and max value.
   //    Sensor is active and can be used in calibration mode.
   //    Off() or On() ends calibration. 
   //LineSensor.Calibrate();             
   
   // Try to load configuration file from NonVolatile storage
   // if load fails, set defaults
   if (LineSensor.NeedDefaults()) {      
      LineSensor.SetCalibration(220,580);  // put your own calibration values here
   }
   
   // Sensor on (not off, not calibration)
   LineSensor.On();  

   // Reverse = light line on dark background.
   LineSensor.Reverse = false;   // default false => dark line on light background
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  int Value;
   
   printf("Demo command for LineSensor.\n");  

   // Dump all data (equivalent to CLI command 'linesensor dump')   
   LineSensor.Dump(); 
   printf("\n**************\n\n");
   printf("position of line center : %d\n",     LineSensor.LineValue );  

   // Note: ScRead returns true on succes.   
   if (LineSensor.ScRead(Value, SC_LS_VALUE_LEFT))          printf("position of left edge   : %d\n", Value);  
   if (LineSensor.ScRead(Value, SC_LS_VALUE_RIGHT))         printf("position of right edge  : %d\n", Value);  
   if (LineSensor.ScRead(Value, SC_LS_LINEBITS))            printf("one bit for each sensor : 0x%x\n", Value);
   if (LineSensor.ScRead(Value, SC_ARRAY0+3))               printf("raw sensor 3            : %d\n", Value);
   if (LineSensor.ScRead(Value, SC_ARRAY1+3))               printf("Normalised sensor 3     : %d\n", Value);   // calibrated value
   
   // This would clear all readings and calibration data:   
   //LineSensor.Clear();        

   // This would move your robot for calibration: 
   //Mission.Start(FP_FNAME(MSM_LineSensorCalibrate)); 
}   

//-------------
// OTHER CODE 
//-------------
