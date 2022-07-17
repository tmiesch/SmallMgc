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

   created: 8/25/2007 10:26
  filename: D:\Projects\Apgv5_0\ApgLib\Operators.h
   purpose: Defines APG operators and support thereof.

*********************************************************************/
#ifndef Operators_h_5_29_2007
#define Operators_h_5_29_2007

//////////////////////////////////////////////////////////////////////////
// SABNF OPERATOR PROTOTYPES
//////////////////////////////////////////////////////////////////////////
void vALT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vCAT(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vREP(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vRNM(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vPRD(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vTRG(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vTBS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);
void vTLS(void* vpCtx, void* vpOp, ulong ulChar, ulong* ulpState, ulong* ulpMatched);

//////////////////////////////////////////////////////////////////////////
// SABNF OPCODES
//////////////////////////////////////////////////////////////////////////
// generic SABNF opcode - they all begin with an SABNF operator pointer & next pointer
typedef struct
{
  PFN_OP  pfnOp;        // pointer to a general SABNF operator function
  struct OP_GEN* spNext;// pointer to the next opcode following this one
  ulong   ul1;          // filler to make OP_GEN the same size as max sized OP_xxx
  ulong   ul2;          // filler to make OP_GEN the same size as max sized OP_xxx
} OP_GEN;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the alternation ALT SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
} OP_ALT;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the concatenation CAT SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
} OP_CAT;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the repetition REP SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulMin;        // minimum number of repetitions
  ulong   ulMax;        // maximum number of repetitions
} OP_REP;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the predicate PRD SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulAnd;        // APG_TRUE if AND predicate, else NOT predicate
} OP_PRD;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the rule name RNM SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulRuleIndex;  // index into the rule list for this rule name
} OP_RNM;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the terminal range TRG SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
  ulong   ulMin;        // range minimum
  ulong   ulMax;        // range maximum
} OP_TRG;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the terminal binary string TBS SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
  uchar*  ucpString;    // pointer to the binary string in the string table
  ulong   ulLength;     // length of this binary string
} OP_TBS;

typedef struct
{
  PFN_OP  pfnOp;        // pointer to the terminal literal string TLS SABNF operator function
  OP_GEN* spNext;       // pointer to the next operator following this one
  uchar*  ucpString;    // pointer to the literal string in the string table
  ulong   ulLength;     // length of this literal string
} OP_TLS;


#endif /* Operators_h_5_29_2007 */
