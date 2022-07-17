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

   created: 07/02/2009
   APG 6.1: Output JavaScript generated parsers
   purpose: The one and only public include file required for
            APG static library and generated parser use.

*********************************************************************/
#ifndef _Apg_h_15_6_2006_9_36_
#define _Apg_h_15_6_2006_9_36_

#include <stdlib.h>
#include <string.h>

// Begin C-language linkage specification
#if defined(__cplusplus)
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////
// CONFIGURATION CONTROL
// Control of debugging, statistics and tracing information is designed
// through macros so that when not needed, they generate no code.
//
// Debug information
// _APG_CFG_DEBUG - if defined
//                - defines debug assertion, DASSERT
//                - automatically defines _APG_CFG_STATS
//                - automatically defines _APG_CFG_TRACE
//
// statistics gathering
// _APG_CFG_STATS - if defined, turns on statistics collection (except Packrat)
// _APG_CFG_PACKRAT - if defined, turns on parser Packrat statistics collection
//////////////////////////////////////////////////////////////////////////

// Configurable default values
#undef APG_ERR_DEFAULT
#undef APG_ERROR_BUFLEN
#undef APG_VEC_ELEMENT
#undef APG_VEC_ALLOC
#define APG_ERR_DEFAULT   100   // default low-level error message buffer size
#define APG_ERROR_BUFLEN  256   // high-level error reporting, max text message length
#define APG_VEC_ELEMENT   1     // default vector element size (bytes)
#define APG_VEC_ALLOC     100   // default initial number of vector elements allocated

//////////////////////////////////////////////////////////////////////////
// APG ERROR IDENTIFIERS
// APG-defined error IDs are all negative
// User-defined error IDs should all be positive to avoid conflict
//////////////////////////////////////////////////////////////////////////
#undef APG_DBG_ID
#undef APG_MEM_ID
#undef APG_VEC_ID
#undef APG_PAR_ID
#undef APG_CAT_ID
#undef APG_GEN_ID
#define APG_DBG_ID    (-1)  // debug assertion error
#define APG_MEM_ID    (-2)  // memory component procedure error
#define APG_VEC_ID    (-3)  // vector component procedure error
#define APG_PAR_ID    (-4)  // Parser component procedure error
#define APG_TNM_ID    (-5)  // Tree Node Model component procedure error
#define APG_GEN_ID    (-6)  // generator (apg or Apg.exe) procedure error
#define APG_STATS_ID  (-7)  // statistics component procedure error
#define APG_TRACE_ID  (-8)  // trace component procedure error
#define APG_AST_ID    (-9)  // AST component procedure error

//////////////////////////////////////////////////////////////////////////
// DEBUG, ASSERT, TRACING
//////////////////////////////////////////////////////////////////////////
// always defined
// user assert - allows user to define the identifier and  error(memory) component location
//               calling function must have an APG_EXIT label defined
#undef  UASSERT
#define UASSERT(cond, err, id) if(!(int)(cond)) {ulMemErrReport((err), (id), __LINE__, __FILE__); goto APG_EXIT;}

// calls the alert handler with an alert identifier
#undef  ALERTASSERT
#define ALERTASSERT(cond, id) if(!(int)(cond)) {vAlertHandler((id), __LINE__, __FILE__);}

// these macros generate no code unless otherwise defined
#define DASSERT(cond)

#if defined(_APG_CFG_DEBUG)
  // debug assertion
  #undef  DASSERT
  #define DASSERT(cond) if(!(int)(cond)) vAlertHandler(APG_DBG_ID, __LINE__, __FILE__)

  // debug also defines tracing
  #undef  _APG_CFG_TRACE
  #define _APG_CFG_TRACE

  // debug also defines statistics gathering
  #undef  _APG_CFG_STATS
  #define _APG_CFG_STATS
#endif

#if defined(_APG_CFG_TRACE)
  // tracing macros (requires run-time set up in addition to _APG_CFG_TRACE)
  #undef  APG_NODETRACEBEG
  #undef  APG_NODETRACEEND
  #undef  APG_NODETRACECTOR
  #undef  APG_NODETRACECLEAR
  #define APG_NODETRACECTOR(ctx) vNodeTraceCtor((ctx));
  #define APG_NODETRACECLEAR(ctx) vNodeTraceClear((ctx));
  #define APG_NODETRACEBEG(d,c,o) vNodeTraceBegin((d), (c), (o))
  #define APG_NODETRACEEND(d,c,o,s,m) vNodeTraceEnd((d), (c), (o), (s), (m))
