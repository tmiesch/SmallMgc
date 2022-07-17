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

   created: 8/25/2007 10:16
  filename: D:\Projects\Apgv5_0\ApgLib\Operators.c
   purpose: A library of functions that perform the actual node
                operations (ALT, CAT, RNM, REP, PRD, TRG, TBS, TLS)

*********************************************************************/
#include "Apg.h"
#include "Private.h"
#include "Operators.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
// initialization functions
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Does the one-time-per-process initialization of the rule list.
//
// FUNCTION: ulong ulRuleListInit
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) APG_EXEHDR* spExe
//              : pointer to the static executable parser data
//                (includes a relocatable rule list and set of
//                 call back function pointers)
//
//      ARG: 3) void* vpCallbacks
//              : pointer to the static list of call back function pointers
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
ulong ulDumbLookup(APG_RULES* spRules, ulong ulRuleCount, char* cpName, void* pfn)
{
  ulong ulRet = APG_FALSE;
  int iCmp;
  APG_RULES* spRuleEnd = spRules + ulRuleCount;
  for(; spRules < spRuleEnd; ++spRules)
  {
    iCmp = apg_stricmp(spRules->cpRuleName, cpName);
    if(iCmp == 0)
    {
      ulRet = APG_TRUE;
      break;
    }
  }
  return ulRet;
}

ulong ulRuleListInit(APG_PARSER_CTX* spCtx, APG_EXEHDR* spExe)
{
  ulong       ulReturn = APG_FALSE;
  APG_RULES*  spRule;
  APG_RULES*  spRules;
  APG_RULES*  spRuleEnd;
  ulong       ulIndex;
  uchar*      ucpOpcodes;
  uchar*      ucpStrTbl;
  PASSERT(spExe && spCtx && spCtx->vpValidate == (void*)spCtx);
  // convert rule list indexes to addresses
  ucpStrTbl = (uchar*)spExe + spExe->sStrTbl.ulOffset;
  ucpOpcodes = (uchar*)spExe + spExe->sOpcodes.ulOffset;
  memcpy(spCtx->vpRules, (void*)((uchar*)spExe + spExe->sRuleList.ulOffset), spExe->ulRuleCount * sizeof(APG_RULES));
  spRules = (APG_RULES*)(spCtx->vpRules);
  spRuleEnd = spRules + spExe->ulRuleCount;
  spRule = spRules;
  for(spRule = spRules; spRule < spRuleEnd; ++spRule)
  {
    // rule name pointer into string table
    ulIndex = (ulong)spRule->cpRuleName;
    spRule->cpRuleName = (char*)(ucpStrTbl + ulIndex);

    // operator ID to pointer
    ulIndex = (ulong)spRule->vpOp;
    spRule->vpOp = (void*)(ucpOpcodes + ulIndex);

    // initialize callbacks
    spRule->pfnCBSyntax = (SYN_CALLBACK)NULL;
    spRule->pfnCBSemantic = (SEM_CALLBACK)NULL;
  }

  // success
  ulReturn = APG_TRUE;
APG_EXIT:
  return ulReturn;
}

