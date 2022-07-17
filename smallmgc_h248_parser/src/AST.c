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

   created: 8/25/2007 10:14
  filename: D:\Projects\Apgv5_0\ApgLib\Nodes.c
   purpose: Source code file for APG operator statistics and
            node-tracing functions. Reqires CRT I/O functions.
            Compliled only if _APG_CFG_STATS is defined.

*********************************************************************/
#ifndef _AST_c_
#define _AST_c_

#include "Apg.h"
#include "Private.h"
#undef AASSERT
#define AASSERT(cond) if(!(int)(cond)) {ulMemErrReport(spAst->vpMemCtx, APG_AST_ID, __LINE__, __FILE__); goto APG_EXIT;}
#define APG_NODE_ELEMENTS 1000

//////////////////////////////////////////////////////////////////////////
// FUNCTION: vpASTCtor
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : handle of the parser's context
//
//   RETURN: handle to the AST component context
//           NULL on failure
//
// DESCRIPTION: constructs an AST component
//
//////////////////////////////////////////////////////////////////////////
void* vpASTCtor(APG_PARSER_CTX* spCtx)
{
  void* vpRet = NULL;
  APG_AST_CTX* spAst = NULL;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(spCtx && spCtx->vpValidate == (void*)spCtx)
  {
    APG_RULES* spRules = (APG_RULES*)spCtx->vpRules;
    APG_RULES* spRuleEnd = spRules + spCtx->ulRuleCount;
    APG_RULES* spRule;
    char* cpNames;
    ulong ulSize = 0;
    for(spRule = spRules; spRule < spRuleEnd; ++spRule)
    {
      ulSize += (ulong)strlen(spRule->cpRuleName) + 1;
    }

    // allocate the AST context
    ulSize += sizeof(APG_AST_CTX) + (sizeof(char*) * spCtx->ulRuleCount);
    spAst = (APG_AST_CTX*)vpMemAlloc(spCtx->vpMemCtx, ulSize);
    if(!(int)(spAst)) {ulMemErrReport(spCtx->vpMemCtx, APG_AST_ID, __LINE__, __FILE__); goto APG_EXIT;}

    // initialize the AST context
    memset((void*)spAst, 0, ulSize);
    spAst->ulRuleCount = spCtx->ulRuleCount;
    spAst->ulStartRule = spCtx->ulStartRule;
    spAst->cppRuleNames = (char**)((char*)spAst + sizeof(APG_AST_CTX));
    spAst->vpVecNodes = vpVecCtor(spCtx->vpMemCtx, sizeof(APG_AST_NODE), APG_NODE_ELEMENTS);
    if(!(int)(spAst->vpVecNodes)) {ulMemErrReport(spCtx->vpMemCtx, APG_AST_ID, __LINE__, __FILE__); goto APG_EXIT;}
    spAst->vpVecValues = vpVecCtor(spCtx->vpMemCtx, sizeof(APG_AST_VALUE), APG_NODE_ELEMENTS/2);
    if(!(int)(spAst->vpVecNodes)) {ulMemErrReport(spCtx->vpMemCtx, APG_AST_ID, __LINE__, __FILE__); goto APG_EXIT;}
    AASSERT(spAst->vpVecValues);
    spAst->cpNames = (char*)spAst->cppRuleNames + (sizeof(char*) * spCtx->ulRuleCount);
    cpNames = spAst->cpNames;
    for(spRule = spRules; spRule < spRuleEnd; ++spRule)
    {
      strcpy(cpNames, spRule->cpRuleName);
      cpNames += (ulong)strlen(spRule->cpRuleName) + 1;
    }

    // success
    spAst->vpMemCtx = spCtx->vpMemCtx;
    spAst->vpValidate = (void*)spAst;
    vpRet = (void*)spAst;
  }
APG_EXIT:
  if(!vpRet && spAst)
  {
    // clean up resources on failure
    spAst->vpValidate = NULL;
    vMemFree(spAst->vpMemCtx, (void*)spAst);
  }
  return vpRet;
}

void vASTDtor(void* vpCtx)
{
  APG_AST_CTX* spAst = (APG_AST_CTX*)vpCtx;
  if(spAst && spAst->vpValidate == (void*)spAst)
  {
    vVecDtor(spAst->vpVecNodes);
    vVecDtor(spAst->vpVecValues);
    spAst->vpValidate = NULL;
    vMemFree(spAst->vpMemCtx, (void*)spAst);
  }
}

