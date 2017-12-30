//-----------------------------------------------------------------------------
// DemoPFKeyTable.cpp
//
// Copyright (c) 2013-2017 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the Programmable Function Keys via a table.
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

#define DEMO_NAME DemoPfKeyTable

//-------------
// OVERVIEW
//-------------
/*
   //---------------------------
   // Generic info about PFKEY's
   //---------------------------

   Up to 14 keys of the remote control (RC) can be configured* to be
   'programmable function keys' (PFKeys). There are 2 versions:
   1. If one of the keys is pressed, the 'PFKey handler' is called with 
      the key # as parameter value. 
   2. If one of the keys is pressed, the String from the provided table 
      is passed to the CommandLineInterpreter.

   You can swich between PFKey handlers from your program or on the console.
   This enables re-use of your scarce RC keys in different contexts.
   
   This allows you to link your own actions to the PFKeys.     
   
   Notes:                            
   * The active PFKey handler handles all function keys.
   * On first call to PFKeyHandlerSet(), the PFKey handler is registered and 
     activated. 
   * Only one PFKey handler can be active at the time.
   * When PFKeyHandlerSet() is called with another PFKey handler, the previous 
     one remains registered but is no longer active.
   * Call PFKeyHandlerSet() again to re-activate a PFKey handler in your program.
   * Use console command pflist to see all registered PFKey handlers.
   * Use console command pfset <#> to activate registered PFKey handler.
   * Console command pf <#> is equivalent to pressing PFKey # on your remote. 
   * The mapping of RC (remote control) codes to PFKeys is configured in 
     'RobotLib_conf.h'.  
 
   //----------
   // This DEMO
   //----------

   This demo shows how to create your own PFKey table, how to register it
   and how to re-activate it.

*/     

//-------------
// DECLARATIONS 
//-------------

// For access from other source files, add a external reference
// to your table to project.h like
extern const TPfKeyTable PfKeyTableDemo;

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
   printf("DemoSetup for PfKeyTable.\n");

   // The only setup required is registration of our table.
   PfKey.HandlerSet(PfKeyTableDemo); 
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for PfKeyTable\n");      
   printf("Use commands 'pflist' and 'pfset' to manage PFKEY tables (and handlers) runtime\n");
   printf("Command 'pf <n>' is equivalent to pressing PF key n\n\n");
   
   // re-activate our PFKey handler
   PfKey.HandlerSet(PfKeyTableDemo); 

   printf("Handler 'PfKeyTableDemo2' re-activated\n");      
}                                

//-------------
// OTHER CODE 
//-------------


static TPfKeyRecord PfKeyRecord[] = {
   // Key   Name           String
	{  1,    "ShowPos",     "position"},
	{  4,    "DebugPfKey",  "dlevel 2 3"}, 
	{  5,    "PosReset",    "positionreset" }, 
	
	{  0,    "",            ""} // Indicate end of list
};

const TPfKeyTable PfKeyTableDemo = { "PfDemoTable", PfKeyRecord };