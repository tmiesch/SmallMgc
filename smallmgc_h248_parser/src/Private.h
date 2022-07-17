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

   created: 8/25/2007 10:27
  filename: D:\Projects\Apgv5_0\ApgLib\Private.h
   purpose: Private APG library header file. Required for building
            APG static library only. Not require for building applications.

*********************************************************************/
#ifndef _Private_h_14_12_2006_10_25_
#define _Private_h_14_12_2006_10_25_
#undef PASSERT
#define PASSERT(cond) if(!(int)(cond)) {ulMemErrReport(spCtx->vpMemCtx, APG_PAR_ID, __LINE__, __FILE__); goto APG_EXIT;}

/* may be needed for future implementation of PPTs
//////////////////////////////////////////////////////////////////////////
// BIT MANIPULATION MACROS
//////////////////////////////////////////////////////////////////////////
// returns ulValue/4 rounded up to the nearest integer
#define ulCeiling4(ulValue) ((ulong)(ulValue))&3UL ? ((((ulong)(ulValue))>>2UL) + 1UL) : ((ulong)((ulValue))>>2UL)

// returns ulValue/8 rounded up to the nearest integer
#define ulCeiling8(ulValue) ((ulong)(ulValue))&7UL ? ((((ulong)(ulValue))>>3UL) + 1UL) : ((ulong)((ulValue))>>3UL)

// void vSetBit(uchar* ucpA, ulong ulIndex);
// uchar* ucpA   - pointer to an unsigned character array
// ulong ulIndex - index of a bit in the array
// set/clear the bit
static uchar const _s_ucaBitMask_[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
#define vSetBit(ucpA, ulIndex) ((uchar*)(ucpA))[((ulong)(ulIndex))>>3UL] |= (1UL << (((ulong)(ulIndex))&7UL))
#define vClearBit(ucpA, ulIndex) ((uchar*)(ucpA))[((ulong)(ulIndex))>>3UL] &= _s_ucaBitMask_[(((ulong)(ulIndex))&7UL)]

// uchar* ucpA   - pointer to an unsigned character array
// ulong ulIndex - index of a bit in the array
// returns the value of the indexed bit
#define ulGetBit(ucpA, ulIndex) (ulong)(((((uchar*)(ucpA))[((ulong)(ulIndex))>>3UL] >> (((ulong)(ulIndex))&7UL)) & 1UL))

// uchar* ucpA   - pointer to an unsigned character array
// ulong ulIndex - index of a 2-bit in the array
// ulong ulValue - value to set the 2-bits to
//                 value must be <= 3 or result is undefined
// sets the two-bit value at index in a byte array
static uchar const _s_ucaSetMask_[4] = {252,243,207,63};
#define vSetTwoBitValue(ucpA, ulIndex, ulValue)\
  ((uchar*)(ucpA))[((ulong)(ulIndex))>>2UL] =\
  (((uchar*)(ucpA))[((ulong)(ulIndex))>>2UL]&_s_ucaSetMask_[((ulong)(ulIndex))&3UL])|(uchar)(ulValue<<((((ulong)(ulIndex))&3UL)<<1UL))

// uchar* ucpA   - pointer to an unsigned character array
// ulong ulIndex - index of a 2-bit in the array
// returns the indexed two-bit value
static uchar const _s_ucaGetMask_[4] = {3,12,48,192};
#define ulGetTwoBitValue(ucpA, ulIndex)\
  (ulong)(((((uchar*)(ucpA))[((ulong)(ulIndex))>>2UL] & _s_ucaGetMask_[((ulong)(ulIndex))&3UL]) >> ((((ulong)(ulIndex))&3UL)<<1UL)))
*/

//////////////////////////////////////////////////////////////////////////
// PRIVATE ERROR COMPONENT
//////////////////////////////////////////////////////////////////////////
typedef struct
{
  ulong       ulAvailable;
  ulong       ulUsed;
  ulong       ulTotal;
  void*       vpValidate;
  void*       vpMemCtx;
  APG_MEM_ERR*  spErrors;
} APG_ERR_CTX;

