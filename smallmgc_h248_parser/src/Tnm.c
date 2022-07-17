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

   created: 03/26/2009
   purpose: Tree Node Model (TNM)
            Provides direct access Abstract Syntax Tree (AST) nodes.
            Each node in the AST is identified with a unique node ID (NID) .
            Each node contains:
            1) the parsed phrase matched to the node
            2) the NID of its parent
            3) the rule name ID (RID) for the node
            4) the rule name of the node
            5) an iterator over all nodes of the same RID
            6) an iterator over all of its siblings
            7) an iterator over all of its children

*********************************************************************/
#include "Apg.h"
#include "Private.h"
#undef TNMASSERT
#define TNMASSERT(cond) if(!(int)(cond)) {ulMemErrReport(spCtx->vpMemCtx, APG_TNM_ID, __LINE__, __FILE__); goto APG_EXIT;}

static ulong ulChildNodes(APG_TNM_CTX* spCtx, ulong ulParentNID, ulong ulASTNodeIndex);
static void vCountNodes(APG_RULES* spParserRuleList, APG_TNM_RULELIST* spRuleList, ulong ulRuleCount, APG_AST_VALUE* spASTValues, ulong ulNodeCount);
//////////////////////////////////////////////////////////////////////////
// PARSE TREE NODE MODEL  COMPONENT FUNCTIONS
//////////////////////////////////////////////////////////////////////////
void* vpTnmCtor(void* vpParserCtx)
{
  void* vpCtx = NULL;
  APG_PARSER_CTX* spParserCtx = (APG_PARSER_CTX*)vpParserCtx;
  APG_AST_CTX* spAst;
  APG_TNM_CTX* spCtx = NULL;
  APG_RULES* spParserRules;
  APG_RULES* spParserRule;
  APG_TNM_NODE* spNode;
  ulong ulTest;

  if(!(spParserCtx && spParserCtx->vpValidate == vpParserCtx)){goto APG_EXIT;}
  if(!(spParserCtx->vpAstCtx)){goto APG_EXIT;}
  spAst = (APG_AST_CTX*)spParserCtx->vpAstCtx;
  if(!(spAst && spAst->vpValidate == (void*)spAst)){goto APG_EXIT;}
  if(!(spAst->vpVecNodes && spAst->vpVecValues)){goto APG_EXIT;}
  if(!(ulVecSize(spAst->vpVecNodes) > 0) && (ulVecSize(spAst->vpVecValues) > 0)){goto APG_EXIT;}

  // allocate the TNM context
  spCtx = (APG_TNM_CTX*)vpMemAlloc(spParserCtx->vpMemCtx, sizeof(APG_TNM_CTX));
  if(!spCtx){goto APG_EXIT;}
  memset((void*)spCtx, 0, sizeof(APG_TNM_CTX));
  spCtx->vpMemCtx = spParserCtx->vpMemCtx;
  spCtx->sSrc = spParserCtx->sSrc;
  spCtx->ulStartRule = spParserCtx->ulStartRule;
  spCtx->ulRuleCount = spParserCtx->ulRuleCount;
  spCtx->spAstNodes = (APG_AST_NODE*)vpVecFront(spAst->vpVecNodes);
  TNMASSERT(spCtx->spAstNodes);
  spCtx->spAstValues = (APG_AST_VALUE*)vpVecFront(spAst->vpVecValues);
  TNMASSERT(spCtx->spAstValues);
  spCtx->ulNodeCount = ulVecSize(spAst->vpVecValues);
  TNMASSERT(spCtx->ulNodeCount);

  // allocate the rule list
  spCtx->spRuleList = (APG_TNM_RULELIST*)vpMemAlloc(spCtx->vpMemCtx, sizeof(APG_TNM_RULELIST)*spCtx->ulRuleCount);
  TNMASSERT(spCtx->spRuleList);
  memset((void*)spCtx->spRuleList, 0, sizeof(APG_TNM_RULELIST)*spCtx->ulRuleCount);
  spParserRules = (APG_RULES*)spParserCtx->vpRules;

  // allocate the branch list
  spCtx->ulpBranches = (ulong*)vpMemAlloc(spCtx->vpMemCtx, sizeof(ulong)*spCtx->ulNodeCount);
  TNMASSERT(spCtx->ulpBranches);
  memset((void*)spCtx->ulpBranches, 0, sizeof(ulong)*spCtx->ulNodeCount);

  // allocate the node list
  spCtx->spNodes = (APG_TNM_NODE*)vpMemAlloc(spCtx->vpMemCtx, sizeof(APG_TNM_NODE)*spCtx->ulNodeCount);
  TNMASSERT(spCtx->spNodes);
  memset((void*)spCtx->spNodes, 0, sizeof(APG_TNM_NODE)*spCtx->ulNodeCount);

  // count all nodes for each rule in the AST
  vCountNodes(spParserRules, spCtx->spRuleList, spCtx->ulRuleCount, spCtx->spAstValues, spCtx->ulNodeCount);

  // complete the first node
  spCtx->ulNodeIndex = 0;
  spNode = spCtx->spNodes + spCtx->ulNodeIndex;
  spParserRule = spParserRules + spCtx->ulStartRule;
  spNode->cpRuleName = spParserRule->cpRuleName;
  spNode->ulRID = spCtx->ulStartRule;
  spNode->ulNID = spCtx->ulNodeIndex;
  spNode->ulParentNID = UNDEFINED;
  spNode->ulFirstChildNID = UNDEFINED;
  spNode->ulChildCount = 0;
  spNode->ulFirstSiblingNID = spNode->ulNID;
  spNode->ulSiblingCount = 1;
  spNode->sPhrase.ucpStr = spCtx->sSrc.ucpStr + spCtx->spAstValues[0].sPhrase.ulOffset;
  spNode->sPhrase.ulLen = spCtx->spAstValues[0].sPhrase.ulLen;
  spNode->vpValidate = (void*)spNode;

  // initialize the first branch
  spCtx->spRuleList[spCtx->ulStartRule].ulIndex = 0;
  spCtx->ulpBranches[spCtx->spRuleList[spCtx->ulStartRule].ulBranchIndex] = spCtx->ulNodeIndex;
  spCtx->spRuleList[spCtx->ulStartRule].ulIndex++;
  spCtx->ulNodeIndex++;

  // recursively complete all other nodes
  ulTest = ulChildNodes(spCtx, spNode->ulNID, 0);
  TNMASSERT(ulTest);

  // success
  spCtx->vpValidate = (void*)spCtx;
  vpCtx = (void*)spCtx;
APG_EXIT:
  if(!vpCtx && spCtx)
  {
    // release all resources
    if(spCtx->spRuleList){vMemFree(spParserCtx->vpMemCtx, (void*)spCtx->spRuleList);}
    if(spCtx->ulpBranches){vMemFree(spParserCtx->vpMemCtx, (void*)spCtx->ulpBranches);}
    if(spCtx->spNodes){vMemFree(spParserCtx->vpMemCtx, (void*)spCtx->spNodes);}
    memset((void*)spCtx, 0, sizeof(*spCtx));
    vMemFree(spParserCtx->vpMemCtx, (void*)spCtx);
  }
  return vpCtx;
}

