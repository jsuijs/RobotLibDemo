//-----------------------------------------------------------------------------
// DemoArIo.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use ArIo.
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

#define DEMO_NAME DemoArIo

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
TArIo ArIo(ARIO_DEFAULT_I2C_ADDRESS);   

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for ArIo - Arduino IO.\n");
   
   // Add ArIo to the taks list, so it reads
   // the level of the digital inputs and 
   // - if configured (see below) - analog inputs.
   // And... writes any changes in digital output levels.
   MainTasks.Add(FP_FNAME(ArIo));                       
      
   // The ArIO has 6 ADC ports, A0, A1, A2, A3, A6 & A7.
   // By default, none of them are read to keep the i2c 
   // message short.
    
   ArIo.NrAdc = 3;   // This activates reading of the 
                     // lower 3 adc ports (A0, A1 & A2).
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for ArIo - Arduino IO.\n");    
   
   // Use digitalRead to get the (buffered) level of a pin.
   if (ArIo.digitalRead(AR_D4)) {
      printf("Arduino digital port 4 is high!\n");
   } else {
      printf("Arduino digital port 4 is low...\n");
   }                                
  
   // You can also read the buffer directly to
   // get all Digital Input (DIn) levels at once.  
   printf("Ardiuno inputs: 0x%04x\n", ArIo.DIn);
   
   // use 'pinMode' to set pin 13 as output
   ArIo.pinMode(AR_D13, AR_OUTPUT); 
   // Note: pinMode triggers an extra i2c message
   
   // use digitalWrite to set pin 13 high                                 
   ArIo.digitalWrite(AR_D13, 1);   
   // Note: digitalWrite commands update a local copy.
   // Takt will update the Arduino based on this copy.
   
   // Read (buffered) analog input.
   printf("Analog input AR_A0 is %d\n", ArIo.analogRead(AR_A0));
   // Note: by default, no analog inputs are read / buffered.
   // See setup how to read the required number of analog ports.  
   
   // Get the analog value from the buffer
   printf("Second analog input is %d\n", ArIo.AnalogIn[1]);
   // Note: A4 and A5 are skipped in this array.
   // Index 3 corresponds to AR_A3, index 4 to AR_A6.  
}   

//-------------
// OTHER CODE 
//-------------