void vASTClear(void* vpCtx, APG_BSTR* spSrc)
{
  APG_AST_CTX* spAst = (APG_AST_CTX*)vpCtx;
  if(spAst && spAst->vpValidate == (void*)spAst)
  {
    // clear all AST nodes
    vVecClear(spAst->vpVecNodes);
    vVecClear(spAst->vpVecValues);
    spAst->sSrc = *spSrc;
    spAst->ulRNMParent = UNDEFINED;
    spAst->ulInPRD = APG_FALSE;
    spAst->ulNodeIndex = 0;
    spAst->ulValueIndex = 0;
  }
  return;
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: spASTIterCtor
//
//      ARG: 1) void* vpCtx
//              : AST component handle
//
//   RETURN: pointer to an AST iterator
//           NULL on failure
//
// DESCRIPTION: constructs an AST iterator
//              Allocates memory
//              must call vASTIterDtor to free the allocated memory
//
//////////////////////////////////////////////////////////////////////////
APG_AST_ITERATOR* spASTIterCtor(void* vpCtx)
{
  APG_AST_ITERATOR* spRet = NULL;
  ulong ulSize = 0;
  APG_AST_ITERATOR* spIter = NULL;
  APG_AST_ITER_HELPER* spHelper = NULL;
  APG_AST_CTX* spAst = (APG_AST_CTX*)vpCtx;
  APG_AST_VALUE* spValue;
  APG_AST_NODE* spNode;

  // validate
  DASSERT(spAst && spAst->vpValidate == (void*)spAst);
  if(spAst && spAst->vpValidate == (void*)spAst && ulVecSize(spAst->vpVecValues))
  {
    // validate nodes and values
    spNode = (APG_AST_NODE*)vpVecFront(spAst->vpVecNodes);
    AASSERT(spNode && spNode->ulDown);
    spValue = (APG_AST_VALUE*)vpVecFront(spAst->vpVecValues);
    AASSERT(spValue);

    // allocate
    ulSize = sizeof(APG_AST_ITERATOR) + sizeof(APG_AST_ITER_HELPER);
    spIter = (APG_AST_ITERATOR*)vpMemAlloc(spAst->vpMemCtx, ulSize);
    AASSERT(spIter);

    // initialize to root node
    memset((void*)spIter, 0, ulSize);
    spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
    spHelper->spAst = spAst;
    spHelper->vpMemCtx = spAst->vpMemCtx;
    spHelper->ulSize = ulSize;
    spHelper->vpValidate = (void*)spIter;

    // get the rule name
    spIter->ulRuleID = spValue->ulRuleIndex;
    spIter->cpRuleName = spAst->cppRuleNames[spValue->ulRuleIndex];
    AASSERT(spIter->cpRuleName);

    // get the phrase
    spIter->sPhrase.ucpStr = spAst->sSrc.ucpStr + spValue->sPhrase.ulOffset;
    spIter->sPhrase.ulLen = spValue->sPhrase.ulLen;

    // set the direction
    spIter->ulDown = APG_TRUE;
    spIter->ulUp = APG_FALSE;

    // success
    spRet = spIter;
  }

APG_EXIT:
  if(!spRet && spIter){vMemFree(spAst->vpMemCtx, (void*)spIter);}
  return spRet;
}

void vASTIterDtor(APG_AST_ITERATOR* spIter)
{
  if(spIter)
  {
    APG_AST_ITER_HELPER* spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
    if((spHelper->vpValidate == (void*)spIter))
    {
      spHelper->vpValidate = NULL;
      vMemFree(spHelper->vpMemCtx, (void*)spIter);
    }
  }
}

ulong ulASTFirst(APG_AST_ITERATOR* spIter)
{
  ulong ulRet = APG_FALSE;
  APG_AST_ITER_HELPER* spHelper = NULL;
  APG_AST_VALUE* spValue;
  APG_AST_NODE* spNode;
  APG_AST_CTX* spAst;

  // validate
  if(!spIter){goto APG_EXIT;}
  spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
  if(!(spHelper->vpValidate == (void*)spIter)){goto APG_EXIT;}
  spAst = spHelper->spAst;

  // validate nodes and values
  spNode = (APG_AST_NODE*)vpVecFront(spAst->vpVecNodes);
  AASSERT(spNode && spNode->ulDown);
  spValue = (APG_AST_VALUE*)vpVecFront(spAst->vpVecValues);
  AASSERT(spValue);

  // reset indexes
  spHelper->ulNodeIndex = 0;

  // get the rule name
  spIter->ulRuleID = spValue->ulRuleIndex;
  spIter->cpRuleName = spAst->cppRuleNames[spValue->ulRuleIndex];
  AASSERT(spIter->cpRuleName);

  // get the phrase
  spIter->sPhrase.ucpStr = spAst->sSrc.ucpStr + spValue->sPhrase.ulOffset;
  spIter->sPhrase.ulLen = spValue->sPhrase.ulLen;

  // set the direction
  spIter->ulDown = APG_TRUE;
  spIter->ulUp = APG_FALSE;

  // success
  ulRet = APG_TRUE;

APG_EXIT:
  return ulRet;
}

ulong ulASTLast(APG_AST_ITERATOR* spIter)
{
  ulong ulRet = APG_FALSE;
  APG_AST_ITER_HELPER* spHelper = NULL;
  APG_AST_VALUE* spValue;
  APG_AST_NODE* spNode;
  APG_AST_CTX* spAst;
  ulong ulNodeIndex;

  // validate
  if(!spIter){goto APG_EXIT;}
  spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
  if(!(spHelper->vpValidate == (void*)spIter)){goto APG_EXIT;}
  spAst = spHelper->spAst;

  // validate nodes
  ulNodeIndex = ulVecSize(spAst->vpVecNodes);
  if(ulNodeIndex == 0){goto APG_EXIT;}
  ulNodeIndex--;
  spNode = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeIndex);
  AASSERT(spNode && !spNode->ulDown);

  // validate values
  spValue = (APG_AST_VALUE*)vpVecAt(spAst->vpVecValues, spNode->ulValueIndex);
  AASSERT(spValue);

  // update the indexes
  spHelper->ulNodeIndex = ulNodeIndex;

  // get the rule name
  spIter->ulRuleID = spValue->ulRuleIndex;
  spIter->cpRuleName = spAst->cppRuleNames[spValue->ulRuleIndex];
  AASSERT(spIter->cpRuleName);

  // set the phrase
  spIter->sPhrase.ucpStr = spAst->sSrc.ucpStr + spValue->sPhrase.ulOffset;
  spIter->sPhrase.ulLen = spValue->sPhrase.ulLen;

  // set the direction
  spIter->ulDown = APG_FALSE;
  spIter->ulUp = APG_TRUE;

  // success
  ulRet = APG_TRUE;

APG_EXIT:
  return ulRet;
}