#else
  #define APG_NODETRACECTOR(ctx)
  #define APG_NODETRACECLEAR(ctx)
  #define APG_NODETRACEBEG(d,c,o)
  #define APG_NODETRACEEND(d,c,o,s,m)
#endif

#if defined(_APG_CFG_PACKRAT)
  #undef  APG_PACKRATCLEAR
  #undef  APG_PACKRATSTATS
  #define APG_PACKRATCLEAR(ctx) vPackratClear((ctx))
  #define APG_PACKRATSTATS(ctx, rule, char, state, match) vPackratCollect((ctx), (rule), (char), (state), (match))

  // packrat stats also defines statistics gathering
  #undef  _APG_CFG_STATS
  #define _APG_CFG_STATS
#else
  #define APG_PACKRATCLEAR(ctx)
  #define APG_PACKRATSTATS(ctx, rule, char, state, match)
#endif

#if defined(_APG_CFG_STATS)
  // statistics gathering
  #undef  APG_NODESTATS
  #undef  APG_NODESTATSEOS
  #undef  APG_NODESTATS_BACK
  #define APG_NODESTATSCLEAR(ctx) vNodeStatsClear((ctx));
  #define APG_NODESTATS(ctx, func, type, ch, state, match, index) vNodeStats((ctx), (func), (type), (ch), (state), (match), (index))
  #define APG_NODESTATSEOS(x, c, state) vNodeStatsEos((x), (c), (state));
  #define APG_NODESTATS_BACK(ctx, i, s, m) vNodeStatsBackTrack((ctx), (i), (s), (m))

  // node statistics function identifiers
  #undef  OP_ID_EOS
  #undef  NODE_BEGIN
  #undef  NODE_END
  #undef  NODE_CHILD_COUNT
  #undef  NODE_EOS_VISIT
  #undef  NODE_EOS_ACCEPT
  #undef  NODE_BACKTRACK_BEG
  #undef  NODE_BACKTRACK_END
  #define OP_ID_EOS           (OP_ID_MAX+2)
  #define NODE_BEGIN          (OP_ID_MAX+3)
  #define NODE_END            (OP_ID_MAX+4)
  #define NODE_CHILD_COUNT    (OP_ID_MAX+5)
  #define NODE_EOS_VISIT      (OP_ID_MAX+6)
  #define NODE_EOS_ACCEPT     (OP_ID_MAX+7)
  #define NODE_BACKTRACK_BEG  (OP_ID_MAX+9)
  #define NODE_BACKTRACK_END  (OP_ID_MAX+10)
#else
  #define APG_NODESTATSCLEAR(ctx)
  #define APG_NODESTATS(ctx, func, type, ch, state, match, index)
  #define APG_NODESTATSEOS(x, c, state)
  #define APG_NODESTATS_BACK(ctx, i, s, m)
#endif

//////////////////////////////////////////////////////////////////////////
// BASIC TYPES
//////////////////////////////////////////////////////////////////////////
typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned long   ulong;

// non-ANSI minimum/maximum macros
#undef min
#undef max
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

// ulong true/false values
#undef  APG_TRUE
#undef  APG_FALSE
#define APG_TRUE   1UL
#define APG_FALSE  0UL

// often used to represent an undefined or infinite integer, etc.
#undef  UNDEFINED
#define UNDEFINED 0xFFFFFFFF

// binary string pointer (fixed - pointer to a fixed address)
typedef struct
{
  uchar*  ucpStr;
  ulong   ulLen;
} APG_BSTR;

// binary string location (relocatable - offset from an unspecified address)
typedef struct
{
  ulong ulOffset;
  ulong ulLen;
} APG_BLOC;

//////////////////////////////////////////////////////////////////////////
// TOOLS
//////////////////////////////////////////////////////////////////////////

// CYCLIC REDUNDANCY CHECK
ulong ulCRC32(uchar *buffer, ulong length);
ulong ulcrc32(uchar *buffer, ulong length);
ulong ulChainedCRC32(ulong ulPrevValue, uchar *buffer, ulong length);
ulong ulChainedCRC32Init();

// stricmp is non-ANSI - not all CRTs support it
int apg_stricmp(const char* cpLhs, const char* cpRhs);

