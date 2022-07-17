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

   created: 8/25/2007 10:07
  filename: D:\Projects\Apgv5_0\ApgLib\Catalog.c
   purpose: A catalog is a simple, hard-coded, relational database of the
            sub-string phrases matched by each occurrence of a
            rule name in the AST. The database consists
            of two tables and a single index. They are:
             - a table of phrases
             - a table of phrase lists
             - a sorted list of pointers to the phrases in the table of phrases

            In the table of phrases are the sub-strings matched by each occurrence
            of each rule name found in the AST.

            In the table of phrase lists is a list of first-level child phrases.
            There is one phrase list for each phrase. This is a contiguous,
            left-to-right list of the phrases for the first-level children of the
            phrase. That is, all children one level down in the AST.

            The table of phrase pointers is used to sort the phrases such that
            all lists of child phrases are contiguous in a left-to-right order.

 *** see .\Samples\SDP\Media.c for a simple example of catalog use.
  
*********************************************************************/
#include "Apg.h"
#include "Private.h"

//////////////////////////////////////////////////////////////////////////
// CATALOG COMPONENT FUNCTIONS
//////////////////////////////////////////////////////////////////////////
static void vSetIter(APG_CAT_ITERATOR* spIter, void** vppList, ulong ulCount);
static void vSetIterMember(void* vpCatalogCtx, APG_CAT_ITERATOR* spIter, ulong ulIndex);

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: catalog component constructor
//
// FUNCTION: void* vpCatalogCtor
//
//      ARG: 1) void* vpParserCtx
//              : pointer to a valid parser component
//
//   RETURN: pointer to a catalog component context
//
//////////////////////////////////////////////////////////////////////////
void* vpCatalogCtor(void* vpParserCtx)
{
  APG_CATALOG_CTX*  spReturn = NULL;
  APG_CATALOG_CTX*  spCtx = NULL;
  APG_RULES*        spRule;
  ulong             ulRuleIndex;
  ulong             ulCBIndex;
  APG_PARSER_CTX*   spParser = (APG_PARSER_CTX*)vpParserCtx;
  DASSERT(spParser && spParser->vpValidate == (void*)spParser);

  // get a catalog context
  spCtx = (APG_CATALOG_CTX*)vpMemAlloc(spParser->vpMemCtx, sizeof(APG_CATALOG_CTX));
  if(!spCtx)
  {
    ulErrReport(spParser->vpMemCtx, APG_CAT_ID, __LINE__, __FILE__);
    goto APG_EXIT;
  }

  memset((void*)spCtx, 0, sizeof(APG_CATALOG_CTX));
  spCtx->spParser = spParser;
  spCtx->vpMemCtx = spParser->vpMemCtx;

  // count the call back functions
  spCtx->ulCallBacks = 0;
  spRule = (APG_RULES*)spParser->vpRules;
  for(ulRuleIndex = 0; ulRuleIndex < spParser->ulRuleCount; ++ulRuleIndex, ++spRule)
  {
    if(spRule->pfnCallBack != NULL)
    {
      // count the call back functions
      ++spCtx->ulCallBacks;
    }
  }

  if(spCtx->ulCallBacks)
  {
    // create the ID maps
    spCtx->ulpGrammarToCatalog = (ulong*)vpMemAlloc(spCtx->vpMemCtx, sizeof(ulong) * spParser->ulRuleCount);
    CASSERT(spCtx->ulpGrammarToCatalog);
    spCtx->ulpCatalogToGrammar = (ulong*)vpMemAlloc(spCtx->vpMemCtx, sizeof(ulong) * spCtx->ulCallBacks);
    CASSERT(spCtx->ulpCatalogToGrammar);
    ulCBIndex = 0;
    spRule = (APG_RULES*)spParser->vpRules;
    for(ulRuleIndex = 0; ulRuleIndex < spParser->ulRuleCount; ++ulRuleIndex, ++spRule)
    {
      if(spRule->pfnCallBack != NULL)
      {
        spCtx->ulpGrammarToCatalog[ulRuleIndex] = ulCBIndex;
        spCtx->ulpCatalogToGrammar[ulCBIndex] = ulRuleIndex;
        ++ulCBIndex;
      }
      else
      {
        spCtx->ulpGrammarToCatalog[ulRuleIndex] = UNDEFINED;
      }
    }

    // get the rule vector
    spCtx->vpRuleArrayCtx = vpVecCtor(spCtx->vpMemCtx, sizeof(APG_RULE_ARRAY), spCtx->ulCallBacks);
    CASSERT(spCtx->vpRuleArrayCtx);
    
    // get the phrase vector
    spCtx->vpPhraseCtx = vpVecCtor(spCtx->vpMemCtx, sizeof(APG_PHRASE), 100);
    CASSERT(spCtx->vpPhraseCtx);
    
    // get the phrase list vector
    spCtx->vpListCtx = vpVecCtor(spCtx->vpMemCtx, sizeof(APG_PHRASE*), 100);
    CASSERT(spCtx->vpListCtx);
  }
  
  // success
  spCtx->vpValidate = (void*)spCtx;
  spReturn = spCtx;
  
APG_EXIT:
  if(!spReturn)
  {
    // release allocations
    vCatalogDtor((void*)spCtx);
  }

  return spReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: catalog component destructor
//
// FUNCTION: void vCatalogDtor
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//                NULL pointer ignored, no action, no error
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void vCatalogDtor(void* vpCatalogCtx)
{
  if(vpCatalogCtx)
  {
    // free the vectors
    APG_CATALOG_CTX* spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
    DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
    if(spCtx->ulpGrammarToCatalog){vMemFree(spCtx->vpMemCtx, (void*)spCtx->ulpGrammarToCatalog);}
    if(spCtx->ulpCatalogToGrammar){vMemFree(spCtx->vpMemCtx, (void*)spCtx->ulpCatalogToGrammar);}
    if(spCtx->vpRuleArrayCtx){vVecDtor(spCtx->vpRuleArrayCtx);}
    if(spCtx->vpPhraseCtx){vVecDtor(spCtx->vpPhraseCtx);}
    if(spCtx->vpListCtx){vVecDtor(spCtx->vpListCtx);}
    
    // free the context
    spCtx->vpValidate = NULL;
    vMemFree(spCtx->vpMemCtx, (void*)spCtx);
  }
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: clears the catalog of all data from a previous
//              call to ulCatalog()
//
// FUNCTION: void vCatalogClear
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
void  vCatalogClear(void* vpCatalogCtx)
{
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  // clear the previous catalog, if any
  if(spCtx->vpRuleArrayCtx){vVecClear(spCtx->vpRuleArrayCtx);}
  if(spCtx->vpPhraseCtx){vVecClear(spCtx->vpPhraseCtx);}
  if(spCtx->vpListCtx){vVecClear(spCtx->vpListCtx);}
}

typedef struct
{
  APG_PHRASE* spPhrase; 
} STACK_FRAME;

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: catalogs all phrases in a previously-generated AST
// does a single, depth-first traversal of the AST
// and generates all tables of the catalog database.
//
// FUNCTION: ulong ulCatalog
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//   RETURN: returns the number of phrases found
//
//////////////////////////////////////////////////////////////////////////
ulong ulCatalog(void* vpCatalogCtx)
{
  ulong             ulReturn = 0;
  APG_RULES*        spRuleList;
  APG_PT_NODE*      spNode;
  APG_PT_NODE*      spPtNodeBeg;
  APG_PT_NODE*      spPtNodeEnd;
  APG_PT_VALUE*     spValue;
  APG_PT_VALUE*     spPtValueBeg;
  APG_RULE_ARRAY*   spRuleArrayBeg;
  APG_PHRASE*       spPhrase;
  APG_PHRASE*       spPhraseBeg;
  APG_PHRASE*       spPhraseEnd;
  APG_PHRASE*       spParentPhrase;
  APG_PHRASE        sDummyPhrase;
  void*             vpStack = NULL;
  STACK_FRAME*      spFrame;
  uchar*            ucpSrc;
  ulong             ulTreeDepth;
  ulong             ulRule;
  ulong             ulPhraseCount;
  ulong             ulPhrasePointerListCount;
  ulong             ulPhraseIndex;
  APG_PHRASE**      sppPhraseList;
  APG_PHRASE**      sppPhraseListBeg;
  APG_PHRASE**      sppPhraseListEnd;
  APG_PARSER_CTX*   spParser;
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  // early exit if no call back functions
  spParser = spCtx->spParser;
  if(spCtx->ulCallBacks == 0){goto APG_EXIT;}
  
  // clear the previous catalog, if any
  vCatalogClear(vpCatalogCtx);

  // parser set up
  ucpSrc = spParser->sSrc.ucpStr;
  spRuleList = (APG_RULES*)spParser->vpRules;

  // get the AST
  spPtNodeBeg = (APG_PT_NODE*)vpVecFront(spParser->vpPtNodes);
  CASSERT(spPtNodeBeg);
  spPtNodeEnd = spPtNodeBeg + ulVecSize(spParser->vpPtNodes);
  spPtValueBeg = (APG_PT_VALUE*)vpVecFront(spParser->vpPtValues);
  CASSERT(spPtValueBeg);
  ulPhraseCount = ulVecSize(spParser->vpPtValues);

  // early exit if no phrases
  if(ulPhraseCount == 0){goto APG_EXIT;}

  // create and initialize the parent stack
  vpStack = vpVecCtor(spCtx->vpMemCtx, sizeof(STACK_FRAME), 0);
  CASSERT(vpStack);
  memset((void*)&sDummyPhrase, 0, sizeof(sDummyPhrase));
  sDummyPhrase.ulID = UNDEFINED;
  
  // get space for the rule array & phrases
  spRuleArrayBeg = (APG_RULE_ARRAY*)vpVecPushn(spCtx->vpRuleArrayCtx, NULL, spCtx->ulCallBacks);
  CASSERT(spRuleArrayBeg);
  memset((void*)spRuleArrayBeg, 0, ulVecBytes(spCtx->vpRuleArrayCtx));
  spPhraseBeg = (APG_PHRASE*)vpVecPushn(spCtx->vpPhraseCtx, NULL, ulPhraseCount);
  CASSERT(spPhraseBeg);
  memset((void*)spPhraseBeg, 0, ulVecBytes(spCtx->vpPhraseCtx));
  spPhraseEnd = spPhraseBeg + ulPhraseCount;

  // first pass, traverse the AST
  // - count number of children of each node
  // - count number of phrases (occurrences) of each rule
  ulTreeDepth = 0;
  spParentPhrase = &sDummyPhrase;
  spNode = spPtNodeBeg;
  for(; spNode < spPtNodeEnd; ++spNode)
  {
    DASSERT(spNode->ulPairedNode != APG_PT_UNDEFINED);
    spValue = spPtValueBeg + spNode->ulValue;
    DASSERT(spRuleList[spValue->ulRuleIndex].usSemantics);
    if(spNode->ulDirection == APG_PT_DOWN)
    {
      // pre-branch processing (direction down)
      spPhrase = spPhraseBeg + spNode->ulValue;
      spPhrase->ulID = spCtx->ulpGrammarToCatalog[spValue->ulRuleIndex];
      spPhrase->cpRuleName = spRuleList[spValue->ulRuleIndex].cpRuleName;
      spPhrase->vpParent = (void*)spParentPhrase;
      spPhrase->sPhrase.ucpStr = &ucpSrc[spValue->sValue.ulOffset];
      spPhrase->sPhrase.ulLen = spValue->sValue.ulLen;
      spPhrase->ulTreeDepth = ulTreeDepth;
      spPhrase->ulIdentifier = spValue->ulIdentifier;
      
      // increment the phrase count for this rule
      spRuleArrayBeg[spPhrase->ulID].ulCount += 1;
      
      // increment the parent's child phrase count
      ++spParentPhrase->ulChildCount;
      
      // update the parent phrase
      ++ulTreeDepth;
      spFrame = (STACK_FRAME*)vpVecPush(vpStack, NULL);
      CASSERT(spFrame);
      spFrame->spPhrase = spParentPhrase;
      spParentPhrase = spPhrase;
    }
    else
    {
      // post branch (direction up), update previous phrase to this parent's child phrase
      --ulTreeDepth;
      spFrame = (STACK_FRAME*)vpVecPop(vpStack);
      CASSERT(spFrame);
      spParentPhrase = spFrame->spPhrase;
    }
  }

  DASSERT(0 == ulTreeDepth);
  DASSERT(0 == ulVecSize(vpStack));

  // second pass - generate catalog index
  ulPhrasePointerListCount = 0;
  for(ulRule = 0; ulRule < spCtx->ulCallBacks; ++ulRule)
  {
    spRuleArrayBeg[ulRule].ulIndex = 0;
    ulPhrasePointerListCount += spRuleArrayBeg[ulRule].ulCount;
  }

  for(spPhrase = spPhraseBeg; spPhrase < spPhraseEnd; ++spPhrase)
  {
    spPhrase->ulIndex = 0;
    ulPhrasePointerListCount += spPhrase->ulChildCount;
  }

  // initialize the rule & phrase lists
  sppPhraseListBeg = (APG_PHRASE**)vpVecPushn(spCtx->vpListCtx, NULL, ulPhrasePointerListCount);
  CASSERT(sppPhraseListBeg);
  sppPhraseListEnd = sppPhraseListBeg + ulPhrasePointerListCount;
  sppPhraseList = sppPhraseListBeg;
  for(ulRule = 0; ulRule < spCtx->ulCallBacks; ++ulRule)
  {
    // set the pointer to the beginning of the phrase list for this rule
    spRuleArrayBeg[ulRule].sppPhrase = sppPhraseList;
    sppPhraseList += spRuleArrayBeg[ulRule].ulCount;
  }

  for(spPhrase = spPhraseBeg; spPhrase < spPhraseEnd; ++spPhrase)
  {
    // set the pointer to the beginning of the child phrase list for this phrase
    spPhrase->vpChildList = (void*)sppPhraseList;
    sppPhraseList += spPhrase->ulChildCount;
  }

  // do the first phrase (special case because it has no parent)
  spPhrase = spPhraseBeg;
  spPhrase->vpParent = NULL;
  sppPhraseList = spRuleArrayBeg[spPhrase->ulID].sppPhrase;
  ulPhraseIndex = spRuleArrayBeg[spPhrase->ulID].ulIndex++;
  sppPhraseList[ulPhraseIndex] = spPhrase;
  DASSERT(spRuleArrayBeg[spPhrase->ulID].ulIndex <= spRuleArrayBeg[spPhrase->ulID].ulCount);

  // scan all remaining phrases in the order they appear
  for(++spPhrase; spPhrase < spPhraseEnd; ++spPhrase)
  {
    // add this phrase to the rule array phrase list
    sppPhraseList = spRuleArrayBeg[spPhrase->ulID].sppPhrase;
    ulPhraseIndex = (spRuleArrayBeg[spPhrase->ulID].ulIndex)++;
    sppPhraseList[ulPhraseIndex] = spPhrase;
    DASSERT(spRuleArrayBeg[spPhrase->ulID].ulIndex <= spRuleArrayBeg[spPhrase->ulID].ulCount);

    // add this phrase to the child list of its parent
    spParentPhrase = (APG_PHRASE*)spPhrase->vpParent;
    sppPhraseList = (APG_PHRASE**)spParentPhrase->vpChildList;
    ulPhraseIndex = (spParentPhrase->ulIndex)++;
    sppPhraseList[ulPhraseIndex] = spPhrase;
    DASSERT(spParentPhrase->ulIndex <= spParentPhrase->ulChildCount);
  }

  // get permanent pointers to the lists for efficient access in helper functions
  spCtx->spRuleArray = (APG_RULE_ARRAY*)vpVecFront(spCtx->vpRuleArrayCtx);
  CASSERT(spCtx->spRuleArray);
  spCtx->spPhrases = (APG_PHRASE*)vpVecFront(spCtx->vpPhraseCtx);
  CASSERT(spCtx->spPhrases);
  spCtx->sppLists = (APG_PHRASE**)vpVecFront(spCtx->vpListCtx);
  CASSERT(spCtx->sppLists);

  // success
  ulReturn = ulPhraseCount;

APG_EXIT:
  // clean up the temporary stack
  if(vpStack){vVecDtor(vpStack);}
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: get an iterator to the list of phrases for
//              a specific rule
//
// NOTE: a valid iterator points to a phrase and has the following properties
//   ulong     ulParentID;   // rule name ID of the parent rule
//   char*     cpParentName; // ASCII string rule name of the parent rule
//   ulong     ulPhraseID;   // rule name ID of the phrase
//   char*     cpPhraseName; // ASCII string rule name of the phrase
//   APG_BSTR  sPhrase;      // the actual phrase as a binary string 
//   ulong     ulPhraseIdentifier; // unique-to-catalog identifier, unique for each individual phrase
//   ulong     ulParentIdentifier; // unique identifier of the parent phrase
//
// See function ulPrintMediaFields() in .\Samples\SDP\Media.c
// for a simple example of iterator use.
//
// FUNCTION: APG_CAT_ITERATOR* spCatalogGetPhraseList
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//      ARG: 2) ulong ulID
//              : rule to get the phrase list for
//                should be RULE_XXX_rulename where XXX.h is the header file
//                of the parser and "rulename" is a grammar rule name
//
//      ARG: 3) APG_CAT_ITERATOR* spIter
//              : pointer to an empty APG_CAT_ITERATOR struct
//
//   RETURN: on success, pointer to the completed iterator (spIter)
//           NULL on failure
//
//////////////////////////////////////////////////////////////////////////
APG_CAT_ITERATOR* spCatalogGetPhraseList(void* vpCatalogCtx, ulong ulID, APG_CAT_ITERATOR* spIter)
{
  APG_CAT_ITERATOR* spReturn = NULL;
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  APG_RULE_ARRAY*   spRule;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);

  // translate the user's grammar rule ID to call back ID
  ulID = spCtx->ulpGrammarToCatalog[ulID];
  DASSERT(ulID < spCtx->ulCallBacks);
  spRule = &spCtx->spRuleArray[ulID];
  if(spRule->ulCount)
  {
    // set the user's info in the iterator
    vSetIter(spIter, (void**)spRule->sppPhrase, spRule->ulCount);
    vSetIterMember(vpCatalogCtx, spIter, 0);
    
    // success
    spReturn = spIter;
  }

  return spReturn;
}

// get an iterator for the list of child phrases of the next phrase in spPhraseIter
//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: get an iterator to a list of child phrases for a specific phrase
//
// FUNCTION: APG_CAT_ITERATOR* spCatalogGetChildList
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//      ARG: 2) APG_CAT_ITERATOR* spPhraseIter
//              : pointer to a valid iterator returned from either
//                spCatalogGetPhraseList() or spCatalogGetChildList()
//                This iterator defines the phrase for which this function
//                extracts the child phrase list
//
//      ARG: 3) APG_CAT_ITERATOR* spIter
//              : on return points to the caller's empty iterator
//                the child phrase iterator is returned in this struct
//
//   RETURN: on success, a pointer to a phrase iterator (spIter above) for the list
//           of child phrases 
//
//////////////////////////////////////////////////////////////////////////
APG_CAT_ITERATOR* spCatalogGetChildList(void* vpCatalogCtx, APG_CAT_ITERATOR* spPhraseIter, APG_CAT_ITERATOR* spIter)
{
  APG_CAT_ITERATOR* spReturn = NULL;
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  APG_PHRASE*       spPhrase;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spIter);

  if(spPhraseIter && spPhraseIter->ulCount && spPhraseIter->ulIndex < spPhraseIter->ulCount)
  {
    spPhrase = (APG_PHRASE*)spPhraseIter->vppList[spPhraseIter->ulIndex];
    if(spPhrase->ulChildCount)
    {
      vSetIter(spIter, (void**)spPhrase->vpChildList, spPhrase->ulChildCount);
      vSetIterMember(vpCatalogCtx, spIter, 0);
      
      // success
      spReturn = spIter;
    }
  }
  
  return spReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Sets a phrase list iterator to the beginning element of the list
//
// FUNCTION: APG_CAT_ITERATOR* spCatalogIterBegin
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//      ARG: 2) APG_CAT_ITERATOR* spIter
//              : pointer to the iterator to reset
//
//   RETURN: spIter on success
//           NULL on failure
//
//////////////////////////////////////////////////////////////////////////
APG_CAT_ITERATOR* spCatalogIterBegin(void* vpCatalogCtx, APG_CAT_ITERATOR* spIter)
{
  APG_CAT_ITERATOR* spReturn = NULL;
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spIter);

  if(spIter && spIter->ulCount)
  {
    vSetIterMember(vpCatalogCtx, spIter, 0);
    
    // success
    spReturn = spIter;
  }

  return spReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Increments the iterator to the next phrase in the list
//
// FUNCTION: APG_CAT_ITERATOR* spCatalogIterNext
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//      ARG: 2) APG_CAT_ITERATOR* spIter
//              : pointer to the iterator to increment
//
//   RETURN: spIter on success
//           NULL on end of list
//
//////////////////////////////////////////////////////////////////////////
APG_CAT_ITERATOR* spCatalogIterNext(void* vpCatalogCtx, APG_CAT_ITERATOR* spIter)
{
  APG_CAT_ITERATOR* spReturn = NULL;
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  DASSERT(spIter);
  
  if(spIter && spIter->ulCount)
  {
    if((spIter->ulIndex + 1) < spIter->ulCount)
    {
      ++spIter->ulIndex;
      vSetIterMember(vpCatalogCtx, spIter, spIter->ulIndex);

      // success
      spReturn = spIter;
    }
  }
  
  return spReturn;

}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Traverses the sub-tree of the AST with a specific
// phrase - that is a specific instance of a rule name in the AST -
// as the sub-tree root.
//
// FUNCTION: ulong ulCatalogTreeTraversal
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//      ARG: 2) ulong ulNodeIdentifier
//              : unique identifier of the phrase
//                (found in the iterator pointing to the phrase)
//
//      ARG: 3) void* vpData
//              : pointer to the user data required by the call back functions
//                or NULL if none
//
//   RETURN: true on success, false on failure
//
//////////////////////////////////////////////////////////////////////////
ulong   ulCatalogTreeTraversal(void* vpCatalogCtx, ulong ulNodeIdentifier, void* vpData)
{
  ulong           ulReturn = APG_FALSE;
  APG_CATALOG_CTX* spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  APG_PARSER_CTX* spParser;
  APG_RULES*      spRuleList;
  APG_PT_NODE*    spNext;
  APG_PT_NODE*    spEnd;
  APG_PT_NODE*    spPtNodes;
  APG_PT_VALUE*   spPtValues;
  APG_PT_VALUE*   spValue;
  PFN_CALLBACK    pfnCallBack;
  ulong           ulValue, ulValueLen;
  ulong           ulCallBackReturn;
  DASSERT(spCtx && spCtx->vpValidate == (void*)spCtx);
  
  spParser = spCtx->spParser;
  spRuleList = (APG_RULES*)spParser->vpRules;
  spPtNodes = (APG_PT_NODE*)vpVecFront(spParser->vpPtNodes);
  PASSERT(spPtNodes);
  spPtValues = (APG_PT_VALUE*)vpVecFront(spParser->vpPtValues);
  PASSERT(spPtValues);
  spNext = spPtNodes;
  spEnd = spNext + ulVecSize(spParser->vpPtNodes);

  // fast forward to the starting node
  for(; spNext < spEnd; ++spNext)
  {
    DASSERT((spNext->ulPairedNode != APG_PT_UNDEFINED));
    spValue = spPtValues + spNext->ulValue;
    if(spValue->ulIdentifier == ulNodeIdentifier && spNext->ulDirection == APG_PT_DOWN)
    {
      // spNext is the starting node
      break;
    }
  }  
  PASSERT(spNext < spEnd);
  spEnd = spPtNodes + spNext->ulPairedNode;

  // traverse the remaining AST, if any
  for(; spNext < spEnd; ++spNext)
  {
    DASSERT((spNext->ulPairedNode != APG_PT_UNDEFINED));
    spValue = spPtValues + spNext->ulValue;
    ulValue = spValue->sValue.ulOffset;
    ulValueLen = spValue->sValue.ulLen;
    pfnCallBack = spRuleList[spValue->ulRuleIndex].pfnCallBack;
    if(pfnCallBack)
    {
      // process this node
      if(spNext->ulDirection == APG_PT_DOWN)
      {
        ulCallBackReturn = pfnCallBack(vpData, SEM_PRE, ulValue, ulValueLen);
        if(ulCallBackReturn == PFN_SKIP)
        {
          // skip branch below
          spNext = spPtNodes + spNext->ulPairedNode;
        }
        else
        {
          // reports an error for anything but OK
          PASSERT(ulCallBackReturn == PFN_OK);
        }
      }
      else
      {
        ulCallBackReturn = pfnCallBack(vpData, SEM_POST, ulValue, ulValueLen);
        // ignore return value
      }
    }
  }
  
  // success
  ulReturn = APG_TRUE;
  
APG_EXIT:
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: copy a binary string to a null-termed character string
// a static function that may be called without reference to a catalog component
//
// FUNCTION: ulong ulCatalogGetString
//
//      ARG: 1) APG_BSTR* spBstr
//              : pointer to the binary string to copy
//
//      ARG: 2) char* cpBuf
//              : pointer to the buffer to recieve the null-term string
//                caller must insure that buffer is big enough to hold the string
//                and the null terminator
//
//   RETURN: the length of the string copied(not including the null terminator)
//           0 if no string is copied
//
//////////////////////////////////////////////////////////////////////////
ulong ulCatalogGetString(APG_BSTR* spBstr, char* cpBuf)
{
  ulong ulReturn = 0;
  DASSERT(spBstr && cpBuf);
  
  if(spBstr->ucpStr && spBstr->ulLen)
  {
    memcpy((void*)cpBuf, spBstr->ucpStr, spBstr->ulLen);
    cpBuf[spBstr->ulLen] = 0;
    ulReturn = spBstr->ulLen;
  }
  
  return ulReturn;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: for use by catalog components only
// Set the phrase list and phrase count of an iterator.
//
// FUNCTION: static void vSetIter
//
//      ARG: 1) APG_CAT_ITERATOR* spIter
//              : pointer to an empty iterator struct
//
//      ARG: 2) void** vppList
//              : pointer to a list of pointers to the phrases in the list
//
//      ARG: 3) ulong ulCount
//              : the number of phrases in the list
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
static void vSetIter(APG_CAT_ITERATOR* spIter, void** vppList, ulong ulCount)
{
  spIter->ulCount = ulCount;
  spIter->vppList = vppList;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: for use by catalog components only
// Set the current phrase that an iterator points to.
//
// FUNCTION: static void vSetIterMember
//
//      ARG: 1) void* vpCatalogCtx
//              : pointer to a valid catalog component context
//
//      ARG: 2) APG_CAT_ITERATOR* spIter
//              : pointer to an initialized iterator 
//
//      ARG: 3) ulong ulIndex
//              : index of the list member phrase
//
//   RETURN: void
//
//////////////////////////////////////////////////////////////////////////
static void vSetIterMember(void* vpCatalogCtx, APG_CAT_ITERATOR* spIter, ulong ulIndex)
{
  APG_CATALOG_CTX*  spCtx = (APG_CATALOG_CTX*)vpCatalogCtx;
  APG_PHRASE*       spPhrase;
  APG_PHRASE*       spParentPhrase;
  DASSERT(ulIndex < spIter->ulCount);

  spPhrase = (APG_PHRASE*)spIter->vppList[ulIndex];
  spParentPhrase = (APG_PHRASE*)spPhrase->vpParent;
  if(spParentPhrase == NULL)
  {
    spIter->ulParentID = UNDEFINED;
    spIter->cpParentName = "<parent of start rule>";
    spIter->ulParentIdentifier = UNDEFINED;
  }
  else
  {
    spIter->ulParentID = spCtx->ulpCatalogToGrammar[spParentPhrase->ulID];
    spIter->cpParentName = spParentPhrase->cpRuleName;
    spIter->ulParentIdentifier = spParentPhrase->ulIdentifier;
  }
  spIter->ulPhraseID = spCtx->ulpCatalogToGrammar[spPhrase->ulID];
  spIter->cpPhraseName = spPhrase->cpRuleName;
  spIter->sPhrase = spPhrase->sPhrase;
  spIter->ulPhraseIdentifier = spPhrase->ulIdentifier;
  spIter->ulIndex = ulIndex;
}
