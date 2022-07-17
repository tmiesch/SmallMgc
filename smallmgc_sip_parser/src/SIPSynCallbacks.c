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

	 created: Sat Nov 24 14:46:59 2007
	filename: SIP.c
	  author: Lowell Thomas
	          lowell@coasttocoastresearch.com
	          http://www.coasttocoastresearch.com

	 purpose: Implementation of parser component SIP.

*********************************************************************/
#include "Apg.h"
#include "SIP.h"

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_HYPHEN(c) ((c) == '-')
#define IS_ALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define IS_ALPHANUM(c) (IS_DIGIT(c) || IS_ALPHA(c))
#define IS_ALPHAHYPHEN(c) (IS_DIGIT(c) || IS_ALPHA(c) || IS_HYPHEN(c))
#define IS_HEX(c) (IS_DIGIT(c) || (((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F')))
#define IS_USER_UNRESERVED(c) ((c)==36||(c)==38||(c)==43||(c)==44||(c)==47||(c)==59||(c)==61||(c)==63)
#define IS_PARAM_UNRESERVED(c) ((c)==36||(c)==38||(c)==43||(c)==47||(c)==58||(c)==91||(c)==93)
#define IS_RESERVED(c) ((c)==36||(c)==38||(c)==43||(c)==44||(c)==47||(c)==58||(c)==59||(c)==61||(c)==63||(c)==64)
#define IS_MARK(c) ((c)==33||(c)==39||(c)==40||(c)==41||(c)==42||(c)==45||(c)==46||(c)==95||(c)==126)
#define IS_UNRESERVED(c) (IS_ALPHANUM(c)||IS_MARK(c))
#define IS_TOKEN(c) (IS_ALPHANUM(c)||(c)=='-'||(c)=='.'||(c)=='!'||(c)=='%'||(c)=='*'||(c)=='_'||(c)=='+'||(c)=='`'||(c)=='\''||(c)=='~')
#define IS_WORD0(c) ((c)==33||(c)==34||(c)==37||(c)==39||(c)==40||(c)==41||(c)==42||(c)==43||(c)==45||(c)==46||(c)==47)
#define IS_WORD1(c) ((c)==58||(c)==60||(c)==62||(c)==63||(c)==91||(c)==92||(c)==93||(c)==95||(c)==96||(c)==123||(c)==125||(c)==126)
#define IS_WORD(c) (IS_ALPHANUM(c)||IS_WORD0(c)||IS_WORD1(c))
#define IS_WSP(c) ((c) == 32 || (c) == 9)
#define IS_UTF8_CONT(c) ((c)>=0x80 && (c)<=0xBF)
#define IS_ESCAPED(p) ((ulong)(p)[0] == '%' && IS_HEX((ulong)(p)[1]) && IS_HEX((ulong)(p)[2]))
#define IS_LINE_END(p) ((ulong)(p)[0] == 13 && (ulong)(p)[1] == 10)
#define IS_LINE_CONTINUE(p) (IS_LINE_END(p) && IS_WSP(p[2]))
#define IS_UTF8_NONASCII1(p) (((ulong)(p)[0]>=0xC0 && (ulong)(p)[0]<=0xDF) && IS_UTF8_CONT((ulong)(p)[1]))
#define IS_UTF8_NONASCII2(p) ((((ulong)(p)[0])>=0xE0 && ((ulong)(p)[0])<=0xEF) && IS_UTF8_CONT((ulong)(p)[1])&& IS_UTF8_CONT((ulong)(p)[2]))
#define IS_UTF8_NONASCII3(p) ((((ulong)(p)[0])>=0xF0 && ((ulong)(p)[0])<=0xF7) && IS_UTF8_CONT((ulong)(p)[1])&& IS_UTF8_CONT((ulong)(p)[2])&& IS_UTF8_CONT((ulong)(p)[3]))
#define IS_UTF8_NONASCII4(p) ((((ulong)(p)[0])>=0xF8 && ((ulong)(p)[0])<=0xFB) && IS_UTF8_CONT((ulong)(p)[1])&& IS_UTF8_CONT((ulong)(p)[2])&& IS_UTF8_CONT((ulong)(p)[3])&& IS_UTF8_CONT((ulong)(p)[4]))
#define IS_UTF8_NONASCII5(p) ((((ulong)(p)[0])>=0xFC && ((ulong)(p)[0])<=0xFd) && IS_UTF8_CONT((ulong)(p)[1])&& IS_UTF8_CONT((ulong)(p)[2])&& IS_UTF8_CONT((ulong)(p)[3])&& IS_UTF8_CONT((ulong)(p)[4])&& IS_UTF8_CONT((ulong)(p)[5]))
#define IS_QUOTED_PAIR(p) ((char)(p)[0]=='\\' && (((char)(p)[1]>=0x00&&(uchar)(p)[1]<=0x09) || ((uchar)(p)[1]>=0x0B&&(uchar)(p)[1]<=0x0C) || ((uchar)(p)[1]>=0x0E&&(uchar)(p)[1]<=0x7F)))

// default syntax initialization function (no syntax call back functions)
// if spUserBuffer is not NULL it must be at least RULE_COUNT_SIP elements in length
// if spUserBuffer is NULL a static buffer is used
// spSyntax[] must be one of:
// - NULL (0):
//     *NO* call back function called by ulParserSyntaxAnalysis()
// - a valid SYN_CALLBACK function pointer:
//     the call back function *IS* called by ulParserSyntaxAnalysis()
// - anything else:
//     unpredictable behaviour in ulParserSyntaxAnalysis()

// syntax call back switch template
/*
static ulong syn_XXX(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
    }
  }
  return ulRet;
}
*/

