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
#ifndef _Trace_c_31_7_2006_11_13_
#define _Trace_c_31_7_2006_11_13_

#include "Apg.h"
// satisfies a Code::Blocks linkage quirk
void vNodeTraceDummy(){}
#ifdef _APG_CFG_TRACE
#include "Private.h"
#include "Operators.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
// NODE TRACING FUNCTIONS
//////////////////////////////////////////////////////////////////////////
#undef TASSERT
#define TASSERT(cond) if(!(int)(cond)) {ulMemErrReport(spCtx->vpMemCtx, APG_TRACE_ID, __LINE__, __FILE__); goto APG_EXIT;}
#define ROW_LENGTH 16 // maximum number of source characters to display on one trace line

static void vIndent(ulong ulIndent)
{
  ulong i;
  for(i = 0; i < ulIndent; ++i)
  {
    printf(" ");
  }
}

static void vDisplayAscii(uchar* ucpSrc, ulong ulLen)
{
  ulong i;

  DASSERT(ucpSrc);
  for(i = 0; i < ulLen; ++i)
  {
    uchar ucChar;
    if(ucpSrc[i] >= 32 && ucpSrc[i] <= 126){ucChar = ucpSrc[i];}
    else {ucChar = '~';}
    printf("%c", ucChar);
  }
}

static void vDisplayHex(uchar* ucpSrc, ulong ulLen)
{
  ulong i;

  DASSERT(ucpSrc);
  printf("%02X", ucpSrc[0]);
  for(i = 1; i < ulLen; ++i)
  {
    if( i % 2 == 0){printf(" ");}
    printf("%02X", ucpSrc[i]);
  }
}

// displays the phrase in ASCII followed by a duplicate in HEX
static void vPhrasePrint(uchar* ucpPhrase, ulong ulLen)
{
  DASSERT(ucpPhrase);
  vDisplayAscii(ucpPhrase, ulLen);
  printf(" 0x");
  vDisplayHex(ucpPhrase, ulLen);
}

// prints ulLen characters in both ASCII and HEX
// indents the line with ulIndent spaces
// prefixes the line with [offset from beginning of full source]
// no charriage return
static void vRowPrint(uchar* ucpSrc, ulong ulOffset, ulong ulLen, ulong ulIndent, ulong ulSpace)
{
  DASSERT(ucpSrc);
  vIndent(ulIndent);
  printf("[%04lu]", ulOffset);
  if(ulLen > 0)
  {
    uchar* ucpPhrase = &ucpSrc[ulOffset];
    vDisplayAscii(ucpPhrase, ulLen);
    vIndent(ulSpace);
    printf(" 0x");
    vDisplayHex(ucpPhrase, ulLen);
  }
  else
  {
    printf(" EOS");
  }
}

// prints the matched source
// if ulSrcLen <= ROW_LENGTH, prints the entire source on one line
// if ulSrcLen > ROW_LENGTH, prints the first ulMaxLen characters on line 1
//   and the last ROW_LENGTH (or less) on line 2
static void vMatchedPrint(uchar* ucpSrc, ulong ulOffset, ulong ulSrcLen, ulong ulIndent)
{
  ulong ulPrefix, ulPrefixLen;
  ulong ulPostfix, ulPostfixLen;

  DASSERT(ucpSrc);
  ulPrefix = ulOffset;
  ulPrefixLen = min(ROW_LENGTH, ulSrcLen);
  vRowPrint(ucpSrc, ulPrefix, ulPrefixLen, ulIndent + 2, 0);
  printf("\n");
  if(ulSrcLen > ulPrefixLen)
  {
    ulong ulSpace = 0;
    ulPostfixLen = min(ROW_LENGTH, ulSrcLen - ulPrefixLen);
    if(ulPostfixLen < ROW_LENGTH){ulSpace = ROW_LENGTH - ulPostfixLen;}
    ulPostfix = ulOffset + ulSrcLen - ulPostfixLen;
    vRowPrint(ucpSrc, ulPostfix, ulPostfixLen, ulIndent + 2, ulSpace);
    printf("\n");
  }
}