ulong ulASTNext(APG_AST_ITERATOR* spIter)
{
  ulong ulRet = APG_FALSE;
  APG_AST_ITER_HELPER* spHelper = NULL;
  APG_AST_VALUE* spValue;
  APG_AST_NODE* spNode;
  APG_AST_CTX* spAst;
  ulong ulNodeIndex;
  ulong ulValueIndex;

  // validate
  if(!spIter){goto APG_EXIT;}
  spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
  if(!(spHelper->vpValidate == (void*)spIter)){goto APG_EXIT;}
  spAst = spHelper->spAst;

  // get the next node
  ulNodeIndex = spHelper->ulNodeIndex + 1;
  spNode = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeIndex);
  if(!spNode){goto APG_EXIT;}

  // get the associated value
  ulValueIndex = spNode->ulValueIndex;
  spValue = (APG_AST_VALUE*)vpVecAt(spAst->vpVecValues, ulValueIndex);
  if(!spValue){goto APG_EXIT;}

  // update the indexes
  spHelper->ulNodeIndex = ulNodeIndex;

  // get the rule name
  spIter->ulRuleID = spValue->ulRuleIndex;
  spIter->cpRuleName = spAst->cppRuleNames[spValue->ulRuleIndex];
  DASSERT(spIter->cpRuleName);

  // set the phrase
  spIter->sPhrase.ucpStr = spAst->sSrc.ucpStr + spValue->sPhrase.ulOffset;
  spIter->sPhrase.ulLen = spValue->sPhrase.ulLen;

  // set the direction
  spIter->ulDown = spNode->ulDown ? APG_TRUE: APG_FALSE;
  spIter->ulUp = spNode->ulDown ? APG_FALSE: APG_TRUE;

  // success
  ulRet = APG_TRUE;