//////////////////////////////////////////////////////////////////////////
// PRIVATE MEMORY COMPONENT
//////////////////////////////////////////////////////////////////////////
// a memory cell
struct APG_MEM_CELL_STRUCT
{
  struct APG_MEM_CELL_STRUCT* spPrev;
  struct APG_MEM_CELL_STRUCT* spNext;
  ulong ulSize; // size (bytes) of memory allocation
  ulong ulSeq;  // sequence number of this cell
};

typedef struct APG_MEM_CELL_STRUCT APG_MEM_CELL;
typedef struct
{
  ulong           ulActiveCellCount;  // number of cells on the active list
  PFN_ALLOCATOR   pfnAllocator;       // pointer to the allocator function
  PFN_DEALLOCATOR pfnDeAllocator;     // pointer to the de-allocator function
  APG_MEM_STATS   sStats;             // memory statistics
  APG_ERR_CTX*    spErrCtx;           // pointer to the err component
  void*           vpValidate;         // validation handle
  APG_MEM_CELL*   spActiveList;       // pointer to the first (and last) cell
                                      // in a circularly, doubly linked list
} APG_MEM_CTX;

//////////////////////////////////////////////////////////////////////////
// PRIVATE PARSER COMPONENT
//////////////////////////////////////////////////////////////////////////
// executable data header
// NOTE: there is a circular dependence between these structs and the generator (apg or Apg.exe)
//       modification will break the generator
typedef struct
{
  ulong ulLength;           // length of executable data (including this header)
  ulong ulCrc32;            // CRC32 hash to validate the data transfer
  ulong ulInitialized;      // true if the parser instance has been initialized
  ulong ulRuleCount;        // number of rule names
  ulong ulOpcodeCount;      // total number of opcodes

  // section location identifiers
  // all sections are aligned on 4-byte word boundaries
  // all section lengths are in bytes
  APG_BLOC sHeader;
  APG_BLOC sStrTbl;
  APG_BLOC sOpcodes;
  APG_BLOC sRuleList;
} APG_EXEHDR;

// trace control
typedef struct
{
  ulong ulTreeDepth;
  ulong ulOffset;
  ulong ulLen;
  ulong ulState;
  char* cpRuleName;
} APG_TRACE_INFO;

typedef struct
{
  // trace print controls
  ulong ulTrace;
  ulong ulCountOnly;
  ulong ulBegin;
  ulong ulEnd;
  ulong ulMatch;
  ulong ulEmpty;
  ulong ulNoMatch;
  ulong ulActive;
  ulong ulTRG;
  ulong ulTLS;
  ulong ulTBS;
  ulong ulPRD;
  ulong ulALT;
  ulong ulCAT;
  ulong ulREP;
  ulong ulRNM;
  ulong ulRuleNames;
  ulong ulHideRules;
  ulong* ulpRNMIDs;       // list of the individual rule name IDs to trace

  // trace statistics controls
  ulong ulTreeDepth;
  ulong ulNodesTraced;
  ulong ulTRGCount;
  ulong ulTLSCount;
  ulong ulTBSCount;
  ulong ulPRDCount;
  ulong ulALTCount;
  ulong ulCATCount;
  ulong ulREPCount;
  ulong ulRNMCount;
  ulong* ulpRuleNameCounts;// individual rule name RNM node visits
  void* vpValidate;
} APG_TRACE_CONTROL;

// rule list
typedef struct
{
  char*         cpRuleName;     // pointer to the (null-terminated) ASCII rule name
  void*         vpOp;           // pointer to the first opcode of the rule
  SYN_CALLBACK  pfnCBSyntax;    // pointer to the syntax call back function for this rule
  SEM_CALLBACK  pfnCBSemantic;  // pointer to the semantic call back function for this rule
} APG_RULES;

