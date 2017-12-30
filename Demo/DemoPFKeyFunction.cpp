//-----------------------------------------------------------------------------
// DemoPFKeyFunction.cpp
//
// Copyright (c) 2013-2017 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the Programmable Function Keys.
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

#define DEMO_NAME DemoPFKeyFunction

//-------------
// OVERVIEW
//-------------
/*    
   Please take a look at DemoPFKeyTable for generic information on 
   PFKEY's if you are not familiar with function keys in RobotLib.
    
   This demo shows how to create your own PFKey handler, how to register it
   and how to re-activate it.
   This allows you to link your own actions to the PFKeys.     
     
*/     

//-------------
// DECLARATIONS 
//-------------

// For access from other source files, add a prototype of
// your PFKey handler to project.h like
const char *PFKeyFunctionDemo(int Nr, bool InfoOnly);

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
   printf("DemoSetup for PFKey.\n");

   // The only setup required is registration of our PFKey handler.
   PfKey.HandlerSet(FP_FNAME(PFKeyFunctionDemo)); 
   
   // Did you notice the FP_FNAME macro?
   // This stands for 'FunctionPointer_FunctionNAME' and expands  
   //
   //    PFKeyHandlerSet(FP_FNAME(PFKeyDemo));
   // to 
   //    PFKeyHandlerSet(PFKeyDemo, "PFKeyDemo");  
   //
   // So it provides (as required) two parameters to PFKeyHandlerSet,
   // the pointer to the function (it's address in target memory) and
   // its name in the source code. Neat trick, isn't it?  
   //
   // The name of the PFKey handler is used for logging and by
   // the 'pf list' console command.          
   //
   // You could of course provide two parameters yourself and this would
   // allow for name different from the fucction. Just make sure you 
   // update both parameters when approriate. 
   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for PFKey\n");      
   printf("Use commands 'pflist' and 'pfset' to manage PFKEY handlers runtime\n");
   printf("Command 'pf <n>' is equivalent to pressing PF key n\n\n");
   
   // re-activate our PFKey handler
   PfKey.HandlerSet(FP_FNAME(PFKeyFunctionDemo)); 

   printf("Handler 'PFKeyDemo' re-activated\n");      
}                                

//-------------
// OTHER CODE 
//-------------

//-----------------------------------------------------------------------------
// PFKeyFunctionDemo - PFKey handler by_function demo
//-----------------------------------------------------------------------------
// This function handles the 'Programmable Function Keys'
//  
// If this handler is active and a PFKey is pressed, the handler gets
// called once, with the function key number as parameter, where 1 is
// the first function key.
//-----------------------------------------------------------------------------
const char *PFKeyFunctionDemo(int Nr, bool InfoOnly)
{
   const char *Ret = NULL;

   switch(Nr) {   // case of PFKeyDemo
      case 1 : { // PFKey 1   
         Ret = "Sharp list";
         if (!InfoOnly) {
            CmdParser.ProcessLine("sharp list");             
         }
         break;             
      }   
      case 2 : {  // PFKey 2
         Ret = "PID list";
         if (!InfoOnly) {
            CmdParser.ProcessLine("pid list");
         }
         break;   
      }      
      case 3 : {  // PFKey 3
         Ret = "Move to 1000,0";
         if (!InfoOnly) {
            CmdParser.ProcessLine("um xy 1000 0 300 0");
         }
         break;   
      }
      case 10 : { // PFKey 10
         Ret = "Your code!";
         if (!InfoOnly) {                      

            //---------------------------------
            // Put any (non-blocking) code here
            //---------------------------------
            printf("PFKey #10 pressed\n");

         }
         break;   
      }      
   } 
   return Ret; 
}
