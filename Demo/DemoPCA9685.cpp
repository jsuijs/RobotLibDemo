//-----------------------------------------------------------------------------
// DemoPCA9685.cpp - 8 ch PWM module with i2c interface
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use a PCA9685 to control 3 servo's. Up to 16 servo's
// can be controled by one PCA9685. 
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

#define DEMO_NAME DemoPCA9685

//-----------------------------------------------------------------------------
// DECLARATION

// For access from other source files, add prototypes of
// each servo to project.h like
extern TServo   ServoL, ServoR;   
// or
extern TServo   ServoL;
extern TServo   ServoR;

//-----------------------------------------------------------------------------
// INSTANCES

// The PCA9685 instance. Functions PCA9685_Init() and PCA9685_ServoSet() expect this 
// instance to be named 'PCA9685'. If you want another name, for instance for a second
// controller, you need to create your own versions of these functions.

TPCA9685    PCA9685;

// Define two servo's and link them to our PCA9685 controller            
TServo      ServoL("Servo-L", PCA9685_ServoSet, 0);
TServo      ServoR("Servo-R", PCA9685_ServoSet, 1);


// Note: Servo instances are automaticly added to the
// chain of Servos, so they can be accessed by the
// servo menu commands. Just try 'servo list'.




//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup of PCA8685\n");
   
   // init default PCA9685 to contol servo's
   PCA9685_ServoInit();   
     
   //------------------------------
   // Setup defaults for each servo
   //------------------------------
   
   ServoL.Min  = 600;     // minimal pulse width (us)
   ServoL.Max  = 2600;    // maximal pulse width (us)
   ServoL.A    = 103;     // A & B define relation between Hoek and pulse width
   ServoL.B    = 1190;         
   ServoL.Angle(90);      // Initial value
   ServoL.Step = 20;      // Max turn speed of servo, set AFTER initial Angle() 
   ServoL.Enable(true);   // Enable servo output
   MsTasks.Add(FP_FNAME(ServoL));   // Add servos to task list, required to support slow moves (step != 0)


   ServoR.Min  = 600;
   ServoR.Max  = 2600;      
   ServoR.A    = -103;
   ServoR.B    = 2380;         
   ServoR.Step = 20;
   ServoR.Angle(90);
   ServoR.Enable(true);
   MsTasks.Add(FP_FNAME(ServoR)); // Add servos to task list, required to support slow moves (step != 0)
  
  // add commands (already in Framework.cpp) 
//   CliAddCommands(CliServo,            "Servo");  
}


//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Servo use via PCA9685:\n");
   printf(" Servo A: Servo Raw value to set servo to 0 degrees.\n");
   printf(" Servo B: Delta of Servo Raw value for each degree, sign deterimes direction\n");
   printf(" Use servo commands to determine parameters.\n");   
   
   printf("\nPCA9685.DumpRegisters:\n");
   PCA9685.DumpRegisters();
}