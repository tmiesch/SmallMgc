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

   created: 8/25/2007 10:25
  filename: D:\Projects\Apgv5_0\ApgLib\Vector.c
   purpose: Source code file for the vector component.

*********************************************************************/
#include "Apg.h"
#include "Private.h"
#undef VASSERT
#define VASSERT(cond) if(!(int)(cond)) {ulMemErrReport(spCtx->vpMemCtx, APG_VEC_ID, __LINE__, __FILE__); goto APG_EXIT;}

//////////////////////////////////////////////////////////////////////////
// VECTOR COMPONENT FUNCTIONS
//////////////////////////////////////////////////////////////////////////
// private vector instance context
typedef struct
{
  void* vpMemCtx;         // context to the underlying memory component
  char* cpData;           // pointer to the vector's data buffer
  ulong ulElementSize;    // number of bytes in one element
  ulong ulReserved;       // number of elements that have been reserved on the buffer
  ulong ulUsed;           // number of the reserved elements that have been used
  ulong ulReallocations;  // statistics - number buffer reallocations done
  void* vpValidate;       // must be equal to the struct address
} APG_VECTOR_CTX;

typedef struct
{
  ulong ulElementSize;
  ulong ulElements;
  ulong ulValidator;
} APG_VECTOR_EXPORT_HDR;

