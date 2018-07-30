//-----------------------------------------------------------------------------
// DemoCmdStringRepository.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.
//
// This demo shows how to use CmdStringRepository.
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

#define DEMO_NAME DemoCmdStringRepository

//-------------
// OVERVIEW
//-------------
/*
   Please take a look at DemoPfKeyFunction for generic information on
   PFKeys if you are not familiar with function keys in RobotLib.

   The CommandStringRepository allows to specify a command string
   and optionally link it to a key on the remote control.
   The CmdStringRepository can be managed via it's console menu or from
   code as shown in this demo.
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
   printf("DemoSetup for CmdStringRepository.\n");

   // There are 3 ways to add commands to the repository.

   // 1: add a command with no reference to a remote control key
   //    This would be usefull since you can browse the command repository
   //    (with your remote control) and select commands like these.
   //    Specify a name to identify the command (brief but distinctive)
   //    and the command string itself.
   CmdStringRepository.Add("arc60", "um arc 60 300 100 0 0");

   // 2: add a command and link it to a pre-defined function key (e.g. pf5)
   CmdStringRepository.AddPf(5, "arc90", "um arc 90 300 100 0 0");

   // 3: add a command and link it to an unused RC key by using it's RC5 code:
   CmdStringRepository.Add(4537, "arc60", "um arc 60 300 100 0 0");

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
   printf("Demo command for CmdStringRepository.\n");

   printf("\n");
   printf("Usefull commands when using the CmdStringRepository for PfKey handling: \n");
   printf("\n");
   printf(" 'pf list' shows an list of all handlers. The active handler is marked \n");
   printf(" with a star and for this handler, the values of each PFkey are shown.  \n");
   printf("\n");
   printf(" If CmdStringRepository is not the active handler, activate it with \n");
   printf(" 'pf set n', where n is the number of the CmdStringRepository handler\n");
   printf(" shown in with pf list.\n");
   printf("\n");
   printf(" You can set a specific key with 'repos addpf <nr> <string>', where nr is\n");
   printf(" the selected PfKey and string the command string within quotes. For example\n");
   printf(" 'pf addpf 1 \"? pf\"' sets pfkey 1 up to trigger the pf help command.\n");
   printf(" \n");
   printf("But there is more than PF keys in the CmdStringRepository!\n");
   printf(" Check it out with '? repos' \n");
   printf("\n");
}

//-------------
// OTHER CODE
//-------------
