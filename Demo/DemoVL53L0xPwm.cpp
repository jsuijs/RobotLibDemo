//-----------------------------------------------------------------------------
// DemoVL53L0xPwm.cpp
//
// Copyright (c) 2013-2021 Joep Suijs - All rights reserved.
//
// This demo shows how to use VL53L0x ToF ranging sensor.
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

#define DEMO_NAME DemoVL53L0xPwm

//-------------
// OVERVIEW
//-------------
/*

*/

//-------------
// DECLARATIONS
//-------------
// For access from other source files, add prototypes to project.h like

//-------------
// INSTANCES
//-------------
TPulseIn VL53L0xL(0.1);  // Scale time to Value. Each microseconds is 0.1 mm.
TPulseIn VL53L0xM(0.1);  // Scale time to Value. Each microseconds is 0.1 mm.
TPulseIn VL53L0xR(0.1);  // Scale time to Value. Each microseconds is 0.1 mm.

// ISRs for interrupt-on-change. Each uses a TPulseIn instance to measure high-time.
void ExtiIsr13(bool PinValue) { if (PinValue) VL53L0xR.Start(); else VL53L0xR.End(); }
void ExtiIsr14(bool PinValue) { if (PinValue) VL53L0xM.Start(); else VL53L0xM.End(); }
void ExtiIsr15(bool PinValue) { if (PinValue) VL53L0xL.Start(); else VL53L0xL.End(); }


//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for VL53L0x ToF ranging sensormodule with PWM output.\n");

   // Init pins to Exti - interrupt on change. Corresponding ISRs are defined above.
   HalExtiInitPin(GPIOB, GPIO_Pin_13, EXTI_PortSourceGPIOB, EXTI_PinSource13, EXTI_Line13, EXTI_Trigger_Rising_Falling);
   HalExtiInitPin(GPIOB, GPIO_Pin_14, EXTI_PortSourceGPIOB, EXTI_PinSource14, EXTI_Line14, EXTI_Trigger_Rising_Falling);
   HalExtiInitPin(GPIOB, GPIO_Pin_15, EXTI_PortSourceGPIOB, EXTI_PinSource15, EXTI_Line15, EXTI_Trigger_Rising_Falling);

   // Optional add output to Registry
   // (Corresponding TPulseIn instances contain distance in mm)
   Registry.Add("VL53L0xL", VL53L0xL.Value);
   Registry.Add("VL53L0xM", VL53L0xM.Value);
   Registry.Add("VL53L0xR", VL53L0xR.Value);
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for VL53L0x laser ranging sensor.\n");

   printf("VL53L0x L/M/R distances: %d %d %d (mm)\n", VL53L0xL.Value, VL53L0xM.Value, VL53L0xR.Value);
}

//-------------
// OTHER CODE
//-------------