static ulong ulNonAscii(uchar* ucpSrc, ulong ulLenEnd)
{
  ulong ulLen = 0;
  if(ulLen + 1 < ulLenEnd && IS_UTF8_NONASCII1(ucpSrc)){ulLen += 2;}
  else if(ulLen + 2 < ulLenEnd && IS_UTF8_NONASCII2(ucpSrc)){ulLen += 3;}
  else if(ulLen + 3 < ulLenEnd && IS_UTF8_NONASCII3(ucpSrc)){ulLen += 4;}
  else if(ulLen + 4 < ulLenEnd && IS_UTF8_NONASCII4(ucpSrc)){ulLen += 5;}
  else if(ulLen + 5 < ulLenEnd && IS_UTF8_NONASCII5(ucpSrc)){ulLen += 6;}
  return ulLen;
}
static ulong ulUtf8Char(uchar* ucpSrc, ulong ulLenEnd)
{
  ulong ulLen = 0;
  if(ulLen < ulLenEnd && (ucpSrc[0] >= 33 && ucpSrc[0] <= 126)){ulLen++;}
  else{ulLen = ulNonAscii(ucpSrc, ulLenEnd);}
  return ulLen;
}
static ulong ulLWS(uchar* ucpSrc, ulong ulLenEnd)
{
  ulong ulLen = 0;
  if(IS_WSP(ucpSrc[ulLen])){++ulLen;}
  else if(ulLen + 3 < ulLenEnd && (IS_LINE_CONTINUE(ucpSrc))){ulLen += 3;}
  return ulLen;
}
static ulong ulSWS(uchar* ucpSrc, ulong ulLenEnd)
{
  ulong ulLen = 0;
  while(ulLen < ulLenEnd)
  {
    uchar* ucpChar = &ucpSrc[ulLen];
    if(IS_WSP(ucpSrc[ulLen])){++ulLen;}
    else if(ulLen + 3 < ulLenEnd && (IS_LINE_CONTINUE(ucpChar))){ulLen += 3;}
    else{break;}
  }
  return ulLen;
}
static ulong ulUtf8Trim(uchar* ucpSrc, ulong ulLenEnd)
{
  ulong ulLen = 0;
  while(ulLen < ulLenEnd)
  {
    // 1*utf8char
    ulong ulTemp = ulUtf8Char(&ucpSrc[ulLen], ulLenEnd - ulLen);
    if(ulTemp == 0){break;}
    ulLen += ulTemp;
  }
  while(ulLen < ulLenEnd)
  {
    // (LWS 1*utf8char)
    ulong ulTemp = ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);
    if(ulTemp == 0){break;}
    ulLen += ulTemp;
    ulTemp = ulLen;
    while(ulLen < ulLenEnd)
    {
      // 1*utf8char
      ulong ulTemp = ulUtf8Char(&ucpSrc[ulLen], ulLenEnd - ulLen);
      if(ulTemp == 0){break;}
      ulLen += ulTemp;
    }
    if(ulLen == ulTemp){break;}
  }
  return ulLen;
}
static ulong ulSWSChar(uchar* ucpSrc, ulong ulLenEnd, ulong ulPre, uchar ucChar, ulong ulPost)
{
  ulong ulLen = 0;
  ulong ulRet = 0;
  if(ulLen < ulLenEnd)
  {
    if(ulPre){ulLen += ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);}
    if(ulLen < ulLenEnd && ucpSrc[ulLen] == ucChar)
    {
      ulLen++;
      ulLen += ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);
      if(ulLen < ulLenEnd){ulRet = ulLen;}
    }
  }
  return ulRet;
}
static ulong ulQdtext(uchar* ucpSrc, ulong ulLenEnd)
{
  ulong ulLen = ulSWS(ucpSrc, ulLenEnd);
  if(ulLen == 0)
  {
    uchar ucChar = ucpSrc[ulLen];
    if(ucChar == 0x21){ulLen++;}
    else if(ucChar >= 0x23 && ucChar <= 0x5B){ulLen++;}
    else if(ucChar >= 0x5D && ucChar <= 0x7E){ulLen++;}
    else
    {
      uchar* ucpChar = &ucpSrc[ulLen];
      if(((ulLen+1)<ulLenEnd) && IS_UTF8_NONASCII1(ucpChar)){ulLen += 2;}
      else if(((ulLen+2)<ulLenEnd) && IS_UTF8_NONASCII1(ucpChar)){ulLen += 3;}
      else if(((ulLen+3)<ulLenEnd) && IS_UTF8_NONASCII1(ucpChar)){ulLen += 4;}
      else if(((ulLen+4)<ulLenEnd) && IS_UTF8_NONASCII1(ucpChar)){ulLen += 5;}
      else if(((ulLen+5)<ulLenEnd) && IS_UTF8_NONASCII1(ucpChar)){ulLen += 6;}
    }
  }
  return ulLen;
}

