//-----------------------------------------------------------------------------
// DemoCommandLine.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to add your own commands to the command line interface.
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

#define DEMO_NAME DemoCommandLine

//-------------
// DECLARATIONS 
//-------------

// For access from other source files, add prototypes to project.h 
extern const TCliGroup CliDemo;

//-------------
// INSTANCES 
//-------------

// No instances at the top of the file. The table is at the end of
// the file, behind definition of the command functions.

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for CommandLine.\n");

   // Add our commands to the CLI module
   CliAddCommands(CliDemo, "CliDemo commands"); 
   
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Use '?' to see CliDemo commands\n");
}                                

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Setup of command above is simple. The real magic is below. Here is one
// function for each command we want to add. After that, a tables is created 
// with a record for each command. This table is used above to register this 
// group of commands.
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Each command function has the same format:
//    void CliCmd_*(int ParamCounter, int *Params)
// Since the functions are generally used in a table in the same file,  they 
// can be declared static.
//
// ParamCounter holds the number of parameter that are passed to the
// function. The content of the parameters is contained in the Params array.
// In many cases, a command requires a fixed number of parameters. We'll see
// below that Cli can be configured to enforce this for us and we don't have
// to check this again. Often, our CliCmd function only function is to convert
// from it's specific format to the format of the desired fuction.
// Below are a few examples of such conversion functions, and one example 
// to show more complex use of parameters.
//-----------------------------------------------------------------------------

// command functions that fit on one line
static void CliCmd_XyzPwm     (int NrParams, TCiParams *P) { UmPwm(P[0].PInt, P[1].PInt);                   }
static void CliCmd_XyzCurve   (int NrParams, TCiParams *P) { Mission.Start(MSM_PwmCurve, "MSM_PwmCurve");   }

// If you prefere, you could use a classic formatting:
static void CliCmd_XyzCal(int NrParams, TCiParams *P) 
{ 
   Mission.Start(MSM_MotorCal, "MSM_MotorCal");   
}

//-----------------------------------------------------------------------------
// CliCmd_CliDemo - Shows how a more complex command is implemented
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void CliCmd_XyzDemo(int NrParams, TCiParams *P)
{
   if (NrParams == 0) {
      printf("Test: No params.\n");
      return;      
   }
   
   printf("CliCmd_CliDemo() - command with %d parameters\n", NrParams);   
   for (int i=0; i<NrParams; i++) {
      printf("Int value of param %d : %d\n", i, P[i].PInt);      
   }
}

//-----------------------------------------------------------------------------
// CliCmd_XyzParams - Shows use of float & string parameters
//-----------------------------------------------------------------------------
// The params of the command are passed via an array of structs. Each field of
// his struct matches one parameters and has multiple represenstations of this
// parameter.
// Note:
//  * PInt is the integer representation. In case of parameter type:
//       float:      the rounded value of the float
//       string:     zero
//       variable:   the value from the registry
//       dialog:     selected value 
// * PFloat is the float representation. In other cases, the same value
//    than PInt.
// * Scr is the string representation. When a string is specified, it
//   contains a zero-terminated string without quotes and with the
//   the following sequences parsed: '\\', '\r', '\n', '\t' and '\"'.
//   In all other cases, it contains the original string (int, float,
//   variable name with '$', dialog between []).
//-----------------------------------------------------------------------------
static void CliCmd_XyzParams(int NrParams, TCiParams *P) 
{ 
   for (int i=0; i<NrParams; i++) {   
      printf("XyzFloat param %d - %d / %f / %s\n",
         i, P[i].PInt, P[i].PFloat, P[i].Src); 
   }
}

//-----------------------------------------------------------------------------  
// Now we have defined the desired functions, we need to create a table with
// one record per command. 
// Each record has the next fields:
//    Function    Pointer to the function to be called.   
//                The format of this functions needs to be
//                static void CliCmd_*(int ParamCounter, int *Params)
//                (where static is optional)
//    Keyword     This string is the commando that can be typed on the CLI.
//    #param      The number of params required by this command. 
//                If this value is 0 or above, CLI will check the number of 
//                parameters in advance. If an incorrect number of parameters 
//                is entered, CLI will issue an error and the command function 
//                will not be called.
//                If this value is -1, CLI will call the command function 
//                regardless of the number of parameters. This allows for a
//                command with optional parameters and requires the command
//                function to validate the parameters.
//    help text   Text to be printed for this command if the user issues '?' or
//                when an incorrect number of parameters is entered. 
//                The text is prepended with the keyword.    
//
// DemoSetup (above) shows how this table is used to register our commands.  
//
// Note: it is convenient to place this table at the end of the file, behind
//       the functions used, so you don't need function prototypes.            
//-----------------------------------------------------------------------------    

//-----------------------------------------------------------------------------
// In addition to the table, we use a group-record to indicate it is
// a group of command with a common first parameter, 'xyz' in this example. 
//
// So 'xyz demo' will call the function CliCmd_XyzDemo from the table above.
//-----------------------------------------------------------------------------

// Copy from here

//-----------------------------------------------------------------------------
// The table below contains one record per command.
// See DemoCommandLine.cpp for details.
//-----------------------------------------------------------------------------
static const t_cli_record CliRecords[] = {  
   // Function          keyword  #param   help_text
   { CliCmd_XyzDemo,    "demo",    -1,    "        - Demo command, parameters optional." },

   { CliCmd_XyzPwm,     "pwm",      2,    "<l> <r> - umpwm(l, r) (via UniversalMover)" },
   { CliCmd_XyzCurve,   "curve",    0,    "        - start MSM_PwmCurve" },
   { CliCmd_XyzCal,     "cal",      0,    "        - start MSM_MotorCal" },   
   { CliCmd_XyzParams,  "params",  -1,    "        - show use of float/string params" },   

   // Don't change or remove the last line
   { 0, "", 0, ""   } // last line has function pointer value 0 
};

const TCliGroup CliDemo = { "xyz", CliRecords};

// Copy to here & update

 
