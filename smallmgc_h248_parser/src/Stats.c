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
#ifndef _Nodes_c_31_7_2006_11_13_
#define _Nodes_c_31_7_2006_11_13_

#include "Apg.h"
#ifdef _APG_CFG_STATS
#include "Private.h"
#undef SASSERT
#define SASSERT(cond) if(!(int)(cond)) {ulMemErrReport(spCtx->vpMemCtx, APG_STATS_ID, __LINE__, __FILE__); goto APG_EXIT;}

void vNodeStatsCtor(APG_PARSER_CTX* spCtx)
{
  ulong ulSize;
  ulong ulIndex;
  APG_RULES* spRuleList;
  APG_PARSER_STATS* spStats = NULL;

  // validate the parser context
  if(!(spCtx && spCtx->vpValidate == (void*)spCtx)){goto APG_EXIT;}
  if(spCtx->vpStatsCtx){vNodeStatsDtor(spCtx->vpStatsCtx);}

  // allocate the memory
  ulSize = sizeof(APG_PARSER_STATS) + (2 * spCtx->ulRuleCount * sizeof(APG_NODE_STATS));
  spRuleList = (APG_RULES*)spCtx->vpRules;
  for(ulIndex = 0; ulIndex < spCtx->ulRuleCount; ulIndex++)
  {
    ulSize += 1 + (ulong)strlen(spRuleList[ulIndex].cpRuleName);
  }
  spStats = (APG_PARSER_STATS*)vpMemAlloc(spCtx->vpMemCtx, ulSize);
  SASSERT(spStats);
  spStats->spRules = (APG_NODE_STATS*)((char*)spCtx->vpStatsCtx + sizeof(APG_PARSER_STATS));
  spStats->spSortedRules = (APG_NODE_STATS*)((char*)spCtx->vpStatsCtx + sizeof(APG_PARSER_STATS) +
    spCtx->ulRuleCount * sizeof(APG_NODE_STATS));

  // clear the allocated memory
  memset((void*)spStats, 0, ulSize);
  spStats->spRules = (APG_NODE_STATS*)((char*)spStats + sizeof(APG_PARSER_STATS));
  spStats->spSortedRules = (APG_NODE_STATS*)((char*)spStats->spRules + spCtx->ulRuleCount * sizeof(APG_NODE_STATS));

  // success
  spStats->ulRuleCount = spCtx->ulRuleCount;
  spStats->ulSize = ulSize;
  spStats->vpValidate = (void*)spStats;
  spCtx->vpStatsCtx = (void*)spStats;
APG_EXIT:
  if(spStats && (spStats->vpValidate != (void*)spStats))
  {
    spStats->vpValidate = NULL;
    vMemFree(spCtx->vpMemCtx, (void*)spStats);
  }
}

void vNodeStatsClear(APG_PARSER_CTX* spCtx)
{
  ulong ulSize;
  APG_PARSER_STATS* spStats = NULL;

  // validate
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  spStats = (APG_PARSER_STATS*)spCtx->vpStatsCtx;
  if(!(spStats && spStats->vpValidate == (void*)spStats))
  {
    // construct the component
    vNodeStatsDtor(spCtx);
    vNodeStatsCtor(spCtx);
    spStats = (APG_PARSER_STATS*)spCtx->vpStatsCtx;
  }

  // clear for parse
  ulSize = spStats->ulSize;
  memset((void*)spStats, 0, ulSize);
  spStats->spRules = (APG_NODE_STATS*)((char*)spStats + sizeof(APG_PARSER_STATS));
  spStats->spSortedRules = (APG_NODE_STATS*)((char*)spStats->spRules + spCtx->ulRuleCount * sizeof(APG_NODE_STATS));
  spStats->ulCharsTotal = spCtx->sSrc.ulLen;
  spStats->ulRuleCount = spCtx->ulRuleCount;
  spStats->ulSize = ulSize;
  spStats->vpValidate = (void*)spStats;
  return;
}