static ulong ulNameCompare(uchar* ucpName, uchar* ucpCompare, ulong ulLen)
{
  int iCmp;
  uchar ucSave = ucpName[ulLen];
  ucpName[ulLen] = 0;
  iCmp = apg_stricmp((char*)ucpName, (char*)ucpCompare);
  ucpName[ulLen] = ucSave;
  return (iCmp == 0) ? APG_TRUE: APG_FALSE;
}
static ulong syn_message_header(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    uchar* ucpSrc = spData->ucpSrc + ulOffset;
    ulong ulLenEnd = spData->ulSrcLen - ulOffset;
    ulong ulLen = 0;
    ulong ulTest = APG_FALSE;
    ulong ulNameLen = 0;
    ulong ulRuleID = RULE_COUNT_SIP;
    ulong ulState, ulMatched;
    uchar ucChar;
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    while(APG_TRUE)
    {
      // always test for EOS
      if(ulOffset >= spData->ulSrcLen){break;}

      // find the name
      while(ulLen < ulLenEnd)
      {
        if(IS_ALPHAHYPHEN(ucpSrc[ulLen])){ulLen++;}
        else{break;}
      }
      ulNameLen = ulLen;

      // suck up the HCOLON
      ulTest = APG_FALSE;
      while(ulLen < ulLenEnd && IS_WSP(ucpSrc[ulLen])){ulLen++;}
      if(ulLen < ulLenEnd && ucpSrc[ulLen] == ':')
      {
        ulTest = APG_TRUE;
        ulLen++;
        ulLen += ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);
      }
      if(!ulTest)
      {
        // try to find an extension header
        ulLen = 0;
        ulNameLen = 0;
        vExecuteRule(spData, RULE_SIP_HEADER_NAME, &ulState, ulOffset, &ulMatched);
        if(ulState == SYN_MATCH)
        {
          ulNameLen = ulMatched;
          ulLen = ulMatched;

          // look for the HCOLON again
          while(ulLen < ulLenEnd && IS_WSP(ucpSrc[ulLen])){ulLen++;}
          if(ulLen < ulLenEnd && ucpSrc[ulLen] == ':')
          {
            ulTest = APG_TRUE;
            ulLen++;
            ulLen += ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);
          }
        }
      }
      if(!ulTest){break;}

      // identify the header
      ulTest = APG_TRUE;
      ucChar = (ucpSrc[0] < 97) ? ucpSrc[0] + 32 : ucpSrc[0];
      switch(ucChar)
      {
        case 'a':
        if(ulNameLen == 5 && ulNameCompare(ucpSrc, "allow", ulNameLen)){ulRuleID = RULE_SIP_ALLOW;}
        else if(ulNameLen == 6 && ulNameCompare(ucpSrc, "accept", ulNameLen)){ulRuleID = RULE_SIP_ACCEPT;}
        else if(ulNameLen == 10 && ulNameCompare(ucpSrc, "alert-info", ulNameLen)){ulRuleID = RULE_SIP_ALERT_INFO;}
        else if(ulNameLen == 13 && ulNameCompare(ucpSrc, "authorization", ulNameLen)){ulRuleID = RULE_SIP_AUTHORIZATION;}
        else if(ulNameLen == 15)
        {
          if(ulNameCompare(ucpSrc, "accept-encoding", ulNameLen)){ulRuleID = RULE_SIP_ACCEPT_ENCODING;}
          else if(ulNameCompare(ucpSrc, "accept-language", ulNameLen)){ulRuleID = RULE_SIP_ACCEPT_LANGUAGE;}
        }
        else if(ulNameLen == 19 && ulNameCompare(ucpSrc, "authentication-info", ulNameLen)){ulRuleID = RULE_SIP_AUTHENTICATION_INFO;}
        else {ulTest = APG_FALSE;}
        break;

        case 'c':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_CONTENT_TYPE;}
        else if(ulNameLen == 4 && ulNameCompare(ucpSrc, "cseq", ulNameLen)){ulRuleID = RULE_SIP_CSEQ;}
        else if(ulNameLen == 7)
        {
          if(ulNameCompare(ucpSrc, "call-id", ulNameLen)){ulRuleID = RULE_SIP_CALL_ID;}
          else if(ulNameCompare(ucpSrc, "contact", ulNameLen)){ulRuleID = RULE_SIP_CONTACT;}
        }
        else if(ulNameLen == 9 && ulNameCompare(ucpSrc, "call-info", ulNameLen)){ulRuleID = RULE_SIP_CALL_INFO;}
        else if(ulNameLen == 12 && ulNameCompare(ucpSrc, "content-type", ulNameLen)){ulRuleID = RULE_SIP_CONTENT_TYPE;}
        else if(ulNameLen == 14 && ulNameCompare(ucpSrc, "content-length", ulNameLen)){ulRuleID = RULE_SIP_CONTENT_LENGTH;}
        else if(ulNameLen == 16)
        {
          if(ulNameCompare(ucpSrc, "content-encoding", ulNameLen)){ulRuleID = RULE_SIP_CONTENT_ENCODING;}
          else if(ulNameCompare(ucpSrc, "content-language", ulNameLen)){ulRuleID = RULE_SIP_CONTENT_LANGUAGE;}
        }
        else if(ulNameLen == 19 && ulNameCompare(ucpSrc, "content-disposition", ulNameLen)){ulRuleID = RULE_SIP_CONTENT_DISPOSITION;}
        else {ulTest = APG_FALSE;}
        break;

        case 'd':
        if(ulNameLen == 4 && ulNameCompare(ucpSrc, "date", ulNameLen)){ulRuleID = RULE_SIP_DATE;}
        else {ulTest = APG_FALSE;}
        break;

        case 'e':
        if(ulNameLen == 7 && ulNameCompare(ucpSrc, "expires", ulNameLen)){ulRuleID = RULE_SIP_EXPIRES;}
        else if(ulNameLen == 10 && ulNameCompare(ucpSrc, "error-info", ulNameLen)){ulRuleID = RULE_SIP_ERROR_INFO;}
        else {ulTest = APG_FALSE;}
        break;

        case 'f':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_FROM;}
        else if(ulNameCompare(ucpSrc, "from", ulNameLen)){ulRuleID = RULE_SIP_FROM;}
        else {ulTest = APG_FALSE;}
        break;

        case 'i':
        if(ulNameCompare(ucpSrc, "in-reply-to", ulNameLen)){ulRuleID = RULE_SIP_IN_REPLY_TO;}
        else {ulTest = APG_FALSE;}
        break;

        case 'k':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_SUPPORTED;}
        else {ulTest = APG_FALSE;}
        break;

        case 'l':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_CONTENT_LENGTH;}
        break;

        case 'm':
        if(ulNameLen == 12)
        {
          if(ulNameCompare(ucpSrc, "max-forwards", ulNameLen)){ulRuleID = RULE_SIP_MAX_FORWARDS;}
          else if(ulNameCompare(ucpSrc, "mime-version", ulNameLen)){ulRuleID = RULE_SIP_MIME_VERSION;}
        }
        else if(ulNameCompare(ucpSrc, "min-expires", ulNameLen)){ulRuleID = RULE_SIP_MIN_EXPIRES;}
        else {ulTest = APG_FALSE;}
        break;

        case 'o':
        if(ulNameCompare(ucpSrc, "organization", ulNameLen)){ulRuleID = RULE_SIP_ORGANIZATION;}
        else {ulTest = APG_FALSE;}
        break;

        case 'p':
        if(ulNameLen == 8 && ulNameCompare(ucpSrc, "Priority", ulNameLen)){ulRuleID = RULE_SIP_PRIORITY;}
        else if(ulNameLen == 13 && ulNameCompare(ucpSrc, "Proxy-Require", ulNameLen)){ulRuleID = RULE_SIP_PROXY_REQUIRE;}
        else if(ulNameLen == 18 && ulNameCompare(ucpSrc, "Proxy-Authenticate", ulNameLen)){ulRuleID = RULE_SIP_PROXY_AUTHENTICATE;}
        else if(ulNameLen == 19 && ulNameCompare(ucpSrc, "Proxy-Authorization", ulNameLen)){ulRuleID = RULE_SIP_PROXY_AUTHORIZATION;}
        else {ulTest = APG_FALSE;}
        break;

        case 'r':
        if(ulNameLen == 5 && ulNameCompare(ucpSrc, "Route", ulNameLen)){ulRuleID = RULE_SIP_ROUTE;}
        else if(ulNameLen == 7 && ulNameCompare(ucpSrc, "Require", ulNameLen)){ulRuleID = RULE_SIP_REQUEST;}
        else if(ulNameLen == 8 && ulNameCompare(ucpSrc, "Reply-To", ulNameLen)){ulRuleID = RULE_SIP_REPLY_TO;}
        else if(ulNameLen == 11 && ulNameCompare(ucpSrc, "Retry-After", ulNameLen)){ulRuleID = RULE_SIP_RETRY_AFTER;}
        else if(ulNameCompare(ucpSrc, "Record-Route", ulNameLen)){ulRuleID = RULE_SIP_RECORD_ROUTE;}
        else {ulTest = APG_FALSE;}
        break;

        case 's':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_SUBJECT;}
        else if(ulNameLen == 6 && ulNameCompare(ucpSrc, "server", ulNameLen)){ulRuleID = RULE_SIP_SERVER;}
        else if(ulNameLen == 7 && ulNameCompare(ucpSrc, "Subject", ulNameLen)){ulRuleID = RULE_SIP_SUBJECT;}
        else if(ulNameCompare(ucpSrc, "Supported", ulNameLen)){ulRuleID = RULE_SIP_SUPPORTED;}
        else {ulTest = APG_FALSE;}
        break;

        case 't':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_TO;}
        else if(ulNameLen == 2 && ulNameCompare(ucpSrc, "to", ulNameLen)){ulRuleID = RULE_SIP_TO;}
        else if(ulNameCompare(ucpSrc, "timestamp", ulNameLen)){ulRuleID = RULE_SIP_TIMESTAMP;}
        else {ulTest = APG_FALSE;}
        break;

        case 'u':
        if(ulNameLen == 10 && ulNameCompare(ucpSrc, "user-agent", ulNameLen)){ulRuleID = RULE_SIP_USER_AGENT;}
        else if(ulNameCompare(ucpSrc, "unsupported", ulNameLen)){ulRuleID = RULE_SIP_UNSUPPORTED;}
        else {ulTest = APG_FALSE;}
        break;

        case 'v':
        if(ulNameLen == 1){ulRuleID = RULE_SIP_VIA;}
        else if(ulNameCompare(ucpSrc, "via", ulNameLen)){ulRuleID = RULE_SIP_VIA;}
        else {ulTest = APG_FALSE;}
        break;

        case 'w':
        if(ulNameLen == 7 && ulNameCompare(ucpSrc, "warning", ulNameLen)){ulRuleID = RULE_SIP_WARNING;}
        else if(ulNameCompare(ucpSrc, "www-authenticat", ulNameLen)){ulRuleID = RULE_SIP_WWW_AUTHENTICATE;}
        else {ulTest = APG_FALSE;}
        break;

        default:
        ulTest = APG_FALSE;
        break;
      }

      ulState = SYN_NOMATCH;
      ulMatched = 0;
      if(!ulTest){ulRuleID = RULE_SIP_EXTENSION_HEADER;} // try extension header

      // execute the header rule
      vExecuteRule(spData, ulRuleID, &ulState, ulOffset+ulLen, &ulMatched);
      break;
    }
    if(ulState != SYN_NOMATCH)
    {
      // suck up the line ender
      ulTest = ulLen + ulMatched;
      if(ulTest + 1 < ulLenEnd && ucpSrc[ulTest] == 13 && ucpSrc[ulTest + 1] == 10){ulLen += 2;}
      else{ulState = SYN_NOMATCH;}
    }
    if(ulState != SYN_NOMATCH)
    {
      *ulpLen = ulLen + ulMatched;
      *ulpState = *ulpLen ? SYN_MATCH : SYN_EMPTY;
    }
  }
  return ulRet;
}

