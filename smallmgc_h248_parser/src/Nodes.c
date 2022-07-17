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

   created: 8/25/2007 10:14
  filename: D:\Projects\Apgv5_0\ApgLib\Nodes.c
   purpose: Source code file for APG operator statistics and
            node-tracing functions. Reqires CRT I/O functions.
            Compliled only if _APG_CFG_STATS is defined.
  
*********************************************************************/
#ifndef _Nodes_c_31_7_2006_11_13_
#define _Nodes_c_31_7_2006_11_13_
#include "Apg.h"
#include "Private.h"
#include "Operators.h"
#include "OperatorsPPT.h"

#ifdef _APG_CFG_STATS
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
// NODE TRACING FUNCTIONS
//////////////////////////////////////////////////////////////////////////
#define TRACE_DOWN  (MATCH + 10)
#define TRACE_UP    (MATCH + 11)

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
static void vStaticPrint(APG_PARSER_CTX* spCtx, ulong ulChar,
                     void* vpOpcode, ulong ulDirection, ulong ulState, ulong ulMatched)
{
  OP_GEN*     spOpcode = (OP_GEN*)vpOpcode;
  APG_RULES*  spRuleList = (APG_RULES*)spCtx->vpRules;
  ulong       ulSrcLen;
  uchar*      ucpSrc = &spCtx->sSrc.ucpStr[ulChar];
  ulong       ulIndex;
  OP_RNM*     spRnm;
  
  // indent the line
  for(ulIndex = 0; ulIndex < spCtx->sStats.ulIndent; ++ulIndex)
  {
    //Application::instance().logger().trace("  ");
  }

  if(ulDirection == TRACE_DOWN)
  {
    // print the nodes going down
    // print the OPCODE type
    if(spOpcode->pfnOp == vTRG)
    {
      OP_TRG* spOp = (OP_TRG*)vpOpcode;
      //Application::instance().logger().trace("TRG(down)(");
      //Application::instance().logger().trace("%%x%X - %%x%X)\n", spOp->ulMin, spOp->ulMax);
    }
    else if(spOpcode->pfnOp == vTBS)
    {
      OP_TBS* spOp = (OP_TBS*)vpOpcode;
      //Application::instance().logger().trace("TBS(down)(");
      //Application::instance().logger().trace("%%x%02X", (int)spOp->ucpString[0]);
      for(ulIndex = 1; ulIndex < spOp->ulLength; ++ulIndex)
      {
        //Application::instance().logger().trace(".%02X", (int)spOp->ucpString[ulIndex]);
      }
      //Application::instance().logger().trace(")\n");
    }
    else if(spOpcode->pfnOp == vTLS)
    {
      OP_TLS* spOp = (OP_TLS*)vpOpcode;
      //Application::instance().logger().trace("TLS(down)(");
      //Application::instance().logger().trace("%c", (char)spOp->ucpString[0]);
      for(ulIndex = 1; ulIndex < spOp->ulLength; ++ulIndex)
      {
        //Application::instance().logger().trace("%c", (char)spOp->ucpString[ulIndex]);
      }
      //Application::instance().logger().trace(")\n");
    }
    //else if(spOpcode->pfnOp == vALT)Application::instance().logger().trace("ALT(down)\n");
    //else if(spOpcode->pfnOp == vCAT)Application::instance().logger().trace("CAT(down)\n");
    //else if(spOpcode->pfnOp == vREP)Application::instance().logger().trace("REP(down)\n");
    //else if(spOpcode->pfnOp == vPRD)Application::instance().logger().trace("PRD(down)\n");
    else if(spOpcode->pfnOp == vRNM)
    {
      spRnm = (OP_RNM*)spOpcode;
      //Application::instance().logger().trace("RNM(down)(%s)\n", spRuleList[spRnm->ulRuleIndex].cpRuleName);
    }
    //else Application::instance().logger().trace("UNKNOWN OP(down)\n");
  }
  else
  {
    // print the OPCODE type
    if(spOpcode->pfnOp == vTRG)Application::instance().logger().trace("TRG(up)");
    //else if(spOpcode->pfnOp == vTBS)Application::instance().logger().trace("TBS(up)");
    //else if(spOpcode->pfnOp == vTLS)Application::instance().logger().trace("TLS(up)");
    //else if(spOpcode->pfnOp == vALT)Application::instance().logger().trace("ALT(up)");
    //else if(spOpcode->pfnOp == vCAT)Application::instance().logger().trace("CAT(up)");
    //else if(spOpcode->pfnOp == vREP)Application::instance().logger().trace("REP(up)");
    //else if(spOpcode->pfnOp == vPRD)Application::instance().logger().trace("PRD(up)");
    else if(spOpcode->pfnOp == vRNM)
    {
      spRnm = (OP_RNM*)spOpcode;
      //Application::instance().logger().trace("RNM(up)(%s)", spRuleList[spRnm->ulRuleIndex].cpRuleName);
    }
    //else Application::instance().logger().trace("UNKNOWN OP(up)");
    
    // print going up
    switch(ulState)
    {
    case SYN_PRE:
      //Application::instance().logger().trace("[ACTIVE *error*]\n");
      break;
    case SYN_NOMATCH:
      //Application::instance().logger().trace("[NOMATCH][");
      //ulSrcLen = 10;
      //Application::instance().logger().trace("%%x%02X", ucpSrc[0]);
      //for(ulIndex = 1; ulIndex < ulSrcLen; ++ulIndex)
      //{
      //  Application::instance().logger().trace(".%02X", (int)ucpSrc[ulIndex]);
      //}
      //Application::instance().logger().trace("]\n");
      break;
    case SYN_EMPTY:
      //Application::instance().logger().trace("[EMPTY]\n");
      break;
    case SYN_MATCH:
      {
        //Application::instance().logger().trace("[MATCH(%lu)][", ulMatched);
        ulSrcLen = min(ulMatched, 10);
        //Application::instance().logger().trace("%%x%02X", ucpSrc[0]);
        //for(ulIndex = 1; ulIndex < ulSrcLen; ++ulIndex)
        //{
        //  Application::instance().logger().trace(".%02X", (int)ucpSrc[ulIndex]);
        //}
        //Application::instance().logger().trace("]\n");
      }
      break;
    default:
      //Application::instance().logger().trace("[UNKNOWN *error*]\n");
      break;
    }
  }
  
  fflush(stdout);
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: (PPT opcodes version) Prints a human-readable version of each opcode as it is
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
static void vStaticPrintPPT(APG_PARSER_CTX* spCtx, ulong ulChar,
                         void* vpOpcode, ulong ulDirection, ulong ulState, ulong ulMatched)
{
  OP_PPT_GEN*     spOpcode = (OP_PPT_GEN*)vpOpcode;
  APG_RULES*  spRuleList = (APG_RULES*)spCtx->vpRules;
  ulong       ulSrcLen;
  uchar*      ucpSrc = &spCtx->sSrc.ucpStr[ulChar];
  ulong       ulIndex;
  OP_PPT_RNM*     spRnm;
  
  // indent the line
  for(ulIndex = 0; ulIndex < spCtx->sStats.ulIndent; ++ulIndex)
  {
    //Application::instance().logger().trace("  ");
  }
  
  if(ulDirection == TRACE_DOWN)
  {
    // print the nodes going down
    // print the OPCODE type
    if(spOpcode->pfnOp == vPPT_TRG)
    {
      OP_PPT_TRG* spOp = (OP_PPT_TRG*)vpOpcode;
      //Application::instance().logger().trace("TRG(down)(");
      //Application::instance().logger().trace("%%x%X - %%x%X)\n", spOp->ulMin, spOp->ulMax);
    }
    else if(spOpcode->pfnOp == vPPT_TBS)
    {
      OP_PPT_TBS* spOp = (OP_PPT_TBS*)vpOpcode;
      //Application::instance().logger().trace("TBS(down)(");
      //Application::instance().logger().trace("%%x%02X", (int)spOp->ucpString[0]);
      for(ulIndex = 1; ulIndex < spOp->ulLength; ++ulIndex)
      {
        //Application::instance().logger().trace(".%02X", (int)spOp->ucpString[ulIndex]);
      }
      //Application::instance().logger().trace(")\n");
    }
    else if(spOpcode->pfnOp == vPPT_TLS)
    {
      OP_PPT_TLS* spOp = (OP_PPT_TLS*)vpOpcode;
      //Application::instance().logger().trace("TLS(down)(");
      //Application::instance().logger().trace("%c", (char)spOp->ucpString[0]);
      for(ulIndex = 1; ulIndex < spOp->ulLength; ++ulIndex)
      {
        //Application::instance().logger().trace("%c", (char)spOp->ucpString[ulIndex]);
      }
      //Application::instance().logger().trace(")\n");
    }
    //else if(spOpcode->pfnOp == vPPT_ALT)Application::instance().logger().trace("ALT(down)\n");
    //else if(spOpcode->pfnOp == vPPT_CAT)Application::instance().logger().trace("CAT(down)\n");
    //else if(spOpcode->pfnOp == vPPT_REP)Application::instance().logger().trace("REP(down)\n");
    //else if(spOpcode->pfnOp == vPPT_PRD)Application::instance().logger().trace("PRD(down)\n");
    else if(spOpcode->pfnOp == vPPT_RNM)
    {
      spRnm = (OP_PPT_RNM*)spOpcode;
    //  Application::instance().logger().trace("RNM(down)(%s)\n", spRuleList[spRnm->ulRuleIndex].cpRuleName);
    }
    //else Application::instance().logger().trace("UNKNOWN OP(down)\n");
  }
  else
  {
    // print the OPCODE type
    if(spOpcode->pfnOp == vPPT_TRG)Application::instance().logger().trace("TRG(up)");
    //else if(spOpcode->pfnOp == vPPT_TBS)Application::instance().logger().trace("TBS(up)");
    //else if(spOpcode->pfnOp == vPPT_TLS)Application::instance().logger().trace("TLS(up)");
    //else if(spOpcode->pfnOp == vPPT_ALT)Application::instance().logger().trace("ALT(up)");
    //else if(spOpcode->pfnOp == vPPT_CAT)Application::instance().logger().trace("CAT(up)");
    //else if(spOpcode->pfnOp == vPPT_REP)Application::instance().logger().trace("REP(up)");
    //else if(spOpcode->pfnOp == vPPT_PRD)Application::instance().logger().trace("PRD(up)");
    else if(spOpcode->pfnOp == vPPT_RNM)
    {
      spRnm = (OP_PPT_RNM*)spOpcode;
      //Application::instance().logger().trace("RNM(up)(%s)", spRuleList[spRnm->ulRuleIndex].cpRuleName);
    }
    //else Application::instance().logger().trace("UNKNOWN OP(up)");
    
    // print going up
    switch(ulState)
    {
    case SYN_PRE:
      //Application::instance().logger().trace("[ACTIVE *error*]\n");
      break;
    case SYN_NOMATCH:
      //Application::instance().logger().trace("[NOMATCH(char %lu, matched 0)]\n", ulChar);
      break;
    case SYN_EMPTY:
      //Application::instance().logger().trace("[EMPTY]\n");
      break;
    case SYN_MATCH:
      {
        //Application::instance().logger().trace("[MATCH(char %lu, matched %lu)][", ulChar, ulMatched);
        ulSrcLen = min(ulMatched, 10);
        //Application::instance().logger().trace("%%x%02X", ucpSrc[0]);
        for(ulIndex = 1; ulIndex < ulSrcLen; ++ulIndex)
        {
          //Application::instance().logger().trace(".%02X", (int)ucpSrc[ulIndex]);
        }
        //Application::instance().logger().trace("]\n");
      }
      break;
    default:
      //Application::instance().logger().trace("[UNKNOWN *error*]\n");
      break;
    }
  }
  
  fflush(stdout);
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
void vNodeTracePrint(APG_PARSER_CTX* spCtx, ulong ulChar,
                     void* vpOpcode, ulong ulDirection, ulong ulState, ulong ulMatched)
{
  if(spCtx->ulPPT)
  {
    vStaticPrintPPT(spCtx, ulChar, vpOpcode, ulDirection, ulState, ulMatched);
  }
  else
  {
    vStaticPrint(spCtx, ulChar, vpOpcode, ulDirection, ulState, ulMatched);
  }
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
  if(spCtx->ulTrace)
  {
    // print going down the AST
    vNodeTracePrint(spCtx, ulChar, vpOpcode, TRACE_DOWN, ACTIVE, 0);
    
    // increment the indent
    ++spCtx->sStats.ulIndent;
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
  if(spCtx->ulTrace)
  {
    // decrement the indent
    --spCtx->sStats.ulIndent;
    
    // print going up the AST
    vNodeTracePrint(spCtx, ulChar, vpOpcode, TRACE_UP, ulState, ulMatched);
  }
}

static void vNodeStatsBegin(APG_PARSER_STATS* spStats);
static void vNodeStatsEnd(APG_PARSER_STATS* spStats, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched);
static void vNodeStatsPpt(APG_PARSER_STATS* spStats, ulong ulState);
static void vNodeStatsEof(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulChar, ulong ulState);
static void vNodeStatsChildCount(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched);
static void vNodeStatsBackTrack(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched);

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: void vNodeStats
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context 
//
//      ARG: 2) ulong ulType
//              : 
//
//      ARG: 3) ulong ulID
//              : 
//
//      ARG: 4) ulong ulChar
//              : 
//
//      ARG: 5) ulong ulState
//              : 
//
//      ARG: 6) ulong ulMatched
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
void vNodeStats(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched)
{
  APG_PARSER_STATS* spStats = &spCtx->sStats;

  switch(ulType)
  {
  case NODE_BEGIN:
    vNodeStatsBegin(spStats);
  	break;
  case NODE_END:
    vNodeStatsEnd(spStats, ulID, ulChar, ulState, ulMatched);
  	break;
  case NODE_CHILD_COUNT:
    vNodeStatsChildCount(spStats, ulID, ulState, ulMatched);
      break;
  case NODE_EOF_VISIT:
    vNodeStatsEof(spCtx, NODE_EOF_VISIT, ulChar, ulState);
    break;
  case NODE_EOF_ACCEPT:
    vNodeStatsEof(spCtx, NODE_EOF_ACCEPT, ulChar, ulState);
    break;
  case NODE_PPT:
    vNodeStatsPpt(spStats, ulState);
    break;
  case NODE_BACKTRACK:
    vNodeStatsBackTrack(spStats, ulID, ulState, ulMatched);
    break;
  default:
    DASSERT(APG_FALSE);
      break;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: static void vNodeStatsBackTrack
//
//      ARG: 1) APG_PARSER_STATS* spStats
//              : pointer to a valid parser component context 
//
//      ARG: 2) ulong ulID
//              : 
//
//      ARG: 3) ulong ulState
//              : 
//
//      ARG: 4) ulong ulMatched
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
static void vNodeStatsBackTrack(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched)
{
  switch(ulID)
  {
  case OP_ID_ALT:
  case OP_ID_REP:
    if(ulState == NOMATCH)
    {
      ++spStats->ulBackTrackCount;
      spStats->ulBackTrackChars += ulMatched;
    }
    break;
  case OP_ID_PRD:
    ++spStats->ulBackTrackCount;
    spStats->ulBackTrackChars += ulMatched;
    break;
  default:
    DASSERT(APG_FALSE);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: static void vNodeStatsEof
//
//      ARG: 1) APG_PARSER_CTX* spCtx
//              : pointer to a valid parser component context 
//
//      ARG: 2) ulong ulType
//              : 
//
//      ARG: 3) ulong ulChar
//              : 
//
//      ARG: 4) ulong ulState
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
static void vNodeStatsEof(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulChar, ulong ulState)
{
  APG_PARSER_STATS* spStats = &spCtx->sStats;
  if(ulType == NODE_EOF_VISIT)
  {
    ++spStats->sEof.ulVisits;
  }
  else
  {
    if(ulState == MATCH || ulState == EMPTY)
    {
      ++spStats->sEof.ulMatched;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: static void vNodeStatsPpt
//
//      ARG: 1) APG_PARSER_STATS* spStats
//              : pointer to a statistics struct
//
//      ARG: 2) ulong ulState
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
static void vNodeStatsPpt(APG_PARSER_STATS* spStats, ulong ulState)
{
  switch(ulState)
  {
  case MATCH:
    ++spStats->sPpt.ulMatched;
    ++spStats->sTotal.ulMatched;
    break;
  case EMPTY:
    ++spStats->sPpt.ulEmpty;
    ++spStats->sTotal.ulEmpty;
    break;
  case NOMATCH:
    ++spStats->sPpt.ulNotMatched;
    ++spStats->sTotal.ulNotMatched;
    break;
  case ACTIVE:
    ++spStats->sPpt.ulPossibleMatched;
    ++spStats->sTotal.ulPossibleMatched;
    break;
  default:
    DASSERT(APG_FALSE);
    break;
  }

  ++spStats->sPpt.ulVisits;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: static void vNodeStatsChildCount
//
//      ARG: 1) APG_PARSER_STATS* spStats
//              : pointer to a statistics struct
//
//      ARG: 2) ulong ulID
//              : 
//
//      ARG: 3) ulong ulState
//              : 
//
//      ARG: 4) ulong ulMatched
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
static void vNodeStatsChildCount(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched)
{
  switch(ulID)
  {
  case OP_ID_ALT:
    ++spStats->sAlt.ulChildCount;
    ++spStats->sTotal.ulChildCount;
    break;
  case OP_ID_CAT:
    ++spStats->sCat.ulChildCount;
    ++spStats->sTotal.ulChildCount;
    break;
  case OP_ID_REP:
    ++spStats->sRep.ulChildCount;
    ++spStats->sTotal.ulChildCount;
    break;
  default:
    DASSERT(APG_FALSE);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: static void vNodeStatsBegin
//
//      ARG: 1) APG_PARSER_STATS* spStats
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
static void vNodeStatsBegin(APG_PARSER_STATS* spStats)
{
  // increment the depth indicator
  ++spStats->ulNodesDepth;
  if(spStats->ulNodesDepth > spStats->ulNodesMaxDepth)
  {
    spStats->ulNodesMaxDepth = spStats->ulNodesDepth;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: static void vNodeStatsEnd
//
//      ARG: 1) APG_PARSER_STATS* spStats
//              : pointer to a statistics struct
//
//      ARG: 2) ulong ulID
//              : 
//
//      ARG: 3) ulong ulChar
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
static void vNodeStatsEnd(APG_PARSER_STATS* spStats, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched)
{
  ulong ulMaxChar;
  APG_NODE_STATS* spNode;

  switch(ulID)
  {
  case OP_ID_RNM:
    spNode = &spStats->sRnm;
    break;

  case OP_ID_ALT:
    spNode = &spStats->sAlt;
    break;

  case OP_ID_CAT:
    spNode = &spStats->sCat;
    break;

  case OP_ID_REP:
    spNode = &spStats->sRep;
    break;

  case OP_ID_PRD:
    spNode = &spStats->sPrd;
    break;

  case OP_ID_TRG:
    spNode = &spStats->sTrg;
    break;
      
  case OP_ID_TLS:
    spNode = &spStats->sTls;
    break;

  case OP_ID_TBS:
    spNode = &spStats->sTbs;
    break;

  default:
    DASSERT(APG_FALSE);
    break;
  }

  // update the node stats
  ++spNode->ulVisits;
  ++spStats->sTotal.ulVisits;
  switch(ulState)
  {
  case MATCH:
    ++spNode->ulMatched;
    ++spStats->sTotal.ulMatched;
    break;
  case EMPTY:
    ++spNode->ulEmpty;
    ++spStats->sTotal.ulEmpty;
    break;
  case NOMATCH:
    ++spNode->ulNotMatched;
    ++spStats->sTotal.ulNotMatched;
    break;
  default:
    DASSERT(APG_FALSE);
    break;
  }
  
  // keep track of maximum character matched in the input string
  ulMaxChar = ulChar + ulMatched;
  if(ulMaxChar > spStats->ulCharsMaxMatch){spStats->ulCharsMaxMatch = ulMaxChar;}
  
  // decrement the depth indicator
  --spStats->ulNodesDepth;
}

//////////////////////////////////////////////////////////////////////////
// PACKRAT STATISTICS FUNCTIONS
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: void* vpPackratCtor
//
//      ARG: 1) void* vpParserCtx
//              : pointer to a valid parser component context
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
void* vpPackratCtor(void* vpParserCtx)
{
  void* vpReturn = NULL;
  APG_PACKRAT_CTX* spCtx;
  ulong ulTableSize;
  ulong ulIndex;
  APG_PARSER_CTX* spParserCtx = (APG_PARSER_CTX*)vpParserCtx;
  DASSERT(spParserCtx && spParserCtx->vpValidate == (void*)spParserCtx);

  // clear previous packrat stats, if any
  vPackratDtor(vpParserCtx);
  
  spCtx = (APG_PACKRAT_CTX*)vpMemAlloc(spParserCtx->vpMemCtx, sizeof(APG_PACKRAT_CTX));
  if(!spCtx){ulErrReport(spParserCtx->vpMemCtx, APG_PAR_ID, __LINE__, __FILE__); goto APG_EXIT;}
  memset((void*)spCtx, 0, sizeof(*spCtx));
  
  spCtx->vpMemCtx = spParserCtx->vpMemCtx;
  spCtx->ulRuleCount = spParserCtx->ulRuleCount;
  spCtx->ulCharCount = spParserCtx->sSrc.ulLen + 1;
  ulTableSize = spCtx->ulRuleCount * spCtx->ulCharCount;
  spCtx->spTable = (APG_PACKRAT*)vpMemAlloc(spCtx->vpMemCtx, ulTableSize * sizeof(APG_PACKRAT));
  PASSERT(spCtx->spTable);
  
  for(ulIndex = 0; ulIndex < ulTableSize; ++ulIndex)
  {
    spCtx->spTable[ulIndex].ulCount = 0;
  }
  
  // success
  spParserCtx->vpPackrat = (void*)spCtx;
  spCtx->vpValidate = (void*)spCtx;
  vpReturn = (void*)spCtx;
  
APG_EXIT:
  if(!vpReturn)
  {
    if(spCtx)
    {
      // clean up any allocated resources
      if(spCtx->spTable){vMemFree(spCtx->vpMemCtx, (void*)spCtx->spTable);}
      spParserCtx->vpPackrat = NULL;
      spCtx->vpValidate = NULL;
      vMemFree(spCtx->vpMemCtx, (void*)spCtx);
    }
  }
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: void vPackratDtor
//
//      ARG: 1) void* vpParserCtx
//              : pointer to a valid parser component context
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
void  vPackratDtor(void* vpParserCtx)
{
  APG_PARSER_CTX* spParserCtx = (APG_PARSER_CTX*)vpParserCtx;
  if(spParserCtx && spParserCtx->vpValidate == (void*)spParserCtx)
  {
    APG_PACKRAT_CTX* spCtx = (APG_PACKRAT_CTX*)spParserCtx->vpPackrat;
    if(spCtx && spCtx->vpValidate == (void*)spCtx)
    {
      if(spCtx->spTable){vMemFree(spCtx->vpMemCtx, (void*)spCtx->spTable);}
      spParserCtx->vpPackrat = NULL;
      spCtx->vpValidate = NULL;
      vMemFree(spCtx->vpMemCtx, (void*)spCtx);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: void vPackratCollect
//
//      ARG: 1) void* vpCtx
//              : pointer to a Packrat component context
//
//      ARG: 2) ulong ulRuleIndex
//              : 
//
//      ARG: 3) ulong ulChar
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
void vPackratCollect(void* vpCtx, ulong ulRuleIndex, ulong ulChar, ulong ulState, ulong ulMatched)
{
  APG_PACKRAT_CTX* spCtx = (APG_PACKRAT_CTX*)vpCtx;
  APG_PACKRAT* spPackrat;
  ulong ulIndex;
  if(spCtx && spCtx->vpValidate == vpCtx)
  {
    DASSERT(ulRuleIndex < spCtx->ulRuleCount);
    
    ulIndex = spCtx->ulCharCount * ulRuleIndex + ulChar;
    spPackrat = spCtx->spTable + ulIndex;
    if(spPackrat->ulCount == 0)
    {
      spPackrat->ulState = ulState;
      spPackrat->ulMatched = ulMatched;
    }
    
    ++spPackrat->ulCount;
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// FUNCTION: ulong ulPackratTableCheck
//
//      ARG: 1) void* vpCtx
//              : pointer to a Packrat component context
//
//      ARG: 2) ulong ulRuleIndex
//              : 
//
//      ARG: 3) ulong ulChar
//              : 
//
//      ARG: 4) ulong* ulpState
//              : 
//
//      ARG: 5) ulong* ulpMatched
//              : 
//
//   RETURN: 
//
//////////////////////////////////////////////////////////////////////////
ulong ulPackratTableCheck(void* vpCtx, ulong ulRuleIndex, ulong ulChar, ulong* ulpState, ulong* ulpMatched)
{
  ulong ulReturn = APG_FALSE;
  APG_PACKRAT_CTX* spCtx = (APG_PACKRAT_CTX*)vpCtx;
  APG_PACKRAT* spPackrat;
  ulong ulIndex;
  if(spCtx && spCtx->vpValidate == vpCtx)
  {
    ulIndex = spCtx->ulCharCount * ulRuleIndex + ulChar;
    spPackrat = spCtx->spTable + ulIndex;
    if(spPackrat->ulCount)
    {
      *ulpState = spPackrat->ulState;
      *ulpMatched = spPackrat->ulMatched;
      ulReturn = APG_TRUE;
    }
  }
  
  return ulReturn;
}

#endif // _APG_CFG_STATS

#endif // _Nodes_c_31_7_2006_11_13_