//////////////////////////////////////////////////////////////////////////
// ALERTS
// Reports location of terminal errors.
// - can be customized with pfnSetAlertHandler(), recommended
// - default alert handler is exit(0)
//////////////////////////////////////////////////////////////////////////
// NOTE: for C structured exception handling (access violations, division by zero, etc.)
//       add (OS-dependent) exception handlers that call vAlertHandler()
//       Highly recommended!
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ALERT HANDLER
//////////////////////////////////////////////////////////////////////////
typedef void (*PFN_ALERT)(int iID, unsigned int uiLine, char* cpFile);
void vAlertHandler(int iID, unsigned int uiLine, char* cpFile);
PFN_ALERT pfnSetAlertHandler(PFN_ALERT pfnNewAlertHandler);

//////////////////////////////////////////////////////////////////////////
// MEMORY MANAGEMENT COMPONENT
// provides garbage collection
// includes low-level error history recording
// records source file and line number of APG system errors
//   without need of memory allocation - ok to report memory errors
//////////////////////////////////////////////////////////////////////////
typedef struct
{
  ulong ulAllocations;
  ulong ulReAllocations;
  ulong ulFrees;
  ulong ulCells;
  ulong ulMaxCells;
  ulong ulHeapBytes;
  ulong ulMaxHeapBytes;
} APG_MEM_STATS;

// basic unit of error-reporting information
typedef struct
{
  ulong ulID;   // error ID, APG_MEM_ID, etc. or user defined
  ulong ulLine; // line number where error occurred
  char* cpFile; // source code file name where error occurred
} APG_MEM_ERR;

typedef void* (*PFN_ALLOCATOR)(size_t);
typedef void  (*PFN_DEALLOCATOR)(void*);
void*   vpMemCtorA(PFN_ALLOCATOR pfnAllocator, PFN_DEALLOCATOR pfnDeAllocator);
void*   vpMemCtor();
void    vMemDtor(void* vpCtx);
ulong   ulMemValidate(void* vpCtx);
void*   vpMemAlloc(void* vpCtx, ulong ulBytes);
void*   vpMemRealloc(void* vpCtx, void* vpData, ulong ulBytes);
void    vMemFree(void* vpCtx, void* vpData);
ulong   ulMemCheckPoint(void* vpCtx);
void    vMemFreeToCheckPoint(void* vpCtx, ulong ulChk);
void    vMemClear(void* vpCtx); // same as vMemFreeToCheckPoint(vpCtx, 0);
void    vMemErrClear(void* vpCtx);
ulong   ulMemErrReport(void* vpCtx, ulong ulID, ulong ulLine, char* cpLine);
ulong   ulMemErrMsg(void* vpCtx, ulong ulIndex, ulong* ulpID, ulong* ulpLine, char** cpFile);
ulong   ulMemErrCount(void* vpCtx);
ulong   ulMemErrTotal(void* vpCtx);
ulong   ulMemStats(void* vpCtx, APG_MEM_STATS* spStats);
ulong   ulMemErrs(void* vpCtx, APG_MEM_ERR* spErrs);

//////////////////////////////////////////////////////////////////////////
// VECTOR
// a dynamic buffer for an arbitrary number of fixed-size elements
//////////////////////////////////////////////////////////////////////////
void* vpVecCtor(void* vpMemCtx, ulong ulElementSize, ulong ulInitialAlloc);
void  vVecDtor(void* vpCtx);
void* vpVecPush(void* vpVec, void* vpElement);
void* vpVecPushn(void* vpCtx, void* vpElement, ulong ulCount);
void* vpVecPop(void* vpCtx);
void* vpVecPopn(void* vpCtx, ulong ulCount);
void* vpVecPopTo(void* vpCtx, ulong ulIndex);
void* vpVecFront(void* vpCtx);
void* vpVecBack(void* vpCtx);
void* vpVecAt(void* vpCtx, ulong ulIndex);
ulong ulVecBytes(void* vpCtx);
ulong ulVecSize(void* vpCtx);
ulong ulVecElementSize(void* vpCtx);
ulong ulVecReallocations(void* vpCtx);
void  vVecClear(void* vpCtx);
ulong ulVecExport(void* vpCtx, void* vpBuffer);
void* vpVecImport(void* vpMemCtx, void* vpData);

// might need this later
//ulong ulVecPopFromTo(void* vpCtx, ulong ulFromIndex, ulong ulToIndex);