static ulong ulGrow(APG_VECTOR_CTX*   spCtx, ulong ulElements);

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: vector component constructor
//
// FUNCTION: void* vpVecCtor
//
//      ARG: 1) void* vpMemCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//      ARG: 2) ulong ulElementSize
//              : byte size of a single vector element
//                if 0, APG_VEC_ELEMENT is used
//
//      ARG: 3) ulong ulInitialAlloc
//              : number of elements to initially allocate
//                if 0, APG_VEC_ALLOC is used
//
//   RETURN: pointer to vector context
//           NULL on memory allocation failure
//
//////////////////////////////////////////////////////////////////////////
void* vpVecCtor(void* vpMemCtx, ulong ulElementSize, ulong ulInitialAlloc)
{
  void*           vpReturn = NULL;
  DASSERT(vpMemCtx && ulMemValidate(vpMemCtx));
  if(vpMemCtx && ulMemValidate(vpMemCtx))
  {
    APG_VECTOR_CTX* spCtx;
    if(ulElementSize == 0){ulElementSize = APG_VEC_ELEMENT;}
    if(ulInitialAlloc == 0){ulInitialAlloc = APG_VEC_ALLOC;}

    // allocate the vector context
    spCtx = (APG_VECTOR_CTX*)vpMemAlloc(vpMemCtx, sizeof(APG_VECTOR_CTX));
    if(!spCtx){ulMemErrReport(vpMemCtx, APG_VEC_ID, __LINE__, __FILE__); goto APG_EXIT;}

    // allocate the initial vector buffer
    memset((void*)spCtx, 0, sizeof(*spCtx));
    spCtx->vpMemCtx = vpMemCtx;
    spCtx->cpData = (char*)vpMemAlloc(vpMemCtx, ulElementSize * ulInitialAlloc);
    if(!spCtx->cpData)
    {
      // free the context and return failure
      vMemFree(vpMemCtx, (void*)spCtx);
      VASSERT(APG_FALSE);
    }

    // initialize the context
    spCtx->vpMemCtx = vpMemCtx;
    spCtx->ulElementSize = ulElementSize;
    spCtx->ulReserved = ulInitialAlloc;

    // success
    spCtx->vpValidate = (void*)spCtx;
    vpReturn = (void*)spCtx;
  }
APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: vector component destructor
//              frees all memory allocated
//
// FUNCTION: void vVecDtor
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//                if NULL, no action, no error
//
//   RETURN: void, frees all memory allocated
//
//////////////////////////////////////////////////////////////////////////
void  vVecDtor(void* vpCtx)
{
  // no error and no action if vpCtx = NULL
  if(vpCtx)
  {
    APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
    VASSERT(spCtx->vpValidate == (void*)spCtx);
    // delete the data buffer & context
    spCtx->vpValidate = NULL;
    vMemFree(spCtx->vpMemCtx, (void*)spCtx->cpData);
    vMemFree(spCtx->vpMemCtx, (void*)spCtx);
  }
APG_EXIT:
  return;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: return the number of bytes currently in use on the vector
//              may be smaller than the number of bytes allocated
//
// FUNCTION: ulong ulVecBytes
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: return the number of bytes currently on the vector
//
//////////////////////////////////////////////////////////////////////////
ulong ulVecBytes(void* vpCtx)
{
  ulong ulRet = 0;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  ulRet = spCtx->ulUsed * spCtx->ulElementSize;
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: return the number of elements currently on the vector
//
// FUNCTION: ulong ulVecSize
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: returns the number of elements currently on the vector
//
//////////////////////////////////////////////////////////////////////////
ulong ulVecSize(void* vpCtx)
{
  ulong ulRet = 0;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  ulRet = spCtx->ulUsed;
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: return the number of bytes in one element
//
// FUNCTION: ulong ulVecElementSize
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: returns the number of bytes in one element
//
//////////////////////////////////////////////////////////////////////////
ulong ulVecElementSize(void* vpCtx)
{
  ulong ulRet = 0;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  ulRet = spCtx->ulElementSize;
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: returns the number of times the buffer size has been reallocated
//
// FUNCTION: ulong ulVecReallocations
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: returns the number of times the buffer size has been reallocated
//
//////////////////////////////////////////////////////////////////////////
ulong ulVecReallocations(void* vpCtx)
{
  ulong ulRet = 0;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  ulRet = spCtx->ulReallocations;
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: release all elements from the buffer
//              just resets a pointer
//              no data is actually deleted
//              no memory is released or reallocated
//
// FUNCTION: void vVecClear
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: void
//           no error and no action if vpCtx = NULL
//
//////////////////////////////////////////////////////////////////////////
void  vVecClear(void* vpCtx)
{
  if(vpCtx)
  {
    APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
    VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
    spCtx->ulUsed = 0;
  }
APG_EXIT:
  return;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: push one element on end of vector
//              automatically increases the memory allocation if necessary
//
// FUNCTION: void* vpVecPush
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) void* vpElement
//              : pointer to the element to push
//                if NULL, space for new element allocated only
//
//   RETURN: returns pointer to new element, NULL on memory error
//
//////////////////////////////////////////////////////////////////////////
void* vpVecPush(void* vpCtx, void* vpElement)
{
  void*       vpReturn = NULL;
  ulong       ulTest = APG_TRUE;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  // check allocation
  if(spCtx->ulUsed >= spCtx->ulReserved)
  {
    ulTest = ulGrow(spCtx, 1);
    VASSERT(ulTest);
  }

  vpReturn = (void*)(spCtx->cpData + (spCtx->ulUsed * spCtx->ulElementSize));
  if(vpElement)
  {
    // copy new element to vector
    memcpy(vpReturn, vpElement, spCtx->ulElementSize);
  }

  // success, return a pointer to the new element
  ++spCtx->ulUsed;

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: push one or more elements on the vector
//              automatically increases the memory allocation if necessary
//
// FUNCTION: void* vpVecPushn
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) void* vpElement
//              : pointer to the first element to push
//                if NULL, space for ulCount new elements allocated only
//
//      ARG: 3) ulong ulCount
//              : number of elements to push, may be zero
//
//   RETURN: returns pointer to first new element
//           NULL if zero elements pushed or on memory error
//
//////////////////////////////////////////////////////////////////////////
void* vpVecPushn(void* vpCtx, void* vpElement, ulong ulCount)
{
  void*       vpReturn = NULL;
  ulong       ulTest = APG_TRUE;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  if(ulCount)
  {
    // check allocation
    if((spCtx->ulUsed + ulCount) > spCtx->ulReserved)
    {
      ulTest = ulGrow(spCtx, ulCount);
      VASSERT(ulTest);
    }

    vpReturn = (void*)(spCtx->cpData + (spCtx->ulUsed * spCtx->ulElementSize));
    if(vpElement)
    {
      // copy new elements to vector
      memcpy(vpReturn, vpElement, ulCount * spCtx->ulElementSize);
    }
    spCtx->ulUsed += ulCount;
  }

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: pop one element from the vector
//              resets a pointer only
//              no data is deleted, no memory allocation is done
//
// FUNCTION: void* vpVecPop
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: returns pointer to the popped element, NULL if vector is empty
//
//////////////////////////////////////////////////////////////////////////
void* vpVecPop(void* vpCtx)
{
  void*       vpReturn = NULL;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  if(spCtx->ulUsed)
  {
    // pop one element
    --spCtx->ulUsed;
    vpReturn = (void*)(spCtx->cpData + (spCtx->ulUsed * spCtx->ulElementSize));
  }

  // return a pointer to the popped element
APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: pops one or more elements from the vector
//              resets a pointer only
//              no data is deleted, no memory allocation is done
//
// FUNCTION: void* vpVecPopn
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) ulong ulCount
//              : number of elements to pop
//                may be greater than number of elements on the vector
//                may NOT be zero, otherwise there would be an ambiguity in the return value
//
//   RETURN: returns pointer to the first popped element (which remains until next vpVecPush())
//            NULL if vector is empty
//            NULL if ulCount is zero
//
//////////////////////////////////////////////////////////////////////////
void* vpVecPopn(void* vpCtx, ulong ulCount)
{
  void*       vpReturn = NULL;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(ulCount);

  if(ulCount == 0){goto APG_EXIT;}
  if(spCtx->ulUsed == 0){goto APG_EXIT;}
  if(spCtx->ulUsed > ulCount)
  {
    // pop ulCount elements
    spCtx->ulUsed -= ulCount;
    vpReturn = (void*)(spCtx->cpData + (spCtx->ulUsed * spCtx->ulElementSize));
  }
  else
  {
    // pop all remaining elements
    spCtx->ulUsed = 0;
    vpReturn = spCtx->cpData;
  }

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: pops all elements to and including a specified index
//
// FUNCTION: void* vpVecPopTo
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) ulong ulIndex
//              : index of the first element to pop
//                may NOT be greater than the index of the last element,
//                otherwise there would be an ambiguity in the return value
//
//   RETURN:
//          returns pointer to the first popped element
//          NULL if vector is empty
//          NULL if index is higher than that of last element
//
//////////////////////////////////////////////////////////////////////////
void* vpVecPopTo(void* vpCtx, ulong ulIndex)
{
  void*       vpReturn = NULL;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  if(spCtx->ulUsed == 0 || ulIndex >= spCtx->ulUsed){goto APG_EXIT;}

  // success
  spCtx->ulUsed = ulIndex;
  vpReturn = (void*)(spCtx->cpData + ulIndex);

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: returns a pointer to the first element
//
// FUNCTION: void* vpVecFront
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: returns a pointer to the first element
//            NULL if vector is empty
//
//////////////////////////////////////////////////////////////////////////
void* vpVecFront(void* vpCtx)
{
  void*       vpReturn = NULL;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  if(spCtx->ulUsed)
  {
    vpReturn = (void*)spCtx->cpData;
  }

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: returns a pointer to the last element
//
// FUNCTION: void* vpVecBack
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//   RETURN: returns pointer to the last element
//            NULL if vector is empty
//
//////////////////////////////////////////////////////////////////////////
void* vpVecBack(void* vpCtx)
{
  void*       vpReturn = NULL;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  if(spCtx->ulUsed)
  {
    vpReturn = (void*)(spCtx->cpData + (spCtx->ulUsed - 1) * spCtx->ulElementSize);
  }

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: returns a pointer to a specified element
//
// FUNCTION: void* vpVecAt
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) ulong ulIndex
//              : index(zero-based) of the element to find
//
//   RETURN: returns pointer to the element at position ulIndex
//         NULL if vector is empty
//         NULL if ulIndex is out of range
//
//////////////////////////////////////////////////////////////////////////
void* vpVecAt(void* vpCtx, ulong ulIndex)
{
  void*       vpReturn = NULL;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  if(ulIndex < spCtx->ulUsed)
  {
    vpReturn = (void*)(spCtx->cpData + (ulIndex * spCtx->ulElementSize));
  }

APG_EXIT:
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: export a vector to a user buffer
//
// FUNCTION: ulong ulVecExport
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) void* vpBuffer
//              : pointer to the buffer to receive the exported vector
//                if NULL, the size is returned with no export
//
//   RETURN:
//    returns the size of the exported vector
//    - if vpBuffer is NULL, the size is returned with no export
//
//////////////////////////////////////////////////////////////////////////
ulong ulVecExport(void* vpCtx, void* vpBuffer)
{
  ulong ulDataSize;
  ulong ulReturnSize;
  APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
  VASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  ulDataSize = spCtx->ulElementSize * spCtx->ulUsed;
  ulReturnSize = ulDataSize + sizeof(APG_VECTOR_EXPORT_HDR);
  if(vpBuffer)
  {
    APG_VECTOR_EXPORT_HDR* spHdr = (APG_VECTOR_EXPORT_HDR*)vpBuffer;
    spHdr->ulElements = spCtx->ulUsed;
    spHdr->ulElementSize = spCtx->ulElementSize;
    memcpy((void*)(spHdr + 1), (void*)spCtx->cpData, ulDataSize);
    spHdr->ulValidator = 0;
    spHdr->ulValidator = ulCRC32((uchar*)spHdr, ulReturnSize);
  }

APG_EXIT:
  return ulReturnSize;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: create a new vector and restore a previously exported vector
//
// FUNCTION: void* vpVecImport
//
//      ARG: 1) void* vpMemCtx
//              : pointer to a valid memory context
//
//      ARG: 2) void* vpImport
//              : pointer to a previously exported vector
//
//   RETURN:
//    returns a vector context
//    NULL if import fails
//
//////////////////////////////////////////////////////////////////////////
void* vpVecImport(void* vpMemCtx, void* vpImport)
{
  void* vpCtx = NULL;
  void* vpReturn = NULL;
  void* vpTest;
  ulong ulSave, ulTest, ulSize;
  APG_MEM_CTX*  spMemCtx;
  DASSERT(vpMemCtx);
  spMemCtx = (APG_MEM_CTX*)vpMemCtx;
  DASSERT(spMemCtx && spMemCtx->vpValidate == (void*)spMemCtx);
  DASSERT(vpImport);

  while(APG_TRUE)
  {
    APG_VECTOR_EXPORT_HDR* spHdr = (APG_VECTOR_EXPORT_HDR*)vpImport;

    // validate the import
    if(!spHdr){break;}
    ulSave = spHdr->ulValidator;
    spHdr->ulValidator = 0;
    ulSize = sizeof(*spHdr) + spHdr->ulElements * spHdr->ulElementSize;
    ulTest = ulCRC32((uchar*)spHdr, ulSize);
    spHdr->ulValidator = ulSave;
    if(ulSave != ulTest){break;}

    // create a new vector component
    vpCtx = (APG_VECTOR_CTX*)vpVecCtor(vpMemCtx, spHdr->ulElementSize, spHdr->ulElements);
    if(!vpCtx){break;}

    // install the import vector
    vpTest = vpVecPushn(vpCtx, (void*)(spHdr + 1), spHdr->ulElements);
    if(!vpTest)
    {
      // clean up after failure
      vVecDtor(vpCtx);
      break;
    }

    // success
    vpReturn = vpCtx;
    break;
  }

  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: reallocate the buffer
//              - increase the buffer size to double the previous space plus
//                  specified number of elements
//              - copy existing data to the new buffer
//
// FUNCTION: static ulong ulGrow
//
//      ARG: 1) APG_VECTOR_CTX* spCtx
//              : pointer to a valid vector context previously returned
//                from vpVecCtor()
//
//      ARG: 2) ulong ulElements
//              : minimum number of elements to make additional room for
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
static ulong ulGrow(APG_VECTOR_CTX* spCtx, ulong ulElements)
{
  ulong ulReturn = APG_FALSE;
  void* vpNewData;
  ulong ulReserved;

  // allocate new space
  ulReserved = 2 * (spCtx->ulReserved + ulElements);
  vpNewData = vpMemRealloc(spCtx->vpMemCtx, (void*)spCtx->cpData, spCtx->ulElementSize * ulReserved);
  VASSERT(vpNewData);

  // success
  spCtx->cpData = (char*)vpNewData;
  spCtx->ulReserved = ulReserved;
  ++spCtx->ulReallocations;
  ulReturn = APG_TRUE;

APG_EXIT:
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// PRESERVED - MAY NEED THIS AT A LATER DATE
//////////////////////////////////////////////////////////////////////////
// pops elements in the range [ulFromIndex, ulToIndex)
// that is, elements ulFromIndex, ulFromIndex + 1, ... ulToIndex -1 are deleted
// vpCtx       - valid vector component handle
// ulFromIndex - index of the first element to pop
// ulToIndex   - index of the first element not to pop
// returns number of elements removed (may be less than requested)
//         NULL if vector is empty
//         NULL if ulFromIndex is higher than that of last element
// ulong ulVecPopFromTo(void* vpCtx, ulong ulFromIndex, ulong ulToIndex)
// {
//   ulong       ulReturn = 0;
//   void*       vpDst;
//   void*       vpSrc;
//   ulong       ulBytesToMove;
//   ulong       ulElementsToRemove;
//   ulong       ulElementsToMove;
//   APG_VECTOR_CTX* spCtx = (APG_VECTOR_CTX*)vpCtx;
//   DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
//
//   if(spCtx->ulUsed == 0 || ulFromIndex >= spCtx->ulUsed){goto APG_EXIT;}
//   if(ulToIndex >= spCtx->ulUsed)
//   {
//     // just remove all elements at and above ulFromIndex
//     ulElementsToRemove = spCtx->ulUsed - ulFromIndex;
//     vpVecPopn(vpCtx, ulElementsToRemove);
//   }
//   else
//   {
//     // copy from ulToIndex down to ulFromIndex
//     ulElementsToRemove = ulToIndex - ulFromIndex;
//     ulElementsToMove = spCtx->ulUsed - ulToIndex;
//     ulBytesToMove = ulElementsToMove * spCtx->ulElementSize;
//     vpDst = (void*)(spCtx->cpData + (ulFromIndex * spCtx->ulElementSize));
//     vpSrc = (void*)(spCtx->cpData + (ulToIndex * spCtx->ulElementSize));
//     memmove(vpDst, vpSrc, ulBytesToMove);
//   }
//
//   spCtx->ulUsed -= ulElementsToRemove;
//   ulReturn = ulElementsToRemove;
//
// APG_EXIT:
//   return ulReturn;
// }