// convert opcode indexes to addresses
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Does the one-time-per-process initialization of the
// complete list of opcodes. The opcodes in the static data that comes
// with the parser are in a relocatable format.
//
// FUNCTION: ulong ulOpcodeInit
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) APG_EXEHDR* spExe
//              : pointer to the static executable parser data
//                (includes a list of opcodes in relocatable format)
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
ulong ulOpcodeInit(APG_PARSER_CTX* spCtx, APG_EXEHDR* spExe)
{
  ulong       ulReturn = APG_FALSE;
  ulong       ulLoop;
  uchar*      ucpOpcodes;
  uchar*      ucpOp;
  uchar*      ucpStrTbl;
  OP_ALT*     spAlt;
  OP_CAT*     spCat;
  OP_RNM*     spRnm;
  OP_REP*     spRep;
  OP_PRD*     spPrd;
  OP_TRG*     spTrg;
  OP_TBS*     spTbs;
  OP_TLS*     spTls;
  OP_GEN*     spGen;
  PASSERT(spExe && spCtx && spCtx->vpValidate == (void*)spCtx);
  ucpStrTbl = (uchar*)spExe + spExe->sStrTbl.ulOffset;
  ucpOpcodes = (uchar*)spExe + spExe->sOpcodes.ulOffset;
  ucpOp = ucpOpcodes;
  for(ulLoop = 0; ulLoop < spExe->ulOpcodeCount; ++ulLoop)
  {
    spGen = (OP_GEN*)ucpOp;
    spGen->spNext = (struct OP_GEN*)(ucpOpcodes + (ulong)spGen->spNext);
    switch((ulong)spGen->pfnOp)
    {
    case OP_ID_ALT:
      spAlt = (OP_ALT*)spGen;
      spAlt->pfnOp = vALT;
      ucpOp += sizeof(*spAlt);
      break;
    case OP_ID_CAT:
      spCat = (OP_CAT*)spGen;
      spCat->pfnOp = vCAT;
      ucpOp += sizeof(*spCat);
      break;
    case OP_ID_REP:
      spRep = (OP_REP*)spGen;
      spRep->pfnOp = vREP;
      ucpOp += sizeof(*spRep);
      break;
    case OP_ID_RNM:
      spRnm = (OP_RNM*)spGen;
      spRnm->pfnOp = vRNM;
      ucpOp += sizeof(*spRnm);
      break;
    case OP_ID_TRG:
      spTrg = (OP_TRG*)spGen;
      spTrg->pfnOp = vTRG;
      ucpOp += sizeof(*spTrg);
      break;
    case OP_ID_TBS:
      spTbs = (OP_TBS*)spGen;
      spTbs->pfnOp = vTBS;
      spTbs->ucpString = ucpStrTbl + (ulong)spTbs->ucpString;
      ucpOp += sizeof(*spTbs);
      break;
    case OP_ID_TLS:
      spTls = (OP_TLS*)spGen;
      spTls->pfnOp = vTLS;
      spTls->ucpString = ucpStrTbl + (ulong)spTls->ucpString;
      ucpOp += sizeof(*spTls);
      break;
    case OP_ID_PRD:
      spPrd = (OP_PRD*)spGen;
      spPrd->pfnOp = vPRD;
      ucpOp += sizeof(*spPrd);
      break;
    default:
      PASSERT(APG_FALSE);
      break;
    }
  }

  // success
  ulReturn = APG_TRUE;
APG_EXIT:
  return ulReturn;
}

// run the start rule
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Runs the start rule. i.e. parses the input string.
//
// FUNCTION: void vRunStartRule
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulStartRule
//              : rule name index of the start rule
//                (see the parser's header file for a list of rule name indexes)
//
//      ARG: 3) ulong* ulpState
//              : pointer to the final parser state (returned)
//
//      ARG: 4) ulong* ulpMatched
//              : pointer to the number of characters matched (returned)
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vRunStartRule(APG_PARSER_CTX* spCtx, ulong ulStartRule, ulong* ulpState, ulong* ulpMatched)
{
  OP_RNM sRnm;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(ulpState && ulpMatched);

  // create a dummy RNM opcode
  sRnm.pfnOp = vRNM;
  sRnm.ulRuleIndex = ulStartRule;
  sRnm.spNext = NULL; // not used in vRNM()
  vASTClear((APG_AST_CTX*)spCtx->vpAstCtx, &spCtx->sSrc);

  // execute the start rule
  vRNM(spCtx, (OP_GEN*)&sRnm, 0, ulpState, ulpMatched);
}

