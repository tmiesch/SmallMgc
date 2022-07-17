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

   created: 8/25/2007 10:11
  filename: D:\Projects\Apgv5_0\ApgLib\Memory.c
   purpose: Source code file for the memory component functions.
    MemErr: Defines the error history component. This is a
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

#define MEM_ID_ALLOC    0
#define MEM_ID_REALLOC  1
#define MEM_ID_FREE     2
static void vMemStats(APG_MEM_STATS* spStats, ulong ulID, APG_MEM_CELL* spIn, APG_MEM_CELL* spOut);
static void vActivePush(APG_MEM_CTX* spCtx, APG_MEM_CELL* spCellIn);
static void vActivePop(APG_MEM_CTX* spCtx, APG_MEM_CELL* spCellIn);

//////////////////////////////////////////////////////////////////////////
// MEMORY MANAGEMENT COMPONENT FUNCTIONS
//   - Lowest-level APG component
//   - Does NO error reporting other than returning a NULL pointer
//     as an allocation failure indicator.
//   - keeps a linked list of memory allocations and does garbage
//     collection on destruction
//   - Does debug-only reporting of invalid input argument errors.
//   - always collects memory statistics in both debug and release modes
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: memory component constructor
//
// FUNCTION: void* vpMemCtorA
//
//      ARG: 1) PFN_ALLOCATOR pfnAllocator
//              : pointer to the memory allocator
//
//      ARG: 2) PFN_DEALLOCATOR pfnDeAllocator
//              : pointer to the memory deallocator
//
//   RETURN: returns pointer to a context
//
//////////////////////////////////////////////////////////////////////////
void*   vpMemCtorA(PFN_ALLOCATOR pfnAllocator, PFN_DEALLOCATOR pfnDeAllocator)
{
  void*           vpReturn = NULL;
  ulong           ulSize;
  APG_MEM_CTX*    spCtx;
  APG_ERR_CTX*    spErrCtx;
  DASSERT(pfnAllocator && pfnDeAllocator);
  if(pfnAllocator && pfnDeAllocator)
  {
    // get a context using custom allocator
    ulSize = sizeof(APG_MEM_CTX) + sizeof(APG_ERR_CTX) + (APG_ERR_DEFAULT * sizeof(APG_MEM_ERR));
    spCtx = (APG_MEM_CTX*)pfnAllocator(ulSize);
    if(spCtx)
    {
      // initialize the memory context
      memset((void*)spCtx, 0, ulSize);
      spCtx->pfnAllocator = pfnAllocator;
      spCtx->pfnDeAllocator = pfnDeAllocator;

      // initialize the err context
      spErrCtx = (APG_ERR_CTX*)(spCtx + 1);
      spErrCtx->spErrors = (APG_MEM_ERR*)(spErrCtx + 1);
      spErrCtx->ulAvailable = ulSize;
      spErrCtx->ulUsed = 0;
      spErrCtx->ulTotal = 0;

      // set valid
      spCtx->spErrCtx = spErrCtx;
      spCtx->vpValidate = (void*)spCtx;
      vpReturn = (void*)spCtx;
    }
  }
  return vpReturn;
}