static ulong syn_sip_version(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      ulong ulSave;
      ulong ulTest = APG_FALSE;
      if(ucpSrc[0] == 'S' && ucpSrc[1] == 'I' && ucpSrc[2] == 'P' && ucpSrc[3] == '/')
      {
        ulLen =4;
        while(ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen])){++ulLen;}
        if(ulLen > 3 && (ulLen < ulLenEnd) && (ucpSrc[ulLen] == '.'))
        {
          ++ulLen;
          ulSave = ulLen;
          while(ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen])){++ulLen;}
          if(ulLen > ulSave){ulTest = APG_TRUE;}
        }
      }
      else
        {
          *ulpState = SYN_MATCH;
          *ulpLen = ulLen;
        }
      if(ulTest)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_content_length(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      while(ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen])){++ulLen;}
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_max_forwards(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      while(ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen])){++ulLen;}
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_token(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      while(ulLen < ulLenEnd)
      {
        if(IS_TOKEN(ucpSrc[ulLen])){++ulLen;}
        else{break;}
      }
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_IPv4Address(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      ulong ulSave;
      ulong ulIndex;
      while(APG_TRUE)
      {
        // first byte
        ulSave = ulLen;
        for(ulIndex = 0; ulIndex < 3 && ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen]); ++ulIndex){++ulLen;}
        if(ulLen == ulSave){break;}
        ulSave = ulLen;
        if(ulLen < ulLenEnd && ucpSrc[ulLen] == '.'){++ulLen;}
        if(ulLen == ulSave){break;}

        // second byte
        ulSave = ulLen;
        for(ulIndex = 0; ulIndex < 3 && ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen]); ++ulIndex){++ulLen;}
        if(ulLen == ulSave){break;}
        ulSave = ulLen;
        if(ulLen < ulLenEnd && ucpSrc[ulLen] == '.'){++ulLen;}
        if(ulLen == ulSave){break;}

        // third byte
        ulSave = ulLen;
        for(ulIndex = 0; ulIndex < 3 && ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen]); ++ulIndex){++ulLen;}
        if(ulLen == ulSave){break;}
        ulSave = ulLen;
        if(ulLen < ulLenEnd && ucpSrc[ulLen] == '.'){++ulLen;}
        if(ulLen == ulSave){break;}

        // fourth byte
        ulSave = ulLen;
        for(ulIndex = 0; ulIndex < 3 && ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen]); ++ulIndex){++ulLen;}
        if(ulLen == ulSave){break;}

        // success
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
        break;
      }
    }
  }
  return ulRet;
}

static ulong syn_cseq(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      ulong ulSave1;
      ulong ulState, ulMatched;
      while(ulLen < ulLenEnd && IS_DIGIT(ucpSrc[ulLen])){++ulLen;}
      if(ulLen > 0)
      {
        ulSave1 = ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);
        if(ulSave1 > 0)
        {
          ulLen += ulSave1;
          vExecuteRule(spData, RULE_SIP_METHOD, &ulState, ulOffset + ulLen, &ulMatched);
          *ulpState = ulState;
          *ulpLen = ulLen + ulMatched;
        }
      }
    }
  }
  return ulRet;
}

