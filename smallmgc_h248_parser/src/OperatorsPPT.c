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

   created: 8/25/2007 10:17
  filename: D:\Projects\Apgv5_0\ApgLib\OperatorsPPT.c
   purpose: Source code file for the PPT versions of the APG operators.
  
*********************************************************************/
#include "Apg.h"
#include "Private.h"
#include "OperatorsPPT.h"

//////////////////////////////////////////////////////////////////////////
// PPT CHARACTER TESTING FUNCTIONS
//////////////////////////////////////////////////////////////////////////
//#define vTestPPT(ctx, ppt, index, state, match) vFuncTestPPT((ctx), (ppt), (index), (state), (match))
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Test a PPT for a specific syntax-alphabet character.
//
// FUNCTION: void vFuncTestPPT
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) uchar* ucpPPT
//              : pointer to the PPT to test
//
//      ARG: 3) ulong ulChar
//              : index of the input string character to test
//
//      ARG: 4) ulong* ulpState
//              : returned - the PPT value for this character
//
//      ARG: 5) ulong* ulpMatched
//              : returned - 1 if *ulpState == MATCH, 0 otherwise
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vFuncTestPPT(APG_PARSER_CTX* spCtx, uchar* ucpPPT, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  ulong ulCharValue;
  
  if(ucpPPT == NULL)
  {
    // no PPT - must run the opcode
    *ulpState = ACTIVE;
    *ulpMatched = 0;
    goto APG_EXIT;
  }
  
  // test for valid character
  if(ulChar < spCtx->sSrc.ulLen)
  {
    // this should be true 99.9% of the time
    ulCharValue = (ulong)spCtx->sSrc.ucpStr[ulChar];
  }
  else if(ulChar == spCtx->sSrc.ulLen)
  {
    // true for EOF character
    ulCharValue = spCtx->ulEOFChar;
    
    // EOF tested
    APG_NODESTATS(spCtx, NODE_EOF_VISIT, OP_ID_EOF, ulChar, 0,0);
  }
  else
  {
    // input string error - invalid character
    *ulpState = NOMATCH;
    *ulpMatched = 0;
    goto APG_EXIT;
  }
  
  *ulpState = ulGetPPTValue(ucpPPT, ulCharValue);
  switch(*ulpState)
  {
  case ACTIVE:
  case NOMATCH:
  case EMPTY:
    *ulpMatched = 0;
    break;
  case MATCH:
    *ulpMatched = 1;
    break;
  default:
    PASSERT(APG_FALSE);
    break;
  }
  
APG_EXIT:
  // collect PPT stats
  APG_NODESTATS(spCtx, NODE_PPT, OP_ID_PPT, ulChar, *ulpState, *ulpMatched);
}

// macro version of vFuncTestPPT()
#define vTestPPT(ctx, ppt, index, state, match)\
{\
ulong ulCharValue;\
*match = 0;\
if(ppt == NULL){*state = ACTIVE; goto APG_EXIT;}\
if(index  < ctx->sSrc.ulLen){ulCharValue = (ulong)ctx->sSrc.ucpStr[index]; goto APG_EVAL;}\
if(index == ctx->sSrc.ulLen){ulCharValue = ctx->ulEOFChar; APG_NODESTATS(ctx, NODE_EOF_VISIT, OP_ID_EOF, index, 0,0); goto APG_EVAL;}\
*state = NOMATCH; goto APG_EXIT;\
APG_EVAL: {*state = GETPPTVALUE(ppt, ulCharValue); if(*state == MATCH) *match = 1;}\
APG_EXIT: APG_NODESTATS(ctx, NODE_PPT, OP_ID_PPT, index, *state, *match);\
}

