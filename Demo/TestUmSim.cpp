//-----------------------------------------------------------------------------
// TestUmSim.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// Not a demo, but a test using the Demo structure.
// THIS IS A TEST FOR THE SIMULATOR - BE CAREFULL TO RUN IT ON A REAL ROBOT !
// This test provides various patterns to test Um movements.
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

#define DEMO_NAME TestUmSim

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS 
//-------------
static void UmSimTest1(int NrParams, TCiParams *P);
static void UmSimTest2(int NrParams, TCiParams *P);
static void UmSimTest3(int NrParams, TCiParams *P);
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for UmSim.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo/test UmSim.\n");

   if (NrParams != 1) {
      printf("Missing test number\n");
      return;
   } 
   
   switch(P[0].PInt) {
      case 1 : {
         UmSimTest1(NrParams, P);
         break;
      }     
      case 2 : {
         UmSimTest2(NrParams, P);
         break;
      }     
      case 3 : {
         UmSimTest3(NrParams, P);
         break;
      }     
      default : {
         printf("Invalid UmSimTest number (%d)\n", P[0].PInt);
         break;
      }
   }   
}   

//-------------
// OTHER CODE 
//------------- 

//----------------------------------------------------------------------------- 
// SF1 - SupportFunction
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
static void SF1(int x)
{
   //--------------------
   // primary start point
   //--------------------

   // forward curve
   UmRobotPosition(3600,0, 180);   
   UmXYPhi2(1000, 1000, x, 299, 200, 0, 200, 300);      
   
   // reverse curve
   UmRobotPosition(3600,0, 0);   
   UmXYPhi2(1000, 1000, x+180, -299, -200, 0, 200, 300);      

   //----------------------
   // secundary start point
   //----------------------

   // forward curve
   UmRobotPosition(3600,2000, 180);   
   UmXYPhi2(1000, 1000, x, 299, 200, 0, 200, 300);      
   
   // reverse curve
   UmRobotPosition(3600,2000, 0);   
   UmXYPhi2(1000, 1000, x+180, -299, -200, 0, 200, 300);      
}

static void SF1a(int x)
{
   // forward curve
   UmRobotPosition(3600,0, 180);   
   UmXYPhi2(500, 0, x, 299, 200, 0, 200, 300);      
   
   // reverse curve
   UmRobotPosition(3600,0, 0);   
   UmXYPhi2(500, 0, x+180, -299, -200, 0, 200, 300);      
}

//----------------------------------------------------------------------------- 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
static void UmSimTest1(int NrParams, TCiParams *P) 
{

   printf("Test UmXYPhi2 (quite a few patterns)\n");
   
   Mover.Reset();   

   // used to be a bug...   
   SF1a(1);
   SF1a(-1);
   SF1a(0);

   // Delta angle close to 180
   SF1(-17);
   SF1(-20);
   SF1(-24);
   
   // Delta angle close to 0
   SF1(-17+180);
   SF1(-20+180);
   SF1(-24+180);
   
   SF1(0);
   SF1(45);
   SF1(90);
   SF1(135);
   SF1(180);
   SF1(225);
   SF1(270);
   SF1(315);
   SF1(-45);   // same as +315

   Mover.List();   
}

//----------------------------------------------------------------------------- 
// SF2 - SupportFunction
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
static void SF2(int x, int y)
{
   
   // forward curve
   UmRobotPosition(x, y, 180);   
   UmXYPhi2(1200, 1000,   0,  299,  200, 0, 500, 300);      
   
   // reverse curve
   UmRobotPosition(x, y, 0);   
   UmXYPhi2(1200, 1000, 180, -299, -200, 0, 500, 300);      

}


//----------------------------------------------------------------------------- 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
static void UmSimTest2(int NrParams, TCiParams *P) 
{

   printf("Test2 UmXYPhi2 \n");
   
   Mover.Reset();   

   SF2(800, 300);

   SF2(2000, -100);
   SF2(2000,  100);
   SF2(2000,  300);
   SF2(2000,  500);
   SF2(2000,  700);
   SF2(2000,  900);
   SF2(2000, 1100);
   SF2(2000, 1300);
   SF2(2000, 1500);
   SF2(2000, 1700);
   SF2(2000, 1900);
   SF2(2000, 2100);
   SF2(2000, 2300);

   SF2(   0, 2400);
   SF2( 200, 2400);
   SF2( 400, 2400);
   SF2( 600, 2400);
   SF2( 800, 2400);
   SF2(1000, 2400);
   SF2(1200, 2400);
   SF2(1400, 2400);
   SF2(1600, 2400);
   SF2(1800, 2400);


   SF2(0, -100);
   SF2(0,  100);
   SF2(0,  300);
   SF2(0,  500);
   SF2(0,  700);
   SF2(0,  900);
   SF2(0, 1100);
   SF2(0, 1300);
   SF2(0, 1500);
   SF2(0, 1700);
   SF2(0, 1900);
   SF2(0, 2100);
   SF2(0, 2300);
  
   Mover.List();   
}


//----------------------------------------------------------------------------- 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
static void UmSimTest3(int NrParams, TCiParams *P) 
{

   printf("Test UmArc2 relative to UmArc\n");
   
   Mover.Reset();   

   UmRobotPosition(0,0, 0);   
   UmXY(1200, 0,  600, 0);      
   UmArc(90,  1200, 600, 0, 0);
   for (int i=0; i<5; i++) {
      UmRobotPosition(0,0, 0);   
      UmXY(1200, 0,  600, 600);      
      UmArc(90,  1200, 600, 0, 0); 
      UmStop(500);
   }
   for (int i=0; i<5; i++) {
      UmRobotPosition(0,0, 0);   
      UmXY(1200, 0,  600, 300);      
      UmArc(90,  1200, 600, 0, 0); 
      UmStop(500);
   }

   Mover.List();   
}