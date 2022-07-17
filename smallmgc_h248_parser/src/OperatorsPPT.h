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

   created: 8/25/2007 10:26
  filename: D:\Projects\Apgv5_0\ApgLib\OperatorsPPT.h
   purpose: Defines PPT versions of APG operators and support thereof.
  
*********************************************************************/
#ifndef OperatorsPPT_h_5_29_2007
#define OperatorsPPT_h_5_29_2007

//////////////////////////////////////////////////////////////////////////
// SABNF OPERATOR PROTOTYPES
// using Predictive Parsing Tables
//////////////////////////////////////////////////////////////////////////

// PPT type flag manipulation
#define PPT_TYPE_1BIT   0x01
#define PPT_TYPE_2BIT   0x02
#define PPT_TYPE_MASK   0x03
#define PPT_TYPE_CLEAR  0xFC
#define PPT_TYPE(ucpPPT) (PPT_TYPE_MASK & *(uchar*)(ucpPPT))
#define PPT_IS_1BIT(ucpPPT) (ulong)(PPT_TYPE(ucpPPT) == PPT_TYPE_1BIT)
#define PPT_SET_TYPE_1BIT(ucpPPT) *(uchar*)(ucpPPT) = ((*(uchar*)(ucpPPT) & PPT_TYPE_CLEAR) | PPT_TYPE_1BIT)
#define PPT_SET_TYPE_2BIT(ucpPPT) *(uchar*)(ucpPPT) = ((*(uchar*)(ucpPPT) & PPT_TYPE_CLEAR) | PPT_TYPE_2BIT)

// get/set a PPT value, regardless of its type
ulong ulGetPPTValue(uchar* ucpPPT, ulong ulIndex);
void  vSetPPTValue(uchar* ucpPPT, ulong ulIndex, ulong ulValue);

// macro version of ulGetPPTValue() used only by PPT operators
#define GETPPTVALUE(ucpPPT, ulIndex) \
PPT_IS_1BIT((ucpPPT)) ? (ulGetBit((ucpPPT), (ulIndex + 2)) ? ACTIVE : NOMATCH) : (ulGetTwoBitValue((ucpPPT), (ulIndex + 1))) 

void vPPT_ALT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_CAT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_REP(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_RNM(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_PRD(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_TRG(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_TBS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPPT_TLS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);

//////////////////////////////////////////////////////////////////////////
// SABNF OPCODES
// using Predictive Parsing Tables
//////////////////////////////////////////////////////////////////////////
// generic SABNF opcode - they all begin with an SABNF operator pointer & PPT pointer
struct  OP_PPT_GEN 
{
  PFN_OP  pfnOp;            // pointer to a general SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;// pointer to the next opcode following this one
  ulong   ul1;              // filler to make OP_PPT_GEN the same size as max sized OP_PPT_xxx
  ulong   ul2;              // filler to make OP_PPT_GEN the same size as max sized OP_PPT_xxx
};

struct OP_PPT_RNM
{
  PFN_OP  pfnOp;            // pointer to the rule name RNM SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulRuleIndex;      // index into the rule list for this rule name
} ;

struct OP_PPT_ALT
{
  PFN_OP  pfnOp;            // pointer to the alternation ALT SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
};

struct OP_PPT_CAT
{
  PFN_OP  pfnOp;            // pointer to the concatenation CAT SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
};

struct OP_PPT_REP
{
  PFN_OP  pfnOp;            // pointer to the repetition REP SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulMin;            // minimum number of repetitions
  ulong   ulMax;            // maximum number of repetitions
};

struct OP_PPT_PRD
{
  PFN_OP  pfnOp;            // pointer to the predicate PRD SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulAnd;            // APG_TRUE if AND predicate, else NOT predicate
};

struct OP_PPT_TRG
{
  PFN_OP  pfnOp;            // pointer to the terminal range TRG SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulMin;            // range minimum
  ulong   ulMax;            // range maximum
};

struct OP_PPT_TBS
{
  PFN_OP  pfnOp;            // pointer to the terminal binary string TBS SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
  uchar*  ucpString;        // pointer to the binary string in the string table
  ulong   ulLength;         // length of this binary string
};

struct OP_PPT_TLS
{
  PFN_OP  pfnOp;            // pointer to the terminal literal string TLS SABNF operator function
  uchar*  ucpPPT;           // pointer to the predictive parsing table
  struct OP_PPT_GEN* spNext;       // pointer to the next operator following this one
  uchar*  ucpString;        // pointer to the literal string in the string table
  ulong   ulLength;         // length of this literal string
};

#endif /* OperatorsPPT_h_5_29_2007 */
