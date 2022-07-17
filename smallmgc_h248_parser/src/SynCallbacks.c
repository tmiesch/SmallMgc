#include "Apg.h"
#include "MEGACO.h"

/***  SYNTAX CALLBACK FUNCTION STARTER KIT ***
// 1) cut and paste to a new file, e.g. MySyntaxCallbacks.c
// 2) complete for all rules for which syntax call back functions are used

// syntax call back function template - example for rule RULE_MEGACODEV_MEGACOMESSAGE
static ulong syn_megacoMessage(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen);
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;

  // unparsed string suffix length
  ulong ulLen = spData->ulSrcLen - ulOffset;
  if(*ulpState == SYN_PRE)
  {
  }
  return ulRet;
}
*********************************************/
// some special ASCII character defines
#undef  HTAB
#undef  LF
#undef  CR
#undef  SP
#undef  PERIOD
#undef  SLASH
#undef  COLON
#undef  SEMI
#undef  BACKSLASH
#undef  BUFSIZE
#define HTAB      9
#define LF        10
#define CR        13
#define SP        32
#define PERIOD    46
#define SLASH     47
#define COLON     58
#define SEMI      59
#define BACKSLASH 92

//#define HTAB      ((uchar)9)
//#define LF        ((uchar)10)
//#define CR        ((uchar)13)
//#define SP        ((uchar)32)
#define COMMA     ((uchar)44)
//#define PERIOD    ((uchar)46)
//#define SLASH     ((uchar)47)
//#define COLON     ((uchar)58)
//#define SEMI      ((uchar)59)
#define EQUAL     ((uchar)61)
//#define BACKSLASH ((uchar)92)
#define LBRKT     ((uchar)123)
#define RBRKT     ((uchar)125)

#define is_wsp(c) ((c)==SP || (c)==HTAB)
#define is_eol(c) ((c)==10 || (c)==13)
#define is_digit(c) ((c) >= 48 && (c) <= 57)
#define is_alphaupper(c) ((c) >= 65 && (c) <= 90)
#define is_alphalower(c) ((c) >= 97 && (c) <= 122)
#define is_alpha(c) (is_alphaupper(c) || is_alphalower(c))
#define is_comment_char(c) (is_safe_char(c) || is_rest_char(c) || (c)==HTAB || (c)==SP || (c)==22)
#define is_safe_char(c) ((c)==33|| ((c)>=36 && (c)<=43) || ((c)>=45 && (c)<=57) || ((c)>=63 && (c)<=90) || ((c)>=94 && (c)<=122) || (c)==92 || (c)==124 || (c)==126)
#define is_rest_char(c) ((c)==35 || (c)==44 || ((c)>=58 && (c)<=62) || (c)==91 || (c)==93 || (c)==123 ||  (c)==125)

static ulong get_comment(uchar* cp, uchar* end);
static ulong get_wsp(uchar* cp, uchar* end);
static ulong get_wsp_char(uchar* cp, uchar* end, uchar ucChar);
static ulong get_nonescape_chars(uchar* ucpBeg, uchar* ucpEnd);
static ulong get_quoted_string(uchar* ucpBeg, uchar* ucpEnd);

///////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
// quotedString  = %x22 *(SafeChar / EOL / %x80-FF / RestChar / WSP) %x22
static ulong get_quoted_string(uchar* ucpBeg, uchar* ucpEnd)
{
  ulong ulRet = 0;
  ulong ulChars = 0;
  ulong ulChar;

  while(APG_TRUE)
  {
    if(*ucpBeg != 0x22){break;}
    ucpBeg++;
    ulChars++;
    while(ucpBeg < ucpEnd)
    {
      ulChar = (ulong)*ucpBeg;
      if(is_safe_char(ulChar) || is_rest_char(ulChar) || is_wsp(ulChar) || is_eol(ulChar)){ulChars++; ucpBeg++;}
      else if(ulChar >= 0x80 && ulChar <= 0xff){ulChars++; ucpBeg++;}
      else{break;}
    }
    if(*ucpBeg != 0x22){break;}

    // success
    ulRet = ulChars + 1;
    break;
  }

  return ulRet;
}

static ulong get_nonescape_chars(uchar* ucpBeg, uchar* ucpEnd)
{
  ulong ulChars = 0;
  ulong ucChar;

//octetString         = *(nonEscapeChar)
//nonEscapeChar       = ("\}" / %x01-7C / %x7E-FF)
  while(APG_TRUE)
  {
    ucChar = *ucpBeg;
    if(ucChar > 0 && ucChar < 0x5c){ucpBeg++; ulChars++;}
    else if(ucChar > 0x5c && ucChar < 0x7d){ucpBeg++; ulChars++;}
    else if(ucChar > 0x7d && ucChar <= 0xff){ucpBeg++; ulChars++;}
    else if(ucChar == 0x5c && ucpBeg + 1 < ucpEnd && ucpBeg[1] == 0x7d){ucpBeg+=2; ulChars+=2;}
    else {break;}
  }

  return ulChars;
}