//////////////////////////////////////////////////////////////////////////
// SABNF OPERATORS
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the ALT operation on a specific ALT opcode.
//
// FUNCTION: void vALT
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) void* vpOp
//              : pointer to the specific opcode defining the operation
//
//      ARG: 3) ulong ulChar
//              : index of the first character of the sub-string to match
//
//      ARG: 4) ulong* ulpState
//              : returned - final operation state (MATCH, EMPTY, NOMATCH)
//
//      ARG: 5) ulong* ulpMatched
//              : returned - number of characters matched
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vALT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_ALT*         spOp = (OP_ALT*)vpOp;
  OP_GEN*         spChild = (OP_GEN*)((uchar*)vpOp + sizeof(OP_ALT));
  OP_GEN*         spEnd = (OP_GEN*)spOp->spNext;
  ulong           ulState;
  ulong           ulMatched;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vALT);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);

  // execute the children opcodes
  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  for(; spChild < spEnd; spChild = (OP_GEN*)spChild->spNext)
  {
    spChild->pfnOp(spCtx, spChild, ulChar, &ulState, &ulMatched);
    APG_NODESTATS(spCtx, NODE_CHILD_COUNT, OP_ID_ALT, ulChar, ulState, ulMatched, 0);
    APG_NODESTATS_BACK(spCtx->vpStatsCtx, OP_ID_ALT, ulState, ulMatched);
    if(ulState != NOMATCH)
    {
      // apply first-success disambiguation rule - stop after first alternate succeeds
      break;
    }
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_ALT, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the CAT operation on a specific ALT opcode.
//
// FUNCTION: void vCAT
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vCAT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_CAT*         spOp = (OP_CAT*)vpOp;
  OP_GEN*         spChild = (OP_GEN*)((uchar*)vpOp + sizeof(OP_CAT));
  OP_GEN*         spEnd = (OP_GEN*)spOp->spNext;
  ulong           ulState;
  ulong           ulMatched = 0;
  ulong           ulCharsMatched;
  ulong           ulNextChar = ulChar;
  APG_AST_CTX*    spAst;
  ulong           ulNodeIndex;
  ulong           ulValueIndex;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;

  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vCAT);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);

  // execute the children opcodes
  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
  if(spAst)
  {
    DASSERT(spAst->vpValidate == (void*)spAst);
    ulNodeIndex = spAst->ulNodeIndex;
    ulValueIndex = spAst->ulValueIndex;
  }
  for(; spChild < spEnd; spChild = (OP_GEN*)spChild->spNext)
  {
    ulCharsMatched = 0;
    spChild->pfnOp(spCtx, spChild, ulNextChar, &ulState, &ulCharsMatched);
    APG_NODESTATS(spCtx, NODE_CHILD_COUNT, OP_ID_CAT, ulChar, ulState, ulMatched, 0);
    ulMatched += ulCharsMatched;
    ulNextChar += ulCharsMatched;
    DASSERT(ulNextChar <= spCtx->sSrc.ulLen);
    if(ulState == NOMATCH)
    {
      // stop if any child fails
      break;
    }
  }

  if(ulState != NOMATCH)
  {
    ulState = ulMatched ? MATCH : EMPTY;
  }
  else if(spAst)
  {
    vpVecPopTo(spAst->vpVecNodes, ulNodeIndex);
    vpVecPopTo(spAst->vpVecValues, ulValueIndex);
    spAst->ulNodeIndex = ulNodeIndex;
    spAst->ulValueIndex = ulValueIndex;
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_CAT, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the REP operation on a specific ALT opcode.
//
// FUNCTION: void vREP
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vREP(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_REP*         spOp = (OP_REP*)vpOp;
  OP_GEN*         spChild = (OP_GEN*)((uchar*)vpOp + sizeof(OP_REP));
  ulong           ulState;
  ulong           ulMatched = 0;
  ulong           ulMatchCount = 0;
  ulong           ulCharsMatched;
  ulong           ulNextChar = ulChar;
  APG_AST_CTX*    spAst;
  ulong           ulNodeIndex;
  ulong           ulValueIndex;
  ulong           ulChildNodeIndex;
  ulong           ulChildValueIndex;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;

  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vREP);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);

  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
  if(spAst)
  {
    DASSERT(spAst->vpValidate == (void*)spAst);
    ulNodeIndex = spAst->ulNodeIndex;
    ulValueIndex = spAst->ulValueIndex;
  }

  // single point of exit loop
  while(APG_TRUE)
  {
    // test the child
    if(spAst)
    {
      ulChildValueIndex = spAst->ulValueIndex;
      ulChildNodeIndex = spAst->ulNodeIndex;
    }
    spChild->pfnOp(spCtx, spChild, ulNextChar, &ulState, &ulCharsMatched);
    APG_NODESTATS(spCtx, NODE_CHILD_COUNT, OP_ID_REP, ulChar, 0, 0, 0);
    APG_NODESTATS_BACK(spCtx->vpStatsCtx, OP_ID_REP, ulState, ulMatched);

    // end of loop tests
    if(ulState == NOMATCH)
    {
      // child failure
      if(spAst)
      {
        vpVecPopTo(spAst->vpVecNodes, ulChildNodeIndex);
        vpVecPopTo(spAst->vpVecValues, ulChildValueIndex);
        spAst->ulNodeIndex = ulChildNodeIndex;
        spAst->ulValueIndex = ulChildValueIndex;
      }
      break;
    }
    if(ulState == EMPTY){break;}            // never retry an empty match
    if(ulNextChar == spCtx->sSrc.ulLen)
    {
      // collect EOF stats
      APG_NODESTATS(spCtx, NODE_EOS_ACCEPT, OP_ID_REP, ulNextChar, ulState, ulCharsMatched, 0);

      // never retry an EOF character
      break;
    }

    // increment for next repetition
    ++ulMatchCount;
    ulMatched += ulCharsMatched;
    ulNextChar += ulCharsMatched;
    if(ulMatchCount == spOp->ulMax){break;}  // don't exceed maximum repetitions
    DASSERT(ulNextChar <= spCtx->sSrc.ulLen);
  }

  // evaluate the match count
  DASSERT(ulMatchCount <= spOp->ulMax);
  if(ulMatchCount >= spOp->ulMin)
  {
    // REP succeeds
    ulState = ulMatched ? MATCH : EMPTY;
  }
  else
  {
    // REP fails
    ulState = NOMATCH;
    APG_NODESTATS_BACK(spCtx->vpStatsCtx, OP_ID_REP, ulState, ulMatched);
    if(spAst)
    {
      vpVecPopTo(spAst->vpVecNodes, ulNodeIndex);
      vpVecPopTo(spAst->vpVecValues, ulValueIndex);
      spAst->ulNodeIndex = ulNodeIndex;
      spAst->ulValueIndex = ulValueIndex;
    }
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_REP, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the PRD operation on a specific ALT opcode.
//
// FUNCTION: void vPRD
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPRD(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_PRD*         spOp = (OP_PRD*)vpOp;
  OP_GEN*         spChild = (OP_GEN*)((uchar*)vpOp + sizeof(OP_PRD));
  ulong           ulState;
  ulong           ulMatched;
  APG_AST_CTX*    spAst;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;

  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPRD);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);

  if(spCtx->vpAstCtx)
  {
    // turn off AST node collection in PRD operators
    spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
    DASSERT(spAst->vpValidate == (void*)spAst);
    spAst->ulInPRD++;
  }

  // test the element
  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  spChild->pfnOp(spCtx, spChild, ulChar, &ulState, &ulMatched);
  DASSERT(ulState != ACTIVE);

  if(spCtx->vpAstCtx)
  {
    spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
    DASSERT(spAst->vpValidate == (void*)spAst);
    spAst->ulInPRD--;
  }

  // process the returned state
  switch(ulState)
  {
  case EMPTY:
  case MATCH:
    if(spOp->ulAnd)
    {
      // AND predicate
      ulState = EMPTY;
    }
    else
    {
      // NOT predicate
      ulState = NOMATCH;
    }
    break;
  case NOMATCH:
    if(spOp->ulAnd)
    {
      // AND predicate
      ulState = NOMATCH;
    }
    else
    {
      // NOT predicate
      ulState = EMPTY;
    }
    break;
  default:
    DASSERT(APG_FALSE);
    break;
  }

  ulMatched = 0;
  APG_NODESTATS(spCtx, NODE_END, OP_ID_PRD, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  APG_NODESTATS_BACK(spCtx->vpStatsCtx, OP_ID_PRD, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the RNM operation on a specific ALT opcode.
//
// FUNCTION: void vRNM
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vRNM(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_RNM*         spOp = (OP_RNM*)vpOp;
  APG_RULES*      spRuleList;
  APG_RULES*      spRule;
  OP_GEN*         spChild;
  ulong           ulSyntaxCallback;
  ulong           ulState = NOMATCH;
  ulong           ulMatched = 0;
  APG_AST_CTX*    spAst;
  ulong           ulAstHasNode = APG_FALSE;
  ulong           ulNodeDown;
  ulong           ulValueDown;
  APG_AST_NODE*   spNodeDown;
  APG_AST_NODE*   spNodeUp;
  APG_AST_VALUE*  spValue;
  ulong           ulPrevRNMParent;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;

  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vRNM);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);

  // single point of exit loop
  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
  while(APG_TRUE)
  {
    spRuleList = (APG_RULES*)spCtx->vpRules;
    spRule = spRuleList + spOp->ulRuleIndex;
    spChild = (OP_GEN*)spRule->vpOp;
    spCtx->sCBData.ulRuleID = spOp->ulRuleIndex;
    if(spAst)
    {
      DASSERT(spAst->vpValidate == (void*)spAst);
      ulAstHasNode = (spAst->cppRuleNames[spOp->ulRuleIndex] && !spAst->ulInPRD) ?APG_TRUE :APG_FALSE;
    }
    if(ulAstHasNode)
    {
      // save off local values needed for processing "up" node
      ulNodeDown = spAst->ulNodeIndex;
      ulValueDown = spAst->ulValueIndex;
      ulPrevRNMParent = spAst->ulRNMParent;

      // push a new "down" node
      spNodeDown = (APG_AST_NODE*)vpVecPush(spAst->vpVecNodes, NULL);
      PASSERT(spNodeDown);
      spNodeDown->ulDown = APG_TRUE;
      spNodeDown->ulOtherIndex = UNDEFINED;
      spNodeDown->ulValueIndex = spAst->ulValueIndex;

      // push a new node value
      spValue = (APG_AST_VALUE*)vpVecPush(spAst->vpVecValues, NULL);
      PASSERT(spValue);
      spValue->ulRuleIndex = spOp->ulRuleIndex;
      spValue->ulParentValue = spAst->ulRNMParent;
      spValue->ulIdentifier = spAst->ulValueIndex;

      // update control values
      spAst->ulRNMParent = spAst->ulValueIndex;
      spAst->ulNodeIndex++;
      spAst->ulValueIndex++;
    }

    //////////////////////////////////////////////////////////////////////////
    // PRE-BRANCH CALL BACK & AST
    ulSyntaxCallback = APG_FALSE;
    if(spRule->pfnCBSyntax)
    {
      // call the call back function pre-branch
      ulState = SYN_PRE;
      ulMatched = 0;
      spRule->pfnCBSyntax(&spCtx->sCBData, &ulState, ulChar, &ulMatched);
      // validate return from user-written functions
      switch(ulState)
      {
        case EMPTY:
        case NOMATCH:
        ulMatched = 0;
        ulSyntaxCallback = APG_TRUE;
        break;

        case MATCH:
        // matched string must not extend past end of input string
        if(ulChar + ulMatched > spCtx->sSrc.ulLen)
        {
          // truncate to end of string
          ulMatched = spCtx->sSrc.ulLen - ulChar;
        }
        ulSyntaxCallback = APG_TRUE;
        break;

        case SYN_PRE:
        default:
        // ignore syntax call back if no action or user error
        ulState = NOMATCH;
        ulMatched = 0;
        ulSyntaxCallback = APG_FALSE;
        break;
      }
    }
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // EXECUTE THE RULE
    if(ulSyntaxCallback == APG_FALSE && spChild->pfnOp != NULL)
    {
      spChild->pfnOp(spCtx, spChild, ulChar, &ulState, &ulMatched);
    }
    APG_PACKRATSTATS(spCtx->vpPackratCtx, spOp->ulRuleIndex, ulChar, ulState, ulMatched);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // POST-BRANCH CALL BACK & AST
    if(spRule->pfnCBSyntax)
    {
      // call the call back function post-branch
      spRule->pfnCBSyntax(&spCtx->sCBData, &ulState, ulChar, &ulMatched);
      switch(ulState)
      {
        case EMPTY:
        case NOMATCH:
        ulMatched = 0;
        break;

        case MATCH:
        // matched string must not extend past end of input string
        if(ulChar + ulMatched > spCtx->sSrc.ulLen)
        {
          // truncate to end of string
          ulMatched = spCtx->sSrc.ulLen - ulChar;
        }
        break;

        default:
        // continue processing on user error
        ulState = NOMATCH;
        ulMatched = 0;
        break;
      }
    }

    if(ulAstHasNode)
    {
      if(ulState == NOMATCH)
      {
        // branch below failed, delete AST nodes associated with failed branch
        vpVecPopTo(spAst->vpVecNodes, ulNodeDown);
        vpVecPopTo(spAst->vpVecValues, ulValueDown);
        spAst->ulNodeIndex = ulNodeDown;
        spAst->ulValueIndex = ulValueDown;
        break;
      }

      // retrieve the down node
      spNodeDown = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeDown);
      PASSERT(spNodeDown);

      // set the value phrase
      spValue = (APG_AST_VALUE*)vpVecAt(spAst->vpVecValues, spNodeDown->ulValueIndex);
      PASSERT(spValue);
      spValue->sPhrase.ulOffset = ulChar;
      spValue->sPhrase.ulLen = ulMatched;

      // push a new "up" node
      spNodeUp = (APG_AST_NODE*)vpVecPush(spAst->vpVecNodes, NULL);
      PASSERT(spNodeUp);

      // refresh node down pointer
      spNodeDown = (APG_AST_NODE*)vpVecAt(spAst->vpVecNodes, ulNodeDown);
      PASSERT(spNodeDown);
      spNodeUp->ulDown = APG_FALSE;
      spNodeUp->ulOtherIndex = ulNodeDown;
      spNodeUp->ulValueIndex = spNodeDown->ulValueIndex;
      spNodeDown->ulOtherIndex = spAst->ulNodeIndex;

      // reset the global controls
      spAst->ulNodeIndex++;
      spAst->ulRNMParent = ulPrevRNMParent;
    }
    break;
  }
  //////////////////////////////////////////////////////////////////////////

  APG_NODESTATS(spCtx, NODE_END, OP_ID_RNM, ulChar, ulState, ulMatched, spOp->ulRuleIndex);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
