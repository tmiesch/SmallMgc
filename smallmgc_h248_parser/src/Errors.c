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

   created: 8/25/2007 10:10
  filename: D:\Projects\Apgv5_0\ApgLib\Errors.c
   purpose: High-level error and warning reporting component.
            Uses the memory and vector components to build a list
            of error structures. Its purpose is to assist applications
            in building a list of errors without halting the application.
            This component cannot be safely used to report
            memory allocation errors - use the low-level "err" component
            instead.

*********************************************************************/
#include "Apg.h"

typedef struct
{
  void* vpValidate; // used to validate the context
  void* vpErrCtx;   // context of the erratum struct
  void* vpMsgCtx;   // context of the erratum text messages
  ulong ulIterIndex;// index of errata message of next iteration
} ERROR_CTX;

typedef struct
{
  ulong ulLineIndex;
  ulong ulMsgIndex;
} APG_ERROR_ITEM;

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Error component constructor.
//
// FUNCTION: void* vpErrorsCtor
//
//      ARG: 1) void* vpMemCtx
//              : pointer to a valid memory component context
//
//   RETURN: pointer to an error component context
//           NULL on failure
//
//////////////////////////////////////////////////////////////////////////
void* vpErrorsCtor(void* vpMemCtx)
{
  void* vpReturn = NULL;
  ERROR_CTX* spCtx;
  DASSERT(vpMemCtx);

  // allocate & clear the data
  spCtx = (ERROR_CTX*)vpMemAlloc(vpMemCtx, sizeof(ERROR_CTX));
  UASSERT(spCtx, vpMemCtx, APG_GEN_ID);
  memset((void*)spCtx, 0, sizeof(ERROR_CTX));

  // allocate a vector of erratum instances
  spCtx->vpErrCtx = vpVecCtor(vpMemCtx, sizeof(APG_ERROR_ITEM), 0);
  UASSERT(spCtx->vpErrCtx, vpMemCtx, APG_GEN_ID);

  // allocate a vector of erratum messages
  spCtx->vpMsgCtx = vpVecCtor(vpMemCtx, sizeof(char), 0);
  UASSERT(spCtx->vpMsgCtx, vpMemCtx, APG_GEN_ID);

  // success
  spCtx->vpValidate = (void*)spCtx;
  vpReturn = (void*)spCtx;

APG_EXIT:
  if(!vpReturn)
  {
    if(spCtx)
    {
      // release all memory allocated here
      spCtx->vpValidate = NULL;
      vMemDtor(vpMemCtx);
    }
  }
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Error component destructor.
//
// FUNCTION: void vErrorsDtor
//
//      ARG: 1) void* vpErrorsCtx
//              : pointer to a valid error component context
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vErrorsDtor(void* vpErrorsCtx)
{
  if(vpErrorsCtx)
  {
    ERROR_CTX* spCtx = (ERROR_CTX*)vpErrorsCtx;
    DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

    // release all memory allocations
    if(spCtx->vpErrCtx)vVecDtor(spCtx->vpErrCtx);
    if(spCtx->vpMsgCtx)vVecDtor(spCtx->vpMsgCtx);
    spCtx->vpValidate = NULL;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Reports an error. Adds an error struct to the list of errors.
//
// FUNCTION: ulong ulErrorsReport
//
//      ARG: 1) void* vpErrorsCtx
//              : pointer to a valid error component context
//
//      ARG: 2) char* cpMsg
//              :
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrorsReport(void* vpErrorsCtx, char* cpMsg)
{
  ulong ulReturn = APG_FALSE;
  ulong ulMsgIndex;
  ulong ulLen;
  void* vpTest;
  char  cNull = 0;
  APG_ERROR_ITEM* spErr;
  ERROR_CTX* spCtx;
  DASSERT(vpErrorsCtx);
  spCtx = (ERROR_CTX*)vpErrorsCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  ulLen = (ulong)strlen(cpMsg);
  if(ulLen >= APG_ERROR_BUFLEN)
  {
    // truncate the message
    ulLen = APG_ERROR_BUFLEN - 1;
  }

  // push the message on the msg vector
  ulMsgIndex = ulVecSize(spCtx->vpMsgCtx);
  vpTest = vpVecPushn(spCtx->vpMsgCtx, (void*)cpMsg, ulLen);
  if(!vpTest){goto APG_EXIT;}

  // explicitly push the null terminator on the string
  // (if this is a truncated string, we can't assume that
  //  cpMsg has a null terminator here)
  vpTest = vpVecPush(spCtx->vpMsgCtx, (void*)&cNull);
  if(!vpTest){goto APG_EXIT;}

  // push the errata on the err vector
  spErr = (APG_ERROR_ITEM*)vpVecPush(spCtx->vpErrCtx, NULL);
  if(!spErr){goto APG_EXIT;}
  spErr->ulMsgIndex = ulMsgIndex;

  // success
  ulReturn = APG_TRUE;

APG_EXIT:
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Get the number of errors on the list.
//
// FUNCTION: ulong ulErrorsCount
//
//      ARG: 1) void* vpErrorsCtx
//              : pointer to a valid error component context
//
//   RETURN: the number of errors on the list
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrorsCount(void* vpErrorsCtx)
{
  ERROR_CTX* spCtx;
  DASSERT(vpErrorsCtx);
  spCtx = (ERROR_CTX*)vpErrorsCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  return ulVecSize(spCtx->vpErrCtx);
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Empty the list.
//
// FUNCTION: void vErrorsClear
//
//      ARG: 1) void* vpErrorsCtx
//              : pointer to a valid error component context
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vErrorsClear(void* vpErrorsCtx)
{
  ERROR_CTX* spCtx;
  DASSERT(vpErrorsCtx);
  spCtx = (ERROR_CTX*)vpErrorsCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  spCtx->ulIterIndex = 0;
  vVecClear(spCtx->vpErrCtx);
  vVecClear(spCtx->vpMsgCtx);
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Initialize an iteration over the list of errors.
//
// FUNCTION: void vErrorsInit
//
//      ARG: 1) void* vpErrorsCtx
//              : pointer to a valid error component context
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vErrorsInit(void* vpErrorsCtx)
{
  ERROR_CTX* spCtx;
  DASSERT(vpErrorsCtx);
  spCtx = (ERROR_CTX*)vpErrorsCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  spCtx->ulIterIndex = 0;
}

// vpErrorsCtx - context of the erratum component
// cpErrMsg - pointer to a buffer to receive the null-term text errata message
// returns true on success
// NOTE: cpErrMsg buffer must be at least APG_ERROR_BUFLEN in length
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Increment to the next error in the list.
//
// FUNCTION: ulong ulErrorsNext
//
//      ARG: 1) void* vpErrorsCtx
//              : pointer to a valid error component context
//
//      ARG: 2) char* cpErrMsg
//              : pointer to a buffer to recieve the next error message
//                must be at least APG_ERROR_BUFLEN in length
//
//   RETURN: true on success, false on end of list
//
//////////////////////////////////////////////////////////////////////////
ulong ulErrorsNext(void* vpErrorsCtx, char* cpErrMsg)
{
  ulong ulReturn = APG_FALSE;
  APG_ERROR_ITEM* spErrata;
  char* cpErr;
  ERROR_CTX* spCtx;
  DASSERT(vpErrorsCtx);
  spCtx = (ERROR_CTX*)vpErrorsCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  // initialize
  *cpErrMsg = 0;

  // get the next errata
  spErrata = (APG_ERROR_ITEM*)vpVecAt(spCtx->vpErrCtx, spCtx->ulIterIndex);
  if(!spErrata){goto APG_EXIT;}

  // get the errata message
  cpErr = (char*)vpVecAt(spCtx->vpMsgCtx, spErrata->ulMsgIndex);
  if(!cpErr){goto APG_EXIT;}

  // success
  strcpy(cpErrMsg, cpErr);
  ++spCtx->ulIterIndex;
  ulReturn = APG_TRUE;

APG_EXIT:
  return ulReturn;
}