void  vTnmDtor(void* vpCtx)
{
  APG_TNM_CTX* spCtx = (APG_TNM_CTX*)vpCtx;
  if(vpCtx && spCtx->vpValidate == vpCtx)
  {
    void* vpMemCtx = spCtx->vpMemCtx;
    // release all resources
    memset(vpCtx, 0, sizeof(APG_TNM_CTX));
    vMemFree(vpMemCtx, vpCtx);
  }
}

// returns a pointer to the first node in the branch list
// creates an iterator over all branch nodes of this rule ID if spIter != NULL
// spIter may be NULL, in which case no iterator is created
APG_TNM_NODE* spTnmRule(void* vpCtx, ulong ulRID, APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  APG_TNM_CTX* spCtx = (APG_TNM_CTX*)vpCtx;
  APG_TNM_RULELIST* spRule;
  TNMASSERT(vpCtx && spCtx->vpValidate == vpCtx);
  TNMASSERT(ulRID < spCtx->ulRuleCount);

  spRule = spCtx->spRuleList + ulRID;
  if(spRule->ulCount > 0)
  {
    if(spIter)
    {
      // build a branch-type iterator
      memset((void*)spIter, 0, sizeof(*spIter));
      spIter->ulType = APG_TNM_ITER_BRANCH;
      spIter->ulFirst = spRule->ulBranchIndex;
      spIter->ulIndex = 0;
      spIter->ulCount = spRule->ulCount;
      spIter->ulpBranches = spCtx->ulpBranches;
      spIter->spNodes = spCtx->spNodes;
      spIter->vpValidate = (void*)spIter;
    }

    // return the first node in the branch list
    spRet = spCtx->spNodes + spCtx->ulpBranches[spRule->ulBranchIndex];
  }
APG_EXIT:
  return spRet;
}

