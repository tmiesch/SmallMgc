/********************************************************************
  APG - an ABNF Parser Generator
  Copyright  (C) 2007 Coast to Coast Research, Inc.
 
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

   created: 8/25/2007 10:10
  filename: D:\Projects\Apgv5_0\ApgLib\Err.c
   purpose: Defines the error history component. This is a
            low-level error reporting facility that DOES NOT
            use I/O or the memory component. It sits parallel to the
            memory component and can be used to safely report
            errors after an "out of memory" allocation error occurs.

            The component has no constructor or destructor. It is
            constructed only by the memory component and its members
            are accessed through the memory component handle.
            - reports error location and an identifier
            - keeps a chronologically ordered list of reports
            - the list has a configurable, fixed length
            - reports after list is full are ignored
  
*********************************************************************/
#include "Apg.h"
#include "Private.h"

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: clear any existing error messages from the queue
//
// FUNCTION: void vErrClear
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vErrClear(void* vpCtx)
{
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
  DASSERT(spMemCtx->vpValidate == (void*)spMemCtx);
  
  if(spCtx)
  {
    DASSERT(spCtx->vpValidate == (void*)spCtx);
    spCtx->ulUsed = 0;
    spCtx->ulTotal = 0;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: report an error - add an error to the queue
//
// FUNCTION: ulong ulErrReport
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//      ARG: 2) ulong ulID
//              : a report identifier
//
//      ARG: 3) ulong ulLine
//              : line number(__LINE__) from where the report was issued
//
//      ARG: 4) char* cpFile
//              : source code file name(__FILE__) from where the report was issued
//
//   RETURN: returns the total number of reports
//           report buffer overflow is ignored, no report recorded, no error notice
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrReport(void* vpCtx, ulong ulID, ulong ulLine, char* cpFile)
{
  APG_ERR*      spError;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
  DASSERT(spMemCtx->vpValidate == (void*)spMemCtx);

  if(spCtx->ulUsed < spCtx->ulAvailable)
  {
    // report the error
    spError = &spCtx->spErrors[spCtx->ulUsed];
    spError->ulID = ulID;
    spError->ulLine = ulLine;
    spError->cpFile = cpFile;
    ++spCtx->ulUsed;
  }
  // else buffer full - ignore
  
  ++spCtx->ulTotal;
  return spCtx->ulTotal;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: find the number of reports in the queue
//
// FUNCTION: ulong ulErrCount
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//   RETURN: returns the number of reports in the buffer
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrCount(void* vpCtx)
{
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
  DASSERT(spMemCtx->vpValidate == (void*)spMemCtx);

  // return the error count
  DASSERT(spCtx && (spCtx->vpValidate = (void*)spCtx));
  return spCtx->ulUsed;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: find the number report attempts (may be more than in the queue)
//
// FUNCTION: ulong ulErrTotal
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//   RETURN: returns the number report attempts
//    if ulTotal > ulUsed, (ulTotal - ulUsed) is the number of buffer overflows
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrTotal(void* vpCtx)
{
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
  DASSERT(spMemCtx->vpValidate == (void*)spMemCtx);
  
  // return the error count
  return spCtx->ulTotal;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: retrieve a report
//              retrieved by index rather than iteration
//
// FUNCTION: ulong ulErrMsg
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//      ARG: 2) ulong ulIndex
//              : chronological sequence number of the report requested
//
//      ARG: 3) APG_ERR* spUserError
//              : pointer to the caller's report location
//
//   RETURN: if true, true and copies a report to the caller's buffer
//           if false, no report with this index exists
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrMsg(void* vpCtx, ulong ulIndex, APG_ERR* spUserError)
{
  ulong           ulReturn;
  APG_ERR*      spError;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
  DASSERT(spMemCtx->vpValidate == (void*)spMemCtx);
  DASSERT(spUserError);

  if(ulIndex < spCtx->ulUsed)
  {
    // success
    spError = &spCtx->spErrors[ulIndex];
    memcpy((void*)spUserError, (spError), sizeof(APG_ERR));
    ulReturn = APG_TRUE;
  }
  else
  {
    // failure
    memset((void*)spUserError, 0, sizeof(APG_ERR));
    ulReturn = APG_FALSE;
  }
  
  return ulReturn;
}
