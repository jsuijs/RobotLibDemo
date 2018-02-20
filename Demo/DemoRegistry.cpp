//-----------------------------------------------------------------------------
// DemoRegistry.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use Registry.
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

#define DEMO_NAME DemoRegistry

//-------------
// OVERVIEW
//-------------
/*   

The registry is a global store for named variables.

Features:
* Variables can be either integers or floats. 
* Get & Set for both types through float values. This will cause small errors
  on high values of int's.
* Variables can be inspected via KDU
* Optionally, a dialog can be added to a registy entry. 'Dialogs' allow the
  use to change the value via kdu. See the CommandParser for details on dialogs.
* Optionally, a reference to a (global or static) variable can be provided.
  Benefits of using references:
  * Variables from classes (like settings and sensor values) can be inspected
    and changed through KDU.
  * Access to the value without the overhead of the registry.    
*/

//-------------
// DECLARATIONS 
//-------------
int DemoVar3;
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for Registry.\n");      

   // Add variable DemoVar1 of type int. Its value is stored in the registry
   Registry.AddInt("Demo-Var1");        
   
   // Add variable DemoVar2 of type float. Its value is stored in the registry
   Registry.AddFloat("Demo-Var2");        
   
   // Add variable DemoVar3. Its value is stored in the provided reference.
   // The reference is typically a static or global variable. DO NOT 
   // ADD REFERENCES TO AUTO VARIABLES!
   // The variable type (int or float) is based on the reference type.
   Registry.Add("Demo-Var3", DemoVar3);        
   
   // Add a variable from an existing class:           
   Registry.Add("Demo-Var4", Position.Degrees);       

   // Add integer variable, assign a value and add a dialog.   
   Registry.AddInt(   "simplevar");
   Registry.SetValue( "simplevar", 200) ; 
   Registry.SetDialog("simplevar", "[i|simple|100..400|50|150]") ;   
   
   // On the dialog above:
   //  i        : type (for now only integer)
   //  simple   : name on the display
   //  100..500 : min and max value of variable
   //  50       : step value
   //  150      : not relevant (this is the default / start value of 
   //             the variabele in the dialog in case its value is 
   //             unknown. But a registry variable has always a known value.)
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  static float MyVar;
   
   printf("Demo command for Registry.\n");     

   printf("Registry has %d entries\n", Registry.GetSize());
   
   // check if variable exists.
   if (Registry.Exists("MyStatic")) { 
      // Variable exists => Add PI directy to the local static
      MyVar = MyVar + 3.1415926;                              
      // Request the value from the registry.
      printf("MyStatic value: %f\n", Registry.ReadFloat("MyStatic"));  
   } else {                                  
      // Create variable with reference to the local *static*
      Registry.Add("MyStatic", MyVar);
   }
   
   // print value of simplevar. This var can be changed via kdu,
   // based on its dialog.
   // NOTE: we request the integer representation here.
   printf("simplevar value: %d\n", Registry.ReadInt("simplevar"));          
}   

//-------------
// OTHER CODE 
//-------------
