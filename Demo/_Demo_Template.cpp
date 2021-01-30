//-----------------------------------------------------------------------------
// _Demo_Template.cpp
//
// Copyright (c) 2013-2020 Joep Suijs - All rights reserved.
//
// This demo shows how to use XYZ.
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
// along with RobotLib. If not, see <http://www.gnu.org/licenses/>.
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
#include "project.h"

#define DEMO_NAME DemoXYZ

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

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for XYZ.\n");
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for XYZ.\n");
}

//-------------
// OTHER CODE
//-------------