// parser context
typedef struct
{
  void*       vpMemCtx;     // context of the inherited Memory component
  APG_BSTR    sSrc;         // input string as a binary string
  APG_CBDATA  sCBData;      // data passed to user-written call back functions
  void*       vpRules;      // pointer to the first rule of the rule list
  ulong       ulRuleCount;  // number of rules in the grammar
  ulong       ulStartRule;  // rule ID of the start rule
  void*       vpAstCtx;     // AST component context
  void*       vpTraceCtx;   // trace control
  void*       vpPackratCtx; // packrat context if _APG_CFG_PACKRAT is defined
  void*       vpStatsCtx;   // parser statistics, if needed
  APG_PARSER_STATE sState;  // final parser state
  void*       vpValidate;   // used to validate the context
} APG_PARSER_CTX;

// for indexed identification of SABNF operators
#define OP_ID_RNM 0
#define OP_ID_ALT 1
#define OP_ID_CAT 2
#define OP_ID_REP 3
#define OP_ID_PRD 4
#define OP_ID_TRG 5
#define OP_ID_TBS 6
#define OP_ID_TLS 7
#define OP_ID_MAX 8

// Packrat table entry
typedef struct
{
  ulong ulState;
  ulong ulMatched;
  ulong ulCount;
} APG_PACKRAT;

typedef struct
{
  void* vpMemCtx;
  APG_PACKRAT* spTable;
  ulong ulRuleCount;
  ulong ulCharCount;
  void* vpValidate;
} APG_PACKRAT_CTX;

//////////////////////////////////////////////////////////////////////////
// AST NODES
// there are two APG_AST_NODE structs for each node in the AST
// one for the down branch direction and one for up
//////////////////////////////////////////////////////////////////////////
typedef struct
{
  ulong ulDown;       // true if direction is down, false if direction is up
  ulong ulOtherIndex; // index to matching APG_AST_NODE
  ulong ulValueIndex;  // index to the APG_AST_VALUE
} APG_AST_NODE;

// there is one APG_AST_VALUE struct for each node in the AST
typedef struct
{
  ulong ulRuleIndex;  // index of this node's rule name
  ulong ulParentValue;// index of this node's parent value
  ulong ulIdentifier; // unique identifier for this node
  APG_BLOC sPhrase;   // location of the node's matched phrase (offset into input string and length)
} APG_AST_VALUE;

// the private AST context
typedef struct
{
  ulong     ulRuleCount;  // number of rules in the grammar
  ulong     ulStartRule;  // rule ID of the start rule
  void*     vpMemCtx;     // the parser's memory context
  char**    cppRuleNames; // array of rule name pointers defining which rules appear in the AST
  char*     cpNames;      // pointer to the buffer holding the AST private copy of the rule names
  APG_BSTR  sSrc;         // input string
  void*     vpVecNodes;   // vector of AST traversal definitions, one for downward traversal of AST, one for up
  void*     vpVecValues;  // vector of AST node definitions
  ulong     ulNodeIndex;  // current vpVecNodes pointer (index of next available node)
  ulong     ulValueIndex; // current vpVecValues pointer (index of next available value)
  ulong     ulRNMParent;  // global place keeper for vRNM node parent value index
  ulong     ulInPRD;      // flag to prevent AST node collection in vPRD operator
  void*     vpValidate;   // pointer to the AST component context when context is valid
} APG_AST_CTX;

typedef struct
{
  APG_AST_CTX* spAst;   // pointer to the AST context
  void* vpMemCtx;       // the parser's memory context
  ulong ulSize;         // size of the allocated iterator context
  ulong ulNodeIndex;    // index of the current node
  void* vpValidate;     // validation test
} APG_AST_ITER_HELPER;

void* vpASTCtor(APG_PARSER_CTX* spCtx);
void  vASTDtor(void* vpCtx);
void  vASTClear(void* vpCtx, APG_BSTR* spSrc);

//////////////////////////////////////////////////////////////////////////
// PARSE TREE NODE MODEL (PTNM) COMPONENT
//////////////////////////////////////////////////////////////////////////
#define APG_TNM_ITER_BRANCH  1
#define APG_TNM_ITER_NODE    2

