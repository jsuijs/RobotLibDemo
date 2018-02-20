//-----------------------------------------------------------------------------
// TestFloatIntPerformance.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use TestFloat.
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

#define DEMO_NAME DemoTestFloat

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

float fMult_F_F(float P1, float P2)
{
   return P1 * P2;
}

int iMult_F_F(float P1, float P2)
{
   return P1 * P2;
}

int iMult_I_I(int P1, int P2)
{
   return P1 * P2;
}

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for TestFloat.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{   
   volatile float xyz_f;
   volatile int   xyz_i;
   int Loops = 1000;
   if (NrParams == 1) Loops = P[0].PInt;
   printf("Demo command for TestFloat (Loops=%d).\n", Loops);       
   
   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_f = fMult_F_F(xyz_f, xyz_f);
   printf("Time f=fFF(ff): %d\n", (int)StopWatchGet());

   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_i = fMult_F_F(xyz_f, xyz_f);
   printf("Time i=fFF(ff): %d\n", (int)StopWatchGet());

   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_i = iMult_F_F(xyz_f, xyz_f);
   printf("Time i=iFF(ff): %d\n", (int)StopWatchGet());

   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_i = iMult_F_F(xyz_i, xyz_i);
   printf("Time i=iFF(ii): %d\n", (int)StopWatchGet());

   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_i = iMult_I_I(xyz_i, xyz_i);
   printf("Time i=iII(ii): %d\n", (int)StopWatchGet());

   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_i = iMult_I_I(xyz_f, xyz_f);
   printf("Time i=iII(ff): %d\n", (int)StopWatchGet());

   StopWatchReset();     
   for (int i=0; i<Loops; i++) xyz_f = iMult_I_I(xyz_i, xyz_i);
   printf("Time f=iII(ii): %d\n", (int)StopWatchGet());
}   

//-------------
// OTHER CODE 
//-------------