static ulong syn_CRLF(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      if(ulLen + 1 < ulLenEnd && ucpSrc[ulLen] == 13 && ucpSrc[ulLen+1] == 10)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = 2;
      }
    }
  }
  return ulRet;
}

static ulong syn_header_value(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      while(ulLen < ulLenEnd)
      {
        uchar* ucpChar = &ucpSrc[ulLen];
        ulong ulTemp = ulUtf8Char(ucpChar, ulLenEnd - ulLen);
        if(ulTemp > 0 && ulLen + ulTemp < ulLenEnd){ulLen += ulTemp;}
        else if(IS_UTF8_CONT(ucpChar[0])){ulLen++;}
        else
        {
          ulTemp = ulLWS(ucpChar, ulLenEnd - ulLen);
          if(ulTemp > 0 && (ulLen + ulTemp < ulLenEnd)){ulLen += ulTemp;}
          else{break;}
        }
      }
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_TEXT_UTF8_TRIM(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = ulUtf8Trim(ucpSrc, ulLenEnd);
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_spaced_punct(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      switch(spData->ulRuleID)
      {
        case RULE_SIP_STAR:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, '*', APG_TRUE);
        break;
        case RULE_SIP_SLASH:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, '/', APG_TRUE);
        break;
        case RULE_SIP_EQUAL:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, '=', APG_TRUE);
        break;
        case RULE_SIP_LPAREN:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, '(', APG_TRUE);
        break;
        case RULE_SIP_RPAREN:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, ')', APG_TRUE);
        break;
        case RULE_SIP_RAQUOT:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_FALSE, '>', APG_TRUE);
        break;
        case RULE_SIP_LAQUOT:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, '<', APG_FALSE);
        break;
        case RULE_SIP_COMMA:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, ',', APG_TRUE);
        break;
        case RULE_SIP_SEMI:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, ';', APG_TRUE);
        break;
        case RULE_SIP_COLON:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, ':', APG_TRUE);
        break;
        case RULE_SIP_RDQUOT:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_FALSE, '"', APG_TRUE);
        break;
        case RULE_SIP_LDQUOT:
        ulLen = ulSWSChar(ucpSrc, ulLenEnd, APG_TRUE, '"', APG_FALSE);
        break;
        default:
        ulLen = 0;
        DASSERT(APG_FALSE);
        break;
      }
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_HCOLON(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulTest = APG_FALSE;
      ulong ulLen = 0;
      while(ulLen < ulLenEnd && IS_WSP(ucpSrc[ulLen])){ulLen++;}
      if(ulLen < ulLenEnd && ucpSrc[ulLen] == ':')
      {
        ulTest = APG_TRUE;
        ulLen++;
        ulLen += ulSWS(&ucpSrc[ulLen], ulLenEnd - ulLen);
      }
      if(ulTest)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_q_string(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  ulong ulLen;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulLen = 0;
      while(APG_TRUE)
      {
        uchar* ucpChar = &ucpSrc[ulLen];
        ulong ulTest = ulQdtext(ucpChar, ulLenEnd - ulLen);
//        if((ulTest == 0) && (ulLen < ulLenEnd) && (IS_QUOTED_PAIR(ucpChar))){ulTest = 2;}
        if((ulTest == 0))
        {
          if((ulLen < ulLenEnd))
          {
            if(IS_QUOTED_PAIR(ucpChar)){ulTest = 2;}
          }
        }
        if(ulTest == 0){break;}
        ulLen += ulTest;
      }
    }
    if(ulLen > 0)
    {
      *ulpState = SYN_MATCH;
      *ulpLen = ulLen;
    }
  }
  return ulRet;
}

static ulong syn_word(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLen = 0;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;

      // must have at least one character
      if(IS_WORD(ucpSrc[ulLen])){++ulLen;}
      if(ulLen > 0 && ulLen < ulLenEnd)
      {
        while(ulLen < ulLenEnd)
        {
          if(IS_WORD(ucpSrc[ulLen])){++ulLen;}
          else{break;}
        }
      }
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
      // else NOMATCH
    }
  }
  return ulRet;
}

static ulong syn_user(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLen = 0;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;

      // must have at least one character
      if(IS_UNRESERVED(ucpSrc[ulLen]) || IS_USER_UNRESERVED(ucpSrc[ulLen])){++ulLen;}
      else if(((ulLen + 3) < ulLenEnd) && IS_ESCAPED(ucpSrc)){ulLen += 3;}
      if(ulLen > 0 && ulLen < ulLenEnd)
      {
        while(ulLen < ulLenEnd)
        {
          ulong ulTest = ulLen;
          uchar* ucpChar = &ucpSrc[ulLen];
          if(IS_UNRESERVED(ucpSrc[ulLen]) || IS_USER_UNRESERVED(ucpSrc[ulLen])){++ulLen;}
          else if(((ulLen + 3) < ulLenEnd) && IS_ESCAPED(ucpChar)){ulLen += 3;}
          if(ulTest == ulLen){break;}
        }
      }
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
      // else NOMATCH
    }
  }
  return ulRet;
}

static ulong syn_pname(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLen = 0;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;

      // must have at least one character
      if(IS_UNRESERVED(ucpSrc[ulLen]) || IS_PARAM_UNRESERVED(ucpSrc[ulLen])){++ulLen;}
      else if(((ulLen + 3) < ulLenEnd) && IS_ESCAPED(ucpSrc)){ulLen += 3;}
      if(ulLen > 0 && ulLen < ulLenEnd)
      {
        while(ulLen < ulLenEnd)
        {
          ulong ulTest = ulLen;
          uchar* ucpChar = &ucpSrc[ulLen];
          if(IS_UNRESERVED(ucpSrc[ulLen]) || IS_PARAM_UNRESERVED(ucpSrc[ulLen])){++ulLen;}
          else if(((ulLen + 3) < ulLenEnd) && IS_ESCAPED(ucpChar)){ulLen += 3;}
          if(ulTest == ulLen){break;}
        }
      }
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
      // else NOMATCH
    }
  }
  return ulRet;
}

static ulong syn_message_body(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;

  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      // grab the remainder of the string
      *ulpLen = spData->ulSrcLen - ulOffset;
      *ulpState = (*ulpLen > 0) ? SYN_MATCH : SYN_EMPTY;
    }
  }
  return ulRet;
}

static ulong syn_LWS(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      // wsp = %d32 / %d9 / %d13.10.32
      // LWS = wsp *wsp
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulong ulLen = 0;
      ulLen = ulSWS(ucpSrc, ulLenEnd);
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
      // else NOMATCH
    }
  }
  return ulRet;
}