//////////////////////////////////////////////////////////////////////////
// ERROR REPORTING FACILITY
//////////////////////////////////////////////////////////////////////////
void* vpErrorsCtor(void* vpMemCtx);
void  vErrorsDtor(void* vpErrorsCtx);
ulong ulErrorsReport(void* vpErrorsCtx, char* cpMsg);
ulong ulErrorsCount(void* vpErrorsCtx);
void  vErrorsClear(void* vpErrorsCtx);
void  vErrorsInit(void* vpErrorsCtx);
ulong ulErrorsNext(void* vpErrorsCtx, char* cpErrMsg);

//////////////////////////////////////////////////////////////////////////
// PARSER
//////////////////////////////////////////////////////////////////////////
// structure passed to all syntax and semantic call back functions
typedef struct
{
  uchar*  ucpSrc;       // pointer to the input string
  ulong   ulSrcLen;     // length, bytes, of the input string
  ulong   ulRuleID;     // ID (defined in generated *.h file) of the rule name
  void*   vpParserCtx;  // handle to the parser component
  void*   vpData;       // user-defined data passed to ulParserSyntaxAnalysis
} APG_CBDATA;

// syntax call back function prototype
// APG_CBDATA* vpData - access to input string and optional user-defined data
// ulong* ulpState    - pointer to the the syntax state
// ulong ulOffset     - offset to the first character of the input string being matched
// ulong* ulpLen      - pointer to the length of the matched sub-string
// return value       - ignored
typedef ulong (*SYN_CALLBACK)(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen);

// execute a rule (ulRuleID) from the syntax call back function
void vExecuteRule(APG_CBDATA* spData, ulong ulRuleID, ulong* ulpState, ulong ulOffset, ulong* ulpLen);

// semantic call back function prototype
// APG_CBDATA* vpData - access to input string and optional user-defined data
// ulong ulState    - the syntax or semantic analysis state
// ulong ulOffset   - offset to the first character of the input string being matched
// ulong ulLen      - length of the matched sub-string
// return value     - default is PFN_OK
//                    PFN_SKIP from pre-branch this will skip the walk of the branch below
//                    PFN_ERROR report a system error
typedef ulong (*SEM_CALLBACK)(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen);

// call back function states
// syntax analysis
#define SYN_NOMATCH 0 // post-branch, string was not matched by grammar syntax
#define SYN_EMPTY   1 // post-branch, syntax was matched by an empty string
#define SYN_MATCH   2 // post-branch, syntax was matched with a string of one or more characters
#define SYN_PRE     3 // pre-branch, set up before matching process begins

// semantic analysis
#define SEM_PRE     4 // pre-branch, set up before branch below current node is traversed
#define SEM_POST    5 // post-branch, return after branch below this node has been traversed

// semantic call back function return values
#define PFN_OK      0 // normal return
#define PFN_SKIP    1 // return from SEM_PRE - skip analysis of the branch below
#define PFN_ERROR   2 // value to use when user needs to report an error

// operator states
#define NOMATCH SYN_NOMATCH // pattern match failed - 0 characters matched
#define EMPTY   SYN_EMPTY   // pattern match succeeded - 0 characters matched
#define MATCH   SYN_MATCH   // pattern match succeeded - 1 or more characters accepted
#define ACTIVE  SYN_PRE     // pre-branch, before pattern matching begins

// parser final state
typedef struct
{
  ulong ulState;
  ulong ulCharactersMatched;
  ulong ulStringLength;
} APG_PARSER_STATE;

// parsing statistics
typedef struct
{
  ulong ulVisits;           // number of node-type visits
  ulong ulNotMatched;       // number of times node-type not matched
  ulong ulEmpty;            // number of times node-type matched empty
  ulong ulMatched;          // number of times node-type matched
  ulong ulCharsMatched;     // number of characters matched at this node-type
  ulong ulChildCount;       // number of ALT, CAT or REP children tested
  ulong ulBackTrackCount;   // counts the number of times back tracking occurred
  char* cpRuleName;         // pointer to the rule name if RNM node, else NULL
} APG_NODE_STATS;