// memory component constructor using malloc() and free() allocation
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: memory component constructor
// uses malloc() and free() memory (de-)allocation
//
// FUNCTION: void* vpMemCtor
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void* vpMemCtor()
{
  return vpMemCtorA(malloc, free);
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: memory component destructor
//
// FUNCTION: void vMemDtor
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//                NULL pointers are ignored, no action and no error
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void   vMemDtor(void* vpCtx)
{
  if(vpCtx)
  {
    APG_MEM_CTX* spCtx = (APG_MEM_CTX*)vpCtx;
    DASSERT(spCtx->vpValidate == (void*)spCtx);
    if(spCtx->vpValidate == (void*)spCtx)
    {
      // release all active cells
      vMemFreeToCheckPoint(vpCtx, 0);

      // release this context
      spCtx->pfnDeAllocator(vpCtx);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: validates an active memory component
//
// FUNCTION: ulong ulMemValidate
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//                NULL pointers are ignored, no action and no error
//
//   RETURN: APG_TRUE if the component is valid, APG_FALSE otherwise.
//
//////////////////////////////////////////////////////////////////////////
ulong ulMemValidate(void* vpCtx)
{
  ulong ulRet = APG_FALSE;
  APG_MEM_CTX* spCtx = (APG_MEM_CTX*)vpCtx;
  if(vpCtx && spCtx->vpValidate == vpCtx)
  {
    ulRet = APG_TRUE;
  }
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: allocates memory
//
// FUNCTION: void* vpMemAlloc
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) ulong ulBytes
//              : number of bytes of memory to allocate
//
//   RETURN: returns a pointer to the allocated memory
//            returns NULL if memory allocation fails
//            and records the error with the error component
//
//////////////////////////////////////////////////////////////////////////
void* vpMemAlloc(void* vpCtx, ulong ulBytes)
{
  void*         vpReturn = NULL;
  APG_MEM_CTX*  spCtx = (APG_MEM_CTX*)vpCtx;
  APG_MEM_CELL* spCell;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(spCtx && spCtx->vpValidate == (void*)spCtx)
  {
    // allocate the memory
    spCell = (APG_MEM_CELL*)spCtx->pfnAllocator(ulBytes + sizeof(APG_MEM_CELL));
    if(spCell)
    {
      spCell->ulSize = ulBytes;

      // push it on the active list
      vActivePush(spCtx, spCell);
      vMemStats(&spCtx->sStats, MEM_ID_ALLOC, spCell, 0);

      // success
      vpReturn = (void*)(spCell + 1);
    }
    else
    {
      // record the error
      ulMemErrReport(vpCtx, APG_MEM_ID, __LINE__, __FILE__);
      vpReturn = NULL;
    }
  }
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: free previously allocated memory
//
// FUNCTION: void vMemFree
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) void* vpData
//              : pointer to the memory allocation to free
//                may be NULL, no error, no action
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vMemFree(void* vpCtx, void* vpData)
{
  APG_MEM_CTX*  spCtx = (APG_MEM_CTX*)vpCtx;
  APG_MEM_CELL* spCell;
  if(vpData)
  {
    DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
    if(spCtx && spCtx->vpValidate == (void*)spCtx)
    {
      spCell = (APG_MEM_CELL*)vpData;
      --spCell;

      // pop from active list
      vMemStats(&spCtx->sStats, MEM_ID_FREE, spCell, 0);
      vActivePop(spCtx, spCell);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: re-allocates memory (upsized or downsized)
//              - any previous data on the memory is copied to the new memory location
//
// FUNCTION: void* vpMemRealloc
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) void* vpData
//              : pointer to the memory allocation to free
//                may be NULL, no error, no action
//
//      ARG: 3) ulong ulBytes
//              : new number of bytes of memory to allocate
//                if 0, no action, no error
//
//   RETURN: returns a pointer to the re-allocated memory
//            NULL if no memory is allocated
//
//////////////////////////////////////////////////////////////////////////
void* vpMemRealloc(void* vpCtx, void* vpData, ulong ulBytes)
{
  void*         vpReturn = NULL;
  APG_MEM_CTX*  spCtx = (APG_MEM_CTX*)vpCtx;
  APG_MEM_CELL* spOldCell;
  APG_MEM_CELL* spNewCell;
  void*         vpDst;
  void*         vpSrc;
  ulong         ulCopy;

  if(vpData && ulBytes)
  {
    DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
    if(spCtx && spCtx->vpValidate == (void*)spCtx)
    {
      spOldCell = (APG_MEM_CELL*)vpData;
      --spOldCell;

      // get the new allocation
      spNewCell = (APG_MEM_CELL*)spCtx->pfnAllocator(ulBytes + sizeof(APG_MEM_CELL));
      if(spNewCell)
      {
        spNewCell->spNext = spOldCell->spNext;
        spNewCell->spPrev = spOldCell->spPrev;
        spOldCell->spNext->spPrev = (struct APG_MEM_CELL_STRUCT*)spNewCell;
        spOldCell->spPrev->spNext = (struct APG_MEM_CELL_STRUCT*)spNewCell;
        spNewCell->ulSeq = spOldCell->ulSeq;
        spNewCell->ulSize = ulBytes;

        // copy the data from old allocation to new
        ulCopy = min(ulBytes, spOldCell->ulSize);
        vpDst = (void*)(spNewCell + 1);
        vpSrc = (void*)(spOldCell + 1);
        memcpy(vpDst, vpSrc, ulCopy);

        // free the old data
        vMemStats(&spCtx->sStats, MEM_ID_REALLOC, spOldCell, spNewCell);
        //APG_MEMSTATS(&spCtx->sStats, MEM_ID_REALLOC, spOldCell, spNewCell);
        spCtx->pfnDeAllocator((void*)spOldCell);
        vpReturn = (void*)(spNewCell + 1);
      }
      else
      {
        // record the error
        ulMemErrReport(vpCtx, APG_MEM_ID, __LINE__, __FILE__);
        vpReturn = NULL;
      }
    }
  }
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: get a check point to the current state of the memory component
// This can be used to restore the memory allocations to the current state.
//
// FUNCTION: ulong ulMemCheckPoint
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//   RETURN: returns a check point index
//
//////////////////////////////////////////////////////////////////////////
ulong ulMemCheckPoint(void* vpCtx)
{
  APG_MEM_CTX*  spCtx = (APG_MEM_CTX*)vpCtx;
  APG_MEM_CELL* spLast;
  ulong         ulChk = 0;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(spCtx && spCtx->vpValidate == (void*)spCtx)
  {
    if(spCtx->spActiveList)
    {
      spLast = (APG_MEM_CELL*)spCtx->spActiveList->spPrev;
      ulChk = spLast->ulSeq + 1;
    }
  }
  return ulChk;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: restore the memory component to a previous state
//
// FUNCTION: void vMemFreeToCheckPoint
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) ulong ulChk
//              : previous state index
//                must be a value returned from a previous call to ulMemCheckPoint()
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vMemFreeToCheckPoint(void* vpCtx, ulong ulChk)
{
  APG_MEM_CTX*  spCtx = (APG_MEM_CTX*)vpCtx;
  APG_MEM_CELL* spLast;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(spCtx && spCtx->vpValidate == (void*)spCtx)
  {
    // release all active blocks above the check point
    while(APG_TRUE)
    {
      // break if list is empty
      if(spCtx->spActiveList == NULL){break;}

      // remove the last cell from the active list
      spLast = (APG_MEM_CELL*)spCtx->spActiveList->spPrev;

      // break if this cell is below check point
      if(spLast->ulSeq < ulChk){break;}

      // remove the cell from the active list
      vMemStats(&spCtx->sStats, MEM_ID_FREE, spLast, 0);
      vActivePop(spCtx, spLast);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: release all memory currently allocated.
//  restore the memory component to the initial state
//
// FUNCTION: void vMemClear
//
//      ARG: 1) void* vpCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vMemClear(void* vpCtx)
{
  vMemFreeToCheckPoint(vpCtx, 0);
}

//////////////////////////////////////////////////////////////////////////
// STATIC HELPER FUNCTIONS
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Updates memory statistics.
//
// FUNCTION: static void vMemStats
//
//      ARG: 1) APG_MEM_STATS* spStats
//              : pointer to a statistics struct
//
//      ARG: 2) ulong ulID
//              : MEM_ID_ALLOC - update allocation stats on allocation
//              : MEM_ID_REALLOC - update reallocation stats
//              : MEM_ID_FREE - update allocation stats on deallocation
//
//      ARG: 3) APG_MEM_CELL* spIn
//              : pointer to the memory cell being re/de/allocated
//
//      ARG: 4) APG_MEM_CELL* spOut
//              : pointer to the new, reallocated memory cell
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
static void vMemStats(APG_MEM_STATS* spStats, ulong ulID, APG_MEM_CELL* spIn, APG_MEM_CELL* spOut)
{
  switch(ulID)
  {
  case MEM_ID_ALLOC:
    ++spStats->ulAllocations;
    ++spStats->ulCells;
    spStats->ulHeapBytes += spIn->ulSize + sizeof(APG_MEM_CELL);
    if(spStats->ulCells > spStats->ulMaxCells)
    {
      spStats->ulMaxCells = spStats->ulCells;
    }
    if(spStats->ulHeapBytes > spStats->ulMaxHeapBytes)
    {
      spStats->ulMaxHeapBytes = spStats->ulHeapBytes;
    }
    break;
  case MEM_ID_REALLOC:
    ++spStats->ulReAllocations;
    spStats->ulHeapBytes += spOut->ulSize - spIn->ulSize;
    if(spStats->ulHeapBytes > spStats->ulMaxHeapBytes)
    {
      spStats->ulMaxHeapBytes = spStats->ulHeapBytes;
    }
    break;
  case MEM_ID_FREE:
    ++spStats->ulFrees;
    --spStats->ulCells;
    spStats->ulHeapBytes -= spIn->ulSize + sizeof(APG_MEM_CELL);
    break;
  }

}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: push a new memory cell on the linked list
//
// FUNCTION: static void vActivePush
//
//      ARG: 1) APG_MEM_CTX* spCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) APG_MEM_CELL* spCellIn
//              : the new cell to be pushed
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
static void vActivePush(APG_MEM_CTX* spCtx, APG_MEM_CELL* spCellIn)
{
  struct APG_MEM_CELL_STRUCT* spLast;
  struct APG_MEM_CELL_STRUCT* spFirst;
  struct APG_MEM_CELL_STRUCT* spCell = (struct APG_MEM_CELL_STRUCT*)spCellIn;

  // sequence number roll over is a fatal error
  // link the cell
  switch(spCtx->ulActiveCellCount)
  {
  case 0:
    spCtx->spActiveList = spCell;
    spCell->spNext = spCell->spPrev = spCell;
    spCell->ulSeq = 0;
    break;
  case 1:
    spLast = spCtx->spActiveList;
    spLast->spNext = spLast->spPrev = spCell;
    spCell->spNext = spCell->spPrev = spLast;
    spCell->ulSeq = spLast->ulSeq + 1;
    break;
  default:
    spFirst = spCtx->spActiveList;
    spLast = spFirst->spPrev;
    spFirst->spPrev = spCell;
    spLast->spNext = spCell;
    spCell->spNext = spFirst;
    spCell->spPrev = spLast;
    spCell->ulSeq = spLast->ulSeq + 1;
    break;
  }

  ++spCtx->ulActiveCellCount;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: removes (pops) a memory cell from the linked list
//
// FUNCTION: static void vActivePop
//
//      ARG: 1) APG_MEM_CTX* spCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) APG_MEM_CELL* spCellIn
//              : the memory cell to be removed (popped)
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
static void vActivePop(APG_MEM_CTX* spCtx, APG_MEM_CELL* spCellIn)
{
  struct APG_MEM_CELL_STRUCT* spPrev;
  struct APG_MEM_CELL_STRUCT* spNext;
  struct APG_MEM_CELL_STRUCT* spCell = (struct APG_MEM_CELL_STRUCT*)spCellIn;

  if(spCtx->spActiveList == spCell)
  {
    // update the pointer to the first cell
    if(spCtx->ulActiveCellCount == 1)
    {
      spCtx->spActiveList = NULL;
    }
    else
    {
      spCtx->spActiveList = spCtx->spActiveList->spNext;
    }
  }

  // remove the cell from the active list
  spPrev = spCell->spPrev;
  spNext = spCell->spNext;
  spPrev->spNext = spCell->spNext;
  spNext->spPrev = spCell->spPrev;

  --spCtx->ulActiveCellCount;

  // free the data
  spCtx->pfnDeAllocator((void*)spCell);
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: clear any existing error messages from the queue
//
// FUNCTION: void vMemErrClear
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vMemErrClear(void* vpCtx)
{
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  DASSERT(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx);
  if(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx)
  {
    APG_ERR_CTX* spErrCtx = spMemCtx->spErrCtx;
    DASSERT(spErrCtx->vpValidate == (void*)spErrCtx);
    if(spErrCtx->vpValidate == (void*)spErrCtx)
    {
      spErrCtx->ulUsed = 0;
      spErrCtx->ulTotal = 0;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: report an error - add an error to the queue
//
// FUNCTION: ulong ulMemErrReport
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
ulong ulMemErrReport(void* vpCtx, ulong ulID, ulong ulLine, char* cpFile)
{
  ulong ulRet = 0;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  APG_MEM_ERR* spError;
  DASSERT(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx);
  if(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx)
  {
    APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
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
    ulRet = spCtx->ulTotal;
  }
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: find the number of reports in the queue
//
// FUNCTION: ulong ulMemErrCount
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//   RETURN: returns the number of reports in the buffer
//
//////////////////////////////////////////////////////////////////////////
ulong ulMemErrCount(void* vpCtx)
{
  ulong ulRet = 0;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  DASSERT(spMemCtx->vpValidate == (void*)spMemCtx);
  if(spMemCtx->vpValidate == (void*)spMemCtx)
  {
    // return the error count
    APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
    DASSERT(spCtx && (spCtx->vpValidate = (void*)spCtx));
    if(spCtx && (spCtx->vpValidate = (void*)spCtx))
    {
      ulRet = spCtx->ulUsed;
    }
  }

  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: find the number report attempts (may be more than in the queue)
//
// FUNCTION: ulong ulMemErrTotal
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//   RETURN: returns the number report attempts
//    if ulTotal > ulUsed, (ulTotal - ulUsed) is the number of buffer overflows
//
//////////////////////////////////////////////////////////////////////////
ulong ulMemErrTotal(void* vpCtx)
{
  ulong ulRet = 0;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  DASSERT(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx);
  if(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx)
  {
    APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
    DASSERT(spCtx && (spCtx->vpValidate = (void*)spCtx));
    if(spCtx && (spCtx->vpValidate = (void*)spCtx))
    {
      ulRet = spCtx->ulTotal;
    }
  }

  // return the error count
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: retrieve a report
//              retrieved by index rather than iteration
//
// FUNCTION: ulong ulMemErrMsg
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//      ARG: 2) ulong ulIndex
//              : chronological sequence number of the report requested
//
//      ARG: 3) APG_MEM_ERR* spUserError
//              : pointer to the caller's report location
//
//   RETURN: if true, true and copies a report to the caller's buffer
//           if false, no report with this index exists
//
//////////////////////////////////////////////////////////////////////////
ulong ulMemErrMsg(void* vpCtx, ulong ulIndex, ulong* ulpID, ulong* ulpLine, char** cpFile)
{
  ulong ulRet = APG_FALSE;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  DASSERT(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx);
  if(vpCtx && spMemCtx->vpValidate == (void*)spMemCtx)
  {
    APG_MEM_ERR* spError;
    APG_ERR_CTX* spErrCtx = spMemCtx->spErrCtx;
    DASSERT(spErrCtx && spErrCtx->vpValidate == (void*)spErrCtx);
    if(spErrCtx && spErrCtx->vpValidate == (void*)spErrCtx)
    {
      if(ulIndex < spErrCtx->ulUsed)
      {
        // success
        spError = &spErrCtx->spErrors[ulIndex];
        *ulpID = spError->ulID;
        *ulpLine = spError->ulLine;
        *cpFile = spError->cpFile;
        ulRet = APG_TRUE;
      }
      else
      {
        // failure
        *ulpID = 0;
        *ulpLine = 0;
        *cpFile = NULL;
        ulRet = APG_FALSE;
      }
    }
  }

  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Return a copy of the memory component statistics.
//
// FUNCTION: ulong ulMemStats
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//      ARG: 2) APG_MEM_STATS* spStats
//              : pointer to a memory components statistics struct
//
//   RETURN: if spStats = NULL, return size of spStats buffer required
//           if spStats != NULL, copy statistics to spStats
//
//////////////////////////////////////////////////////////////////////////
ulong   ulMemStats(void* vpCtx, APG_MEM_STATS* spStats)
{
  ulong ulRet = 0;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  DASSERT(vpCtx && spStats && ulMemValidate(vpCtx));
  if(vpCtx && ulMemValidate(vpCtx))
  {
    if(spStats)
    {
      memcpy((void*)spStats, (void*)&spMemCtx->sStats, sizeof(APG_MEM_STATS));
    }
    ulRet = sizeof(APG_MEM_STATS);
  }
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Get the current list of memory component errors.
//
// FUNCTION: ulong ulMemErrs
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid memory component handle
//                returned from vpMemCtor()
//
//      ARG: 2) APG_MEM_ERR* spErrs
//              : pinter to a buffer to recieve the errors
//
//   RETURN: if vpErrs = NULL, returns the necessary buffer size
//              or zero (0) if no errors
//           if vpErrors != NULL, returns the number of messages,
//              and a copy of the error messages in the vpErrs buffer.
//           returns APG_FALSE if vpCtx is not a valid memory context
//
//////////////////////////////////////////////////////////////////////////
ulong   ulMemErrs(void* vpCtx, APG_MEM_ERR* spErrs)
{
  ulong ulRet = 0;
  ulong ulSize = 0;
  ulong ulIndex;
  APG_MEM_CTX* spMemCtx = (APG_MEM_CTX*)vpCtx;
  if(vpCtx && ulMemValidate(vpCtx))
  {
    // compute the buffer size
    APG_ERR_CTX* spCtx = spMemCtx->spErrCtx;
    for(ulIndex = 0; ulIndex < spCtx->ulUsed; ++ulIndex)
    {
      // success
      ulSize += sizeof(APG_MEM_ERR);
      ulSize += 1 + (ulong)strlen(spCtx->spErrors[ulIndex].cpFile);
    }

    if(spErrs && ulSize > 0)
    {
      // copy the errors to the buffer
      char* cpNames = (char*)spErrs + (spCtx->ulUsed * sizeof(APG_MEM_ERR));
      for(ulIndex = 0; ulIndex < spCtx->ulUsed; ++ulIndex)
      {
        memcpy((void*)spErrs, (void*)&spCtx->spErrors[ulIndex], sizeof(APG_MEM_ERR));
        strcpy(cpNames, spCtx->spErrors[ulIndex].cpFile);
        spErrs->cpFile = cpNames;
        cpNames += 1 + (ulong)strlen(spCtx->spErrors[ulIndex].cpFile);
        ++spErrs;
      }

      // return the number of msgs
      ulRet = spCtx->ulUsed;
    }
    else{ulRet = ulSize;}
  }
  return ulRet;
}