APG_EXIT:
  return ulRet;
}

ulong ulASTPrev(APG_AST_ITERATOR* spIter)
{
  ulong ulRet = APG_FALSE;
  APG_AST_ITER_HELPER* spHelper = NULL;
  APG_AST_VALUE* spValue;
  APG_AST_NODE* spNode;
  APG_AST_CTX* spAst;
  ulong ulNodeIndex;

  // validate
  if(!spIter){goto APG_EXIT;}
  spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
  if(!(spHelper->vpValidate == (void*)spIter)){goto APG_EXIT;}
  spAst = spHelper->spAst;

  // validate nodes
  if(spHelper->ulNodeIndex == 0){goto APG_EXIT;}
  ulNodeIndex = spHelper->ulNodeIndex - 1;
  spNode = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeIndex);
  if(!spNode){goto APG_EXIT;}

  // validate values
  spValue = (APG_AST_VALUE*)vpVecAt(spAst->vpVecValues, spNode->ulValueIndex);
  if(!spValue){goto APG_EXIT;}

  // update the indexes
  spHelper->ulNodeIndex = ulNodeIndex;

  // get the rule name
  spIter->ulRuleID = spValue->ulRuleIndex;
  spIter->cpRuleName = spAst->cppRuleNames[spValue->ulRuleIndex];
  DASSERT(spIter->cpRuleName);

  // set the phrase
  spIter->sPhrase.ucpStr = spAst->sSrc.ucpStr + spValue->sPhrase.ulOffset;
  spIter->sPhrase.ulLen = spValue->sPhrase.ulLen;

  // set the direction
  spIter->ulDown = spNode->ulDown ? APG_TRUE: APG_FALSE;
  spIter->ulUp = spNode->ulDown ? APG_FALSE: APG_TRUE;

  // success
  ulRet = APG_TRUE;

APG_EXIT:
  return ulRet;
}

ulong ulASTSkipBranch(APG_AST_ITERATOR* spIter)
{
  ulong ulRet = APG_FALSE;
  APG_AST_ITER_HELPER* spHelper = NULL;
  APG_AST_VALUE* spValue;
  APG_AST_NODE* spNode;
  APG_AST_CTX* spAst;
  ulong ulNodeIndex;
  ulong ulValueIndex;

  // validate
  if(!spIter){goto APG_EXIT;}
  spHelper = (APG_AST_ITER_HELPER*)((char*)spIter + sizeof(APG_AST_ITERATOR));
  if(!(spHelper->vpValidate == (void*)spIter)){goto APG_EXIT;}
  spAst = spHelper->spAst;

  // validate nodes
  spNode = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, spHelper->ulNodeIndex);
  if(!spNode){goto APG_EXIT;}

  // can only skip from down node to respective up node
  if(!spNode->ulDown){goto APG_EXIT;}

  // find the respective up node
  ulNodeIndex = spNode->ulOtherIndex;
  spNode = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeIndex);
  if(!spNode){goto APG_EXIT;}

  // increment to next branch
  ulNodeIndex++;
  spNode = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeIndex);
  if(!spNode){goto APG_EXIT;}

  // validate values
  ulValueIndex = spNode->ulValueIndex;
  spValue = (APG_AST_VALUE*)vpVecAt(spAst->vpVecValues, ulValueIndex);
  if(!spValue){goto APG_EXIT;}

  // update the indexes
  spHelper->ulNodeIndex = ulNodeIndex;

  // get the rule name
  spIter->ulRuleID = spValue->ulRuleIndex;
  spIter->cpRuleName = spAst->cppRuleNames[spValue->ulRuleIndex];
  DASSERT(spIter->cpRuleName);

  // set the phrase
  spIter->sPhrase.ucpStr = spAst->sSrc.ucpStr + spValue->sPhrase.ulOffset;
  spIter->sPhrase.ulLen = spValue->sPhrase.ulLen;

  // set the direction
  spIter->ulDown = spNode->ulDown ? APG_TRUE: APG_FALSE;
  spIter->ulUp = spNode->ulDown ? APG_FALSE: APG_TRUE;

  // success
  ulRet = APG_TRUE;

APG_EXIT:
  return ulRet;
}

#endif // _AST_c_