void vNodeStatsDtor(APG_PARSER_CTX* spCtx)
{
  if(spCtx && spCtx->vpStatsCtx)
  {
    vMemFree(spCtx->vpMemCtx, spCtx->vpStatsCtx);
    spCtx->vpStatsCtx = NULL;
  }
}

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
void vNodeStats(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched, ulong ulRuleIndex)
{
  APG_PARSER_STATS* spStats = (APG_PARSER_STATS*)spCtx->vpStatsCtx;
  DASSERT(spStats && spStats->vpValidate == (void*)spStats);

  switch(ulType)
  {
  case NODE_BEGIN:
    vNodeStatsBegin(spStats);
  	break;
  case NODE_END:
    vNodeStatsEnd(spStats, ulID, ulChar, ulState, ulMatched, ulRuleIndex);
  	break;
  case NODE_CHILD_COUNT:
    vNodeStatsChildCount(spStats, ulID, ulState, ulMatched);
      break;
  case NODE_EOS_VISIT:
    vNodeStatsEos(spCtx, NODE_EOS_VISIT, ulChar, ulState);
    break;
  case NODE_EOS_ACCEPT:
    vNodeStatsEos(spCtx, NODE_EOS_ACCEPT, ulChar, ulState);
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
void vNodeStatsBackTrack(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched)
{
  switch(ulID)
  {
  case OP_ID_ALT:
    if(ulState == NOMATCH)
    {
      ++spStats->sAlt.ulBackTrackCount;
      ++spStats->sTotal.ulBackTrackCount;
    }
   break;
  case OP_ID_REP:
    if(ulState == NOMATCH)
    {
      ++spStats->sRep.ulBackTrackCount;
      ++spStats->sTotal.ulBackTrackCount;
    }
    break;
  case OP_ID_PRD:
    ++spStats->sPrd.ulBackTrackCount;
    ++spStats->sTotal.ulBackTrackCount;
    break;
  default:
    DASSERT(APG_FALSE);
    break;
  }

}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// FUNCTION: static void vNodeStatsEos
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
void vNodeStatsEos(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulChar, ulong ulState)
{
  APG_PARSER_STATS* spStats = (APG_PARSER_STATS*)spCtx->vpStatsCtx;
  if(ulType == NODE_EOS_VISIT)
  {
    ++spStats->sEos.ulVisits;
  }
  else
  {
    if(ulState == MATCH || ulState == EMPTY)
    {
      ++spStats->sEos.ulMatched;
    }
  }
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
void vNodeStatsChildCount(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched)
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
void vNodeStatsBegin(APG_PARSER_STATS* spStats)
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
void vNodeStatsEnd(APG_PARSER_STATS* spStats, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched, ulong ulRuleIndex)
{
  ulong ulMaxChar;
  APG_NODE_STATS* spNode = NULL;
  APG_NODE_STATS* spRules = NULL;

  switch(ulID)
  {
  case OP_ID_RNM:
    spNode = &spStats->sRnm;
    spRules = &spStats->spRules[ulRuleIndex];
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

  if(spNode)
  {
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
  }

  if(spRules)
  {
    // update the node stats
    ++spRules->ulVisits;
    switch(ulState)
    {
    case MATCH:
      ++spRules->ulMatched;
      break;
    case EMPTY:
      ++spRules->ulEmpty;
      break;
    case NOMATCH:
      ++spRules->ulNotMatched;
      break;
    default:
      DASSERT(APG_FALSE);
      break;
    }
  }

  // keep track of maximum character matched in the input string
  ulMaxChar = ulChar + ulMatched;
  if(ulMaxChar > spStats->ulCharsMaxMatch){spStats->ulCharsMaxMatch = ulMaxChar;}

  // decrement the depth indicator
  --spStats->ulNodesDepth;
}
#endif // _APG_CFG_STATS

#ifdef _APG_CFG_PACKRAT
#include "Private.h"
#include "Operators.h"

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
void vPackratCtor(APG_PARSER_CTX* spParserCtx)
{
  void* vpReturn = NULL;
  APG_PACKRAT_CTX* spCtx;
  ulong ulTableSize;
  ulong ulMemSize;
  ulong ulIndex;

  DASSERT(spParserCtx && spParserCtx->vpValidate == (void*)spParserCtx);
  SASSERT(!spParserCtx->vpPackratCtx);
  ulTableSize = spParserCtx->ulRuleCount * (spParserCtx->sSrc.ulLen + 1);
  ulMemSize = sizeof(APG_PACKRAT_CTX) + (ulTableSize * sizeof(APG_PACKRAT));
  spCtx = (APG_PACKRAT_CTX*)vpMemAlloc(spParserCtx->vpMemCtx, ulMemSize);
  SASSERT(spCtx);

  // clear the stats
  memset((void*)spCtx, 0, ulMemSize);
  spCtx->spTable = (APG_PACKRAT*)((char*)spCtx + sizeof(APG_PACKRAT_CTX));
  spCtx->vpMemCtx = spParserCtx->vpMemCtx;
  spCtx->ulRuleCount = spParserCtx->ulRuleCount;
  spCtx->ulCharCount = spParserCtx->sSrc.ulLen + 1;
  for(ulIndex = 0; ulIndex < ulTableSize; ++ulIndex)
  {
    spCtx->spTable[ulIndex].ulCount = 0;
  }

  // success
  spCtx->vpValidate = (void*)spCtx;
  spParserCtx->vpPackratCtx = (void*)spCtx;
  vpReturn = (void*)spCtx;

APG_EXIT:
  if(!vpReturn)
  {
    if(spCtx)
    {
      // clean up any allocated resources
      spCtx->vpValidate = NULL;
      vMemFree(spCtx->vpMemCtx, (void*)spCtx);
      spParserCtx->vpPackratCtx = NULL;
    }
  }
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
void  vPackratDtor(APG_PARSER_CTX* spParserCtx)
{
  DASSERT(spParserCtx && spParserCtx->vpValidate == (void*)spParserCtx);
  if(spParserCtx->vpPackratCtx)
  {
    spParserCtx->vpPackratCtx = NULL;
    vMemFree(spParserCtx->vpMemCtx, spParserCtx->vpPackratCtx);
  }
}

void vPackratClear(APG_PARSER_CTX* spParserCtx)
{
  DASSERT(spParserCtx && spParserCtx->vpValidate == (void*)spParserCtx);
  vPackratDtor(spParserCtx);
  vPackratCtor(spParserCtx);
  return;
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
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(ulRuleIndex < spCtx->ulRuleCount);
  if(ulRuleIndex < spCtx->ulRuleCount)
  {
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
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  ulIndex = spCtx->ulCharCount * ulRuleIndex + ulChar;
  spPackrat = spCtx->spTable + ulIndex;
  if(spPackrat->ulCount)
  {
    *ulpState = spPackrat->ulState;
    *ulpMatched = spPackrat->ulMatched;
    ulReturn = APG_TRUE;
  }

  return ulReturn;
}

#endif // _APG_CFG_PACKRAT
#endif // _Nodes_c_31_7_2006_11_13_