// find a character with optional white space before and after
// return - 0 if character not found
//        - > 0 equals the number of characters if found
static ulong get_wsp_char(uchar* cp, uchar* end, uchar ucChar)
{
  ulong thischars = 0;
  ulong allchars = 0;
  uchar c;

  while(APG_TRUE)
  {
    // find white space before the character
    while(cp < end)
    {
      c = *cp;
      if(c==SP||c==HTAB||c==LF||c==CR){thischars++;}
      else if(c==SEMI){thischars=get_comment(cp, end);}
      if(thischars == 0){break;}
      allchars += thischars;
      cp += thischars;
      thischars = 0;
    }

    // find the character
    if(*cp != ucChar){allchars = 0; break;}
    cp++;
    allchars++;
    thischars = 0;

    // find white space after the character
    while(cp < end)
    {
      c = *cp;
      if(c==SP||c==HTAB||c==LF||c==CR){thischars++;}
      else if(c==SEMI){thischars=get_comment(cp, end);}
      if(thischars == 0){break;}
      allchars += thischars;
      cp += thischars;
      thischars = 0;
    }

    break;
  }

  return allchars;
}

// return - 0 if not a comment
//        - > 0 equals the number of characters in the comment
static ulong get_comment(uchar* cp, uchar* end)
{
  ulong ret = 0;
  uchar c;
  while(APG_TRUE)
  {
    if((cp == NULL) || (cp >= end) || (*cp!=SEMI)){break;}
    ret++;
    for(++cp; cp < end; ++cp)
    {
      c = *cp;
      if(is_comment_char(c)){ret++;}
      else
      {
        if(c==LF){ret++; break;} // LF is a valid line end
        if(c==CR){ret++; break;} // LF is a valid line end

        // if none of the above, not a valid comment
        ret=0;break;
      }
    }
    break;
  }
  return ret;
}

static ulong get_wsp(uchar* cp, uchar* end)
{
  ulong thischars = 0;
  ulong allchars = 0;
  uchar c;
  while(cp < end)
  {
    c = *cp;
    if(c==SP||c==HTAB||c==LF||c==CR){thischars++;}
    else if(c==SEMI){thischars=get_comment(cp, end);}
    if(thischars == 0){break;}
    allchars += thischars;
    cp += thischars;
    thischars = 0;
  }
  return allchars;
}