// NOTE: ulCharsMaxMatch is approximate:
//   - it fails to be accurate in some cases
//   - this will give an idea of where the syntax failed on a NOMATCH
typedef struct
{
  ulong ulRuleCount;        // number of rule names in the grammar
  ulong ulSize;             // size, bytes, required to hold parser statistics
  APG_NODE_STATS* spSortedRules; // pointer to the list of sorted individual rule stats
  ulong ulNodesDepth;       // maximum parse tree depth
  ulong ulNodesMaxDepth;    // maximum stack depth reached at any time
  ulong ulCharsTotal;       // input string length
  ulong ulCharsMaxMatch;    // maximum number of characters matched at any time
  APG_NODE_STATS sRnm;      // node stats for RNM
  APG_NODE_STATS sAlt;      // node stats for ALT
  APG_NODE_STATS sCat;      // node stats for CAT
  APG_NODE_STATS sRep;      // node stats for REP
  APG_NODE_STATS sPrd;      // node stats for PRD
  APG_NODE_STATS sTrg;      // node stats for TRG
  APG_NODE_STATS sTls;      // node stats for TLS
  APG_NODE_STATS sTbs;      // node stats for TBS
  APG_NODE_STATS sEos;      // node stats for End Of String
  APG_NODE_STATS sTotal;    // sum of all node stats
  APG_NODE_STATS* spRules;  // pointer to the list of individual rule name stats
  void* vpValidate;         // magic number used to validate the struct
} APG_PARSER_STATS;

// trace modes - primary flags
#define APG_TRACE_OFF           0x00000001  // no tracing is done at all

// state selections
#define APG_TRACE_STATE_PRE     0x00000002  // trace the pre-branch node visit (going down the branch)
#define APG_TRACE_STATE_MATCH   0x00000004  // trace state = MATCH (going up the branch)
#define APG_TRACE_STATE_EMPTY   0x00000008  // trace state = EMPTY
#define APG_TRACE_STATE_NOMATCH 0x00000010  // trace state = NOMATCH

// terminal operator type selections
#define APG_TRACE_TRG           0x00000020  // trace TRG operators
#define APG_TRACE_TBS           0x00000040  // trace TBS operators
#define APG_TRACE_TLS           0x00000080  // trace TLS operators

// non-terminal operator type selections
#define APG_TRACE_PRD           0x00000100  // trace PRD operators
#define APG_TRACE_CAT           0x00000200  // trace CAT operators
#define APG_TRACE_ALT           0x00000400  // trace ALT operators
#define APG_TRACE_REP           0x00000800  // trace REP operators
#define APG_TRACE_RNM           0x00001000  // trace RNM operators
#define APG_TRACE_RNM_NAMES     0x00002000  // trace only the RNMs specified in ulpRuleNameIDs
#define APG_TRACE_RNM_HIDE      0x00004000  // if on, inverts the ulpRuleNameIDs list
                                            // the RNMs specified in ulpRuleNameIDs will be hidden, all others displayed
#define APG_TRACE_COUNT_ONLY    0x00008000  // counts the number of node visits that would be printed but printing is suppressed

// trace modes - composite flags
#define APG_TRACE_STATE_ALL       (APG_TRACE_STATE_MATCH | APG_TRACE_STATE_EMPTY | APG_TRACE_STATE_NOMATCH | APG_TRACE_STATE_PRE)
#define APG_TRACE_STATE_POST      (APG_TRACE_STATE_MATCH | APG_TRACE_STATE_EMPTY | APG_TRACE_STATE_NOMATCH)
#define APG_TRACE_STATE_SUCCESS   (APG_TRACE_STATE_MATCH | APG_TRACE_STATE_EMPTY)
#define APG_TRACE_STATE_FAILURE   (APG_TRACE_STATE_NOMATCH)
#define APG_TRACE_OP_TERMINALS    (APG_TRACE_TRG | APG_TRACE_TBS | APG_TRACE_TLS)
#define APG_TRACE_OP_NONTERMINALS (APG_TRACE_RNM | APG_TRACE_PRD | APG_TRACE_CAT | APG_TRACE_ALT | APG_TRACE_REP)
#define APG_TRACE_OP_ALL          (APG_TRACE_OP_TERMINALS | APG_TRACE_OP_NONTERMINALS)
#define APG_TRACE_ALL             (APG_TRACE_OP_ALL | APG_TRACE_STATE_ALL)

