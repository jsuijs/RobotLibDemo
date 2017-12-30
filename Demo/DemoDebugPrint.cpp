//-----------------------------------------------------------------------------
// DemoDebugPrint.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use various print functions for debugging.
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

#define DEMO_NAME DemoDebugPrint

//-------------
// OVERVIEW
//-------------
/*
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
   printf("DemoSetup for DebugPrint.\n");
   
   //--------------------------------------------------------------------------
   // The debug print systeem is part of the RobotLib core, so
   // no additional setup is required.
   // You might want to change settings of the system on startup.
   //--------------------------------------------------------------------------
   
   //--------------------------------------------------------------------------
   // Select the desired header format.
   //   0 - (default) long header, contains level, module, fname, lineno
   //   1 - short header, contains module and a letter to indicate level
   //   2 - shortest header, Letter D plus module number.                                                               
   //   
   // See TDebug::MyPrintf in debug.cpp for more details (use the source, Luke)     
   Debug.HeaderFormat(1);              
      
	//--------------------------------------------------------------------------
   // Change level of indicated module.
   // RLM_RC is the module number and defined in rlib_cpp.h.
   // Modules below 100 are reserved for RobotLib.    
   //
   // Modules from 100 are available for users.
   // Put:
   //    #define NR_USER_DEBUG_CLASSES 20        
   // in prj_RobotLib_conf.h to support module 100 to 119.
   //
   // Available levels are:
   //
   //    DBG_ERROR      0  // minimal output, only errors.
   //    DBG_WARN       1  // Warings & errors 
   //    DBG_INFO       2  // Above + info, shows what module is doing 
   //    DBG_DEBUG      3  // maximal output
   // 
   // Each level includes the lower levels. Levels can be changed runtime via 
   // the command line with 'dlevel <module> <level>'.    
   Debug.SetLevel(RLM_RC, DBG_WARN);      // RC unhandled codes
   Debug.SetLevel(101, DBG_INFO);         // User class

   // When TimeStampResolution is non-zero, timestamps are added to the log 
   // messages. Time stamps are 3 digits within parenthesis in units of
   // milliseconds / TimeStampResolution.
   Debug.TimeStampResolution = 10;  // enable timestamps in 10ms resolution
   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo of DebugPrint\n");
   
   INFO_printf(101,  "CliCmd_Demo called with %d params\n", NrParams);
   WARN_printf(101,  "Be warned there is a debug message that won't be shown unless enter 'dlevel 101 3'\n");
   DEBUG_printf(101, "This won't be visible with the default settings from DemoSetup\n");   
   ERROR_printf(101, "This message can't be disabled by changing the debug level\n");

   // Check level to execute code conditionally
   if (Debug.IsLevel(101, DBG_WARN)) {
      printf("Level 101 is Level is warning or higher (INFO, DEBUG).\n");
      printf("(Did you notice there is no header on these lines?)\n");
      const char *s = "TestString";
      HexDump(s, 8, 0);
   }   
}                                
 