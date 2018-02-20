//-----------------------------------------------------------------------------
// DemoPCF8574.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the PCF8574 8 bit IO extender .
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

#define DEMO_NAME DemoPCF8574

//-------------
// OVERVIEW
//-------------
/*                
   The PCF8574 is a usefull but quite simple 8 bit IO port expander with
   i2c interface. It is 'quasi-bidirectional', which means:
   * writing a 0 to a pin will pull it low, allowing up to 25mA per pin.
     Note: the total current of the chip is limited to 100mA.
   * writing a 1 to a pin will push it high if there is no load (see below).
   * Reading the pin will provide the actual state.
   The good thing about this is you don't have to worry about pin direction.
   On the other hand: this chip won't allow you to drive loads high. It does
   allow you to drive logic inputs of other chips both high and low.
   When you want to drive a LED, make sure the LED is connected to + and
   (via a resistor) to a PCF8574 pin to drive it low.
   
   The PCF8574 class presented here is probably overdone when the PCF8574
   is used for a specific task, like controling an LCD display. The class
   is usefull when the PCF8574 pins are used for independent tasks in 
   your robotlib application or in prototyping situations. 
   This class basically maintains a local copy of the PCF8574 data and keeps 
   this in sync with the actual chip, without too much i2c overhead. This
   allows you to easily read and write indiviual pins.
         
   Note: 
   The 'push high' consists of a short burst of 1mA to cope with capacity
   on the output. After this burst, a pull-up current of 30-300uA is supplied
   to maintain the high state.
   
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------      

// create an instance with the default address (PCF8574_I2C_BASE_ADDRESS, 
// which defautls to 0x40). This is when all de address pins (jumpers) are
// set to 0.
TPCF8574 PCF8574_0;

// Create an instance with another address proving the offset from the
// base address:
TPCF8574 PCF8574_1(1);

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for PCF8574.\n");
   
   // Add each PCF8574 instance to the taks list, so it reads the
   // level of the inputs and writes any changes in output levels.
   MainTasks.Add(FP_FNAME(PCF8574_0));                       
   MainTasks.Add(FP_FNAME(PCF8574_1));                          
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for PCF8574.\n");

   // Note: this demo uses only one of the two instances created. 
   
   switch (NrParams) {
      case 1 : {                    
         printf("PCF8574 digitalRead: Pin %d = %d.\n", P[0].PInt, PCF8574_0.digitalRead(P[0].PInt));
         break;         
      } 
      case 2 : {
         printf("PCF8574 digitalWrite(%d, %d).\n", P[0].PInt, P[1].PInt);
         PCF8574_0.digitalWrite(P[0].PInt, P[1].PInt);
           
         // All writes are sent automaticly to the chip at the next 
         // read/write cycle, which causes a delay of a few tens of 
         // milliseconds. When this is an issue, you could call commit. 
         
         // PCF8574_0.Commit();  // send all output changes *NOW*.         
         
         break;                  
      } 
      default : {
         printf("Use:\n");   
         printf(" Demo <p>    - read pin p.\n");   
         printf(" Demo <p><v> - write value v to pin p.\n");   

         // Print class instance details (internals)         
         PCF8574_0.Print();                 

         // print (use) all 8 bits at once:
         printf("Last input read: %d\n", PCF8574_0.PortIn);

         // similar, you might want to set all outputs at once:
         // PCF8574_0.PortOut = 0x55;
         
         break;         
      } 
   }
}   

//-------------
// OTHER CODE 
//-------------