// create an iterator over all branch nodes for the rule ID of this node
// returns a pointer to the first node in the branch list
// this is the same spTnmRule using spNode->ulRID as the RID
APG_TNM_NODE* spTnmBranches(void* vpCtx, APG_TNM_NODE* spNode, APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  APG_TNM_CTX* spCtx = (APG_TNM_CTX*)vpCtx;
  APG_TNM_RULELIST* spRule;
  TNMASSERT(vpCtx && spCtx->vpValidate == vpCtx);
  TNMASSERT(spNode && spNode->vpValidate == (void*)spNode);
  memset((void*)spIter, 0, sizeof(*spIter));

  // build a branch-type iterator
  spIter->ulType = APG_TNM_ITER_BRANCH;
  spRule = spCtx->spRuleList + spNode->ulRID;
  spIter->ulFirst = spRule->ulBranchIndex;
  spIter->ulIndex = 0;
  spIter->ulCount = spRule->ulCount;
  spIter->ulpBranches = spCtx->ulpBranches;
  spIter->spNodes = spCtx->spNodes;
  spIter->vpValidate = (void*)spIter;

  // return the first node in the branch list
  spRet = spCtx->spNodes + spCtx->ulpBranches[spRule->ulBranchIndex];
APG_EXIT:
  return spRet;
}

// create an iterator over all children for the given node
// returns a pointer to the first child node
APG_TNM_NODE* spTnmChildren(void* vpCtx, APG_TNM_NODE* spNode, APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  APG_TNM_CTX* spCtx = (APG_TNM_CTX*)vpCtx;
  TNMASSERT(vpCtx && spCtx->vpValidate == vpCtx);
  TNMASSERT(spNode && spNode->vpValidate == (void*)spNode);
  memset((void*)spIter, 0, sizeof(*spIter));

  if(spNode->ulChildCount)
  {
    // build a node-type iterator
    spIter->ulType = APG_TNM_ITER_NODE;
    spIter->ulFirst = spNode->ulFirstChildNID;
    spIter->ulIndex = 0;
    spIter->ulCount = spNode->ulChildCount;
    spIter->ulpBranches = spCtx->ulpBranches;
    spIter->spNodes = spCtx->spNodes;
    spIter->vpValidate = (void*)spIter;

    // return the first node in the branch list
    spRet = spCtx->spNodes + spIter->ulFirst;
  }
APG_EXIT:
  return spRet;
}

// create an iterator over all siblings for the given node
// returns a pointer to the first sibling node
APG_TNM_NODE* spTnmSiblings(void* vpCtx, APG_TNM_NODE* spNode, APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  APG_TNM_CTX* spCtx = (APG_TNM_CTX*)vpCtx;
  TNMASSERT(vpCtx && spCtx->vpValidate == vpCtx);
  TNMASSERT(spNode && spNode->vpValidate == (void*)spNode);
  memset((void*)spIter, 0, sizeof(*spIter));

  if(spNode->ulSiblingCount)
  {
    // build a node-type iterator
    spIter->ulType = APG_TNM_ITER_NODE;
    spIter->ulFirst = spNode->ulFirstSiblingNID;
    spIter->ulIndex = 0;
    spIter->ulCount = spNode->ulSiblingCount;
    spIter->ulpBranches = spCtx->ulpBranches;
    spIter->spNodes = spCtx->spNodes;
    spIter->vpValidate = (void*)spIter;

    // return the first node in the branch list
    spRet = spCtx->spNodes + spIter->ulFirst;
  }
APG_EXIT:
  return spRet;
}

// returns a pointer to the parent node
APG_TNM_NODE* spTnmParent(void* vpCtx, APG_TNM_NODE* spNode)
{
  APG_TNM_NODE* spRet = NULL;
  APG_TNM_CTX* spCtx = (APG_TNM_CTX*)vpCtx;
  TNMASSERT(vpCtx && spCtx->vpValidate == vpCtx);
  TNMASSERT(spNode && spNode->vpValidate == (void*)spNode);

  spRet = spCtx->spNodes + spNode->ulParentNID;
APG_EXIT:
  return spRet;
}

