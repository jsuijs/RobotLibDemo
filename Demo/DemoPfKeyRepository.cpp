//-----------------------------------------------------------------------------
// DemoPfKeyRepository.cpp
//
// Copyright (c) 2013-2017 Joep Suijs - All rights reserved.        
//
// This demo shows how to use PfKeyRepository.
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

#define DEMO_NAME DemoPfKeyRepository

//-------------
// OVERVIEW
//-------------
/*  
   Please take a look at DemoPfKeyTable for generic information on 
   PFKeys if you are not familiar with function keys in RobotLib.
              
   This CommandStringRepository allows to specify a command string 
   on the robotlib commandline and link it to a key on the remote control. 
   Function keys of the remote control are not passed to the CmdStringRepostory,
   but can be accessed trough PfKeyRepository.   
   So mainly a runtime feature. But you might want to setup some values at startup.
   This demo shows you how. 
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for PfKeyRepository.\n");

   // It is actually quite simpel. Just provide the
   // command string and link it to the PfKey of your choice.   
   CmdStringRepository.Add(RC_F01, "um arc 90 300 100 0 0");
          
   // Framework already registered the Repository function
   // key handler.
   // You might want to 'add' it again after the last
   // one to make sure it is the active PfKey handler.
   PfKey.HandlerSet(FP_FNAME(PFKeyRepository)); 
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for PfKeyRepository.\n");     
   
   printf("\n");
   printf("Usefull commands when using the Repository PfKey handler: \n");
   printf("\n");
   printf(" 'pf list' shows an list of all handlers (number and   \n");
   printf(" number). The active handler is marked with a star and \n");
   printf(" for thishandler, the values of each PFkey are shown.  \n");
   printf("\n");
   printf(" If PfKeyRepository is not the active handler, activate\n");
   printf(" it with 'pf set n', where n is the number of the \n");
   printf(" PfKeyRepository handler shown in with pf list.\n");
   printf("\n");
   printf(" You can set a specific key with 'repos addpf <nr> <string>',\n");
   printf(" where nr is the selected PfKey and string the command \n");
   printf(" string within quotes. For example 'pf addpf 1 \"? pf\"' \n");
   printf(" sets pfkey 1 up to trigger the pf help command.\n");
   printf("\n"); 
}   

//-------------
// OTHER CODE 
//-------------
