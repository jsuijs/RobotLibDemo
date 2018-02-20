//-----------------------------------------------------------------------------
// DemoMPU6050.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to use MPU6050 IMU unit.     
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

#define DEMO_MPU6050

//-------------
// OVERVIEW
//-------------
/*    
   MPU6050 has an accelerator + gyro. Reading of those is documented.
   The unit can also fuse the two signals and stream the result. This is not
   documented by the supplier, but reverse engineerd. The result is implemented
   by I2cDevLib (which is now ported to Robotlib). 
   The stream contains quite some data and, as a result, a load on the i2c bus.
   
   TMpu6050 is a 'robotlib style' takt class. It is build on the I2cDevLib. 
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------
TMpu6050 MyMpu;

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for MPU6050 IMU unit.\n");
   printf("NOTE: MPU6050 LIB IS STILL IN ALPHA STATE!\n");

   MyMpu.Init();   
   MyMpu.mpu.setFullScaleGyroRange(3);  // least sensitive = least likely to clip
   MyMpu.mpu.setDLPFMode(4);  // 20 Hz bandwidth
   MyMpu.Enabled = true;
   MainTasks.Add(FP_FNAME(MyMpu));   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for MPU6050 IMU unit.\n");
   // read raw accel/gyro measurements from device 
         MyMpu.DumpRaw();         

   MyMpu.Angle = 0;

}   

//-------------
// OTHER CODE 
//-------------