APG_EXIT:
    return;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the TRG operation on a specific ALT opcode.
//
// FUNCTION: void vTRG
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vTRG(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_TRG*         spOp = (OP_TRG*)vpOp;
  ulong           ulState;
  ulong           ulMatched;
  ulong           ulTest;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vTRG);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);

  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  if(ulChar < spCtx->sSrc.ulLen)
  {
    // only accepts one character
    ulTest = (ulong)spCtx->sSrc.ucpStr[ulChar];
    if(ulTest >= spOp->ulMin && ulTest <= spOp->ulMax)
    {
      ulState = MATCH;
      ulMatched = 1;
    }
    else
    {
      ulState = NOMATCH;
      ulMatched = 0;
    }
  }
  else
  {
    // EOS character tested
    ulState = NOMATCH;
    ulMatched = 0;
    APG_NODESTATS(spCtx, NODE_EOS_VISIT, OP_ID_TRG, ulChar, ulState, ulMatched, 0);
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_TRG, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the TBS operation on a specific ALT opcode.
//
// FUNCTION: void vTBS
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vTBS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_TBS* spOp = (OP_TBS*)vpOp;
  ulong  ulState;
  ulong  ulMatched;
  uchar* ucpBegSrc;
  uchar* ucpEndSrc;
  uchar* ucpBegTbl;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vTBS);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);

  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  if(ulChar == spCtx->sSrc.ulLen)
  {
    // EOS character
    ulState = NOMATCH;
    ulMatched = 0;
    APG_NODESTATS(spCtx, NODE_EOS_VISIT, OP_ID_TLS, ulChar, ulState, ulMatched, 0);
    goto TBS_EXIT;
  }

  if(spOp->ulLength > (spCtx->sSrc.ulLen - ulChar))
  {
    // string will over run the remaining characters
    ulState = NOMATCH;
    ulMatched = 0;
    goto TBS_EXIT;
  }

  // test input string against TBS defining string
  ucpBegSrc = &spCtx->sSrc.ucpStr[ulChar];
  ucpEndSrc = ucpBegSrc + spOp->ulLength;
  ucpBegTbl = spOp->ucpString;
  for( ; ucpBegSrc < ucpEndSrc; ++ucpBegSrc, ++ucpBegTbl)
  {
    if(*ucpBegSrc != *ucpBegTbl)
    {
      // no match
      ulState = NOMATCH;
      ulMatched = spOp->ulLength - (ucpEndSrc - ucpBegSrc);
      goto TBS_EXIT;
    }
  }

  // success
  ulState = MATCH;
  ulMatched = spOp->ulLength;