// separator white space (zero or more spaces)
static ulong syn_SWS(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_EMPTY;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      // wsp = %d32 / %d9 / %d13.10.32
      // SWS = *wsp
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLen = 0;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulLen = ulSWS(ucpSrc, ulLenEnd);
      if(ulLen > 0)
      {
        *ulpState = SYN_MATCH;
        *ulpLen = ulLen;
      }
    }
  }
  return ulRet;
}

static ulong syn_alphanum(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      if(IS_ALPHANUM(ucpSrc[0]))
      {
        *ulpState = SYN_MATCH;
        *ulpLen = 1;
      }
    }
  }
  return ulRet;
}

static ulong ulDomainLabel(uchar* ucpSrc, ulong ulEnd)
{
  ulong ulLen = 0;
  if(ulLen < ulEnd && IS_ALPHANUM(ucpSrc[ulLen])){++ulLen;}
  if(ulLen < ulEnd && ulLen > 0)
  {
    while(ulLen < ulEnd && (IS_ALPHANUM(ucpSrc[ulLen]) || IS_HYPHEN(ucpSrc[ulLen]))){++ulLen;}
    if(IS_HYPHEN(ucpSrc[ulLen-1])){ulLen = 0;} // string cannot end in hyphen
  }
  return ulLen;
}

static ulong ulTopLabel(uchar* ucpSrc, ulong ulEnd)
{
  ulong ulLen = 0;
  if(ulLen < ulEnd && IS_ALPHA(ucpSrc[ulLen])){++ulLen;}
  if(ulLen < ulEnd && ulLen > 0)
  {
    while(ulLen < ulEnd && (IS_ALPHANUM(ucpSrc[ulLen]) || IS_HYPHEN(ucpSrc[ulLen]))){++ulLen;}
    if(IS_HYPHEN(ucpSrc[ulLen-1])){ulLen = 0;} // string cannot end in hyphen
  }
  return ulLen;
}

ulong syn_hostname(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    ulong ulLen = 0;
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      while(APG_TRUE)
      {
        // look for first non-alphanum-hyphen && next char
        ulong ulState, ulMatched;
        ulong ulNext0IsPeriod, ulNext1IsAlpha;
        ulong ulTemp = ulLen;
        while(ulLen < ulLenEnd && (IS_ALPHANUM(ucpSrc[ulLen]) || IS_HYPHEN(ucpSrc[ulLen]))){ulLen++;}
        if(ulLen == ulTemp){ulLen = 0; break;}// indicates hostname failure
        ulNext0IsPeriod = (ulLen < ulLenEnd && (ucpSrc[ulLen] == '.')) ? APG_TRUE: APG_FALSE;
        ulNext1IsAlpha = (ulLen+1 < ulLenEnd && IS_ALPHANUM(ucpSrc[ulLen + 1])) ? APG_TRUE: APG_FALSE;

        // if(period && next char != non-alphanum) call toplable && break
        if(ulNext0IsPeriod && !ulNext1IsAlpha)
        {
          ulLen++; // count the following period
          vExecuteRule(spData, RULE_SIP_TOPLABEL, &ulState, ulOffset+ulTemp, &ulMatched);
          if(ulState != SYN_MATCH){ulLen = 0; break;}// indicates hostname failure
          break;
        }

        // if(not period) call toplabel && break
        if(!ulNext0IsPeriod)
        {
          vExecuteRule(spData, RULE_SIP_TOPLABEL, &ulState, ulOffset+ulTemp, &ulMatched);
          if(ulState != SYN_MATCH){ulLen = 0; break;}// indicates hostname failure
          break;
        }

        // else next must be period - call domainlabel
        ulLen++; // count the following period
        vExecuteRule(spData, RULE_SIP_DOMAINLABEL, &ulState, ulOffset+ulTemp, &ulMatched);
        if(ulState != SYN_MATCH){ulLen = 0; break;}// indicates hostname failure
      }
    }
    if(ulLen > 0)
    {
      // hostname success
      *ulpState = SYN_MATCH;
      *ulpLen = ulLen;
    }
  }
  return ulRet;
}

ulong syn_domainlabel(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    ulong ulLen = 0;
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulLen = ulDomainLabel(ucpSrc, ulLenEnd);
    }
    if(ulLen > 0)
    {
      *ulpState = SYN_MATCH;
      *ulpLen = ulLen;
    }
  }
  return ulRet;
}

ulong syn_toplabel(APG_CBDATA* spData, ulong* ulpState, ulong ulOffset, ulong* ulpLen)
{
  ulong ulRet = PFN_OK;
  if(*ulpState == SYN_PRE)
  {
    // always test for EOS
    ulong ulLen = 0;
    *ulpState = SYN_NOMATCH;
    *ulpLen = 0;
    if(ulOffset < spData->ulSrcLen)
    {
      uchar* ucpSrc = spData->ucpSrc + ulOffset;
      ulong ulLenEnd = spData->ulSrcLen - ulOffset;
      ulLen = ulTopLabel(ucpSrc, ulLenEnd);
    }
    if(ulLen > 0)
    {
      *ulpState = SYN_MATCH;
      *ulpLen = ulLen;
    }
  }
  return ulRet;
}

static SYN_CALLBACK g_saSyntax[RULE_COUNT_SIP];
SYN_CALLBACK* spSyntaxCallbackInit_WSP(SYN_CALLBACK* spUserBuffer)
{
  SYN_CALLBACK* spSyntax = g_saSyntax;
  if(spUserBuffer != NULL){spSyntax = spUserBuffer;}
  memset((void*)spSyntax, 0, sizeof(SYN_CALLBACK) * RULE_COUNT_SIP);

  // white space
  spSyntax[RULE_SIP_LWS] = syn_LWS;
  spSyntax[RULE_SIP_SWS] = syn_SWS;

  // spaced punctuation
  spSyntax[RULE_SIP_HCOLON] = syn_HCOLON;
  spSyntax[RULE_SIP_EQUAL] = syn_spaced_punct;
  spSyntax[RULE_SIP_SLASH] = syn_spaced_punct;
  spSyntax[RULE_SIP_SEMI] = syn_spaced_punct;
  spSyntax[RULE_SIP_STAR] = syn_spaced_punct;
  spSyntax[RULE_SIP_LPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_LAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_COMMA] = syn_spaced_punct;
  spSyntax[RULE_SIP_COLON] = syn_spaced_punct;
  spSyntax[RULE_SIP_LDQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_RDQUOT] = syn_spaced_punct;
  return spSyntax;
}