///////////////////////////////////////////////////////////////////////////////
// HANDWRITTEN SYNTAX CALL BACK FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
static ulong syn_domainAddress(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  uchar* ucpNext = ucpSrc;
  ulong ulChars = 0;
  ulong ulRetChars = 0;
  ulong ulType = 0;
  if(*ulpState == SYN_PRE)
  {
    while(ucpNext < ucpEnd)
    {
      if(*ucpNext != '['){break;}
      ulChars++;
      ucpNext++;
      while(ucpNext < ucpEnd && *ucpNext != ']')
      {
        if(ulType == 0)
        {
          if(*ucpNext == ':'){ulType = 6;}
          if(*ucpNext == '.'){ulType = 4;}
        }
        ulChars++;
        ucpNext++;
      }
      if(*ucpNext != ']'){break;}
      ulChars++;
      if(ulType == 0){break;}
//      if(ulType == 4)
//      {
//        // push an IPv4Address on the AST
//        ulong ulOffset = ucpSrc + 1 - spData->ucpSrc;
//        ulong ulTest = ulAstPushChildNode(spData, RULE_MEGACO_IPV4ADDRESS, ulOffset, ulChars - 2);
//        if(!ulTest){ulRet = PFN_ERROR;}
//      }
//      else if(ulType == 6)
//      {
//        // push an IPv6Address on the AST
//        ulong ulOffset = ucpSrc + 1 - spData->ucpSrc;
//        ulong ulTest = ulAstPushChildNode(spData, RULE_MEGACO_IPV6ADDRESS, ulOffset, ulChars - 2);
//        if(!ulTest){ulRet = PFN_ERROR;}
//      }

      // success
      ulRetChars = ulChars;
      break;
    }
    *ulpLen = ulRetChars;
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_octetString(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_nonescape_chars(ucpSrc, ucpEnd);
    *ulpState = (*ulpLen == 0) ? SYN_EMPTY : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_NAME(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  uchar* ucpNext = ucpSrc;
  ulong ulTest;
  ulong ulChars = 0;
  ulong ulRetChars = 0;
  if(*ulpState == SYN_PRE)
  {
    ulTest = *ucpNext;
    if(!is_alpha(ulTest)){goto APG_EXIT;}
    ulChars++;
    ucpNext++;
    while(ucpNext < ucpEnd)
    {
      ulTest = (ulong)*ucpNext;
      if(is_alpha(ulTest) || is_digit(ulTest) || ulTest == (ulong)0x5f){ulChars++; ucpNext++;}
      else{break;}
      if(ulChars >= 64){break;}
    }

    // success
    ulRetChars = ulChars;
APG_EXIT:
    *ulpLen = ulRetChars;
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_UINT32(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  uchar* ucpNext = ucpSrc;
  ulong ulTest;
  ulong ulChars = 0;
  ulong ulRetChars = 0;
  if(*ulpState == SYN_PRE)
  {
    while(ucpNext < ucpEnd)
    {
      ulTest = (ulong)*ucpNext;
      if(is_digit(ulTest)){ulChars++; ucpNext++;}
      else{break;}
      if(ulChars >= 10){break;}
    }

    // success
    ulRetChars = ulChars;
    *ulpLen = ulRetChars;
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_UINT16(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  uchar* ucpNext = ucpSrc;
  ulong ulTest;
  ulong ulChars = 0;
  ulong ulRetChars = 0;
  if(*ulpState == SYN_PRE)
  {
    while(ucpNext < ucpEnd)
    {
      ulTest = (ulong)*ucpNext;
      if(is_digit(ulTest)){ulChars++; ucpNext++;}
      else{break;}
      if(ulChars >= 5){break;}
    }

    // success
    ulRetChars = ulChars;
    *ulpLen = ulRetChars;
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_LWSP(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp(ucpSrc, ucpEnd);
    *ulpState = (*ulpLen == 0) ? SYN_EMPTY : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_SEP(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp(ucpSrc, ucpEnd);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_EQUAL(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp_char(ucpSrc, ucpEnd, EQUAL);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_COMMA(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp_char(ucpSrc, ucpEnd, COMMA);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_SLASH(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp_char(ucpSrc, ucpEnd, SLASH);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

// syntax call back initialization function
static ulong syn_LBRKT(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp_char(ucpSrc, ucpEnd, LBRKT);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

// syntax call back initialization function
static ulong syn_RBRKT(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_wsp_char(ucpSrc, ucpEnd, RBRKT);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

static ulong syn_quotedString(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  if(*ulpState == SYN_PRE)
  {
    *ulpLen = get_quoted_string(ucpSrc, ucpEnd);
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

//VALUE         = quotedString / 1*(SafeChar / %x80-FF)
static ulong syn_value(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  // pointer to unparsed string suffix
  uchar* ucpSrc = spData->ucpSrc + ulOffset;
  uchar* ucpEnd = ucpSrc + spData->ulSrcLen;
  ulong ulChars = 0;
  ulong ulRetChars = 0;
  ulong ulChar;
  if(*ulpState == SYN_PRE)
  {
    while(APG_TRUE)
    {
      ulChars = get_quoted_string(ucpSrc, ucpEnd);
      if(ulChars == 0)
      {
        while(ucpSrc < ucpEnd)
        {
          ulChar = (ulong)*ucpSrc;
          if(is_safe_char(ulChar) || (ulChar >= 0x80 && ulChar <= 0xff)){ulChars++; ucpSrc++;}
          else{break;}
        }
        if(ulChars == 0){break;}
      }

      // success
      ulRetChars = ulChars;
      break;
    }
    *ulpLen = ulRetChars;
    *ulpState = (*ulpLen == 0) ? SYN_NOMATCH : SYN_MATCH;
  }
  return ulRet;
}

// syntax call back initialization function
void vSyntaxCallbackInit(SYN_CALLBACK* spSyntax, ulong ulLevel)
{
  memset((void*)spSyntax, 0, sizeof(SYN_CALLBACK) * RULE_COUNT_MEGACO);

  // each level includes all callback functions from the lower levels as well
  // eg. ulLevel = 4 will include levels 1, 2, and 3 as well as 4
  switch(ulLevel)
  {
    case 6:
    spSyntax[RULE_MEGACO_QUOTEDSTRING] = syn_quotedString;
    spSyntax[RULE_MEGACO_VALUE] = syn_value;
    case 5:
    spSyntax[RULE_MEGACO_UINT16] = syn_UINT16;
    spSyntax[RULE_MEGACO_UINT32] = syn_UINT32;
    case 4:
    spSyntax[RULE_MEGACO_NAME] = syn_NAME;
    case 3:
    spSyntax[RULE_MEGACO_DOMAINADDRESS] = syn_domainAddress;
    case 2:
    spSyntax[RULE_MEGACO_OCTETSTRING] = syn_octetString;
    case 1: // lowest level improvement - white space
    spSyntax[RULE_MEGACO_LWSP]  = syn_LWSP;
    spSyntax[RULE_MEGACO_SEP]   = syn_SEP;
    spSyntax[RULE_MEGACO_EQUAL] = syn_EQUAL;
    spSyntax[RULE_MEGACO_SLASH] = syn_SLASH;
    spSyntax[RULE_MEGACO_COMMA] = syn_COMMA;
    spSyntax[RULE_MEGACO_LBRKT] = syn_LBRKT;
    spSyntax[RULE_MEGACO_RBRKT] = syn_RBRKT;
    break;
    case 0: // no handwritten call back functions
    default:
    break;
  }
}

void vAstInit_MEGACO(ulong* ulpAst)
{
  ulong ul;
  memset((void*)ulpAst, 0, sizeof(ulong) * RULE_COUNT_MEGACO);
  for(ul = 0; ul < RULE_COUNT_MEGACO; ul++)
  {
    ulpAst[ul] = APG_TRUE;
  }

}



