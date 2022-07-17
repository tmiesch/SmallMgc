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

   created: 8/25/2007 10:17
  filename: D:\Projects\Apgv5_0\ApgLib\Parser.c
   purpose: Source code file for the parser component.

*********************************************************************/
#include "Apg.h"
#include "Private.h"
#include "Operators.h"

//////////////////////////////////////////////////////////////////////////
// PARSER COMPONENT FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: one-time initialization of the parser
//              Must be called once per process before a parser component
//              can be constructed and used.
//              Translates relocatable parser data to process pointers.
//
// FUNCTION: ulong ulParserInit
//
//      ARG: 1) void* vpParserInit
//              : vpXXXInit from a valid APG parser header file XXX.h
//
//      ARG: 2) void* vpMemCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
static ulong ulParserInit(void* vpParserInit, void* vpMemCtx)
{
  ulong             ulReturn = APG_FALSE;
  APG_EXEHDR*       spExe;
  ulong             ulTest;
  ulong             ulHash;
  APG_PARSER_CTX    sCtx;
  APG_PARSER_CTX*   spCtx = &sCtx;
  DASSERT(sizeof(ulong) == sizeof(void*));  // some algorithms and functions assume this
  DASSERT(vpParserInit && vpMemCtx && ulMemValidate(vpMemCtx));
  if(vpParserInit && vpMemCtx && ulMemValidate(vpMemCtx))
  {
    // exit if already initialized
    spExe = (APG_EXEHDR*)vpParserInit;
    if(spExe->ulInitialized == (ulong)spExe)
    {
      ulReturn = APG_TRUE;
      goto APG_EXIT;
    }

    // set up dummy parser context for error handling
    spCtx->vpValidate = (void*)spCtx;
    spCtx->vpMemCtx = vpMemCtx;

    // validate the data
    ulTest = spExe->ulCrc32;
    spExe->ulCrc32 = 0;
    ulHash = ulCRC32((uchar*)spExe, spExe->ulLength);
    PASSERT(ulHash == ulTest);
    spExe->ulCrc32 = ulTest;

    // update opcodes in place - convert indexes to addresses
    ulTest = ulOpcodeInit(spCtx, spExe);
    PASSERT(ulTest);

    // success
    spExe->ulInitialized = (ulong)spExe;
    ulReturn = APG_TRUE;
  }
APG_EXIT:
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: create a new APG parser component
// ulParserInit() must have previously been called in the current process.
// Parser component construction fails otherwise. The reason for failing
// instead of automatically calling ulParserInit() is to prevent the
// possibility of two threads simultaneously trying to construct components.
// ulParserInit() is not thread safe.
//
// FUNCTION: void* vpParserCtor
//
//      ARG: 1) void* vpParserInit
//              : vpXXXInit from a valid APG parser header file XXX.h
//
//      ARG: 2) void* vpMemCtx
//              : pointer to a memory context previously returned
//                from vpMemCtor() or vpMemCtorA()
//
//   RETURN: pointer to a parser component context
//           NULL on failure
//
//////////////////////////////////////////////////////////////////////////
void* vpParserCtor(void* vpParserInit, void* vpMemCtx)
{
  void*             vpReturn = NULL;
  APG_PARSER_CTX*   spCtx = NULL;
  APG_EXEHDR*       spExe;
  ulong             ulMemIndex;
  APG_RULES*        spRuleList;
  DASSERT(vpParserInit);
  if(vpParserInit)
  {
    spExe = (APG_EXEHDR*)vpParserInit;

    // create a memory context for this parser
    if(!vpMemCtx)
    {
      ulMemErrReport(vpMemCtx, APG_PAR_ID, __LINE__, __FILE__);
      goto APG_EXIT;
    }

    if(!spExe)
    {
      ulMemErrReport(vpMemCtx, APG_PAR_ID, __LINE__, __FILE__);
      goto APG_EXIT;
    }

    if(spExe->ulInitialized != (ulong)spExe)
    {
      if(!ulParserInit(vpParserInit, vpMemCtx))
      {
        ulMemErrReport(vpMemCtx, APG_PAR_ID, __LINE__, __FILE__);
        goto APG_EXIT;
      }
    }

    // allocate a context
    ulMemIndex = ulMemCheckPoint(vpMemCtx);
    spCtx = (APG_PARSER_CTX*)vpMemAlloc(vpMemCtx, sizeof(APG_PARSER_CTX));
    if(!spCtx)
    {
      ulMemErrReport(vpMemCtx, APG_PAR_ID, __LINE__, __FILE__);
      goto APG_EXIT;
    }

    // allocate a rule list
    spRuleList = (APG_RULES*)vpMemAlloc(vpMemCtx, sizeof(APG_RULES) * spExe->ulRuleCount);
    if(!spRuleList)
    {
      ulMemErrReport(vpMemCtx, APG_PAR_ID, __LINE__, __FILE__);
      goto APG_EXIT;
    }

    // initialize the context
    memset((void*)spCtx, 0, sizeof(APG_PARSER_CTX));
    spCtx->vpMemCtx = vpMemCtx;
    spCtx->vpValidate = (void*)spCtx;

    // initialize the rule list
    spCtx->vpRules = (void*)spRuleList;
    spCtx->ulRuleCount = spExe->ulRuleCount;
    if(!ulRuleListInit(spCtx, spExe))
    {
      ulMemErrReport(vpMemCtx, APG_PAR_ID, __LINE__, __FILE__);
      goto APG_EXIT;
    }

    // must have a stats struct here if there is one
    APG_NODESTATSCLEAR(spCtx);

    // Code::Blocks workaround (links to Tnm.o without doing anything)
    vTnmDtor(NULL);
    vNodeTraceDummy();

    // success
    vpReturn = (void*)spCtx;
  }
APG_EXIT:
  if(!vpReturn && spCtx)
  {
    memset((void*)spCtx, 0, sizeof(APG_PARSER_CTX));
    vMemFreeToCheckPoint(vpMemCtx, ulMemIndex);
  }
  return vpReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Parser component destructor
//              frees allocated memory
//
// FUNCTION: void vParserDtor
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//   RETURN: void
//           silent return on invalid input
//
//////////////////////////////////////////////////////////////////////////
void vParserDtor(void* vpCtx)
{
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  if(spCtx && spCtx->vpValidate == (void*)spCtx)
  {
    // free all memory resources allocated to this parser component
    void* vpMemCtx = spCtx->vpMemCtx;
    memset((void*)spCtx, 0, sizeof(APG_PARSER_CTX));
    vMemFree(vpMemCtx, (void*)spCtx);
  }
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: ulParserSyntaxInit
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) SYN_CALLBACK* sppSyntax
//              : pointer to a list of syntax call back function pointers
//
//   RETURN: APG_TRUE on success, APG_FALSE otherwise
//
// DESCRIPTION: Associates a syntax call back function pointer
//              (possibly NULL) with each rule name in the grammar.
//              A call to this function is optional,
//              but if used must be called prior to the call to ulParserSyntaxAnalysis().
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserSyntaxInit(void* vpCtx, SYN_CALLBACK* sppSyntax)
{
  ulong ulRet = APG_FALSE;
  ulong ulIndex = 0;
  APG_RULES* spRules;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  PASSERT(spCtx && (spCtx->vpValidate == (void*)spCtx));
  spRules = (APG_RULES*)spCtx->vpRules;
  if(sppSyntax)
  {
    // set or reset the syntax call back functions
    for(; ulIndex < spCtx->ulRuleCount; ++ulIndex)
    {
      spRules[ulIndex].pfnCBSyntax = sppSyntax[ulIndex];
    }
  }
  else
  {
    // clear the syntax call back functions
    for(; ulIndex < spCtx->ulRuleCount; ++ulIndex)
    {
      spRules[ulIndex].pfnCBSyntax = NULL;
    }
  }
  ulRet = APG_TRUE;
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: ulParserSyntaxInit
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) SEM_CALLBACK* sppSemantic
//              : pointer to a list of semantic call back function pointers
//
//   RETURN: APG_TRUE on success, APG_FALSE otherwise
//
// DESCRIPTION: Associates a semantic call back function pointer
//              (possibly NULL) with each rule name in the grammar.
//              A call to this function is optional,
//              but if used must be called prior to the call to ulParserSemanticAnalysis().
//              An AST node must be defined for each rule name with a non-NULL function pointer
//              (see ulParserAstInit) or the defined function will not be called.
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserSemanticInit(void* vpCtx, SEM_CALLBACK* sppSemantic)
{
  ulong ulRet = APG_FALSE;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  PASSERT(spCtx && (spCtx->vpValidate == (void*)spCtx));
  if(spCtx && (spCtx->vpValidate == (void*)spCtx))
  {
    APG_RULES* spRules = (APG_RULES*)spCtx->vpRules;
    ulong ulIndex = 0;
    if(sppSemantic)
    {
      // set/reset the semantic call back functions
      for(; ulIndex < spCtx->ulRuleCount; ++ulIndex)
      {
        spRules[ulIndex].pfnCBSemantic = sppSemantic[ulIndex];
      }
    }
    else
    {
      // clear the semantic call back functions
      for(; ulIndex < spCtx->ulRuleCount; ++ulIndex)
      {
        spRules[ulIndex].pfnCBSemantic = NULL;
      }
    }
    ulRet = APG_TRUE;
  }
APG_EXIT:
  return ulRet;
}

// ulpAst points to a list of true/false values, one for each rule ID
// if ulpAst = NULL, the AST is removed if one already exists
//////////////////////////////////////////////////////////////////////////
// FUNCTION: ulParserAstInit
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong* ulpAst
//              : pointer to a list of true/false values,
//                one for each rule name
//
//   RETURN: APG_TRUE on success, APG_FALSE otherwise
//
// DESCRIPTION: An AST node will be constructed for only those rule name nodes
//              with a "true" value in the input list.
//              A call to this function is optional,
//              but if used must be called prior to the call to ulParserSyntaxAnalysis().
//              If this function is not called prior to ulParseSyntaxAnalysis()
//              or if all values are "false", no AST will be constructed
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserAstInit(void* vpCtx, ulong* ulpAst)
{
  ulong ulRet = APG_FALSE;
  char* cpNames;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  DASSERT(spCtx && (spCtx->vpValidate == (void*)spCtx));
  if(spCtx && (spCtx->vpValidate == (void*)spCtx))
  {
    ulong ulIndex = 0;
    if(spCtx->vpAstCtx)
    {
      // remove the previous AST
      vASTDtor(spCtx->vpAstCtx);
      spCtx->vpAstCtx = NULL;
    }
    if(ulpAst)
    {
      // set/reset the AST node selection
      APG_AST_CTX* spAst = (APG_AST_CTX*)vpASTCtor(spCtx);
      PASSERT(spAst);
      cpNames = spAst->cpNames;
      for(; ulIndex < spCtx->ulRuleCount; ++ulIndex)
      {
        if(ulpAst[ulIndex]){spAst->cppRuleNames[ulIndex] = cpNames;}
        else{spAst->cppRuleNames[ulIndex] = NULL;}
        cpNames += (ulong)strlen(cpNames) + 1;
      }
      spCtx->vpAstCtx = (void*)spAst;
    }
    ulRet = APG_TRUE;
  }
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: ulParserTraceInit
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulMode
//              : a logical OR of selected tracing modes
//                see, for example, APG_TRACE_ALL in Apg.h
//
//      ARG: 3) ulong ulBegin
//              : the first trace line to display
//
//      ARG: 4) ulong ulDelta
//              : if ulDelta > 0, display only lines
//                ulBegin to (ulBegin + ulDelta)
//
//      ARG: 5) ulong ulEnd
//              : if ulDelta = 0, and ulEnd > ulBegin, disply only lines
//                ulBegin to ulEnd
//
//   RETURN: APG_TRUE on success, APG_FALSE otherwise
//
// DESCRIPTION: ApgLib must be built with the _APG_CFG_DEBUG and/or _APG_CFG_TRACE
//              flags defined, or this function will have no effect.
//              This function must be called prior to ulParserSyntaxAnalysis
//              or no trace will be displayed.
//              ulMode controls the nodes and states that are traced.
//              ulBegin, ulDelta and ulEnd control the trace output lines that are displayed.
//              ulBegin = ulDelta = ulEnd = 0 will display ALL lines.
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserTraceInit(void* vpCtx, ulong ulMode, ulong ulBegin, ulong ulDelta, ulong ulEnd)
{
  ulong ulRet = APG_FALSE;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  APG_TRACE_CONTROL* spTraceCtl;
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  while(APG_TRUE)
  {
    // clear the trace
    APG_NODETRACECTOR(spCtx);
    if(!spCtx->vpTraceCtx){break;}

    // check the OFF bit first
    if(ulMode & APG_TRACE_OFF){ulRet = APG_TRUE; break;}

    // validate the mode selection - must have at least one OP bit and one STATE bit
    if(!(ulMode & APG_TRACE_OP_ALL)){break;}
    if(!(ulMode & APG_TRACE_STATE_ALL)){break;}

    // validate the line selection
    if((ulDelta == 0) && (ulEnd > 0) && (ulEnd <= ulBegin)){break;}

    // set the tracing modes
    spTraceCtl = (APG_TRACE_CONTROL*)spCtx->vpTraceCtx;
    spTraceCtl->ulTrace = APG_TRUE;
    if(ulMode & APG_TRACE_COUNT_ONLY){spTraceCtl->ulCountOnly = APG_TRUE;}
    if(ulMode & APG_TRACE_STATE_MATCH){spTraceCtl->ulMatch = APG_TRUE;}
    if(ulMode & APG_TRACE_STATE_EMPTY){spTraceCtl->ulEmpty = APG_TRUE;}
    if(ulMode & APG_TRACE_STATE_NOMATCH){spTraceCtl->ulNoMatch = APG_TRUE;}
    if(ulMode & APG_TRACE_STATE_PRE){spTraceCtl->ulActive = APG_TRUE;}
    if(ulMode & APG_TRACE_RNM){spTraceCtl->ulRNM = APG_TRUE;}
    if(ulMode & APG_TRACE_PRD){spTraceCtl->ulPRD = APG_TRUE;}
    if(ulMode & APG_TRACE_ALT){spTraceCtl->ulALT = APG_TRUE;}
    if(ulMode & APG_TRACE_CAT){spTraceCtl->ulCAT = APG_TRUE;}
    if(ulMode & APG_TRACE_REP){spTraceCtl->ulREP = APG_TRUE;}
    if(ulMode & APG_TRACE_TRG){spTraceCtl->ulTRG = APG_TRUE;}
    if(ulMode & APG_TRACE_TBS){spTraceCtl->ulTBS = APG_TRUE;}
    if(ulMode & APG_TRACE_TLS){spTraceCtl->ulTLS = APG_TRUE;}

    // set tracing limits
    spTraceCtl->ulBegin = ulBegin;
    if(ulDelta > 0)
    {
      spTraceCtl->ulEnd = spTraceCtl->ulBegin + ulDelta - 1;
    }
    else if(ulEnd == 0)
    {
      spTraceCtl->ulEnd = 0xffffffff;
    }
    else
    {
      spTraceCtl->ulEnd = ulEnd - 1;
    }

    // success
    ulRet = APG_TRUE;
    break;
  }
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: ulParserTraceSelectRule
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulRuleID
//              : ID of the individual rule name to display
//
//   RETURN: APG_TRUE on success, APG_FALSE otherwise
//
// DESCRIPTION: ApgLib must be built with the _APG_CFG_DEBUG and/or _APG_CFG_TRACE
//              flags defined, or this function will have no effect.
//              This function must be called after ulParserTraceInit
//              and prior to ulParserSyntaxAnalysis.
//              This function is required to limit the tracing to specific rule name nodes.
//              If not called, ALL rule name nodes will be traced.
//              Call this function multiple times to trace more than one rule name node.
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserTraceSelectRule(void* vpCtx, ulong ulRuleID)
{
  ulong ulRet = APG_FALSE;
  APG_TRACE_CONTROL* spTraceCtl;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  while(APG_TRUE)
  {
    // validate the input
    if(!spCtx->vpTraceCtx){break;}
    if(ulRuleID >= spCtx->ulRuleCount){break;}

    // set the trace bits
    spTraceCtl = (APG_TRACE_CONTROL*)spCtx->vpTraceCtx;
    spTraceCtl->ulRNM = APG_TRUE;
    spTraceCtl->ulRuleNames = APG_TRUE;
    spTraceCtl->ulpRNMIDs[ulRuleID] = APG_TRUE;

    // success
    ulRet = APG_TRUE;
    break;
  }
APG_EXIT:
  return ulRet;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Parses an input string.
//
// FUNCTION: ulong ulParserSyntaxAnalysis
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) ulong ulStartRule
//              : specifies the grammar rule to use as the start rule
// Zero (0) is always the same start rule used when the parser was generated.
// However, any other rule may be used for parsing an input string.
// This may be any identifier RULE_XXX_rulename where XXX.h is the header
// file of the parser and "rulename" is the name of a grammar rule.
//
//      ARG: 3) uchar* ucpSrc
//              : pointer to the first byte of the input string to parse
//
//      ARG: 4) ulong ulSrcLen
//              : number of bytes in the input string
//
//      ARG: 5) void* vpData
//              : pointer to any user-defined data - used only
//                by the call back functions
//
//   RETURN: true on success, false on failure.
//    Failure means a process failure such as "out of memory". Syntax failure
//    is indicated in the parser state, not the return value. (see ulParserState())
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserSyntaxAnalysis(void* vpCtx, ulong ulStartRule, uchar* ucpSrc, ulong ulSrcLen, void* vpData)
{
  ulong           ulReturn = APG_FALSE;
  ulong           ulState = ACTIVE;
  ulong           ulMatched;
//  ulong           ulErrors;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  PASSERT(ucpSrc);
  PASSERT(ulStartRule < spCtx->ulRuleCount);

  // set up for this parse
  spCtx->sSrc.ucpStr = ucpSrc;
  spCtx->sSrc.ulLen = ulSrcLen;
  spCtx->sCBData.ucpSrc = ucpSrc;
  spCtx->sCBData.ulSrcLen = ulSrcLen;
  spCtx->sCBData.ulRuleID = 0;
  spCtx->sCBData.vpParserCtx = vpCtx;
  spCtx->sCBData.vpData = vpData;
  memset((void*)&spCtx->sState, 0, sizeof(spCtx->sState));

  // create configurable structs
  APG_NODESTATSCLEAR(spCtx);
  APG_PACKRATCLEAR(spCtx);
  APG_NODETRACECLEAR(spCtx);
  vASTClear(spCtx->vpAstCtx, &spCtx->sSrc);

  // run the start rule
  // TBD: return a binary result
  spCtx->ulStartRule = ulStartRule;
  spCtx->sCBData.ulRuleID = ulStartRule;
  vRunStartRule(spCtx, ulStartRule, &ulState, &ulMatched);

  // return the parser state
  spCtx->sState.ulStringLength = spCtx->sSrc.ulLen;
  spCtx->sState.ulState = NOMATCH;
  if(ulMatched == spCtx->sSrc.ulLen)
  {
    spCtx->sState.ulState = ulState;
    spCtx->sState.ulCharactersMatched = ulMatched;
    spCtx->sState.ulStringLength = spCtx->sSrc.ulLen;
  }
  else
  {
    spCtx->sState.ulStringLength = spCtx->sSrc.ulLen;
    spCtx->sState.ulState = NOMATCH;
  }

  // success
  ulReturn = APG_TRUE;
APG_EXIT:
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: vExecuteRule
//
//      ARG: 1) APG_CBDATA* spData
//              : pointer to a syntax call back function data struct
//
//      ARG: 2) ulong ulRuleID
//              : ID of the individual rule name to parse
//
//      ARG: 3) ulong* ulpState
//              : pointer to the parser's state
//                on return it is the result of the parse of the tree below
//
//      ARG: 4) ulong ulOffset
//              : offset into the input string of the phrase to parse
//
//      ARG: 5) ulong* ulpLen
//              : pointer to the length of the matched phrase
//                on return it is the actual length of the matched phrase, if any
//
//   RETURN: void
//
// DESCRIPTION: Parses a syntax tree with ulRuleID as the tree root.
//              Can only be called from within a syntax call back function.
//
//////////////////////////////////////////////////////////////////////////
void vExecuteRule(APG_CBDATA* spData, ulong ulRuleID, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)spData->vpParserCtx;
  OP_RNM          sRnm;

  // protect against bad user input
  PASSERT(spData && spData->ucpSrc);
  PASSERT(ulpState);
  PASSERT(ulpLen);
  PASSERT(ulOffset < spData->ulSrcLen);
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  PASSERT(ulRuleID < spCtx->ulRuleCount);

  // create a dummy RNM opcode
  sRnm.pfnOp = vRNM;
  sRnm.ulRuleIndex = ulRuleID;
  sRnm.spNext = NULL; // not used in vRNM()

  // execute the start rule
  vRNM(spCtx, (OP_GEN*)&sRnm, ulOffset, ulpState, ulpLen);
APG_EXIT:
  return;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Semantic analysis of an AST
//              generated by a call to ulParserSyntaxAnalysis().
//
// FUNCTION: ulong ulParserSemanticAnalysis
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) void* vpData
//              : pointer to any user-defined data - used only
//                by the call back functions
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserSemanticAnalysis(void* vpCtx, void* vpData)
{
  ulong           ulReturn = APG_FALSE;
  ulong ulTest;
  ulong ulCallBackReturn;
  ulong ulValue;
  SEM_CALLBACK    pfnCallBack;
  APG_RULES* spRuleList;
  APG_AST_ITERATOR* spIter = NULL;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  spRuleList = (APG_RULES*)spCtx->vpRules;

  spIter = spASTIterCtor(spCtx->vpAstCtx);
  ulTest = spIter ? APG_TRUE: APG_FALSE;
  while(ulTest)
  {
    pfnCallBack = spRuleList[spIter->ulRuleID].pfnCBSemantic;
    if(pfnCallBack)
    {
      // process this node
      if(spIter->ulDown)
      {
        ulValue = (ulong)(spIter->sPhrase.ucpStr - spCtx->sSrc.ucpStr);
        ulCallBackReturn = pfnCallBack(&spCtx->sCBData, SEM_PRE, ulValue, spIter->sPhrase.ulLen);
        if(ulCallBackReturn == PFN_SKIP)
        {
          // skip branch below
          ulTest = ulASTSkipBranch(spIter);
        }
        else
        {
          // reports an error for anything but OK
          PASSERT(ulCallBackReturn == PFN_OK);
        }
      }
      else
      {
        ulValue = (ulong)(spIter->sPhrase.ucpStr - spCtx->sSrc.ucpStr);
        ulCallBackReturn = pfnCallBack(&spCtx->sCBData, SEM_POST, ulValue, spIter->sPhrase.ulLen);
        // ignore return value
      }
    }
    ulTest = ulASTNext(spIter);
  }

  // success
  ulReturn = APG_TRUE;

APG_EXIT:
  if(spIter){vASTIterDtor(spIter);}
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: returns the parser state
//
// FUNCTION: ulong ulParserState
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) APG_PARSER_STATE* spState
//              : pointer to the user's buffer to hold the state information
//
//   RETURN: returns the parser state
//           if spState = NULL, returns size of spState buffer required
//           if spState != NULL, returns size & state in spState
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserState(void* vpCtx, APG_PARSER_STATE* spState)
{
  ulong           ulReturn = 0;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;;
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  if(spState)
  {
    // return the current parser state
    memcpy((void*)spState, (void*)&spCtx->sState, sizeof(APG_PARSER_STATE));
  }
  ulReturn = sizeof(APG_PARSER_STATE);
APG_EXIT:
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: returns parser statistics (if available)
//              _APG_CFG_DEBUG or _APG_CFG_STATS must be defined
//
// FUNCTION: ulong ulParserStats
//
//      ARG: 1) void* vpCtx
//              : pointer to a valid parser component context
//
//      ARG: 2) APG_PARSER_STATS* spStats
//              : pointer to the user's buffer to hold the statistics information
//
//   RETURN: returns parser statistics
//           if spStats = NULL, returns size of spStats buffer required
//           if spStats != NULL, returns size & statistics in spStats
//
//////////////////////////////////////////////////////////////////////////
ulong ulParserStats(void* vpCtx, APG_PARSER_STATS* spStats)
{
  ulong ulReturn = 0;
  ulong ulIndex;
  char* cpNames;
  APG_PARSER_STATS* spParserStats;
  APG_RULES* spRuleList;
  APG_PARSER_CTX* spCtx = (APG_PARSER_CTX*)vpCtx;

  // validate the parser context
  PASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  // check to see if there are any stats for this parse
  spParserStats = (APG_PARSER_STATS*)spCtx->vpStatsCtx;
  if(!spParserStats){goto APG_EXIT;}

  // validate the stats
  PASSERT(spParserStats->vpValidate == (void*)spParserStats);
  if(!spStats)
  {
    // return the size to the user
    ulReturn = spParserStats->ulSize;
    goto APG_EXIT;
  }

  // get a copy of the internal stats
  memcpy((void*)spStats, (void*)spParserStats, sizeof(APG_PARSER_STATS));

  // get a copy of the individual rule name stats
  spStats->spRules = (APG_NODE_STATS*)((char*)spStats + sizeof(APG_PARSER_STATS));
  memcpy((void*)spStats->spRules, (void*)spParserStats->spRules, sizeof(APG_NODE_STATS) * spCtx->ulRuleCount);
  spStats->spSortedRules = (APG_NODE_STATS*)((char*)spStats->spRules + sizeof(APG_NODE_STATS) * spCtx->ulRuleCount);
  cpNames = (char*)spStats->spSortedRules + sizeof(APG_NODE_STATS) * spCtx->ulRuleCount;

  // get a copy of the rule names and set the rule name pointers
  spRuleList = (APG_RULES*)spCtx->vpRules;
  for(ulIndex = 0; ulIndex < spCtx->ulRuleCount; ++ulIndex)
  {
    spStats->spRules[ulIndex] = spParserStats->spRules[ulIndex];
    strcpy(cpNames, spRuleList[ulIndex].cpRuleName);
    spStats->spRules[ulIndex].cpRuleName = cpNames;
    cpNames += 1 + (ulong)strlen(cpNames);
  }

  spStats->vpValidate = (void*)spStats;
  ulReturn = spStats->ulSize;
APG_EXIT:
  return ulReturn;
}