// rule list
typedef struct
{
  char* cpRuleName;
  ulong ulBranchIndex;  // index of first NID in the Parse Tree branchfor this rule
  ulong ulIndex;        // used to build the branch list
  ulong ulCount;        // number of nodes in the Parse Tree for this rule
} APG_TNM_RULELIST;

// component
typedef struct
{
  void*           vpMemCtx;     // the memory component handle of the parser
  APG_BSTR        sSrc;         // the input string
  ulong           ulStartRule;  // the start rule ID
  ulong           ulRuleCount;  // the number of rule names in the grammar
  APG_TNM_RULELIST* spRuleList; // pointer to the rule list
  ulong           ulNodeCount;  // number of nodes in the AST
  ulong           ulNodeIndex;
  ulong*          ulpBranches;
  APG_TNM_NODE*  spNodes;
  APG_AST_NODE*    spAstNodes;
  APG_AST_VALUE*   spAstValues;

  // valid context check
  void*           vpValidate;
} APG_TNM_CTX;

//////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////
// operator function prototype
// void* vpCtx         - pointer to a parser context
// void* vpOp          - pointer to the opcode to be executed
// ulong ulChar        - offset of the first character of the sub-string to be matched
// ulong* ulpState     - returns operation state (EMPTY, MATCH or NOMATCH)
// ulong* ulpMatched   - returns the number of sub-string characters matched
typedef void (*PFN_OP)(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);

// defined in Operators.c
ulong ulRuleListInit(APG_PARSER_CTX* spCtx, APG_EXEHDR* spExe);
ulong ulOpcodeInit(APG_PARSER_CTX* spCtx, APG_EXEHDR* spExe);
void  vRunStartRule(APG_PARSER_CTX* spCtx, ulong ulStartRule, ulong* ulpState, ulong* ulpMatched);

#if defined(_APG_CFG_TRACE)
  void vNodeTraceCtor(APG_PARSER_CTX* spCtx);
  void vNodeTraceDtor(APG_PARSER_CTX* spCtx);
  void vNodeTraceClear(APG_PARSER_CTX* spCtx);
  void vNodeTraceBegin(APG_PARSER_CTX* spCtx, ulong ulChar, void* vpOpcode);
  void vNodeTraceEnd(APG_PARSER_CTX* spCtx, ulong ulChar, void* vpOpcode, ulong ulState, ulong ulMatched);
#endif

#if defined(_APG_CFG_STATS)
  void vNodeStats(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched, ulong ulRuleIndex);
  void vNodeStatsBackTrack(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched);
  void vNodeRnmVisit(APG_PARSER_CTX* spCtx, ulong ulRuleIndex, ulong ulSuffix, ulong ulState);
  void vNodeRnmSetup(APG_PARSER_CTX* spCtx, ulong ulFunc);
  void vNodeStatsCtor(APG_PARSER_CTX* spCtx);
  void vNodeStatsDtor(APG_PARSER_CTX* spCtx);
  void vNodeStatsClear(APG_PARSER_CTX* spCtx);
  void vNodeStatsEos(APG_PARSER_CTX* spCtx, ulong ulType, ulong ulChar, ulong ulState);
  void vNodeStatsBegin(APG_PARSER_STATS* spStats);
  void vNodeStatsEnd(APG_PARSER_STATS* spStats, ulong ulID, ulong ulChar, ulong ulState, ulong ulMatched, ulong ulRuleIndex);
  void vNodeStatsChildCount(APG_PARSER_STATS* spStats, ulong ulID, ulong ulState, ulong ulMatched);
#endif

#if defined(_APG_CFG_PACKRAT)
// packrat statistics collection
void  vPackratCtor(APG_PARSER_CTX* spCtx);
void  vPackratDtor(APG_PARSER_CTX* spCtx);
void  vPackratClear(APG_PARSER_CTX* spCtx);
void  vPackratCollect(void* vpCtx, ulong ulRuleIndex, ulong ulChar, ulong ulState, ulong ulMatched);
ulong ulPackratTableCheck(void* vpCtx, ulong ulRuleIndex, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
#endif

#endif // _Private_h_14_12_2006_10_25_