// prints the unmatched source
// ROW_LENGTH (or less) characters printed on one line
static void vNoMatchedPrint(uchar* ucpSrc, ulong ulOffset, ulong ulSrcLen, ulong ulIndent)
{
  ulong ulPrefix, ulPrefixLen;

  DASSERT(ucpSrc);
  ulPrefix = ulOffset;
  ulPrefixLen = min(ROW_LENGTH, ulSrcLen);
  vRowPrint(ucpSrc, ulPrefix, ulPrefixLen, ulIndent + 2, 0);
  printf("\n");
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Prints a human-readable version of each opcode as it is
//              traversed along with the traversal direction and an indication
//              of the input sub-string that it is attempting to match.
//
// FUNCTION: static void vStaticPrint
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulChar
//              : index of the first input string character of the sub-string
//                being matched
//
//      ARG: 3) void* vpOpcode
//              : pointer to the opcode attempting the match
//
//      ARG: 4) ulong ulDirection
//              : traversal direction, TRACE_DOWN or TRACE_UP
//
//      ARG: 5) ulong ulState
//              : the parser state (ACTIVE, MATCH, EMPTY or NOMATCH)
//
//      ARG: 6) ulong ulMatched
//              : the number of characters in the matched sub-string
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
static void vNodeTracePrint(APG_PARSER_CTX* spCtx, ulong ulChar, void* vpOpcode, ulong ulState, ulong ulMatched)
{
  APG_TRACE_CONTROL* spTrace = (APG_TRACE_CONTROL*)spCtx->vpTraceCtx;
  OP_GEN*     spOpcode = (OP_GEN*)vpOpcode;
  uchar*      ucpSrc = spCtx->sSrc.ucpStr;
  ulong       ulSrcLen;
  ulong       ulIndent = ((APG_TRACE_CONTROL*)spCtx->vpTraceCtx)->ulTreeDepth;
  char        caState[64];
  char*       cpState = caState;
  char*       cpOperator;
  void        (*pfnPrint)(uchar* ucpSrc, ulong ulOffset, ulong ulSrcLen, ulong ulIndent);

  pfnPrint = vNoMatchedPrint;
  ulSrcLen = spCtx->sSrc.ulLen - ulChar;
  switch(ulState)
  {
    case ACTIVE:
    strcpy(caState, "PRE-BRANCH");
    break;

    case MATCH:
    strcpy(caState, "MATCH");
    sprintf(caState, "MATCH(%lu, %lu)", ulChar, ulMatched);
    pfnPrint = vMatchedPrint;
    ulSrcLen = ulMatched;
    break;

    case EMPTY:
    strcpy(caState, "EMPTY");
    break;

    case NOMATCH:
    strcpy(caState, "NOMATCH");
    break;

    default:
    strcpy(caState, "UNKNOWN");
    break;
  }

  if(spTrace->ulNodesTraced == 0)
  {
    printf("\n");
    printf("********************\n");
    printf("*** Parser Trace ***\n");
    printf("********************\n");
  }
  // indent the line
  vIndent(ulIndent);

  if(spOpcode->pfnOp == vRNM)
  {
    OP_RNM* spOp = (OP_RNM*)vpOpcode;
    APG_RULES*  spRule = (APG_RULES*)spCtx->vpRules;
    spRule = spRule + spOp->ulRuleIndex;
    cpOperator = "RNM";
    printf("%s[%s][%s]\n", cpOperator, cpState, spRule->cpRuleName);
    pfnPrint(ucpSrc, ulChar, ulSrcLen, ulIndent);
  }
  else if(spOpcode->pfnOp == vTRG)
  {
    OP_TRG* spOp = (OP_TRG*)vpOpcode;
    cpOperator = "TRG";
    printf("%s[%s][%%x%02X-%02X]", cpOperator, cpState, (unsigned int)spOp->ulMin, (unsigned int)spOp->ulMin);
    if((spOp->ulMin >= 32 && spOp->ulMin <= 126) && (spOp->ulMax >= 32 && spOp->ulMax <= 126))
    {printf("[%c-%c]", (char)spOp->ulMin, (char)spOp->ulMax);}
    printf("\n");
    pfnPrint(ucpSrc, ulChar, ulSrcLen, ulIndent);
  }
  else if(spOpcode->pfnOp == vTBS)
  {
    OP_TBS* spOp = (OP_TBS*)vpOpcode;
    cpOperator = "TBS";
    printf("%s[%s][", cpOperator, cpState);
    vPhrasePrint(spOp->ucpString, spOp->ulLength);
    printf("]\n");
    pfnPrint(ucpSrc, ulChar, ulSrcLen, ulIndent);
  }
  else if(spOpcode->pfnOp == vTLS)
  {
    OP_TLS* spOp = (OP_TLS*)vpOpcode;
    cpOperator = "TLS";
    printf("%s[%s][", cpOperator, cpState);
    vPhrasePrint(spOp->ucpString, spOp->ulLength);
    printf("]\n");
    pfnPrint(ucpSrc, ulChar, ulSrcLen, ulIndent);
  }
  else if(spOpcode->pfnOp == vALT)
  {
    cpOperator = "ALT";
    printf("%s[%s]\n", cpOperator, cpState);
  }
  else if(spOpcode->pfnOp == vCAT)
  {
    cpOperator = "CAT";
    printf("%s[%s]\n", cpOperator, cpState);
  }
  else if(spOpcode->pfnOp == vREP)
  {
    OP_REP* spOp = (OP_REP*)vpOpcode;
    cpOperator = "REP";
    printf("%s[%s][%lu, %lu]\n", cpOperator, cpState, spOp->ulMin, spOp->ulMax);
  }
  else if(spOpcode->pfnOp == vPRD)
  {
    cpOperator = "PRD";
    printf("%s[%s]\n", cpOperator, cpState);
  }
  else
  {
    cpOperator = "UNKNOWN";
    printf("%s[%s]\n", cpOperator, cpState);
  }

}

void vNodeTraceCtor(APG_PARSER_CTX* spCtx)
{
  ulong ulSize;
  APG_TRACE_CONTROL* spTrace = NULL;
  TASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(spCtx->vpTraceCtx){vNodeTraceDtor(spCtx);}

  // allocate the trace collection space
  ulSize = sizeof(APG_TRACE_CONTROL) + (2 * sizeof(ulong) * spCtx->ulRuleCount);
  spTrace = (APG_TRACE_CONTROL*)vpMemAlloc(spCtx->vpMemCtx, ulSize);
  TASSERT(spTrace);

  // clear the trace space
  memset((void*)spTrace, 0, ulSize);
  spTrace->ulpRNMIDs = (ulong*)((char*)spTrace + sizeof(APG_TRACE_CONTROL));
  spTrace->ulpRuleNameCounts = spTrace->ulpRNMIDs + spCtx->ulRuleCount;
  spCtx->vpTraceCtx = (void*)spTrace;
APG_EXIT:
  if(!spCtx->vpTraceCtx && spTrace)
  {
    spTrace->vpValidate = NULL;
    vMemFree(spCtx->vpMemCtx, (void*)spTrace);
    spCtx->vpTraceCtx = NULL;
  }
}

void vNodeTraceDtor(APG_PARSER_CTX* spCtx)
{
  if(spCtx && spCtx->vpValidate == (void*)spCtx)
  {
    if(spCtx->vpTraceCtx)
    {
      vMemFree(spCtx->vpMemCtx, spCtx->vpTraceCtx);
      spCtx->vpTraceCtx = NULL;
    }
  }
}

void vNodeTraceClear(APG_PARSER_CTX* spCtx)
{
  APG_TRACE_CONTROL* spTrace = NULL;
  TASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(!spCtx->vpTraceCtx){vNodeTraceCtor(spCtx);}
  TASSERT(spCtx->vpTraceCtx);
  spTrace = (APG_TRACE_CONTROL*)spCtx->vpTraceCtx;

  // clear all data collected during a parse
  // leave all mode data as set by ulParserTraceInit() & ulParserTraceSelectRule() untouched
  memset((void*)spTrace->ulpRuleNameCounts, 0, (sizeof(ulong) * spCtx->ulRuleCount));
  spTrace->ulTreeDepth = 0;
  spTrace->ulNodesTraced = 0;
  spTrace->ulTRGCount = 0;
  spTrace->ulTLSCount = 0;
  spTrace->ulTBSCount = 0;
  spTrace->ulPRDCount = 0;
  spTrace->ulALTCount = 0;
  spTrace->ulCATCount = 0;
  spTrace->ulREPCount = 0;
  spTrace->ulRNMCount = 0;
APG_EXIT:
  return;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// FUNCTION: void vNodeTraceBegin
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulChar
//              :
//
//      ARG: 3) void* vpOpcode
//              :
//
//   RETURN:
//
//////////////////////////////////////////////////////////////////////////
void vNodeTraceBegin(APG_PARSER_CTX* spCtx, ulong ulChar, void* vpOpcode)
{
  APG_TRACE_CONTROL* spTrace = (APG_TRACE_CONTROL*)spCtx->vpTraceCtx;
  if(spTrace && spTrace->ulTrace)
  {
    // print going down the AST
    if(spTrace->ulActive)
    {
      ulong ulPrintOp = APG_FALSE;
      OP_GEN* spOpcode = (OP_GEN*)vpOpcode;

      if(spOpcode->pfnOp == vRNM && spTrace->ulRNM)
      {
        if(spTrace->ulRuleNames)
        {
          OP_RNM* spRnm = (OP_RNM*)spOpcode;
          if(spTrace->ulpRNMIDs[spRnm->ulRuleIndex] && !spTrace->ulHideRules){ulPrintOp = APG_TRUE;}
          if(!spTrace->ulpRNMIDs[spRnm->ulRuleIndex] && spTrace->ulHideRules){ulPrintOp = APG_TRUE;}
        }
        else{ulPrintOp = APG_TRUE;}
      }
      if(spOpcode->pfnOp == vPRD && spTrace->ulPRD){ulPrintOp = APG_TRUE;}
      if(spOpcode->pfnOp == vREP && spTrace->ulREP){ulPrintOp = APG_TRUE;}
      if(spOpcode->pfnOp == vALT && spTrace->ulALT){ulPrintOp = APG_TRUE;}
      if(spOpcode->pfnOp == vCAT && spTrace->ulCAT){ulPrintOp = APG_TRUE;}
      if(spOpcode->pfnOp == vTRG && spTrace->ulTRG){ulPrintOp = APG_TRUE;}
      if(spOpcode->pfnOp == vTBS && spTrace->ulTBS){ulPrintOp = APG_TRUE;}
      if(spOpcode->pfnOp == vTLS && spTrace->ulTLS){ulPrintOp = APG_TRUE;}
      if(ulPrintOp)
      {
        if(spTrace->ulNodesTraced >= spTrace->ulBegin && spTrace->ulNodesTraced <= spTrace->ulEnd)
        {
          if(!spTrace->ulCountOnly)
          {
            vNodeTracePrint(spCtx, ulChar, vpOpcode, ACTIVE, 0);
          }
        }
        ++spTrace->ulNodesTraced;
      }
    }

    // increment the indent
    ++spTrace->ulTreeDepth;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// FUNCTION: void vNodeTraceEnd
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulChar
//              :
//
//      ARG: 3) void* vpOpcode
//              :
//
//      ARG: 4) ulong ulState
//              :
//
//      ARG: 5) ulong ulMatched
//              :
//
//   RETURN:
//
//////////////////////////////////////////////////////////////////////////
void vNodeTraceEnd(APG_PARSER_CTX* spCtx, ulong ulChar, void* vpOpcode, ulong ulState, ulong ulMatched)
{
  APG_TRACE_CONTROL* spTrace = (APG_TRACE_CONTROL*)spCtx->vpTraceCtx;
  if(spTrace && spTrace->ulTrace)
  {
    ulong ulPrintState = APG_FALSE;
    ulong ulPrintOp = APG_FALSE;
    OP_GEN*     spOpcode = (OP_GEN*)vpOpcode;
    // decrement the indent
    --spTrace->ulTreeDepth;

    // do the node visit counting
    if(spOpcode->pfnOp == vRNM)
    {
      OP_RNM* spRnm = (OP_RNM*)spOpcode;
      spTrace->ulRNMCount++;
      spTrace->ulpRuleNameCounts[spRnm->ulRuleIndex]++;
    }
    else if(spOpcode->pfnOp == vPRD){spTrace->ulPRDCount++;}
    else if(spOpcode->pfnOp == vREP){spTrace->ulREPCount++;}
    else if(spOpcode->pfnOp == vALT){spTrace->ulALTCount++;}
    else if(spOpcode->pfnOp == vCAT){spTrace->ulCATCount++;}
    else if(spOpcode->pfnOp == vTRG){spTrace->ulTRGCount++;}
    else if(spOpcode->pfnOp == vTBS){spTrace->ulTBSCount++;}
    else if(spOpcode->pfnOp == vTLS){spTrace->ulTLSCount++;}

    // print going up the AST
    switch(ulState)
    {
      case ACTIVE:
      if(spTrace->ulActive){ulPrintState = APG_TRUE;}
      break;

      case MATCH:
      if(spTrace->ulMatch){ulPrintState = APG_TRUE;}
      break;

      case EMPTY:
      if(spTrace->ulEmpty){ulPrintState = APG_TRUE;}
      break;

      case NOMATCH:
      if(spTrace->ulNoMatch){ulPrintState = APG_TRUE;}
      break;
    }
    if(spOpcode->pfnOp == vRNM && spTrace->ulRNM)
    {
      if(spTrace->ulRuleNames)
      {
        OP_RNM* spRnm = (OP_RNM*)spOpcode;
        if(spTrace->ulpRNMIDs[spRnm->ulRuleIndex] && !spTrace->ulHideRules){ulPrintOp = APG_TRUE;}
        if(!spTrace->ulpRNMIDs[spRnm->ulRuleIndex] && spTrace->ulHideRules){ulPrintOp = APG_TRUE;}
      }
      else{ulPrintOp = APG_TRUE;}
    }
    if(spOpcode->pfnOp == vPRD && spTrace->ulPRD){ulPrintOp = APG_TRUE;}
    if(spOpcode->pfnOp == vREP && spTrace->ulREP){ulPrintOp = APG_TRUE;}
    if(spOpcode->pfnOp == vALT && spTrace->ulALT){ulPrintOp = APG_TRUE;}
    if(spOpcode->pfnOp == vCAT && spTrace->ulCAT){ulPrintOp = APG_TRUE;}
    if(spOpcode->pfnOp == vTRG && spTrace->ulTRG){ulPrintOp = APG_TRUE;}
    if(spOpcode->pfnOp == vTBS && spTrace->ulTBS){ulPrintOp = APG_TRUE;}
    if(spOpcode->pfnOp == vTLS && spTrace->ulTLS){ulPrintOp = APG_TRUE;}
    if(ulPrintState && ulPrintOp)
    {
      // do the printed node counting
      if(spTrace->ulNodesTraced >= spTrace->ulBegin && spTrace->ulNodesTraced <= spTrace->ulEnd)
      {
        if(!spTrace->ulCountOnly)
        {
          vNodeTracePrint(spCtx, ulChar, vpOpcode, ulState, ulMatched);
        }
      }
      ++spTrace->ulNodesTraced;
    }
  }
}
#endif // _APG_CFG_TRACE
#endif // _Nodes_c_31_7_2006_11_13_
