//-----------------------------------------------------------------------------
// DemoLipoBattery.cpp
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.        
//
// This demo shows how to use LipoBattery.
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

#define DEMO_NAME DemoLipoBattery

//-------------
// OVERVIEW
//-------------
/*
    
The LipoBattery class is used to guard the battery.

* Use a voltage divider to lower the battery voltage to the range supported 
  by the ADC (0-3V or maybe slighly higher on some systems).
  
* The class will check the voltage at regular intervals and provide the results
  in Raw, Volt*10, Percent and range (0: <low, 1: low..med, 2: med..hi, 3: >hi)
  
* An optional callback function can be supplied. This function is called after
  the battery check. It can be used to update a battery indicator, signal 
  battery low etc.
 
*/

//-------------
// DECLARATIONS 
//-------------
void LowBatteryGuard();
 
//-------------
// INSTANCES 
//-------------   

//
// The TLipoBattery class takes the ADC read function and a number
// (channel or alike, depending on ADC read function) as parameters.
//
// Note: It seems like a good idea to call the instance Battery
// and not LipoBattery. Future libraries might support other
// types of batteries with the same interface.
TLipoBattery Battery(HalAdcRead, 1);   // Battery voltage on HalAdc channel 1

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for LipoBattery.\n"); 
                                           
   // Init ADC 
   HalAdcInit();
                                           
   // Put Battery task in MainTakt list for proper timing.   
   MainTasks.Add(FP_FNAME(Battery));                           
   
   // Check battery once every 1000 ms (when in MainTakt list)   
   Battery.Interval.SetMs(1000);      

   // Volt meter calibration, raw shows 3800 when volt meter reads 16.35  
   Battery.Scale = 16.35 / 3800;
   
   // Defaults thresholds, change if required
   //Battery.ThresholdH  = 42 * 4;    // 4.2V * 4 cells, Above High / 100%
   //Battery.ThresholdM  = 35 * 4;    // Between High en Medium
   //Battery.ThresholdL  = 32 * 4;    // Beteen Medium en Low => battery empty   

   // Optional callback routine   
   Battery.SetCallBack(FP_FNAME(LowBatteryGuard)); // call it after every check             
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for LipoBattery.\n");   
   
   if (NrParams != 0) {
      // any parameter
      Battery.Dump();   // all info available
   } else {
      // No params
      Battery.Print();
   }
}   

//-------------
// OTHER CODE 
//-------------

//----------------------------------------------------------------------------- 
// LowBatteryGuard - Callback routine, called after battery check
//----------------------------------------------------------------------------- 
//----------------------------------------------------------------------------- 
void LowBatteryGuard()
{  
   if (Battery.Range < 2)  { // below Medium threshold
      printf("Remaining battery capacity: %d %%.\n", Battery.Percent);     
   }

   if (Battery.Percent < 5) {   
      MorseChar('b');
   }
}