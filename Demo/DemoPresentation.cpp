//-----------------------------------------------------------------------------
// DemoPresentation.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use the Presentation class.
//
// RobotLib tags: RED_SAMPLE
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
// Deze file is een RODE SAMPLE file.
// Een RODE file is een standaard file, die voor iedere toepassing gelijk is.
// SAMPLE files zijn voorbeeld files. Plaats deze in de projectdirectory en 
// pas ze aan voor het project.
//-----------------------------------------------------------------------------
// tags_end
//----------------------------------------------------------------------------- 

#define DEMO_NAME DemoPresentation

//-------------
// OVERVIEW
//-------------
/*     
   The presentation class provides information on the
   status of the robot for presentation. By default it
   provides the robot position (x, y and degrees).
   Additional data can be added. 

   The format of the data is: 
   [DATA] P_x:4 P_y:0 Hd:0 [/DATA]

   The data is sent to the console port.
   
   When the data is sent, is controlled by mode:
      0 - Off  (do not print)
      1 - Auto (print when the robot has moved)
      2 - On   (print at fixed interval)        
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------
static int Test0;
static int Test1;

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for Presentation.\n");  
   
   // give test vars a distinctive value
   Test0 = 12345;
   Test1 = 98765;
   
   // Add tag 'dm' to provide Test0 data. 
   Presentation.Add("dm", Test0); 
   
   // Show only data when robot moves.
   Presentation.Mode = 1;
   
   // Show data once every 100ms                                   
   Presentation.Interval.SetMs(100);
                                         
   // Show configuration
   Presentation.Dump();   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for Presentation.\n");

   if (NrParams == 0) {  
      printf("Demo <n> (n=0..2) changes dm setup\n");
      Presentation.Dump();
      return;   
   }

   switch(P[0].PInt) { 
      case 0 : {              
         printf("Delete dm tag\n");
         Presentation.Delete("dm");
         break;
      }
      case 1 : {
         printf("Set dm tag to Test0 (12345)\n");
         Presentation.Add("dm", Test0);
         break;
      }
      case 2 : {
         printf("Set dm tag to Test1 (98765)\n");
         Presentation.Add("dm", Test1);
         break;
      }
   }
}   

//-------------
// OTHER CODE 
//-------------
