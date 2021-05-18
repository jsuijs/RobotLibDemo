//-----------------------------------------------------------------------------
// TestSensor2.cpp
//
// Copyright (c) 2013-2021 Joep Suijs - All rights reserved.
//
// Not a demo, but a test using the Demo structure.
// This test controls the 'StepperTestRig' to move the target and reports
// target position & sensor reading(s).
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

#define DEMO_NAME TestSensor

//-------------
// OVERVIEW
//-------------
/*
*/

//-------------
// DECLARATIONS
//-------------
bool MSM_Test(TMissionState &M);

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

   if (NrParams == 1) {
      I2cWrite_Byte_Word(0x32, 3, P[0].PInt); // move to target position
   }

   if ((NrParams < 3) || (NrParams > 4)) {
      printf("Use: demo distance_mm step_mm #samples [delay]\n");
      printf(" or  demo target_point_mm\n");
      return;
   }

   // Store parameters for Mission State Machine
	Mission.ParamSet(0, P[0].PInt);  // Total backward distance
	Mission.ParamSet(1, P[1].PInt);  // stepsize
	Mission.ParamSet(2, P[2].PInt) ; // # of samples/step
   if (NrParams == 4) {
   	Mission.ParamSet(3, P[3].PInt) ; // delay
   } else {
      Mission.ParamSet(3, 0) ; // delay
   }

	// Start mission
	Mission.Start(FP_FNAME(MSM_Test));
}

//-------------
// OTHER CODE
//-------------

//-----------------------------------------------------------------------------
// MSM_Test - Mission State Machine
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool MSM_Test(TMissionState &M)
{
   // Static vars retain their value between calls of the MSM.
   static int StepCounter;
   static int SampleCounter;
   static int SampleDelay;

   // Get the relevant numbered mission param & store it in a properly named
   // auto-var
   int TotalDist  = M.ParamGet(0);
   int StepSize   = M.ParamGet(1);
   int NrSamples  = M.ParamGet(2);
   int Delay      = M.ParamGet(3);

   int TestRigState;
   I2cRead_Byte_Byte(0x32, 2, TestRigState);

   switch (M.State) {
      case 0 : { // initial state

            // Initial action
            StepCounter = 0;
//            I2cWrite_Byte_Byte(0x32, 5, 20); // set step delay
            I2cWrite_Byte_Word(0x32, 3, 0); // move to start position
            M.State += 10; // immediate to the next state
         }
         break;

      case 10 : { // wait for the testrig to finish
         if (M.NewState) {
            break;   // allow time for TestRig to update state
         }
         if (TestRigState == 0)  M.State = 20;
         if (TestRigState == 99) printf("RigState: %d\n", TestRigState);
         break;
         }

      case 20 : { // measure
            if (M.NewState) {
               // OnEntry action
               SampleCounter = 0;
               SampleDelay = 0;
            }

            if (TestSensorValue == NULL) {
               printf("Error: TestSensorValue is NULL, terminate\n");
               return true;
            }

            if (SampleDelay >= Delay) {
               printf("x / sample: %d %d", StepCounter * StepSize, *TestSensorValue);
               if (TestSensorValue1 != NULL) printf(" %d", *TestSensorValue1);
               if (TestSensorValue2 != NULL) printf(" %d", *TestSensorValue2);
               if (TestSensorValue3 != NULL) printf(" %d", *TestSensorValue3);
               if (TestSensorValue4 != NULL) printf(" %d", *TestSensorValue4);
               printf("\n");

               SampleDelay = 0;
               SampleCounter ++;
               if (SampleCounter > NrSamples) {
                  M.State += 10;         // To the next state
               }
            } else {
               SampleDelay++;
            }
         }
         break;

      case 30 : { // Drive to next point
            if (M.NewState) {
               // OnEntry action

               if ((StepCounter * StepSize) >= TotalDist) {
                  // samples done, drive back to start
                  M.State = 40;
                  break;
               } else {
                  // next step
                  StepCounter ++;
                  I2cWrite_Byte_Word(0x32, 3, StepCounter * StepSize);
                  printf("%d Step to %d (%d %d)\n", (int)millis(), StepCounter * StepSize, StepCounter, StepSize);
               }
            }

            M.State = 10;          // sampling
         }
         break;

      case 40 : { // Drive back to start point
            if (M.NewState) {
               // OnEntry action
               I2cWrite_Byte_Word(0x32, 3,  0); // move to start position
            }

            return true;   // done
         }
         break;

      default : { // error (or done?)
            printf("Default state (%d), ending mission\n", M.State);
            return true;            // mission end
         }
         break;
   }
   return false;                    // Mission not done yet
}
