//-----------------------------------------------------------------------------
// DemoMissionSample.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// A sample - Not realy a Demo, but easy-to-grab code snippets.
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

#define DEMO_NAME DemoMissionSample

//-------------
// OVERVIEW
//-------------
/*
*/


//-------------
// DECLARATIONS 
//-------------
bool MSM_Sample(TMissionHandler *M, int &State, int NewState);
 
//-------------
// INSTANCES 
//-------------

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for MissionSample.\n");
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for MissionSample.\n");
   printf("Not realy a demo, just grab the code snippets!\n");   
    
   return;  // DO NOT make the robot move ;) 
   Mission.ParamSet(0, 16);     // Set param 0 for the main mission
   Mission.Start(FP_FNAME(MSM_Sample)); 
}   

//-------------
// OTHER CODE 
//-------------

//Andere missie demos:
//* combi met menu en pfkey           
//* uitgebreide versie van deze template (tekst


//----------------------------------------------------------------------------- 
// MSM_Sample - Mission State Machine sample
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
bool MSM_Sample(TMissionHandler *M, int &State, int NewState)
{ 
   
   // static vars for the things you need to rembember between calls of the msm.   
   static int    Test;
   static TPoint Doel;                

   // Get the relevant numbered mission params & store it in a properly named
   // auto-var
   int Bonus   = M->ParamGet(0);  
   
   // auto var's here is a good idea 
   int Result;

   // <ignore> to avoid compile warnings - ignore this 
   Doel.X = Bonus;
   Result = Doel.X;
   Bonus  = Result;
   // </ignore>   
   
   switch (State) { 
      case 0 : { // initial state

            // Initial action
            
            State += 10; // immediate to the next state
         }
         break;               

      case 10 : { // generic sample  
            if (NewState) {
               // OnEntry action
            }     
                                                                 
            // Continues Action
            
            // Condition
            if (1) {                // Condition
               State += 10;         // To the next state
            }  
            if (Bonus == 17) {      // Condition
               return true;         // signal mission is done.
            }
         }
         break;               

      case 20 : { // Mover sample  
            if (NewState) {
               // OnEntry action
               UmXY(100, 100, 300, 0); 
            }                                                          

            // Continues Action

            // Condition
            if (Mover.IsDone()) {   // When movement is done
               State += 10;         // To the next state
            }     
         }
         break;               

      case 30 : { // Sub-mission sample  
            if (NewState) {    
               // OnEntry action
               M->Sub->ParamSet(0, 17);               // set param 0 to 17
               M->Sub->Start(FP_FNAME(MSM_Sample)); // start sub-mission

            }                                                          

            // Continues Action

            // Condition
            if (M->Sub->IsDone()) { // When sub-mission is done
               State += 10;         // To the next state
            }
         }
         break;               
                 
      default : { // error (or done?)
            printf("Default state (%d), ending mission\n", State);
            return true;            // mission end
         }
         break;
   }           
   return false;                    // Mission not done yet
} 
