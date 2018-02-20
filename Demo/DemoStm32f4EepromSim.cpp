//-----------------------------------------------------------------------------
// DemoStm32f4EepromSim.cpp
//
// Copyright (c) 2013-2018 Joep Suijs - All rights reserved.        
//
// This demo shows how to simulate an eeprom in flash.
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

#warning DEMO HAS NOT BEEN NOT TESTED! Please report succes or issues.

#define DEMO_NAME DemoEepromSim
static int _main(void); // required to use STMicro demo (see below)
               
//-------------
// OVERVIEW
//-------------
/*               
   This is a demo-stub of robotlib, to show the use of STMicro sample code.

   The sample code shows the use of flash memory to simulate an eeprom.
   
*/

//-------------
// DECLARATIONS 
//-------------
// For access from other source files, add prototypes to project.h like

 
//-------------
// INSTANCES 
//-------------    

//-----------------------------------------------------------------------------            
// DefaultDemoSetup - 
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void DefaultDemoSetup()
{       
   printf("DemoSetup for  Eeprom simulation in flash.\n");

 
}

//----------------------------------------------------------------------------- 
// CliCmd_DefaultDemo -           
//-----------------------------------------------------------------------------            
//-----------------------------------------------------------------------------            
void CliCmd_DefaultDemo(int NrParams, TCiParams *P)
{  
   printf("Demo command for Eeprom simulation in flash.\n");
   _main();
}   

//-------------
// OTHER CODE 
//-------------

/*
   *** Copyright of Robotlib ends here. ***
   
   Below is copy of a STMicro example. Minor modifications are made to
   re-use it in the context of a robotlib sample. 

*/   



/**
  ******************************************************************************
  * @file    EEPROM_Emulation/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "eeprom.h"

/** @addtogroup EEPROM_Emulation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
static int _main(void) // Robotlib
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  
   /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();

  /* EEPROM Init */
  EE_Init();

/* --- Store successively many values of the three variables in the EEPROM ---*/
  /* Store 0x1000 values of Variable1 in EEPROM */
  for (VarValue = 1; VarValue <= 0x1000; VarValue++)
  {
    EE_WriteVariable(VirtAddVarTab[0], VarValue);
  }

  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);


  /* Store 0x2000 values of Variable2 in EEPROM */
  for (VarValue = 1; VarValue <= 0x2000; VarValue++)
  {
    EE_WriteVariable(VirtAddVarTab[1], VarValue);
  }

  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);


  /* Store 0x3000 values of Variable3 in EEPROM */
  for (VarValue = 1; VarValue <= 0x3000; VarValue++)
  {
    EE_WriteVariable(VirtAddVarTab[2], VarValue);
  }

  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
  EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);

  while (1);      
  return 0; // Robotlib, suppress warning
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