// gets a PPT value, regardless of its type
// uchar* ucpPPT - pointer to a PPT
//               - if UNDEFINED, no PPT exists
// ulong ulChar - zero-based index of the string character to get a PPT value for
// ulong ulIndex - zero-vased index of the character to get
// NOTE: first two bits are type flag, PPT values follow the type
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Get a PPT value.
//
// FUNCTION: ulong ulGetPPTValue
//
//      ARG: 1) uchar* ucpPPT
//              : pointer to the PPT
//
//      ARG: 2) ulong ulIndex
//              : index of the syntax-alphabet character to get the value for
//
//   RETURN: the PPT value (ACTIVE, MATCH, EMPTY or NOMATCH)
//
//////////////////////////////////////////////////////////////////////////
ulong ulGetPPTValue(uchar* ucpPPT, ulong ulIndex)
{
  ulong ulReturn;

  if((ulong)ucpPPT != UNDEFINED)
  {
    if(PPT_IS_1BIT(ucpPPT))
    {
      ulIndex += 2;
      ulReturn = ulGetBit(ucpPPT, ulIndex) ? ACTIVE : NOMATCH;
    } 
    else
    {
      ++ulIndex;
      ulReturn = ulGetTwoBitValue(ucpPPT, ulIndex);
    }
  }
  else
  {
    ulReturn = ACTIVE;
  }

  return ulReturn;
}

// uchar* ucpPPT - pointer to a PPT
// ulong ulIndex - zero-based index of the PPT value to set
// ulong ulValue - NOMATCH, EMPTY, MATCH or ACTIVE
//                 otherwise, set value is undefined
// NOTE: first two bits are type flag, PPT values follow the type
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Set a PPT value for a specific syntax-alphabet character.
// (Used when constructing the PPT only. Not used when parsing.)
//
// FUNCTION: void vSetPPTValue
//
//      ARG: 1) uchar* ucpPPT
//              : pointer to the PPT
//
//      ARG: 2) ulong ulIndex
//              : index of the syntax-alphabet character to get the value for
//
//      ARG: 3) ulong ulValue
//              : the value to set (ACTIVE, MATCH, EMPTY or NOMATCH)
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vSetPPTValue(uchar* ucpPPT, ulong ulIndex, ulong ulValue)
{
  if(PPT_IS_1BIT(ucpPPT))
  {
    ulIndex += 2;
    if(ulValue == NOMATCH)
    {
      vClearBit(ucpPPT, ulIndex);
    }
    else
    {
      vSetBit(ucpPPT, ulIndex);
    }
  } 
  else
  {
    ++ulIndex;
    vSetTwoBitValue(ucpPPT, ulIndex, ulValue);
  }
}