SYN_CALLBACK* spSyntaxCallbackInit_msghdrs(SYN_CALLBACK* spUserBuffer)
{
  SYN_CALLBACK* spSyntax = g_saSyntax;
  if(spUserBuffer != NULL){spSyntax = spUserBuffer;}
  memset((void*)spSyntax, 0, sizeof(SYN_CALLBACK) * RULE_COUNT_SIP);

  // white space
  spSyntax[RULE_SIP_LWS] = syn_LWS;
  spSyntax[RULE_SIP_SWS] = syn_SWS;

  // spaced punctuation
  spSyntax[RULE_SIP_HCOLON] = syn_HCOLON;
  spSyntax[RULE_SIP_EQUAL] = syn_spaced_punct;
  spSyntax[RULE_SIP_SLASH] = syn_spaced_punct;
  spSyntax[RULE_SIP_SEMI] = syn_spaced_punct;
  spSyntax[RULE_SIP_STAR] = syn_spaced_punct;
  spSyntax[RULE_SIP_LPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_LAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_COMMA] = syn_spaced_punct;
  spSyntax[RULE_SIP_COLON] = syn_spaced_punct;
  spSyntax[RULE_SIP_LDQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_RDQUOT] = syn_spaced_punct;

  // message headers
  spSyntax[RULE_SIP_MESSAGE_HEADER] = syn_message_header;

  return spSyntax;
}

SYN_CALLBACK* spSyntaxCallbackInit_hostname(SYN_CALLBACK* spUserBuffer)
{
  SYN_CALLBACK* spSyntax = g_saSyntax;
  if(spUserBuffer != NULL){spSyntax = spUserBuffer;}
  memset((void*)spSyntax, 0, sizeof(SYN_CALLBACK) * RULE_COUNT_SIP);

  // white space
  spSyntax[RULE_SIP_LWS] = syn_LWS;
  spSyntax[RULE_SIP_SWS] = syn_SWS;

  // spaced punctuation
  spSyntax[RULE_SIP_HCOLON] = syn_HCOLON;
  spSyntax[RULE_SIP_EQUAL] = syn_spaced_punct;
  spSyntax[RULE_SIP_SLASH] = syn_spaced_punct;
  spSyntax[RULE_SIP_SEMI] = syn_spaced_punct;
  spSyntax[RULE_SIP_STAR] = syn_spaced_punct;
  spSyntax[RULE_SIP_LPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_LAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_COMMA] = syn_spaced_punct;
  spSyntax[RULE_SIP_COLON] = syn_spaced_punct;
  spSyntax[RULE_SIP_LDQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_RDQUOT] = syn_spaced_punct;

  // message headers
  spSyntax[RULE_SIP_MESSAGE_HEADER] = syn_message_header;

  // host name
  spSyntax[RULE_SIP_HOSTNAME] = syn_hostname;
  spSyntax[RULE_SIP_DOMAINLABEL] = syn_domainlabel;
  spSyntax[RULE_SIP_TOPLABEL] = syn_toplabel;

  return spSyntax;
}

SYN_CALLBACK* spSyntaxCallbackInit_ALL(SYN_CALLBACK* spUserBuffer)
{
  SYN_CALLBACK* spSyntax = g_saSyntax;
  if(spUserBuffer != NULL){spSyntax = spUserBuffer;}
  memset((void*)spSyntax, 0, sizeof(SYN_CALLBACK) * RULE_COUNT_SIP);

  // white space
  spSyntax[RULE_SIP_LWS] = syn_LWS;
  spSyntax[RULE_SIP_SWS] = syn_SWS;

  // spaced punctuation
  spSyntax[RULE_SIP_HCOLON] = syn_HCOLON;
  spSyntax[RULE_SIP_EQUAL] = syn_spaced_punct;
  spSyntax[RULE_SIP_SLASH] = syn_spaced_punct;
  spSyntax[RULE_SIP_SEMI] = syn_spaced_punct;
  spSyntax[RULE_SIP_STAR] = syn_spaced_punct;
  spSyntax[RULE_SIP_LPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RPAREN] = syn_spaced_punct;
  spSyntax[RULE_SIP_RAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_LAQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_COMMA] = syn_spaced_punct;
  spSyntax[RULE_SIP_COLON] = syn_spaced_punct;
  spSyntax[RULE_SIP_LDQUOT] = syn_spaced_punct;
  spSyntax[RULE_SIP_RDQUOT] = syn_spaced_punct;

  // message headers
  spSyntax[RULE_SIP_MESSAGE_HEADER] = syn_message_header;

  // host name
  spSyntax[RULE_SIP_HOSTNAME] = syn_hostname;
  spSyntax[RULE_SIP_DOMAINLABEL] = syn_domainlabel;
  spSyntax[RULE_SIP_TOPLABEL] = syn_toplabel;

  // misc. other
  spSyntax[RULE_SIP_ALPHANUM] = syn_alphanum;
  spSyntax[RULE_SIP_TOKEN] = syn_token;
  spSyntax[RULE_SIP_WORD] = syn_word;
  spSyntax[RULE_SIP_CRLF] = syn_CRLF;
  spSyntax[RULE_SIP_TEXT_UTF8_TRIM] = syn_TEXT_UTF8_TRIM;
  spSyntax[RULE_SIP_HEADER_VALUE] = syn_header_value;
  spSyntax[RULE_SIP_Q_STRING] = syn_q_string;
  spSyntax[RULE_SIP_PNAME] = syn_pname;
  spSyntax[RULE_SIP_PVALUE] = syn_pname;
  spSyntax[RULE_SIP_MESSAGE_BODY] = syn_message_body;
  spSyntax[RULE_SIP_USER] = syn_user;
  spSyntax[RULE_SIP_CONTENT_LENGTH] = syn_content_length;
  spSyntax[RULE_SIP_SIP_VERSION] = syn_sip_version;
  spSyntax[RULE_SIP_CSEQ] = syn_cseq;
  spSyntax[RULE_SIP_IPV4ADDRESS] = syn_IPv4Address;
  spSyntax[RULE_SIP_MAX_FORWARDS] = syn_max_forwards;
  spSyntax[RULE_SIP_PROTOCOL_VERSION] = syn_token;
  spSyntax[RULE_SIP_IETF_TOKEN] = syn_token;
  spSyntax[RULE_SIP_HEADER_NAME] = syn_token;
  spSyntax[RULE_SIP_EXTENSION_METHOD] = syn_token;
  return spSyntax;
}