// parser member functions
void* vpParserCtor(void* vpParserInit, void* vpMemCtx);
void  vParserDtor(void* vpCtx);
ulong ulParserSyntaxInit(void* vpCtx, SYN_CALLBACK* sppSyntax);
ulong ulParserSemanticInit(void* vpCtx, SEM_CALLBACK* sppSemantic);
ulong ulParserAstInit(void* vpCtx, ulong* ulpAst);
ulong ulParserTraceInit(void* vpCtx, ulong ulMode, ulong ulBegin, ulong ulDelta, ulong ulEnd);
ulong ulParserTraceSelectRule(void* vpCtx, ulong ulRuleID);
ulong ulParserSyntaxAnalysis(void* vpCtx, ulong ulStartRule, uchar* ucpSrc, ulong ulSrcLen, void* vpData);
ulong ulParserSemanticAnalysis(void* vpCtx, void* vpData);
ulong ulParserState(void* vpCtx, APG_PARSER_STATE* spState);
ulong ulParserStats(void* vpCtx, APG_PARSER_STATS* spStats);

//////////////////////////////////////////////////////////////////////////
// AST COMPONENT (Abstract Syntax Tree)
// supplies iterators for depth-first traversal of the AST
//////////////////////////////////////////////////////////////////////////
typedef struct
{
  ulong ulDown;     // true if node direction is down, false otherwise
  ulong ulUp;       // redundant direction indicator - true if node direction is up
  ulong ulRuleID;   // rule ID of the node rule
  char* cpRuleName; // rule name of the node rule
  APG_BSTR sPhrase; // binary string identifying the parsed node phrase
} APG_AST_ITERATOR;

APG_AST_ITERATOR* spASTIterCtor(void* vpCtx);
void vASTIterDtor(APG_AST_ITERATOR* spIter);
ulong ulASTFirst(APG_AST_ITERATOR* spIter);
ulong ulASTLast(APG_AST_ITERATOR* spIter);
ulong ulASTNext(APG_AST_ITERATOR* spIter);
ulong ulASTPrev(APG_AST_ITERATOR* spIter);
ulong ulASTSkipBranch(APG_AST_ITERATOR* spIter);

//////////////////////////////////////////////////////////////////////////
// TREE NODE MODEL (TNM) COMPONENT
// supplies iterators for horizontal traversal of the AST
//////////////////////////////////////////////////////////////////////////
// node
//1) the rule name ID (RID) for the node
//2) the rule name of the node
//3) the NID of its parent
//4) the parsed phrase matched to the node
//5) an iterator over all of its siblings
//6) an iterator over all of its children
//7) an iterator over all nodes of the same RID
typedef struct
{
  char* cpRuleName;         // the rule name
  ulong ulRID;              // ID of the rule name
  ulong ulNID;              // node ID (unique across all AST nodes)
  ulong ulParentNID;        // node ID of this node's parent
  ulong ulFirstChildNID;    // node ID of the left-most child of this node
  ulong ulChildCount;       // number of child nodes
  ulong ulFirstSiblingNID;  // node ID of the first sibling of this node
  ulong ulSiblingCount;     // number of siblings this node has
  APG_BSTR sPhrase;         // binary string holding this node's matched phrase
  void* vpValidate;         // magic number for struct validation
} APG_TNM_NODE;

// iterator
typedef struct
{
  ulong ulType;           // branch list or node list iterator
  ulong ulIndex;          // relative index of the node the iterator is currently pointing to
  ulong ulCount;          // number of members of the list
  ulong ulFirst;          // index of the first member of the list
  ulong* ulpBranches;     // pointer to the first member of the branch list
  APG_TNM_NODE* spNodes;  // pointer to the first member of the node list
  void* vpValidate;
} APG_TNM_ITER;

void* vpTnmCtor(void* vpParserCtx);
void  vTnmDtor(void* vpCtx);
APG_TNM_NODE* spTnmRule(void* vpCtx, ulong ulRID, APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmBranches(void* vpCtx, APG_TNM_NODE* spNode, APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmChildren(void* vpCtx, APG_TNM_NODE* spNode, APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmSiblings(void* vpCtx, APG_TNM_NODE* spNode, APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmFirst(APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmLast(APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmNext(APG_TNM_ITER* spIter);
APG_TNM_NODE* spTnmPrev(APG_TNM_ITER* spIter);

// Just to solve Code::Blocks linkage quirks
void vNodeTraceDummy();

// End C-language linkage specification
#if defined(__cplusplus)
}
#endif

#endif // _Apg_h_15_6_2006_9_36_
