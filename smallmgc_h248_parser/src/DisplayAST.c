/********************************************************************
  APG - an ABNF Parser Generator
  Copyright  (C) 2010 Coast to Coast Research, Inc.

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

*********************************************************************/
#include "Apg.h"
#include "Private.h"
//#include "Operators.h"
//#include "ApgUtilities.h"
#include <stdio.h>

// static function prototypes
static void vDisplayAscii(uchar* ucpSrc, ulong ulLen);
static void vDisplayHex(uchar* ucpSrc, ulong ulLen);
static void vPhrasePrint(ulong ulIndent, ulong ulLineLen, ulong ulAscii, ulong ulHex, uchar* ucpPhrase, ulong ulLen);

static void vIndent(ulong ulIndent)
{
  while(ulIndent--)
  {
    printf(" ");
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Print a visual display of a AST.
//              Format:
//              <indent>[first character, length] rule name
//              ASCII display 0xHEX display
//
//              <indent> - spaced 2 * the parse tree depth
//              first character - zero-based offset from beginning of string to first character of phrase
//              length - number of characters in the phrase
//              rule name - name of the phrase's rule name
//              ASCII display - the phrase in ASCII (non-printing characters are represented with tilde "~"
//              HEX display - characters are represented by the hexidecimal value of the UTF-8 octet
//
// FUNCTION: void vDisplayParseTree
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vDisplayAST(void* vpCtx, ulong ulLineLen, ulong ulAscii, ulong ulHex)
{
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  APG_AST_CTX* spAst = NULL;
  APG_AST_ITERATOR* spIter = NULL;
  ulong           ulIndent = 0;
  ulong ulTest;

  if(!(spCtx && spCtx->vpValidate == (void*)spCtx)){goto APG_EXIT;}
  spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
  if(!(spAst && spAst->vpValidate == (void*)spAst)){goto APG_EXIT;}

  spIter = spASTIterCtor(spCtx->vpAstCtx);
  if(!spIter){goto APG_EXIT;}
  printf("\n");
  printf("***   ABSTRACT SYNTAX TREE  ***\n");
  printf("*** (depth-first traversal) ***\n");
  ulTest = APG_TRUE;
  while(ulTest)
  {
    if(spIter->ulDown)
    {

      vIndent(2*ulIndent);
      printf("[%lu,%lu]%s\n", (ulong)(spIter->sPhrase.ucpStr - spCtx->sSrc.ucpStr), spIter->sPhrase.ulLen, spIter->cpRuleName);

      // print the phrase
      if(ulAscii || ulHex)
      {
        vPhrasePrint(2*ulIndent, ulLineLen, ulAscii, ulHex, spIter->sPhrase.ucpStr, spIter->sPhrase.ulLen);
      }
      printf("\n");
      ulIndent++;
    }
    else
    {

      ulIndent--;
    }
    ulTest = ulASTNext(spIter);
  }

APG_EXIT:
  if(spIter){vASTIterDtor(spIter);}
  return;
}

struct NAME
{
  char* cpName;
  ulong ulIndex;
};

static int iNameComp(const void* vpLhs, const void* vpRhs)
{
  struct NAME* spLhs = (struct NAME*)vpLhs;
  struct NAME* spRhs = (struct NAME*)vpRhs;
  return apg_stricmp(spLhs->cpName, spRhs->cpName);
}

// display syntax/semantic callback list
void vPrintASTNodes(void* vpParserCtx)
{
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpParserCtx;
  APG_AST_CTX* spAst = NULL;
  struct NAME* spNames = NULL;
  APG_RULES* spRules;
  ulong ulIndex;
  ulong ulCount;
  if(!(spCtx && spCtx->vpValidate == (void*)spCtx)){goto APG_EXIT;}


  printf("\n");
  printf("*** AST node names ***\n");
  spAst = (APG_AST_CTX*)spCtx->vpAstCtx;
  if(!(spAst && spAst->vpValidate == (void*)spAst))
  {
    printf("<none>\n");
    goto APG_EXIT;
  }

  spNames = (struct NAME*)vpMemAlloc(spCtx->vpMemCtx, sizeof(struct NAME) * spCtx->ulRuleCount);
  if(!spNames){goto APG_EXIT;}
  spRules = (APG_RULES*)spCtx->vpRules;
  ulCount = 0;
  for(ulIndex = 0; ulIndex < spCtx->ulRuleCount; ++ulIndex)
  {
    if(spAst->cppRuleNames[ulIndex])
    {
      spNames[ulCount].cpName = spRules[ulIndex].cpRuleName;
      spNames[ulCount].ulIndex = ulIndex;
      ulCount++;
    }
  }
  if(ulCount == 0)
  {
    printf("<none>\n");
    goto APG_EXIT;
  }

  qsort((void*)spNames, (size_t)ulCount, sizeof(struct NAME), iNameComp);
  for(ulIndex = 0; ulIndex < ulCount; ++ulIndex)
  {
    printf("%03lu : %s\n", spNames[ulIndex].ulIndex, spNames[ulIndex].cpName);
  }

APG_EXIT:
  if(spCtx && spNames){vMemFree(spCtx->vpMemCtx, (void*)spNames);}
  return;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: static utility functions
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void vDisplayAscii(uchar* ucpSrc, ulong ulLen)
{
  ulong i;

  if(ucpSrc && ulLen)
  {
    for(i = 0; i < ulLen; ++i)
    {
      uchar ucChar;
      if(ucpSrc[i] >= 32 && ucpSrc[i] <= 126){ucChar = ucpSrc[i];}
      else {ucChar = '~';}
      printf("%c", ucChar);
    }
  }
  else
  {
    printf("<empty>");
  }
}

static void vDisplayHex(uchar* ucpSrc, ulong ulLen)
{
  ulong i;

  if(ucpSrc && ulLen)
  {
    printf("0x%02X", ucpSrc[0]);
    for(i = 1; i < ulLen; ++i)
    {
      if( i % 2 == 0){printf(" ");}
      printf("%02X", ucpSrc[i]);
    }
  }
  else
  {
    printf("<empty>");
  }
}

// displays the phrase in ASCII followed by a duplicate in HEX
static void vPhrasePrint(ulong ulIndent, ulong ulLineLen, ulong ulAscii, ulong ulHex, uchar* ucpPhrase, ulong ulLen)
{
  ulong ulChars;
  ulong ulCharsLeft;
  ulong ulSpace;
  ulong ulLines = 0;
  ulCharsLeft = ulLen;
  DASSERT(ulLineLen);
  while(ulCharsLeft)
  {
    ulChars = min(ulCharsLeft, ulLineLen);
    ulCharsLeft -= ulChars;
    vIndent(ulIndent);
    if(ulAscii)
    {
      vDisplayAscii(ucpPhrase, ulChars);
      ulSpace = 1;
      if(ulChars < ulLineLen && ulLines > 0){ulSpace += ulLineLen - ulChars;}
      vIndent(ulSpace);
      ++ulLines;
    }
    if(ulHex)
    {
      vDisplayHex(ucpPhrase, ulChars);
    }
    printf("\n");

    ucpPhrase += ulChars;
  }
}
