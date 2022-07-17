#include "Apg.h"
#include "MEGACO.h"
#include <stdio.h>
#include <stdlib.h>

// initializes the semantic callback functions and a corresponding AST
void vCallbackInit(SEM_CALLBACK* spSemantic, ulong* ulpAst);

// initialize the syntactic callback functions with the handwritten optimizing callbacks
void vSyntaxCallbackInit(SYN_CALLBACK* spSyntax, ulong ulLevel);

// copied from ApgUtilities
void vDisplayAST(void* vpCtx, ulong ulLineLen, ulong ulAscii, ulong ulHex);

int main()
{
  ulong        ulRet = APG_FALSE;
  APG_PARSER_STATE sState;
  SEM_CALLBACK saSemantic[RULE_COUNT_MEGACO];
  SYN_CALLBACK saSyntactic[RULE_COUNT_MEGACO];
  ulong        ulLevel = 6;
  ulong        ulaAst[RULE_COUNT_MEGACO];
  uchar*       ucpSrc = (uchar*)"!/1 [172.31.32.110]:2944 P=372799741{C=-{mf=A1{er=435{\"Termination_ID_is_not_in_specified_Context\"}}}}";
  ulong        ulLen = (ulong)strlen((char*)ucpSrc);
  ulong        ulTest;
  void*        vpParserCtx;
  void*        vpMemCtx;

  while(APG_TRUE)
  {
    // get a memory context
    vpMemCtx = vpMemCtor();
    if(!vpMemCtx){break;}

    // initialize the parser
    vpParserCtx = vpParserCtor(vpParserInit_MEGACO, vpMemCtx);
    if(!vpParserCtx){break;}

    // initialize the handwritten, sytactic callback functions
    vSyntaxCallbackInit(saSyntactic, ulLevel);
    ulTest = ulParserSyntaxInit(vpParserCtx, saSyntactic);
    if(!ulTest){break;}

    // initialize the semantic callback functions and the corresponding AST
    vCallbackInit(saSemantic, ulaAst);
    ulTest = ulParserSemanticInit(vpParserCtx, saSemantic);
    if(!ulTest){break;}
    ulTest = ulParserAstInit(vpParserCtx, ulaAst);
    if(!ulTest){break;}

    // parse the message
    ulTest = ulParserSyntaxAnalysis(vpParserCtx, RULE_MEGACO_MEGACOMESSAGE, ucpSrc, ulLen, NULL);
    if(!ulTest){break;}

    // check the state
    ulTest = ulParserState(vpParserCtx, &sState);
    if(!ulTest){break;}
    if(sState.ulState != MATCH){break;}

    // translate the message (capture the parsed phrases)
    ulTest = ulParserSemanticAnalysis(vpParserCtx, NULL);
    if(!ulTest){break;}

    // dump the entire AST
    vDisplayAST(vpParserCtx, 64, APG_TRUE, APG_FALSE);


    // clean up
    vMemDtor(vpMemCtx);

    // success
    ulRet = APG_TRUE;
    break;
  }

  if(!ulRet)
  {
    printf("errors noted\n");
  }
  return 0;
}
