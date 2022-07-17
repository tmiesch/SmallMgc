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

	 created: Sat Nov 24 14:46:59 2007
	filename: SIP.c
	  author: Lowell Thomas
	          lowell@coasttocoastresearch.com
	          http://www.coasttocoastresearch.com

	 purpose: Implementation of parser component SIP.

*********************************************************************/
#include "Apg.h"
#include "SDP.h"

// default syntax initialization function (no syntax call back functions)
// if spUserBuffer is not NULL it must be at least RULE_COUNT_SIP elements in length
// if spUserBuffer is NULL a static buffer is used
// spSyntax[] must be one of:
// - NULL (0):
//     *NO* call back function called by ulParserSyntaxAnalysis()
// - a valid SYN_CALLBACK function pointer:
//     the call back function *IS* called by ulParserSyntaxAnalysis()
// - anything else:
//     unpredictable behaviour in ulParserSyntaxAnalysis()

// syntax call back switch template

static ulong syn_XXX(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
    }
  }
  return ulRet;
}

static ulong syn_CRLF(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      if(ulLen + 1 < ulLenEnd && ucpSrc[ulLen] == 13 && ucpSrc[ulLen+1] == 10)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = 2;
      }
    }
  }
  return ulRet;
}


static ulong syn_ann(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_MATCH;
    *ulpLen = spData->ulSrcLen - ulOffset;
    if(ulOffset < spData->ulSrcLen)
    {
    }
  }
  //*ulpState = SYN_MATCH;
  //*ulpLen = 0;

  return ulRet;
}

static SYN_CALLBACK g_saSyntax[RULE_COUNT_SDP];
SYN_CALLBACK* sdpSyntaxCallbackInit_ALL(SYN_CALLBACK* spUserBuffer)
{
  SYN_CALLBACK* spSyntax = g_saSyntax;
  if(spUserBuffer != NULL){spSyntax = spUserBuffer;}
  memset((void*)spSyntax, 0, sizeof(SYN_CALLBACK) * RULE_COUNT_SDP);
  ulong ul;

  for(ul = 0; ul < RULE_COUNT_SDP; ul++)
  {
     // spSyntax[ul] = (SYN_CALLBACK)syn_ann;
  }
  //spSyntax[0] = (SYN_CALLBACK)syn_ann;
  //spSyntax[RULE_SDP_CRLF] = (SYN_CALLBACK)syn_CRLF;

  return spSyntax;
}

void vSdpAstInit(ulong* ulpAst)
{
  memset((void*)ulpAst, 0, sizeof(ulong) * RULE_COUNT_SDP);
  ulong ul;
  for(ul = 0; ul < RULE_COUNT_SDP; ul++)
  {
    ulpAst[ul] = APG_TRUE;
  }
}
