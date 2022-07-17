/********************************************************************
  APG - an ABNF Parser Generator
  Copyright  (C) 2009 Coast to Coast Research, Inc.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
  or write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    author: Lowell Thomas
            lowell@coasttocoastresearch.com
            http://www.coasttocoastresearch.com

   created: 8/25/2007 10:06
  filename: D:\Projects\Apgv5_0\ApgLib\Alert.c
   purpose: Intended for reporting the location of serious procedure
            errors.

*********************************************************************/

#include "Apg.h"

//////////////////////////////////////////////////////////////////////////
// ALERT HANDLER
//////////////////////////////////////////////////////////////////////////
static PFN_ALERT pfnAlertHandler = NULL;

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: sets the alert handler
// NOTE: custom alert handler functions must accept null pointers for cpFile
//       iID user-defined IDs should be >= 0
//       iID < 0 reserved for APG system functions
//
// FUNCTION: PFN_ALERT pfnSetAlertHandler
//
//      ARG: 1) PFN_ALERT pfnNewAlertHandler
//              : pointer to the user-defined alert handler
//                (see vAlertHandler() for the argument list)
//
//   RETURN: returns pointer to the old alert handler
//
//////////////////////////////////////////////////////////////////////////
PFN_ALERT pfnSetAlertHandler(PFN_ALERT pfnNewAlertHandler)
{
  PFN_ALERT pfnSave = pfnAlertHandler;
  pfnAlertHandler = pfnNewAlertHandler;
  return pfnSave;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: public alert handler function. Give the application a
// chance to record the location (file and line number)
// of the error along with an application-defined ID and take
// corrective or termination action.
//
// Default action is to exit. Use pfnSetAlertHandler() to set
// a custom alert handling action
//
// FUNCTION: void vAlertHandler
//
//      ARG: 1) int iID
//              : User-defined alert identifier
//                iID user-defined IDs should be >= 0
//                iID < 0 reserved for APG system functions
//
//      ARG: 2) unsigned int uiLine
//              : line number where alert occurred(__LINE__)
//
//      ARG: 3) char* cpFile
//              : file name where alert occurred(__FILE__)
//                custom alert handler functions must accept null pointers for cpFile
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vAlertHandler(int iID, unsigned int uiLine, char* cpFile)
{
  if(pfnAlertHandler)
  {
    // call presently-defined alert handler
    pfnAlertHandler(iID, uiLine, cpFile);
  }
  else
  {
    // default alert handler
    exit(iID);
  }
}

