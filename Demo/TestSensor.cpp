//-----------------------------------------------------------------------------
// TestSensor.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// Not a demo, but a test using the Demo structure.
// This test drives the robot backwards in steps and reports
// robot position & sensor reading(s).
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

#define DEMO_NAME TestSensor

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS 
//-------------
bool MSM_Test(TMissionHandler *M, int &State, int NewState);
 
//-------------
// INSTANCES 
//-------------
int *TestSensorValue;   // mandatory value
int *TestSensorValue1;  // optional value
int *TestSensorValue2;  // optional value
int *TestSensorValue3;  // optional value
int *TestSensorValue4;  // optional value

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for SensorTest.\n");
   printf("Set TestSensorValue before using Demo command\n");
//   TestSensorValue = NULL;
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo/test sensor.\n");
 
   if (TestSensorValue == NULL) {
      printf("TestSensorValue needs to be set before using this command, like:\n\n");
      printf("   extern int *TestSensorValue;\n"); 
      printf("   TestSensorValue = &SharpL.Distance;\n");
      printf("   (see source for more than one sensorvalue)\n\n");

      return; 
   }
   
   if (NrParams != 3) {
      printf("Use: demo distance_mm step_mm #samples\n");
      return;
   }
   // Store parameters for Mission State Machine
	Mission.ParamSet(0, P[0].PInt);  // Total backward distance
	Mission.ParamSet(1, P[1].PInt);  // stepsize
	Mission.ParamSet(2, P[2].PInt) ; // # of samples/step

	// Start mission 
	Position.Reset();
	Mission.Start(FP_FNAME(MSM_Test)); 
}   

//-------------
// OTHER CODE 
//------------- 

//----------------------------------------------------------------------------- 
// MSM_Test - Mission State Machine 
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
bool MSM_Test(TMissionHandler *M, int &State, int NewState)
{ 
   // Static vars retain their value between calls of the MSM.
   static int StepCounter;       
   static int SampleCounter;
   
   // Get the relevant numbered mission param & store it in a properly named
   // auto-var
   int TotalDist  = M->ParamGet(0);  
   int StepSize   = M->ParamGet(1);  
   int NrSamples  = M->ParamGet(2);  
      
   switch (State) { 
      case 0 : { // initial state

            // Initial action
            StepCounter = 0;
            State += 10; // immediate to the next state
         }
         break;               

      case 10 : { // wait a few ticks for the robot to come to a full stop
         static int Counter;
            if (NewState) {
               // OnEntry action
               Counter = 10;
            }              
            Counter --;
            if (Counter < 0) State = 20;
            break;
         }
            
      case 20 : { // measure
            if (NewState) {
               // OnEntry action
               SampleCounter = 0;
            }     
            
            if (TestSensorValue == NULL) {
               printf("Error: TestSensorValue is NULL, terminate\n");
               return true;
            }
                                    
            printf("x / sample: %d %d", StepCounter * StepSize, *TestSensorValue);            
            if (TestSensorValue1 != NULL) printf(" %d", *TestSensorValue1);
            if (TestSensorValue2 != NULL) printf(" %d", *TestSensorValue2);
            if (TestSensorValue3 != NULL) printf(" %d", *TestSensorValue3);
            if (TestSensorValue4 != NULL) printf(" %d", *TestSensorValue4);
            printf("\n");

            if (SampleCounter > NrSamples) {
               State += 10;         // To the next state
            }  
            SampleCounter ++;
         }
         break;               

      case 30 : { // Drive to next point
            if (NewState) {
               // OnEntry action        
               
               if (Position.X <= -TotalDist) {
                  // samples done, drive back to start
                  State = 40;
                  break;
               } else {
                  // next step
                  StepCounter ++;
                  UmXY(StepCounter * StepSize * -1, 0, -100, 0);
               }               
            }                                                          

            // Condition
            if (Mover.IsDone()) {   // When movement is done
               State = 10;          // sampling
            }     
         }
         break;               

      case 40 : { // Drive back to start point
            if (NewState) {
               // OnEntry action        
               UmXY(0, 0, 100, 0);
            }                                                          

            // Condition
            if (Mover.IsDone()) {   // When movement is done
               return true;   // done
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
