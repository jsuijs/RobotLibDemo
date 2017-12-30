//-----------------------------------------------------------------------------
// DemoJoyStickControl.cpp
//
// Copyright (c) 2013-2017 Joep Suijs - All rights reserved.        
//
// This demo shows how to use JoyStickControl.
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

#define DEMO_NAME DemoJoyStickControl

//-------------
// OVERVIEW
//-------------
/*                                     
   JoyStickControl are remote controls with a JoyStick (...) in contrast with
   'Normal' remote controls, which provide input from buttons (generally,
   via infra-red'

   This demo show how to use a JoyStickControl input to drive a robot like
   an RC car. In this case, the PPM JoyStickControl is used, which reads
   an i2c slave which decodes the PPM RC signal. 
*/

//-------------
// DECLARATIONS 
//-------------
extern TJscPpm JoyStickControl;      
void JscDemoTakt();              // only for this demo...

 
//-------------
// INSTANCES 
//-------------
TJscPpm JoyStickControl;

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for JoyStickControl.\n"); 
   
   // Add JoyStickController to tasklist
   MainTasks.Add(FP_FNAME(JoyStickControl));

   // For the demo only, add DemoTask too
   MainTasks.Add(FP_FNAME(JscDemoTakt));
   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for JoyStickControl.\n");
   JoyStickControl.Dump();
}   

//-------------
// OTHER CODE 
//-------------
void JscDemoTakt()
{  static bool InControl = false; // flag to end control properly.
   
   if ((JoyStickControl.Valid) && (JoyStickControl.Aux2 > 60)) {
      // Valid input & flip Switch SWC ON -> run robot                                                   
      UmSpeedRotationUpdate(!InControl, 5 * (JoyStickControl.LeftY - 120), -2 * (JoyStickControl.RightX - 120), UM_SCM_NONE); 
      InControl = true;        
   }  else {
      // No valid input
      if (InControl) { 
         // We were running the robot -> stop
         UmSpeedRotationUpdate(!InControl, 0, 0, UM_SCM_NONE);
         InControl = false;
      }
   }
}