// returns a pointer to the first node in the list
// returns NULL if iterator is invalid
APG_TNM_NODE* spTnmFirst(APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  DASSERT(spIter && spIter->vpValidate == (void*)spIter);
  if(spIter && spIter->vpValidate == (void*)spIter)
  {
    spIter->ulIndex = 0;
    switch(spIter->ulType)
    {
      case APG_TNM_ITER_BRANCH:
      {
        spRet = spIter->spNodes + spIter->ulpBranches[spIter->ulFirst];
      }
      break;

      case APG_TNM_ITER_NODE:
      spRet = spIter->spNodes + spIter->ulFirst;
      break;

      default:
      DASSERT(APG_TRUE);
      break;
    }
  }
  return spRet;
}

// returns a pointer to the last node in the list
// returns NULL if iterator is invalid
APG_TNM_NODE* spTnmLast(APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  DASSERT(spIter && spIter->vpValidate == (void*)spIter);
  if(spIter && spIter->vpValidate == (void*)spIter)
  {
    spIter->ulIndex = spIter->ulCount - 1;
    switch(spIter->ulType)
    {
      case APG_TNM_ITER_BRANCH:
      {
        spRet = spIter->spNodes + spIter->ulpBranches[spIter->ulFirst + spIter->ulIndex];
      }
      break;

      case APG_TNM_ITER_NODE:
      spRet = spIter->spNodes + spIter->ulFirst + spIter->ulIndex;
      break;

      default:
      DASSERT(APG_TRUE);
      break;
    }
  }
  return spRet;
}

// returns a pointer to the next node in the list
// returns NULL at end of list, or if iterator is invalid
APG_TNM_NODE* spTnmNext(APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  DASSERT(spIter && spIter->vpValidate == (void*)spIter);
  if(spIter && spIter->vpValidate == (void*)spIter)
  {
    if(spIter->ulIndex + 1 < spIter->ulCount)
    {
      ++spIter->ulIndex;
      switch(spIter->ulType)
      {
        case APG_TNM_ITER_BRANCH:
        {
          spRet = spIter->spNodes + spIter->ulpBranches[spIter->ulFirst + spIter->ulIndex];
        }
        break;

        case APG_TNM_ITER_NODE:
        spRet = spIter->spNodes + spIter->ulFirst + spIter->ulIndex;
        break;

        default:
        DASSERT(APG_TRUE);
        break;
      }
    }
  }
  return spRet;
}

// returns a pointer to the previous node in the list
// returns NULL at beginning of list, or if iterator is invalid
APG_TNM_NODE* spTnmPrev(APG_TNM_ITER* spIter)
{
  APG_TNM_NODE* spRet = NULL;
  DASSERT(spIter && spIter->vpValidate == (void*)spIter);
  if(spIter && spIter->vpValidate == (void*)spIter)
  {
    if(spIter->ulIndex > 0)
    {
      --spIter->ulIndex;
      switch(spIter->ulType)
      {
        case APG_TNM_ITER_BRANCH:
        {
          spRet = spIter->spNodes + spIter->ulpBranches[spIter->ulFirst + spIter->ulIndex];
        }
        break;

        case APG_TNM_ITER_NODE:
        spRet = spIter->spNodes + spIter->ulFirst + spIter->ulIndex;
        break;

        default:
        DASSERT(APG_TRUE);
        break;
      }
    }
  }
  return spRet;
}