void vAstInit(ulong* ulpAst)
{
  memset((void*)ulpAst, 0, sizeof(ulong) * RULE_COUNT_SIP);
  ulong ul;
   for(ul = 0; ul < RULE_COUNT_SIP; ul++)
  {
    ulpAst[ul] = APG_TRUE;
  }

  // white space
  /*ulpAst[RULE_SIP_REQUEST] = APG_TRUE;
  ulpAst[RULE_SIP_REQUEST_LINE] = APG_TRUE;
  ulpAst[RULE_SIP_METHOD] = APG_TRUE;
  ulpAst[RULE_SIP_REQUEST_URI] = APG_TRUE;
  ulpAst[RULE_SIP_SIP_VERSION] = APG_TRUE;
  ulpAst[RULE_SIP_RESPONSE] = APG_TRUE;
  ulpAst[RULE_SIP_STATUS_LINE] = APG_TRUE;
  ulpAst[RULE_SIP_STATUS_CODE] = APG_TRUE;
  ulpAst[RULE_SIP_REASON_PHRASE] = APG_TRUE;
  ulpAst[RULE_SIP_SIP_URI] = APG_TRUE;
  ulpAst[RULE_SIP_SIPS_URI] = APG_TRUE;
  ulpAst[RULE_SIP_USERINFO] = APG_TRUE;
  ulpAst[RULE_SIP_HOSTPORT] = APG_TRUE;
  ulpAst[RULE_SIP_URI_PARAMETER] = APG_TRUE;
  ulpAst[RULE_SIP_HEADERS] = APG_TRUE;
  ulpAst[RULE_SIP_HOST] = APG_TRUE;
  ulpAst[RULE_SIP_HOSTNAME] = APG_TRUE;
  ulpAst[RULE_SIP_IPV4ADDRESS] = APG_TRUE;
  ulpAst[RULE_SIP_IPV6REFERENCE] = APG_TRUE;
  ulpAst[RULE_SIP_ABSOLUTEURI] = APG_TRUE;
  ulpAst[RULE_SIP_REQUEST] = APG_TRUE;
  ulpAst[RULE_SIP_SCHEME] = APG_TRUE;
  ulpAst[RULE_SIP_HIER_PART] = APG_TRUE;
  ulpAst[RULE_SIP_OPAQUE_PART] = APG_TRUE;
  ulpAst[RULE_SIP_AUTHORITY] = APG_TRUE;
  ulpAst[RULE_SIP_ABS_PATH] = APG_TRUE;
  ulpAst[RULE_SIP_PATH_SEGMENTS] = APG_TRUE;
  ulpAst[RULE_SIP_MESSAGE_HEADER] = APG_TRUE;
  ulpAst[RULE_SIP_ACCEPT] = APG_TRUE;
  ulpAst[RULE_SIP_EXTENSION_HEADER] = APG_TRUE;
  ulpAst[RULE_SIP_WWW_AUTHENTICATE] = APG_TRUE;
  ulpAst[RULE_SIP_WARNING] = APG_TRUE;
  ulpAst[RULE_SIP_VIA] = APG_TRUE;
  ulpAst[RULE_SIP_USER_AGENT] = APG_TRUE;
  ulpAst[RULE_SIP_UNSUPPORTED] = APG_TRUE;
  ulpAst[RULE_SIP_TO] = APG_TRUE;
  ulpAst[RULE_SIP_TIMESTAMP] = APG_TRUE;
  ulpAst[RULE_SIP_SUPPORTED] = APG_TRUE;
  ulpAst[RULE_SIP_SUBJECT] = APG_TRUE;
  ulpAst[RULE_SIP_SERVER] = APG_TRUE;
  ulpAst[RULE_SIP_ROUTE] = APG_TRUE;
  ulpAst[RULE_SIP_RETRY_AFTER] = APG_TRUE;
  ulpAst[RULE_SIP_REQUIRE] = APG_TRUE;
  ulpAst[RULE_SIP_REPLY_TO] = APG_TRUE;
  ulpAst[RULE_SIP_PROXY_REQUIRE] = APG_TRUE;
  ulpAst[RULE_SIP_PROXY_AUTHORIZATION] = APG_TRUE;
  ulpAst[RULE_SIP_PROXY_AUTHENTICATE] = APG_TRUE;
  ulpAst[RULE_SIP_PRIORITY] = APG_TRUE;
  ulpAst[RULE_SIP_ORGANIZATION] = APG_TRUE;
  ulpAst[RULE_SIP_ACCEPT_ENCODING] = APG_TRUE;
  ulpAst[RULE_SIP_ACCEPT_LANGUAGE] = APG_TRUE;
  ulpAst[RULE_SIP_ALERT_INFO] = APG_TRUE;
  ulpAst[RULE_SIP_ALLOW] = APG_TRUE;
  ulpAst[RULE_SIP_AUTHENTICATION_INFO] = APG_TRUE;
  ulpAst[RULE_SIP_AUTHORIZATION] = APG_TRUE;
  ulpAst[RULE_SIP_CALL_ID] = APG_TRUE;
  ulpAst[RULE_SIP_CALL_INFO] = APG_TRUE;
  ulpAst[RULE_SIP_CONTACT] = APG_TRUE;
  ulpAst[RULE_SIP_CONTENT_DISPOSITION] = APG_TRUE;
  ulpAst[RULE_SIP_CONTENT_ENCODING] = APG_TRUE;
  ulpAst[RULE_SIP_CONTENT_LENGTH] = APG_TRUE;
  ulpAst[RULE_SIP_CONTENT_LANGUAGE] = APG_TRUE;
  ulpAst[RULE_SIP_CONTENT_TYPE] = APG_TRUE;
  ulpAst[RULE_SIP_CSEQ] = APG_TRUE;
  ulpAst[RULE_SIP_DATE] = APG_TRUE;
  ulpAst[RULE_SIP_ERROR_INFO] = APG_TRUE;
  ulpAst[RULE_SIP_EXPIRES] = APG_TRUE;
  ulpAst[RULE_SIP_FROM] = APG_TRUE;
  ulpAst[RULE_SIP_IN_REPLY_TO] = APG_TRUE;
  ulpAst[RULE_SIP_MAX_FORWARDS] = APG_TRUE;
  ulpAst[RULE_SIP_MIME_VERSION] = APG_TRUE;
  ulpAst[RULE_SIP_MIN_EXPIRES] = APG_TRUE;
  */
}
