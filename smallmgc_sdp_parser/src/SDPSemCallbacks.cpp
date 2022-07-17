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
#include "SDP.h"
#include "sdp_parser.h"

// semantic call back function template
static ulong sem_MyFunc(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SDPSTACK_Parser::setSdpParserTest(ulOffset, ulLen);
    break;
  }
  return ulRet;
}





// default semantic initialization function (no AST generated)
// if spUserBuffer is not NULL it must be at least RULE_COUNT_SIP elements in length
// if spUserBuffer is NULL a static buffer is used
// spSemantic[] must be one of the following:
// - NULL (0):
//     *NO* AST node created in ulParserSyntaxAnalysis()
//     *NO* call back function called in ulParserSemanticAnalysis()
// - (SEM_CALLBACK)APG_TRUE (1):
//     AST node *IS* created in ulParserSyntaxAnalysis()
//     *NO* call back function called in ulParserSemanticAnalysis()
// - a valid SEM_CALLBACK function pointer:
//     AST node *IS* created in ulParserSyntaxAnalysis()
//     call back function *IS* called in ulParserSemanticAnalysis()
// - anything else:
//     AST node *IS* created in ulParserSyntaxAnalysis()
//     unpredictable behavior in ulParserSemanticAnalysis()
static SEM_CALLBACK g_saSemantic[RULE_COUNT_SDP];
SEM_CALLBACK* sdpSemanticCallbackInit_SDP(SEM_CALLBACK* spUserBuffer, ulong* ulpAst)
{
  SEM_CALLBACK* spSemantic = g_saSemantic;
  if(spUserBuffer != NULL){spSemantic = spUserBuffer;}
  memset((void*)spSemantic, 0, sizeof(SEM_CALLBACK) * RULE_COUNT_SDP);

  spSemantic[RULE_SDP_ANNOUNCEMENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ANNOUNCEMENT] = APG_TRUE;
  spSemantic[RULE_SDP_SESSION_SECTION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_SESSION_SECTION] = APG_TRUE;
  spSemantic[RULE_SDP_PROTO_VERSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_PROTO_VERSION] = APG_TRUE;
  spSemantic[RULE_SDP_ORIGIN_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ORIGIN_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_SESSION_NAME_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_SESSION_NAME_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_INFORMATION_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_INFORMATION_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_URI_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_URI_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_EMAIL_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_EMAIL_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_PHONE_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_PHONE_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_CONNECTION_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_CONNECTION_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_BANDWIDTH_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BANDWIDTH_FIELD] = APG_TRUE;

  spSemantic[RULE_SDP_TIME_FIELDS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TIME_FIELDS] = APG_TRUE;
  spSemantic[RULE_SDP_REPEAT_FIELDS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_REPEAT_FIELDS] = APG_TRUE;
  spSemantic[RULE_SDP_ZONE_ADJUSTMENTS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ZONE_ADJUSTMENTS] = APG_TRUE;
  spSemantic[RULE_SDP_KEY_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_KEY_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_ATTRIBUTE_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ATTRIBUTE_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_MEDIA_SECTION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_MEDIA_SECTION] = APG_TRUE;
  spSemantic[RULE_SDP_MEDIA_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_MEDIA_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_MEDIA] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_MEDIA] = APG_TRUE;
  spSemantic[RULE_SDP_FMT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_FMT] = APG_TRUE;
  spSemantic[RULE_SDP_PROTO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_PROTO] = APG_TRUE;

  spSemantic[RULE_SDP_MEDIAPORT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_MEDIAPORT] = APG_TRUE;
  spSemantic[RULE_SDP_PORTCOUNT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_PORTCOUNT] = APG_TRUE;
  spSemantic[RULE_SDP_USERNAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_USERNAME] = APG_TRUE;
  spSemantic[RULE_SDP_SESS_ID] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_SESS_ID] = APG_TRUE;
  spSemantic[RULE_SDP_SESS_VERSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_SESS_VERSION] = APG_TRUE;
  spSemantic[RULE_SDP_NETTYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_NETTYPE] = APG_TRUE;
  spSemantic[RULE_SDP_ADDRTYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ADDRTYPE] = APG_TRUE;
  spSemantic[RULE_SDP_EMAIL_ADDRESS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_EMAIL_ADDRESS] = APG_TRUE;
  spSemantic[RULE_SDP_ADDRESS_AND_COMMENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ADDRESS_AND_COMMENT] = APG_TRUE;
  spSemantic[RULE_SDP_DISPNAME_AND_ADDRESS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DISPNAME_AND_ADDRESS] = APG_TRUE;

  spSemantic[RULE_SDP_PHONE_NUMBER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_PHONE_NUMBER] = APG_TRUE;
  spSemantic[RULE_SDP_PHONE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_PHONE] = APG_TRUE;
  spSemantic[RULE_SDP_BWTYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BWTYPE] = APG_TRUE;
  spSemantic[RULE_SDP_BANDWIDTH] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BANDWIDTH] = APG_TRUE;
  spSemantic[RULE_SDP_START_TIME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_START_TIME] = APG_TRUE;
  spSemantic[RULE_SDP_STOP_TIME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_STOP_TIME] = APG_TRUE;
  spSemantic[RULE_SDP_TIME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TIME] = APG_TRUE;
  spSemantic[RULE_SDP_REPEAT_INTERVAL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_REPEAT_INTERVAL] = APG_TRUE;
  spSemantic[RULE_SDP_TYPED_TIME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TYPED_TIME] = APG_TRUE;
  spSemantic[RULE_SDP_FIXED_LEN_TIME_UNIT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_FIXED_LEN_TIME_UNIT] = APG_TRUE;

  spSemantic[RULE_SDP_KEY_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_KEY_TYPE] = APG_TRUE;
  spSemantic[RULE_SDP_BASE64] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BASE64] = APG_TRUE;
  spSemantic[RULE_SDP_BASE64_UNIT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BASE64_UNIT] = APG_TRUE;
  spSemantic[RULE_SDP_BASE64_PAD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BASE64_PAD] = APG_TRUE;
  spSemantic[RULE_SDP_BASE64_CHAR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BASE64_CHAR] = APG_TRUE;
  spSemantic[RULE_SDP_ATTRIBUTE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ATTRIBUTE] = APG_TRUE;
  spSemantic[RULE_SDP_ATT_FIELD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ATT_FIELD] = APG_TRUE;
  spSemantic[RULE_SDP_ATT_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ATT_VALUE] = APG_TRUE;
  spSemantic[RULE_SDP_CONNECTION_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_CONNECTION_TYPE] = APG_TRUE;
  spSemantic[RULE_SDP_ANY_CONNECTION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ANY_CONNECTION] = APG_TRUE;

  spSemantic[RULE_SDP_FQDN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_FQDN] = APG_TRUE;
  spSemantic[RULE_SDP_EXTN_ADDR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_EXTN_ADDR] = APG_TRUE;
  spSemantic[RULE_SDP_IPV4CONNECTION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV4CONNECTION] = APG_TRUE;
  spSemantic[RULE_SDP_IPV4TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV4TYPE] = APG_TRUE;
  spSemantic[RULE_SDP_IPV4MULTICON] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV4MULTICON] = APG_TRUE;
  spSemantic[RULE_SDP_TTL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TTL] = APG_TRUE;
  spSemantic[RULE_SDP_ADDRESSCOUNT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ADDRESSCOUNT] = APG_TRUE;
  spSemantic[RULE_SDP_IPV6CONNECTION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV6CONNECTION] = APG_TRUE;
  spSemantic[RULE_SDP_IPV6TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV6TYPE] = APG_TRUE;
  ulpAst[RULE_SDP_IPV6MULTCON] = APG_TRUE;
  spSemantic[RULE_SDP_IPV6MULTCON] = (SEM_CALLBACK)sem_MyFunc;

  spSemantic[RULE_SDP_TEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TEXT] = APG_TRUE;
  spSemantic[RULE_SDP_BYTE_STRING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_BYTE_STRING] = APG_TRUE;
  spSemantic[RULE_SDP_NON_WS_STRING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_NON_WS_STRING] = APG_TRUE;
  spSemantic[RULE_SDP_TOKEN_CHAR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TOKEN_CHAR] = APG_TRUE;
  spSemantic[RULE_SDP_TOKEN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_TOKEN] = APG_TRUE;
  spSemantic[RULE_SDP_ALPHA_NUMERIC] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ALPHA_NUMERIC] = APG_TRUE;
  spSemantic[RULE_SDP_URI_REFERENCE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_URI_REFERENCE] = APG_TRUE;
  spSemantic[RULE_SDP_DEC_OCTET] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DEC_OCTET] = APG_TRUE;
  spSemantic[RULE_SDP_IPV4MULTICAST] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV4MULTICAST] = APG_TRUE;

  spSemantic[RULE_SDP_IPV4UNICAST] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV4UNICAST] = APG_TRUE;
  spSemantic[RULE_SDP_INT224] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_INT224] = APG_TRUE;
  spSemantic[RULE_SDP_IPV6MULTICAST] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV6MULTICAST] = APG_TRUE;
  spSemantic[RULE_SDP_IPV6UNICAST] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_IPV6UNICAST] = APG_TRUE;
  spSemantic[RULE_SDP_ADDR_SPEC] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ADDR_SPEC] = APG_TRUE;
  spSemantic[RULE_SDP_LOCAL_PART] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_LOCAL_PART] = APG_TRUE;
  spSemantic[RULE_SDP_DOMAIN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DOMAIN] = APG_TRUE;
  spSemantic[RULE_SDP_DOMAIN_LITERAL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DOMAIN_LITERAL] = APG_TRUE;
  spSemantic[RULE_SDP_DCONTENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DCONTENT] = APG_TRUE;
  spSemantic[RULE_SDP_DTEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DTEXT] = APG_TRUE;

  spSemantic[RULE_SDP_NO_WS_CTL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_NO_WS_CTL] = APG_TRUE;
  spSemantic[RULE_SDP_ATEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ATEXT] = APG_TRUE;
  spSemantic[RULE_SDP_ATOM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ATOM] = APG_TRUE;
  spSemantic[RULE_SDP_DOT_ATOM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DOT_ATOM] = APG_TRUE;
  spSemantic[RULE_SDP_DOT_ATOM_TEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DOT_ATOM_TEXT] = APG_TRUE;
  spSemantic[RULE_SDP_QTEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_QTEXT] = APG_TRUE;
  spSemantic[RULE_SDP_QCONTENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_QCONTENT] = APG_TRUE;
  spSemantic[RULE_SDP_QUOTED_STRING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_QUOTED_STRING] = APG_TRUE;
  spSemantic[RULE_SDP_QUOTED_PAIR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_QUOTED_PAIR] = APG_TRUE;
  spSemantic[RULE_SDP_OBS_QP] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_OBS_QP] = APG_TRUE;

  spSemantic[RULE_SDP_OBS_LOCAL_PART] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_OBS_LOCAL_PART] = APG_TRUE;
  spSemantic[RULE_SDP_OBS_DOMAIN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_OBS_DOMAIN] = APG_TRUE;
  spSemantic[RULE_SDP_WORD] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_WORD] = APG_TRUE;
  spSemantic[RULE_SDP_FWS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_FWS] = APG_TRUE;
  spSemantic[RULE_SDP_CTEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_CTEXT] = APG_TRUE;
  spSemantic[RULE_SDP_CCONTENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_CCONTENT] = APG_TRUE;
  spSemantic[RULE_SDP_COMMENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_COMMENT] = APG_TRUE;
  spSemantic[RULE_SDP_CFWS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_CFWS] = APG_TRUE;
  spSemantic[RULE_SDP_OBS_FWS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_OBS_FWS] = APG_TRUE;
  spSemantic[RULE_SDP_SP] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_SP] = APG_TRUE;


  spSemantic[RULE_SDP_HTAB] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_HTAB] = APG_TRUE;
  spSemantic[RULE_SDP_DQUOTE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DQUOTE] = APG_TRUE;
  spSemantic[RULE_SDP_CRLF] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_CRLF] = APG_TRUE;
  spSemantic[RULE_SDP_ALPHA] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_ALPHA] = APG_TRUE;
  spSemantic[RULE_SDP_DIGIT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_DIGIT] = APG_TRUE;
  spSemantic[RULE_SDP_VCHAR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_VCHAR] = APG_TRUE;
  spSemantic[RULE_SDP_WSP] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_WSP] = APG_TRUE;
  spSemantic[RULE_SDP_HEXDIG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_HEXDIG] = APG_TRUE;
  spSemantic[RULE_SDP_POS_DIGIT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SDP_POS_DIGIT] = APG_TRUE;


  return spSemantic;
}