// generate the child nodes in the node list for the given node
// ulASTNodeIndex - index of the AST node for which the child list is to be generated
static ulong ulChildNodes(APG_TNM_CTX* spCtx, ulong ulParentNID, ulong ulASTNodeIndex)
{
  ulong ulRet = APG_FALSE;
  APG_AST_VALUE* spASTValue;
  APG_AST_NODE* spASTNode;
  APG_AST_NODE* spASTNodeEnd;
  APG_AST_NODE* spASTNodeParent;
  APG_TNM_NODE* spNode;
  APG_TNM_NODE* spNodeParent;
  APG_TNM_NODE* spNodeFirstSibling;
  APG_TNM_RULELIST* spRule;
  ulong ulBranchIndex;
  ulong ulTempIndex;
  while(APG_TRUE)
  {
    // does this AST node have any children?
    spASTNodeParent = spCtx->spAstNodes + ulASTNodeIndex;
    if(spASTNodeParent->ulOtherIndex == ulASTNodeIndex + 1)
    {
      // this is a leaf node - no children to process
      ulRet = APG_TRUE;
      break;
    }

    // for each child of the AST node, add a member to the branch list
    spNodeParent = spCtx->spNodes + ulParentNID;
    spASTNode = spASTNodeParent + 1;
    spASTNodeEnd = spCtx->spAstNodes + spASTNodeParent->ulOtherIndex - 1;
    ulTempIndex = spCtx->ulNodeIndex;
    while(spASTNode < spASTNodeEnd)
    {
      // validate that is is a "down" node
      DASSERT(spASTNode->ulDown);

      // add the node to the branch list
      spASTValue = spCtx->spAstValues + spASTNode->ulValueIndex;
      spRule = spCtx->spRuleList + spASTValue->ulRuleIndex;
      ulBranchIndex = spRule->ulBranchIndex + spRule->ulIndex;
      spCtx->ulpBranches[ulBranchIndex] = ulTempIndex;
      spRule->ulIndex++;
      ulTempIndex++;
      spASTNode = spCtx->spAstNodes + spASTNode->ulOtherIndex + 1;
    }

    // for each child of the AST node, add a node to the TNM node list
    ulTempIndex = spCtx->ulNodeIndex;
    spASTNode = spASTNodeParent + 1;
    while(spASTNode < spASTNodeEnd)
    {
      // add the node to the node list
      spASTValue = spCtx->spAstValues + spASTNode->ulValueIndex;
      spRule = spCtx->spRuleList + spASTValue->ulRuleIndex;
      spNode = spCtx->spNodes + spCtx->ulNodeIndex;
      spNode->cpRuleName = spRule->cpRuleName;
      spNode->ulRID = spASTValue->ulRuleIndex;
      spNode->ulNID = spCtx->ulNodeIndex;
      spNode->ulParentNID = spNodeParent->ulNID;
      spNode->ulFirstChildNID = UNDEFINED;
      spNode->ulChildCount = 0;
      spNode->sPhrase.ucpStr = spCtx->sSrc.ucpStr + spASTValue->sPhrase.ulOffset;
      spNode->sPhrase.ulLen = spASTValue->sPhrase.ulLen;
      if(spASTNode == spASTNodeParent + 1)
      {
        // special handling of the first child
        spNodeParent->ulFirstChildNID = spNode->ulNID;
        spNodeParent->ulChildCount = 1;
        spNodeFirstSibling = spNode;
        spNode->ulFirstSiblingNID = spNode->ulNID;
        spNode->ulSiblingCount = 1;
      }
      else
      {
        spNodeParent->ulChildCount++;
        spNode->ulFirstSiblingNID = spNodeFirstSibling->ulNID;
        spNodeFirstSibling->ulSiblingCount++;
        spNode->ulSiblingCount = UNDEFINED;
      }

      spNode->vpValidate = (void*)spNode;
      spCtx->ulNodeIndex++;
      spASTNode = spCtx->spAstNodes + spASTNode->ulOtherIndex + 1;
    }

    // evaluate the children of each child
    spASTNode = spASTNodeParent + 1;
    spNode = spCtx->spNodes + ulTempIndex;
    while(spASTNode < spASTNodeEnd)
    {
      APG_AST_NODE* spASTPairedNode = spCtx->spAstNodes + spASTNode->ulOtherIndex;
      ulChildNodes(spCtx, spNode->ulNID, spASTPairedNode->ulOtherIndex);
      spNode++;
      spASTNode = spCtx->spAstNodes + spASTNode->ulOtherIndex + 1;
    }

    // success
    ulRet = APG_TRUE;
    break;
  }
  return ulRet;
}

// count the number of nodes for each rule
static void vCountNodes(APG_RULES* spParserRuleList, APG_TNM_RULELIST* spRuleList, ulong ulRuleCount, APG_AST_VALUE* spASTValues, ulong ulNodeCount)
{
  ulong ulRuleIndex, ulBranchIndex;
  APG_AST_VALUE* spNext = spASTValues;
  APG_AST_VALUE* spEnd = spNext + ulNodeCount;
  for(; spNext < spEnd; ++spNext)
  {
    spRuleList[spNext->ulRuleIndex].ulCount++;
  }

  // partition the branch list
  ulBranchIndex = 0;
  for(ulRuleIndex = 0; ulRuleIndex < ulRuleCount; ulRuleIndex++)
  {
    if(spRuleList[ulRuleIndex].ulCount)
    {
      spRuleList[ulRuleIndex].cpRuleName = spParserRuleList[ulRuleIndex].cpRuleName;
      spRuleList[ulRuleIndex].ulBranchIndex = ulBranchIndex;
      ulBranchIndex += spRuleList[ulRuleIndex].ulCount;
    }
  }
}

