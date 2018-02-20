//-----------------------------------------------------------------------------
// DemoTiming.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows various ways of timing.
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

#define DEMO_NAME DemoTiming

//-------------
// OVERVIEW
//-------------
/* 
RobotLib has various ways to create timed actions. This demo shows 
blocking delays. You should be reluctant using (longer) blocking delays.
This demo also shows the use of Interval and stopwatch.
If you have a job to be executed at a regular interval, consider using
a Task funtion or class (see TaskFunction demo). Tasks can be called at 
a regular interval to provide a solid timing.   
*/

//-------------
// DECLARATIONS 
//-------------
 
//-------------
// INSTANCES 
//-------------
// Setup Two Interval objects, with 5 secs (5.000 ms) interval.
TInterval DemoInterval(5000);
TInterval DemoDelay(5000);

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for Timing.\n");
  // If required, change Interval with:
  //DemoInterval.Set(10000);       
  
  // Setup DemoDelay object to have AT LEAST 5 seconds delay
  DemoDelay.IntervalMode = false;
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command Timing.\n");  
   static int OnlyOnce = true;

   printf("Issue this command often (optionally, with a parameter).\n");

   if (NrParams != 0) {
      printf("Any parameter on this command will restart intervals\n");
      DemoInterval.SetNext();    // restart Interval 
      DemoDelay.SetNext();       // restart Delay
   }
   
   //-----------------
   // start stopwatch
   //-----------------
   StopWatchReset();
   
   //--------------------------------------------------------------------------
   // millis() returns - as you would expect - the time in milliseconds since
   // the last reset.
   printf("millis() return now %d\n", millis());

   if (OnlyOnce) {
      OnlyOnce = false; // execute blocking stuff only the first time DEMO is called
      
      //--------------------------------------------------------------------------
      printf("One second blocking delay, will cause timing issues\n");
      delay(1000); // blocking delay in, not recommmended after startup.
   
      //--------------------------------------------------------------------------
      printf("100 microseconds blocking delay is okay (but not too often in one cycle)\n\n");
      delayMicroseconds(100); // short blocking delays are acceptable    
   }
   
   //--------------------------------------------------------------------------
   printf("On AVERAGE every 5 seconds, stars will be printed\n");
   if (DemoInterval.Due()) {
      //-----------------
      // Interval is due!
      //-----------------                        
      // This instance is set to 5 seconds with IntervalMode = true
      //
      // If the interval is due at t=5 and Interval.Due() is 
      // called at t=7, it returns true and the next interval is set to t=10.
      // In other words: Interval tries to have the *average* interval time
      // at the set rate.
      // When the Interval.Due() call is delayed for a full period or more 
      // (when it is due at t=5 and check at t=10 or later), the next interval
      // is set to Now + Interval.
      printf("%d ***************************\n", millis());
   }

   //--------------------------------------------------------------------------
   printf("After AT LEAST 5 seconds, dollars will be printed\n");
   if (DemoDelay.Due()) {
      //-----------------
      // Interval is due!
      //-----------------                        
      // This instance is set to 5 seconds with IntervalMode = false
      //
      // If the interval is due at t=5 and Interval.Due() is 
      // called at t=7, it returns true and the next interval is set to t=12.
      // In other words: Interval enfoces to have an interval time of *AT LEAST* 
      // the set rate.
      printf("%d $$$$$$$$$$$$$$$$$$$$$$$$$$$\n", millis());
   }

   //--------------------------------------------------------------------------
   // Read stopwatch
   // Notes:
   // * Use the StopWatch to measure execution time, not for timing. 
   // * There is only one StopWatch and other tasks might reset it. Do
   //   not rely on the StopWatch for critical tasks!
   // * On some systems, the underlying timer is 16 bits. This limits 
   //   the result of the StopWatch to 65ms.
   int j = StopWatchGet();   
   printf("This command took %d microseconden.\n", j);
}   

//-------------
// OTHER CODE 
//-------------
