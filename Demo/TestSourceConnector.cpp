//-----------------------------------------------------------------------------
// TestSourceConnector.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use SourceConnector.
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

#define DEMO_NAME DemoSourceConnector

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
TSourceConnector ScIntP, ScIntF, ScFloatP, ScFloatF;     
int   I1 = 4711;
float F1 = 3.1415926;

float SrcFunctInt(int p)   { return I1 * p; }
float SrcFunctFloat(int p) { return F1 * p; }

void TestScOutputs(TSourceConnector Sc, const char *Name)
{  int i, t;
   float f;
   bool r;
   
   printf("SourceConnector '%s'\n");

   StopWatchReset();
   i =  Sc.ReadInt();
   t = StopWatchGet();
   printf(" ReadInt:    %d (%d)\n", i, t);   
   
   StopWatchReset();
   f =  Sc.ReadFloat();
   t = StopWatchGet();
   printf(" ReadFloat:  %f (%d)\n", f, t);  

   StopWatchReset();
   r = Sc.Read(i);     
   t = StopWatchGet();
   printf(" Read:       %d (%d %d)\n", i, r, t);

   StopWatchReset();
   r = Sc.Read(f);
   t = StopWatchGet();
   printf(" Read:       %f (%d %d)\n", f, r, t);
}

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for TEST SourceConnector.\n");    
   
   ScIntP.SetSrc(I1);                   // connect to int pointer
   ScIntF.SetSrc(SrcFunctInt, 2);       // connect to int function, param/factor 2
   ScFloatP.SetSrc(F1);                 // connect to float pointer
   ScFloatF.SetSrc(SrcFunctFloat, 2);   // connect to float function, param/factor 2   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for TEST SourceConnector.\n");   
   
   TestScOutputs(FP_FNAME(ScIntP));
   TestScOutputs(FP_FNAME(ScIntF));
   TestScOutputs(FP_FNAME(ScFloatP));
   TestScOutputs(FP_FNAME(ScFloatF));
}   

//-------------
// OTHER CODE 
//-------------