//////////////////////////////////////////////////////////////////////////
// PPT initialization functions
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Does the one-time-per-process initialization of the rule list.
//
// FUNCTION: ulong ulPPTRuleListInit
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
ulong ulPPTRuleListInit(APG_PARSER_CTX* spCtx, APG_EXEHDR* spExe, void* vpCallbacks)
{
  ulong       ulReturn = APG_FALSE;
  ulong*      ulpCalls = (ulong*)vpCallbacks;
  APG_RULES*  spRuleList = (APG_RULES*)((uchar*)spExe + spExe->sRuleList.ulOffset);
  APG_RULES*  spRule;
  ulong       ulLoop;
  uchar*      ucpPPT;
  uchar*      ucpOpcodes;
  uchar*      ucpStrTbl;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spExe);
  DASSERT(vpCallbacks);
  
  // convert rule list indexes to addresses
  ucpPPT = (uchar*)spExe + spExe->sPPT.ulOffset;
  ucpStrTbl = (uchar*)spExe + spExe->sStrTbl.ulOffset;
  ucpOpcodes = (uchar*)spExe + spExe->sOpcodes.ulOffset;
  spRule = spRuleList;
  for(ulLoop = 0; ulLoop < spExe->ulRuleCount; ++ulLoop, ++spRule)
  {
    // rule name pointer into string table
    spRule->cpRuleName = (char*)(ucpStrTbl + (ulong)spRule->cpRuleName);
    
    // operator ID to pointer
    spRule->vpOp = (void*)(ucpOpcodes + (ulong)spRule->vpOp);
    
    // PPT index to PPT pointer
    if((ulong)spRule->ucpPPT == UNDEFINED)
    {
      spRule->ucpPPT = NULL;
    } 
    else
    {
      spRule->ucpPPT = ucpPPT + (ulong)spRule->ucpPPT;
    }
    
    // call back function pointer
    if((ulong)spRule->pfnCallBack == UNDEFINED)
    {
      spRule->pfnCallBack = NULL;
    }
    else
    {
      spRule->pfnCallBack = (PFN_CALLBACK)*ulpCalls;
      ++ulpCalls;
    }
  }

  // success
  ulReturn = APG_TRUE;
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Does the one-time-per-process initialization of the
// complete list of opcodes. The opcodes in the static data that comes
// with the parser are in a relocatable format.
//
// FUNCTION: ulong ulPPTOpcodeInit
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
ulong ulPPTOpcodeInit(APG_PARSER_CTX* spCtx, APG_EXEHDR* spExe)
{
  ulong       ulReturn = APG_FALSE;
  ulong       ulLoop;
  uchar*      ucpPPT;
  uchar*      ucpOpcodes;
  uchar*      ucpOp;
  uchar*      ucpStrTbl;
  struct OP_PPT_ALT* spAlt;
  struct OP_PPT_CAT* spCat;
  struct OP_PPT_RNM* spRnm;
  struct OP_PPT_REP* spRep;
  struct OP_PPT_PRD* spPrd;
  struct OP_PPT_TRG* spTrg;
  struct OP_PPT_TBS* spTbs;
  struct OP_PPT_TLS* spTls;
  struct OP_PPT_GEN* spGen;
  DASSERT(spExe);
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  ucpStrTbl = (uchar*)spExe + spExe->sStrTbl.ulOffset;
  ucpPPT = (uchar*)spExe + spExe->sPPT.ulOffset;
  ucpOpcodes = (uchar*)spExe + spExe->sOpcodes.ulOffset;
  ucpOp = ucpOpcodes;
  for(ulLoop = 0; ulLoop < spExe->ulOpcodeCount; ++ulLoop)
  {
    spGen = (struct OP_PPT_GEN*)ucpOp;
    if((ulong)spGen->ucpPPT == UNDEFINED)
    {
      spGen->ucpPPT = NULL;
    } 
    else
    {
      spGen->ucpPPT = ucpPPT + (ulong)spGen->ucpPPT;
    }
    spGen->spNext = (struct OP_PPT_GEN*)(ucpOpcodes + (ulong)spGen->spNext);
    switch((ulong)spGen->pfnOp)
    {
    case OP_ID_ALT:
      spAlt = (struct OP_PPT_ALT*)spGen;
      spAlt->pfnOp = vPPT_ALT;
      ucpOp += sizeof(*spAlt);
      break;
    case OP_ID_CAT:
      spCat = (struct OP_PPT_CAT*)spGen;
      spCat->pfnOp = vPPT_CAT;
      ucpOp += sizeof(*spCat);
      break;
    case OP_ID_REP:
      spRep = (struct OP_PPT_REP*)spGen;
      spRep->pfnOp = vPPT_REP;
      ucpOp += sizeof(*spRep);
      break;
    case OP_ID_RNM:
      spRnm = (struct OP_PPT_RNM*)spGen;
      spRnm->pfnOp = vPPT_RNM;
      ucpOp += sizeof(*spRnm);
      break;
    case OP_ID_TRG:
      spTrg = (struct OP_PPT_TRG*)spGen;
      spTrg->pfnOp = vPPT_TRG;
      ucpOp += sizeof(*spTrg);
      break;
    case OP_ID_TBS:
      spTbs = (struct OP_PPT_TBS*)spGen;
      spTbs->pfnOp = vPPT_TBS;
      spTbs->ucpString = ucpStrTbl + (ulong)spTbs->ucpString;
      ucpOp += sizeof(*spTbs);
      break;
    case OP_ID_TLS:
      spTls = (struct OP_PPT_TLS*)spGen;
      spTls->pfnOp = vPPT_TLS;
      spTls->ucpString = ucpStrTbl + (ulong)spTls->ucpString;
      ucpOp += sizeof(*spTls);
      break;
    case OP_ID_PRD:
      spPrd = (struct OP_PPT_PRD*)spGen;
      spPrd->pfnOp = vPPT_PRD;
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

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Runs the start rule. i.e. parses the input string.
//
// FUNCTION: void vPPTRunStartRule
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
void vPPTRunStartRule(APG_PARSER_CTX* spCtx, ulong ulStartRule, ulong* ulpState, ulong* ulpMatched)
{
  APG_RULES*  spRuleList = (APG_RULES*)spCtx->vpRules;
  struct OP_PPT_RNM sRnm;

  // create a dummy RNM opcode
  sRnm.pfnOp = vPPT_RNM;
  sRnm.ulRuleIndex = ulStartRule;
  sRnm.ucpPPT = spRuleList[ulStartRule].ucpPPT;
  sRnm.spNext = NULL; // not used in vRNM()
  
  // execute the start rule at the first character (ulChar = 0)
  vPPT_RNM(spCtx, (struct OP_PPT_GEN*)&sRnm, 0, ulpState, ulpMatched);
}

//////////////////////////////////////////////////////////////////////////
// SABNF OPERATORS
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the ALT operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_ALT
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
void vPPT_ALT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_ALT*     spOp = (struct OP_PPT_ALT*)vpOp;
  struct OP_PPT_GEN*     spChild = (struct OP_PPT_GEN*)((uchar*)vpOp + sizeof(struct OP_PPT_ALT));
  struct OP_PPT_GEN*     spEnd = (struct OP_PPT_GEN*)spOp->spNext;
  ulong           ulState;
  ulong           ulMatched;
  ulong           ulNodeIndex;
  ulong           ulValueIndex;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_ALT);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);

  // spot the AST
  ulNodeIndex = ulVecSize(spCtx->vpPtNodes);
  ulValueIndex = ulVecSize(spCtx->vpPtValues);

  // examine the children
  for(; spChild < spEnd; spChild = (struct OP_PPT_GEN*)spChild->spNext)
  {
    // test the child PPT
    vTestPPT(spCtx, spChild->ucpPPT, ulChar, &ulState, &ulMatched);
    if(ulState == ACTIVE)
    {
      // execute the child opcode
      spChild->pfnOp(spCtx, spChild, ulChar, &ulState, &ulMatched);
      APG_NODESTATS(spCtx, NODE_CHILD_COUNT, OP_ID_ALT, ulChar, ulState, ulMatched);
      APG_NODESTATS(spCtx, NODE_BACKTRACK, OP_ID_ALT, ulChar, ulState, ulMatched);
    }

    if(ulState != NOMATCH)
    {
      // apply first-success disambiguation rule - stop after first alternate succeeds
      break;
    }
    else
    {
      APG_NODESTATS(spCtx, NODE_BACKTRACK, OP_ID_PRD, ulChar, ulState, ulMatched);
    }
  }
  
  if(ulState == NOMATCH)
  {
    // clear the parser tree
    vpVecPopTo(spCtx->vpPtNodes, ulNodeIndex);
    vpVecPopTo(spCtx->vpPtValues, ulValueIndex);
  }
  DASSERT(ulState != ACTIVE);
  APG_NODESTATS(spCtx, NODE_END, OP_ID_ALT, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the CAT operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_CAT
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_CAT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_CAT*     spOp = (struct OP_PPT_CAT*)vpOp;
  struct OP_PPT_GEN*     spChild = (struct OP_PPT_GEN*)((uchar*)vpOp + sizeof(struct OP_PPT_CAT));
  struct OP_PPT_GEN*     spEnd = (struct OP_PPT_GEN*)spOp->spNext;
  ulong           ulNodeIndex;
  ulong           ulValueIndex;
  ulong           ulState;
  ulong           ulMatched = 0;
  ulong           ulCharsMatched;
  ulong           ulNextChar = ulChar;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_CAT);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);
  
  // spot the AST
  ulNodeIndex = ulVecSize(spCtx->vpPtNodes);
  ulValueIndex = ulVecSize(spCtx->vpPtValues);
  
  // execute the children opcodes
  for(; spChild < spEnd; spChild = (struct OP_PPT_GEN*)spChild->spNext)
  {
    vTestPPT(spCtx, spChild->ucpPPT, ulNextChar, &ulState, &ulCharsMatched);
    if(ulState == ACTIVE)
    {
      // execute the child opcode
      spChild->pfnOp(spCtx, spChild, ulNextChar, &ulState, &ulCharsMatched);
      APG_NODESTATS(spCtx, NODE_CHILD_COUNT, OP_ID_CAT, ulChar, ulState, ulMatched);
    }
    
    ulMatched += ulCharsMatched;
    ulNextChar += ulCharsMatched;
    DASSERT(ulNextChar <= spCtx->sSrc.ulLen);
    if(ulState == NOMATCH)
    {
      // stop if any child fails
      break;
    }
  }
  
  DASSERT(ulState != ACTIVE);
  if(ulState != NOMATCH)
  {
    ulState = ulMatched ? MATCH : EMPTY;
  }

  if(ulState == NOMATCH)
  {
    // clear the parser tree
    vpVecPopTo(spCtx->vpPtNodes, ulNodeIndex);
    vpVecPopTo(spCtx->vpPtValues, ulValueIndex);
  }
  APG_NODESTATS(spCtx, NODE_END, OP_ID_CAT, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the REP operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_REP
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_REP(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_REP*     spOp = (struct OP_PPT_REP*)vpOp;
  struct OP_PPT_GEN*     spChild = (struct OP_PPT_GEN*)((uchar*)vpOp + sizeof(struct OP_PPT_REP));
  ulong           ulNodeIndex;
  ulong           ulValueIndex;
  ulong           ulChildNodeIndex;
  ulong           ulChildValueIndex;
  ulong           ulState;
  ulong           ulMatched = 0;
  ulong           ulMatchCount = 0;
  ulong           ulCharsMatched;
  ulong           ulNextChar = ulChar;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_REP);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);

  // spot the AST
  ulNodeIndex = ulVecSize(spCtx->vpPtNodes);
  ulValueIndex = ulVecSize(spCtx->vpPtValues);
  
  // single point of exit loop
  while(APG_TRUE)
  {
    // test the child PPT
    vTestPPT(spCtx, spChild->ucpPPT, ulNextChar, &ulState, &ulCharsMatched);
    if(ulState == ACTIVE)
    {
      // on a child-by-child basis, prepare to delete AST nodes in branch below
      ulChildNodeIndex = ulVecSize(spCtx->vpPtNodes);
      ulChildValueIndex = ulVecSize(spCtx->vpPtValues);
      
      // execute the child opcode
      spChild->pfnOp(spCtx, spChild, ulNextChar, &ulState, &ulCharsMatched);
      APG_NODESTATS(spCtx, NODE_CHILD_COUNT, OP_ID_REP, ulChar, 0, 0);
      APG_NODESTATS(spCtx, NODE_BACKTRACK, OP_ID_REP, ulNextChar, ulState, ulCharsMatched);
      if(ulState == NOMATCH)
      {
        // delete AST nodes from branch below
        vpVecPopTo(spCtx->vpPtNodes, ulChildNodeIndex);
        vpVecPopTo(spCtx->vpPtValues, ulChildValueIndex);
        APG_NODESTATS(spCtx, NODE_BACKTRACK, OP_ID_REP, ulChar, ulState, ulCharsMatched);
      }
    }
    
    // end of loop tests
    if(ulState == NOMATCH){break;}          // child failure
    if(ulState == EMPTY){break;}            // never retry an empty match
    if(ulNextChar == spCtx->sSrc.ulLen)
    {
      // collect EOF stats
      APG_NODESTATS(spCtx, NODE_EOF_ACCEPT, OP_ID_REP, ulNextChar, ulState, ulCharsMatched);
      
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
  }
  
  if(ulState == NOMATCH)
  {
    // clear the parser tree
    vpVecPopTo(spCtx->vpPtNodes, ulNodeIndex);
    vpVecPopTo(spCtx->vpPtValues, ulValueIndex);
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_REP, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the PRD operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_PRD
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_PRD(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_PRD*     spOp = (struct OP_PPT_PRD*)vpOp;
  struct OP_PPT_GEN*     spChild = (struct OP_PPT_GEN*)((uchar*)vpOp + sizeof(struct OP_PPT_PRD));
  ulong           ulNodeIndex;
  ulong           ulValueIndex;
  ulong           ulState;
  ulong           ulMatched;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_PRD);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (struct OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);

  // test the child PPT
  vTestPPT(spCtx, spChild->ucpPPT, ulChar, &ulState, &ulMatched);
  if(ulState == ACTIVE)
  {
    // spot the AST
    ulNodeIndex = ulVecSize(spCtx->vpPtNodes);
    ulValueIndex = ulVecSize(spCtx->vpPtValues);
    
    // execute the child opcode
    spChild->pfnOp(spCtx, spChild, ulChar, &ulState, &ulMatched);

    // always clear the parser tree
    vpVecPopTo(spCtx->vpPtNodes, ulNodeIndex);
    vpVecPopTo(spCtx->vpPtValues, ulValueIndex);
  }
  
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
  
  APG_NODESTATS(spCtx, NODE_BACKTRACK, OP_ID_PRD, ulChar, ulState, ulMatched);
  ulMatched = 0;
  APG_NODESTATS(spCtx, NODE_END, OP_ID_PRD, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the RNM operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_RNM
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_RNM(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_RNM*     spOp = (struct OP_PPT_RNM*)vpOp;
  APG_RULES*      spRule;
  struct OP_PPT_GEN*     spChild;
  ulong           ulState;
  ulong           ulMatched;
  ulong           ulBranch = PFN_OK;
  ulong           ulPreIndex = APG_PT_UNDEFINED;
  ulong           ulPostIndex = APG_PT_UNDEFINED;
  ulong           ulValueIndex;
  ulong           ulParent;
  APG_PT_NODE*    spPreFrame;
  APG_PT_NODE*    spPostFrame;
  APG_PT_VALUE*   spValue;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_RNM);
  DASSERT(ulChar <= spCtx->sSrc.ulLen);
  APG_NODETRACEBEG(spCtx, ulChar, (struct OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);
  
  // single point of exit loop
  while(APG_TRUE)
  {
    spRule = (APG_RULES*)spCtx->vpRules + spOp->ulRuleIndex;
    spChild = (struct OP_PPT_GEN*)spRule->vpOp;

    //////////////////////////////////////////////////////////////////////////
    // begin PRE-BRANCH CALL BACK & AST
    ulBranch = PFN_OK;
    if(spRule->usSyntax)
    {
      // call the call back function pre-branch
      DASSERT(spRule->pfnCallBack);
      ulBranch = spRule->pfnCallBack(spCtx->vpData, SYN_PRE, ulChar, 0);
    }

    if(ulBranch == PFN_SKIP)
    {
      // skip the processing of this RMN node and the branch below
      // treat this as a no match
      ulState = NOMATCH;
      ulMatched = 0;
      break;
    }
    // else continue

    ulPreIndex = ulVecSize(spCtx->vpPtNodes);
    ulValueIndex = ulVecSize(spCtx->vpPtValues);
    if(spRule->usSemantics)
    {
      // initialize a AST node
      spPreFrame = (APG_PT_NODE*)vpVecPush(spCtx->vpPtNodes, NULL);
      spPreFrame->ulPairedNode = APG_PT_UNDEFINED;
      spPreFrame->ulDirection = APG_PT_DOWN;

      // save & replace the parent node
      ulParent = spCtx->ulPrevNode;
      spCtx->ulPrevNode = ulValueIndex;
      
      // push a node value
      spValue = (APG_PT_VALUE*)vpVecPush(spCtx->vpPtValues, NULL);
    }
    // end PRE-BRANCH CALL BACK & AST
    //////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////////
    // begin EXECUTE THE RULE
    DASSERT(spChild);

    // NOTE: the PPT for this rule has already been tested by the parent
    //       opcode and found to be ACTIVE

    // test the PPT of the child operator
    vTestPPT(spCtx, spChild->ucpPPT, ulChar, &ulState, &ulMatched);
    if(ulState == ACTIVE)
    {
      // execute the child opcode
      spChild->pfnOp(spCtx, spChild, ulChar, &ulState, &ulMatched);
      APG_PACKRATSTATS(spCtx->vpPackrat, spOp->ulRuleIndex, ulChar, ulState, ulMatched);
    }
    // end EXECUTE THE RULE
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // begin POST-BRANCH CALL BACK & AST
    ulBranch = PFN_OK;
    if(spRule->usSyntax)
    {
      // call the call back function post-branch
      DASSERT(spRule->pfnCallBack);
      ulBranch = spRule->pfnCallBack(spCtx->vpData, ulState, ulChar, ulMatched);
    }

    if(ulBranch == PFN_SKIP)
    {
      // treat this as a no match
      ulState = NOMATCH;
      ulMatched = 0;
    }

    if(ulState != MATCH)
    {
      // delete the this pre-branch frame and 
      // all frames, if any, from the branch below
      vpVecPopTo(spCtx->vpPtNodes, ulPreIndex);
      vpVecPopTo(spCtx->vpPtValues, ulValueIndex);
      break;
    }

    if(!spRule->usSemantics)
    {
      // skip the AST node processing
      break;
    }

    // finish building the AST node
    DASSERT(ulPreIndex != APG_PT_UNDEFINED);

    // push a RNM end node on the stack
    ulPostIndex = ulVecSize(spCtx->vpPtNodes);
    spPostFrame = (APG_PT_NODE*)vpVecPush(spCtx->vpPtNodes, NULL);
    spPreFrame = (APG_PT_NODE*)vpVecAt(spCtx->vpPtNodes, ulPreIndex);
    spPostFrame->ulPairedNode = ulPreIndex;
    spPreFrame->ulPairedNode = ulPostIndex;
    spPostFrame->ulValue = ulValueIndex;
    spPreFrame->ulValue = ulValueIndex;
    spPostFrame->ulDirection = APG_PT_UP;
    spValue = (APG_PT_VALUE*)vpVecAt(spCtx->vpPtValues, ulValueIndex);

    // finish the node value
    spValue->sValue.ulLen = ulMatched;
    spValue->sValue.ulOffset = ulChar;
    spValue->ulRuleIndex = spOp->ulRuleIndex;
    spValue->ulParentNode = ulParent;
    spValue->ulIdentifier = ++spCtx->ulSeq;

    // restore the parent node
    spCtx->ulPrevNode = ulParent;
    // end POST-BRANCH CALL BACK & AST
    //////////////////////////////////////////////////////////////////////////

    break;
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_RNM, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the TRG operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_TRG
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_TRG(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  struct OP_PPT_TRG*         spOp = (struct OP_PPT_TRG*)vpOp;
  ulong           ulState;
  ulong           ulMatched;
  
  // should never be called - its PPT is determinate for all characters
  DASSERT(APG_FALSE);
  
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_TRG);
  APG_NODETRACEBEG(spCtx, ulChar, (OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);
  
  // test the PPT for this opcode
  vTestPPT(spCtx, spOp->ucpPPT, ulChar, &ulState, &ulMatched);
  DASSERT(ulState != ACTIVE);

  APG_NODESTATS(spCtx, NODE_END, OP_ID_TRG, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the TBS operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_TBS
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_TBS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_TBS* spOp = (struct OP_PPT_TBS*)vpOp;
  ulong           ulState;
  ulong           ulMatched;
  uchar* ucpBegSrc;
  uchar* ucpEndSrc;
  uchar* ucpBegTbl;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_TBS);
  APG_NODETRACEBEG(spCtx, ulChar, (struct OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);

  // length should always be > 1, length == 1 is translated to TRG
  DASSERT(spOp->ulLength > 1);

  // see if there are enough characters left to match
  ucpBegSrc = &spCtx->sSrc.ucpStr[ulChar];
  ucpEndSrc = ucpBegSrc + spOp->ulLength;
  ucpBegTbl = spOp->ucpString;
  
  // test EOF
  if(spOp->ulLength <= (spCtx->sSrc.ulLen - ulChar))
  {
    // test input string against the table string
    for( ; ucpBegSrc < ucpEndSrc; ++ucpBegSrc, ++ucpBegTbl)
    {
      if(*ucpBegSrc != *ucpBegTbl)
      {
        // no match
        break;
      }
    }
    
    if(ucpBegSrc == ucpEndSrc)
    {
      // its a match
      ulState = MATCH;
      ulMatched = spOp->ulLength;
    }
    else
    {
      ulState = NOMATCH;
      ulMatched = spOp->ulLength - (ucpEndSrc - ucpBegSrc);
    }
  }
  else
  {
    ulState = NOMATCH;
    ulMatched = 0;
  }
  // else fail
  
  APG_NODESTATS(spCtx, NODE_END, OP_ID_TBS, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Performs the TLS operation on a specific ALT opcode.
//
// FUNCTION: void vPPT_TLS
//
//      ARGS: see vPPT_ALT
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vPPT_TLS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  struct OP_PPT_TLS* spOp = (struct OP_PPT_TLS*)vpOp;
  ulong       ulState;
  ulong       ulMatched;
  uchar*      ucpBegSrc;
  uchar*      ucpEndSrc;
  uchar*      ucpBegTbl;
  uchar       ucSrcChar;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spOp->pfnOp == vPPT_TLS);
  APG_NODETRACEBEG(spCtx, ulChar, (struct OP_PPT_GEN*)spOp);
  APG_NODESTATS(spCtx, NODE_BEGIN, 0, 0, 0, 0);
  
  // length should always be > 1, when length == 1 PPT is determinant
  DASSERT(spOp->ulLength > 1);

  // init string scan
  ucpBegSrc = &spCtx->sSrc.ucpStr[ulChar];
  ucpEndSrc = ucpBegSrc + spOp->ulLength;
  ucpBegTbl = spOp->ucpString;
  
  // test EOF
  if(spOp->ulLength <= (spCtx->sSrc.ulLen - ulChar))
  {
    // test input string against the table string
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
        break;
      }
    }
  }
  
  // test for success
  if(ucpBegSrc == ucpEndSrc)
  {
    // success
    ulState = MATCH;
    ulMatched = spOp->ulLength;
  }
  else
  {
    // fail
    ulState = NOMATCH;
    ulMatched = spOp->ulLength - (ucpEndSrc - ucpBegSrc);
  }

  APG_NODESTATS(spCtx, NODE_END, OP_ID_TLS, ulChar, ulState, ulMatched);
  APG_NODETRACEEND(spCtx, ulChar, (OP_PPT_GEN*)spOp, ulState, ulMatched);
  *ulpState = ulState;
  *ulpMatched = ulMatched;
}