/*
// DEBUGGING
#include <stdio.h>
static ulong ulDumpRuleList(APG_TNM_CTX* spCtx)
{
  ulong ulRet = APG_FALSE;
  ulong ulRuleIndex;
  APG_TNM_RULELIST* spRuleList = spCtx->spRuleList;
  while(APG_TRUE)
  {
    // partition the branch list
    printf("\n");
    printf("*** TNM - DUMP RULE LIST ***\n");
    for(ulRuleIndex = 0; ulRuleIndex < spCtx->ulRuleCount; ulRuleIndex++)
    {
      if(spRuleList[ulRuleIndex].ulCount)
      {
        printf("   rule name[%lu]: %s\n", ulRuleIndex, spRuleList[ulRuleIndex].cpRuleName);
        printf("   branch index: %lu\n",spRuleList[ulRuleIndex].ulBranchIndex);
        printf("iteration index: %lu\n",spRuleList[ulRuleIndex].ulIndex);
        printf("          count: %lu\n",spRuleList[ulRuleIndex].ulCount);
        printf("\n");
      }
    }

    // success
    ulRet = APG_TRUE;
    break;
  }
  return ulRet;
}

static ulong ulDumpBranches(APG_TNM_CTX* spCtx)
{
  ulong ulRet = APG_FALSE;
  ulong ulRuleIndex;
  ulong ulIndex;
  ulong* ulpBranches = spCtx->ulpBranches;
  APG_TNM_RULELIST* spRuleList = spCtx->spRuleList;
  while(APG_TRUE)
  {
    // partition the branch list
    printf("\n");
    printf("*** TNM - DUMP BRANCH LIST  ***\n");
    for(ulRuleIndex = 0; ulRuleIndex < spCtx->ulRuleCount; ulRuleIndex++)
    {
      if(spRuleList[ulRuleIndex].ulCount)
      {
        printf("   rule name[%lu]: %s\n", ulRuleIndex, spRuleList[ulRuleIndex].cpRuleName);
        printf("   branch index: %lu\n",spRuleList[ulRuleIndex].ulBranchIndex);
        for(ulIndex = 0; ulIndex < spRuleList[ulRuleIndex].ulCount; ulIndex++)
        {
          ulong ul = spRuleList[ulRuleIndex].ulBranchIndex + ulIndex;
          printf("     node index[%lu]: %lu\n", ul,ulpBranches[ul]);
        }
        printf("\n");
      }
    }

    // success
    ulRet = APG_TRUE;
    break;
  }
  return ulRet;
}

static ulong ulDumpNodes(APG_TNM_CTX* spCtx)
{
  ulong ulRet = APG_FALSE;
  ulong ulIndex;
  APG_TNM_NODE* spNode;
  char caBuffer[1024];
  while(APG_TRUE)
  {
    // partition the branch list
    printf("\n");
    printf("*** TNM - DUMP NODES ***\n");
    spNode = spCtx->spNodes;
    for(ulIndex = 0; ulIndex < spCtx->ulNodeCount; ulIndex++)
    {
      ulDumpNode(spNode);
      spNode++;
    }

    // success
    ulRet = APG_TRUE;
    break;
  }
  return ulRet;
}

static ulong ulDumpNode(APG_TNM_NODE* spNode)
{
  ulong ulRet = APG_FALSE;
  char caBuffer[1024];
  while(APG_TRUE)
  {
    {
      printf("rule name: %s\n", spNode->cpRuleName);
      printf("rule index: %lu\n", spNode->ulRID);
      printf("node index: %lu\n", spNode->ulNID);
      printf("parent index: %lu\n", spNode->ulParentNID);
      printf("first child index: %lu\n", spNode->ulFirstChildNID);
      printf("child count: %lu\n", spNode->ulChildCount);
      printf("first sibling index: %lu\n", spNode->ulFirstSiblingNID);
      printf("sibling count: %lu\n", spNode->ulSiblingCount);
      memcpy((void*)caBuffer, (void*)spNode->sPhrase.ucpStr, spNode->sPhrase.ulLen);
      caBuffer[spNode->sPhrase.ulLen] = 0;
      printf("phrase: %s\n", caBuffer);
      printf("\n");
    }

    // success
    ulRet = APG_TRUE;
    break;
  }
  return ulRet;
}
*/
