//-----------------------------------------------------------------------------
// DemoZK1.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use ZolderKamerUitdaging1.
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

#define DEMO_NAME DemoZK1

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS 
//-------------
int Letter_SmallSpace(int Offset, int Size);
int Letter_R(int Offset, int Size);
int Letter_o(int Offset, int Size);
int Letter_b(int Offset, int Size);
int Letter_T(int Offset, int Size);
int Letter_M(int Offset, int Size);
int Letter_C(int Offset, int Size);

#define SPEED 100
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for ZK1.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   
   int Size = 250;
   int Offset = 0;
   UmRobotPosition(0,0,0);  
  
   Offset += Letter_R(Offset, Size);
   Offset += Letter_o(Offset, Size);  
   Offset += Letter_SmallSpace(Offset, Size);      
   Offset += Letter_b(Offset, Size);
   Offset += Letter_SmallSpace(Offset, Size);  
   Offset += Letter_o(Offset, Size);   
   Offset += Letter_T(Offset, Size);   
   Offset += Letter_M(Offset, Size);   
   Offset += Letter_SmallSpace(Offset, Size);  
   Offset += Letter_C(Offset, Size);   
   UmXY(0, 0, SPEED, 0);

   Mover.List();   
}  

//-------------
// OTHER CODE 
//-------------

int Letter_SmallSpace(int Offset, int Size)
{
   UmXY(Offset+Size/3, 0, SPEED, 0);   
   UmStop(500);
   return Size/3; 
}

int Letter_R(int Offset, int Size)
{
   UmXY(Offset+0, Size*2, SPEED, 0);
   UmStop(500);
   UmXY(Size/2, Size*2, SPEED, 0);
   UmStop(500);
   UmArc(-180, Size/2, SPEED, 0, 0);
   UmStop(500);
   UmXY(0, Size, SPEED, 0);
   UmStop(500);
   UmXY(Size, 0, SPEED, 0);
   UmStop(500);
   
   return Size; 
}

int Letter_o(int Offset, int Size)
{
   UmXY(Offset+Size/2, 0, SPEED, 0);
   UmStop(500);  
   UmArc(360, Size/2, SPEED, 0, 0);
   UmStop(500);
   UmXY(Offset+Size, 0, SPEED, 0);
   UmStop(500); 
   
   return Size;
}

int Letter_b(int Offset, int Size)
{ 
   UmXY(Offset+ 0, Size*2, SPEED, 0);
   UmStop(500);
   UmXY(Offset + 0, Size, SPEED, 0);
   UmStop(500);
   UmXY(Offset + Size/2, Size, SPEED, 0);
   UmStop(500);
   UmArc(-180, Size/2, SPEED, 0, 0);
   UmStop(500);          
   UmXY(Offset+ 0, 0, SPEED, 0);
   UmStop(500);
   
   return Size;
}

int Letter_T(int Offset, int Size)
{ 
   UmXY(Offset+ Size/2,    0, SPEED, 0);
   UmStop(500);
   UmXY(Offset + Size/2,   Size + Size/2, SPEED, 0);
   UmStop(500);
   UmXY(Offset,            Size + Size/2, SPEED, 0);
   UmStop(500);
   UmXY(Offset+Size,       Size + Size/2, SPEED, 0);
   UmStop(500);

   UmXY(Offset + Size/2,   Size + Size/2, SPEED, 0);
   UmStop(500);
   UmXY(Offset+ Size/2,    0,             SPEED, 0);
   UmStop(500);

   UmXY(Offset+ Size,      0,             SPEED, 0);
   UmStop(500);
   
   return Size;
}

int Letter_M(int Offset, int Size)
{ 
   UmXY(Offset, Size, SPEED, 0);
   UmStop(500);
   UmXY(Offset + Size/2, Size/2, SPEED, 0);
   UmStop(500);
   UmXY(Offset + Size, Size, SPEED, 0);
   UmStop(500);
   UmXY(Offset + Size, 0, SPEED, 0);
   UmStop(500);
   
   return Size;
}

int Letter_C(int Offset, int Size)
{
   UmXY(Offset+Size/2, 0, SPEED, 0);
   UmStop(500);   
   UmArc(60, Size/2, SPEED, 0, 0);
   UmStop(500);
   UmRotate(45-180, 0);
   UmStop(500);
   UmArc(-405, Size/2, SPEED, 0, 0);
   UmStop(500);
   UmRotate(45+90, 0);
   UmStop(500);
   UmArc(360, Size/2, SPEED, 0, 0);
   UmStop(500); 
   UmXY(Offset+Size, 0, SPEED, 0);
   UmStop(500); 
   
   return Size;
}
