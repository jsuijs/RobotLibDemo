//-----------------------------------------------------------------------------
// DemoPulseIn_Exti.cpp
//
// Copyright (c) 2013-2020 Joep Suijs - All rights reserved.
//
// This demo shows how to use PulseInExti.
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

#define DEMO_NAME DemoPulseInExti

//-------------
// OVERVIEW
//-------------
/*
   Show use of PulseIn and Exti (external interrupt, on STM32F4) to measure IO pulse width.
   -
   Hardware used is an VL53L0x module with pwm output.
   -
   This demo measures high time of each cycle, which is
   proportional to the distance measured by the sensor.
*/

//-------------
// DECLARATIONS
//-------------

//-------------
// INSTANCES
//-------------
TPulseIn Sensor1(0.1);  // Scale time to Value. Each microseconds is 0.1 mm.

//-----------------------------------------------------------------------------
// DefaultDemoSetup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DefaultDemoSetup()
{
   printf("DemoSetup for PulseIn & Exti.\n");

   // Init Exti (external interrupts) in general
   HalExtiInit();

   //--------------------------------------------------------------------------
   // Setup pin D1 for EXTI.
   // * GPIO Pin is set to input
   // * Both rising and falling edge create an interrupt.
   //   Options: EXTI_Trigger_Rising
   //            EXTI_Trigger_Falling
   //            EXTI_Trigger_Rising_Falling
   //
   // * Interrupt on EXTI_Line1 call 'ExtiIsr1(PinValue)'
   //
   // Note: it seems EXTI_Line1 must be equal to GPIO_Pin_1 for this to work.
   //
   HalExtiInitPin(GPIOD, GPIO_Pin_1, EXTI_PortSourceGPIOD, EXTI_PinSource1, EXTI_Line1 ,EXTI_Trigger_Rising_Falling);


   //-----------------------------------------------------------------------
   // Interrup Service Routine 'ExtiIsr1' below shows use of TPulseIn class.
   //-----------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// CliCmd_DefaultDemo -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{
   printf("Demo command for PulseIn & Exti (external interrupts).\n");
   printf("Sensor1.Value: %d\n", Sensor1.Value);
}

//-------------
// OTHER CODE
//-------------

// void ExtiIsr1(bool PinValue) { if (PinValue) Sensor1.Start(); else Sensor1.End(); }
void ExtiIsr1(bool PinValue)
{
   if (PinValue) {
      Sensor1.Start();  // Start time on Sensor1 (TPulseIn)
   } else {
      Sensor1.End();    // End time on Sensor1 and calculate result value (Sensor1.Value)
   }
}