TBS_EXIT:
  APG_NODESTATS(spCtx, NODE_END, OP_ID_TBS, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the TLS operation on a specific ALT opcode.
//
// FUNCTION: void vTLS
//
//      ARGS: see vALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vTLS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  OP_TLS* spOp = (OP_TLS*)vpOp;
  ulong  ulState;
  ulong  ulMatched;
  uchar* ucpBegSrc;
  uchar* ucpEndSrc;
  uchar* ucpBegTbl;
  uchar  ucSrcChar;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vTLS);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0, 0);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_GEN*)spOp);

  *ulpState = SYN_NOMATCH;
  *ulpMatched = 0;
  if(spOp->ulLength == 0)
  {
    // this is an empty string operator
    ulState = EMPTY;
    ulMatched = 0;
    goto TLS_EXIT;
  }

  if(ulChar == spCtx->sSrc.ulLen)
  {
    // EOS character
    ulState = NOMATCH;
    ulMatched = 0;
    APG_NODESTATS(spCtx, NODE_EOS_VISIT, OP_ID_TLS, ulChar, ulState, ulMatched, 0);
    goto TLS_EXIT;
  }

  if(spOp->ulLength > (spCtx->sSrc.ulLen - ulChar))
  {
    // string will over run the remaining characters
    ulState = NOMATCH;
    ulMatched = 0;
    goto TLS_EXIT;
  }

  // test input string against TLS defining string
  ucpBegSrc = &spCtx->sSrc.ucpStr[ulChar];
  ucpEndSrc = ucpBegSrc + spOp->ulLength;
  ucpBegTbl = spOp->ucpString;
  for( ; ucpBegSrc < ucpEndSrc; ++ucpBegSrc, ++ucpBegTbl)
  {
    ucSrcChar = *ucpBegSrc;
    if(ucSrcChar >= 65 && ucSrcChar <= 90)
    {
      // upper case alpha - make lower
      ucSrcChar |= 0x20;
    }

    if(ucSrcChar != *ucpBegTbl)
    {
      // no match
      ulState = NOMATCH;
      ulMatched = spOp->ulLength - (ucpEndSrc - ucpBegSrc);
      goto TLS_EXIT;
    }
  }

  // success
  ulState = MATCH;
  ulMatched = spOp->ulLength;

TLS_EXIT:
  APG_NODESTATS(spCtx, NODE_END, OP_ID_TLS, ulChar, ulState, ulMatched, 0);
  APG_NODETRACEEND(spCtx, ulChar, (OP_GEN*)spOp, ulState, ulMatched);
  DASSERT(ulState != ACTIVE);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}
