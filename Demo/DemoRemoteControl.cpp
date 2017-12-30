//-----------------------------------------------------------------------------
// DemoRemoteControl.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to handle undefined RC codes.
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

#define DEMO_NAME DemoRemoteControl

//-------------
// OVERVIEW
//-------------
/* 
   RobotLib allows you to link pre-defined functions to remote 
   control (RC) buttons. 
   There are basically three groups of actions:
   
   1. Buttons, claimed by Robotlib. These claims are done through defines
      in Prj_RobotLib_conf.h and handled in RcDispatcher. Amongst these,
      there are up to 12 programmable function keys (PFKeys) you could use.
      See the demos on PfKeys for details.

   2. Codes, linked to command strings in the CmdRepository. In Framework.cpp,
      a number of commands are added for RobotLib and you can add commands
      to this list.

   3. A custom handler of all 'left-over' codes.

   This demo shows options 2 and 3.   
   
   Note: 
   This mechanism is used to handle one-shot 'button numbers' from 
   an RC5 remote control, but can used with the same ease to handle
   button pushes from other sources. Or from a mix of sources
   as long as the button numbers are unique.
*/     

//-------------
// DECLARATIONS 
//-------------

// No declarations required.

//-------------
// INSTANCES 
//-------------

// No instances required.

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for RemoteControl.\n");

   // add command '?' to repository and 
   // link it with RC code 5964
   CmdStringRepository.Add(5964,   "?");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for RemoteControl\n");      
   printf("Use your remote control & check console for messages\n");   
}                                

//-------------
// OTHER CODE 
//-------------

//-----------------------------------------------------------------------------            
// RcUserDispatch - This function is called for unused RC codes
//-----------------------------------------------------------------------------            
// Parameter: RC code value
//-----------------------------------------------------------------------------            
void RcUserDispatch(int Code)
{
   printf("RemoteControl: called with RC code %d.\n", Code);   
   
   switch(Code) {
      case 4128: {
         printf("RemoteControl: Code A received\n");
         break;
      }        
      case 4129: {
         printf("RemoteControl: Code B received\n");
         break;
      }        
      default : {
         printf("RemoteControl: Unhandled RC Code %d received\n", Code);
         break;
      }                                                
   }
}