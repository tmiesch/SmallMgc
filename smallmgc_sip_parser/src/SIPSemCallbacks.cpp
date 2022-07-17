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
#include "sip_parser.h"

// semantic call back function template
static ulong sem_MyFunc(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipParserTest(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_version(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipVersion(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_requestLine(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipRequestLine(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_request(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipRequest(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_response(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipResponse(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_registerm(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipRegisterM(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_from(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipFrom(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_to(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipTo(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_via(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipVia(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_callid(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipCallId(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_cseq(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipCSeq(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_proxy_authen(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipProxyAuthen(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_proxy_authorization(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipProxyAuthorization(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_contact_params(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipContactParams(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_contact(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipContact(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_method(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMethod(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_statusLine(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipStatusLine(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_statusCode(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipStatusCode(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_reasonPhrase(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipReasonPhrase(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_sipURI(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipSipURI(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_sipsURI(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipSipsURI(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_host(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipHost(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_hostName(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipHostName(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_domainLabel(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipDomainLabel(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_topLabel(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipTopLabel(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_requestURI(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipRequestURI(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_absolutURI(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipAbsolutURI(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_ipv4Address(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipIPV4Address(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_messageHeader(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageHeader(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_messageBody(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}


//TODO: parser variables and function must be defined

static ulong sip_invitem(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_ackm(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_optionm(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_byem(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_cancelm(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_extension_method(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_userinfo(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_user(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_user_unreserved(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_password(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_hostport(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_port(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_uri_parameters(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_uri_parameter(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_transport_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_other_transport(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_user_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_other_user(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_method_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_ttl_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_maddr_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_lr_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_other_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_pname(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_pvalue(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_paramchar(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_unreserved(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_headers(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_header(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_hname(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_hvalue(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_hnv_unreserved(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_hier_part(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_net_path(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_abs_path(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_opaque_part(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_uric(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_uric_no_slash(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_path_segments(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_segment(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_pchar(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_scheme(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_authority(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_srvr(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_reg_name(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_query(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_nc_value(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_nonce_count(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_cnonce_value(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_cnonce(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_message_qop(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_digest_uri_value(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_digest_uri(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_username_value(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_username(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_dig_resp(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_digest_response(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_credentials(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_authorization(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_alert_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_language_range(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_alert_info(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_language(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_accept_language(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_coding(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_codings(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_encoding(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_accept_encoding(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_gen_value(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_generic_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_accept_param(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_media_range(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_accept_range(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_accept(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}

static ulong sip_qvalue(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMessageBody(ulOffset, ulLen);
    break;
  }
  return ulRet;
}


static ulong sip_expires(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipExpires(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_userAgent(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipUserAgent(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_maxForwards(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipMaxForwards(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_allow(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipAllow(ulOffset, ulLen);
    break;
  }
  return ulRet;
}
static ulong sip_contentLength(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
 ulong ulRet = PFN_OK;
  switch(ulState)
  {
  case SEM_PRE:
    break;
  case SEM_POST:
    SIPSTACK_Parser::setSipContentLength(ulOffset, ulLen);
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
static SEM_CALLBACK g_saSemantic[RULE_COUNT_SIP];
SEM_CALLBACK* spSemanticCallbackInit_SIP(SEM_CALLBACK* spUserBuffer, ulong* ulpAst)
{
  SEM_CALLBACK* spSemantic = g_saSemantic;
  if(spUserBuffer != NULL){spSemantic = spUserBuffer;}
  memset((void*)spSemantic, 0, sizeof(SEM_CALLBACK) * RULE_COUNT_SIP);
  spSemantic[RULE_SIP_SIP_MESSAGE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SIP_MESSAGE] = APG_TRUE;
  spSemantic[RULE_SIP_REQUEST] = (SEM_CALLBACK)sip_request;
  ulpAst[RULE_SIP_REQUEST] = APG_TRUE;
  spSemantic[RULE_SIP_REQUEST_LINE] = (SEM_CALLBACK)sip_requestLine;
  ulpAst[RULE_SIP_REQUEST_LINE] = APG_TRUE;
  spSemantic[RULE_SIP_SIP_VERSION] = (SEM_CALLBACK)sip_version;
  ulpAst[RULE_SIP_SIP_VERSION] = APG_TRUE;
  spSemantic[RULE_SIP_INVITEM] = (SEM_CALLBACK)sip_invitem;
  ulpAst[RULE_SIP_INVITEM] = APG_TRUE;
  spSemantic[RULE_SIP_ACKM] = (SEM_CALLBACK)sip_ackm;
  ulpAst[RULE_SIP_ACKM] = APG_TRUE;
  spSemantic[RULE_SIP_OPTIONSM] = (SEM_CALLBACK)sip_optionm;
  ulpAst[RULE_SIP_OPTIONSM] = APG_TRUE;
  spSemantic[RULE_SIP_BYEM] = (SEM_CALLBACK)sip_byem;
  ulpAst[RULE_SIP_BYEM] = APG_TRUE;
  spSemantic[RULE_SIP_CANCELM] = (SEM_CALLBACK)sip_cancelm;
  ulpAst[RULE_SIP_CANCELM] = APG_TRUE;
  spSemantic[RULE_SIP_REGISTERM] = (SEM_CALLBACK)sip_registerm;
  ulpAst[RULE_SIP_REGISTERM] = APG_TRUE;
  spSemantic[RULE_SIP_METHOD] = (SEM_CALLBACK)sip_method;
  ulpAst[RULE_SIP_METHOD] = APG_TRUE;
  spSemantic[RULE_SIP_EXTENSION_METHOD] = (SEM_CALLBACK)sip_extension_method;
  ulpAst[RULE_SIP_EXTENSION_METHOD] = APG_TRUE;
  spSemantic[RULE_SIP_RESPONSE] = (SEM_CALLBACK)sip_response;
  ulpAst[RULE_SIP_RESPONSE] = APG_TRUE;
  spSemantic[RULE_SIP_STATUS_LINE] = (SEM_CALLBACK)sip_statusLine;
  ulpAst[RULE_SIP_STATUS_LINE] = APG_TRUE;
  spSemantic[RULE_SIP_STATUS_CODE] = (SEM_CALLBACK)sip_statusCode;
  ulpAst[RULE_SIP_STATUS_CODE] = APG_TRUE;
  spSemantic[RULE_SIP_REASON_PHRASE] = (SEM_CALLBACK)sip_reasonPhrase;
  ulpAst[RULE_SIP_REASON_PHRASE] = APG_TRUE;
  spSemantic[RULE_SIP_SIP_URI] = (SEM_CALLBACK)sip_sipURI;
  ulpAst[RULE_SIP_SIP_URI] = APG_TRUE;
  spSemantic[RULE_SIP_SIPS_URI] = (SEM_CALLBACK)sip_sipsURI;
  ulpAst[RULE_SIP_SIPS_URI] = APG_TRUE;
  spSemantic[RULE_SIP_USERINFO] = (SEM_CALLBACK)sip_userinfo;
  ulpAst[RULE_SIP_USERINFO] = APG_TRUE;
  spSemantic[RULE_SIP_USER] = (SEM_CALLBACK)sip_user;
  ulpAst[RULE_SIP_USER] = APG_TRUE;
  spSemantic[RULE_SIP_USER_UNRESERVED] = (SEM_CALLBACK)sip_user_unreserved;
  ulpAst[RULE_SIP_USER_UNRESERVED] = APG_TRUE;
  spSemantic[RULE_SIP_PASSWORD] = (SEM_CALLBACK)sip_password;
  ulpAst[RULE_SIP_PASSWORD] = APG_TRUE;
  spSemantic[RULE_SIP_HOSTPORT] = (SEM_CALLBACK)sip_hostport;
  ulpAst[RULE_SIP_HOSTPORT] = APG_TRUE;
  spSemantic[RULE_SIP_HOST] = (SEM_CALLBACK)sip_host;
  ulpAst[RULE_SIP_HOST] = APG_TRUE;
  spSemantic[RULE_SIP_HOSTNAME] = (SEM_CALLBACK)sip_hostName;
  ulpAst[RULE_SIP_HOSTNAME] = APG_TRUE;
  spSemantic[RULE_SIP_DOMAINLABEL] = (SEM_CALLBACK)sip_domainLabel;
  ulpAst[RULE_SIP_DOMAINLABEL] = APG_TRUE;
  spSemantic[RULE_SIP_TOPLABEL] = (SEM_CALLBACK)sip_topLabel;
  ulpAst[RULE_SIP_TOPLABEL] = APG_TRUE;
  spSemantic[RULE_SIP_PORT] = (SEM_CALLBACK)sip_port;
  ulpAst[RULE_SIP_PORT] = APG_TRUE;
  spSemantic[RULE_SIP_URI_PARAMETERS] = (SEM_CALLBACK)sip_uri_parameters;
  ulpAst[RULE_SIP_URI_PARAMETERS] = APG_TRUE;
  spSemantic[RULE_SIP_URI_PARAMETER] = (SEM_CALLBACK)sip_uri_parameter;
  ulpAst[RULE_SIP_URI_PARAMETER] = APG_TRUE;
  spSemantic[RULE_SIP_TRANSPORT_PARAM] = (SEM_CALLBACK)sip_transport_param;
  ulpAst[RULE_SIP_TRANSPORT_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_TRANSPORT] = (SEM_CALLBACK)sip_other_transport;
  ulpAst[RULE_SIP_OTHER_TRANSPORT] = APG_TRUE;
  spSemantic[RULE_SIP_USER_PARAM] = (SEM_CALLBACK)sip_user_param;
  ulpAst[RULE_SIP_USER_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_USER] = (SEM_CALLBACK)sip_other_user;
  ulpAst[RULE_SIP_OTHER_USER] = APG_TRUE;
  spSemantic[RULE_SIP_METHOD_PARAM] = (SEM_CALLBACK)sip_method_param;
  ulpAst[RULE_SIP_METHOD_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_TTL_PARAM] = (SEM_CALLBACK)sip_ttl_param;
  ulpAst[RULE_SIP_TTL_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_MADDR_PARAM] = (SEM_CALLBACK)sip_maddr_param;
  ulpAst[RULE_SIP_MADDR_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_LR_PARAM] = (SEM_CALLBACK)sip_lr_param;
  ulpAst[RULE_SIP_LR_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_PARAM] = (SEM_CALLBACK)sip_other_param;
  ulpAst[RULE_SIP_OTHER_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_PNAME] = (SEM_CALLBACK)sip_pname;
  ulpAst[RULE_SIP_PNAME] = APG_TRUE;
  spSemantic[RULE_SIP_PVALUE] = (SEM_CALLBACK)sip_pvalue;
  ulpAst[RULE_SIP_PVALUE] = APG_TRUE;
  spSemantic[RULE_SIP_PARAMCHAR] = (SEM_CALLBACK)sip_paramchar;
  ulpAst[RULE_SIP_PARAMCHAR] = APG_TRUE;
  spSemantic[RULE_SIP_PARAM_UNRESERVED] = (SEM_CALLBACK)sip_unreserved;
  ulpAst[RULE_SIP_PARAM_UNRESERVED] = APG_TRUE;
  spSemantic[RULE_SIP_HEADERS] = (SEM_CALLBACK)sip_headers;
  ulpAst[RULE_SIP_HEADERS] = APG_TRUE;
  spSemantic[RULE_SIP_HEADER] = (SEM_CALLBACK)sip_header;
  ulpAst[RULE_SIP_HEADER] = APG_TRUE;
  spSemantic[RULE_SIP_HNAME] = (SEM_CALLBACK)sip_hname;
  ulpAst[RULE_SIP_HNAME] = APG_TRUE;
  spSemantic[RULE_SIP_HVALUE] = (SEM_CALLBACK)sip_hvalue;
  ulpAst[RULE_SIP_HVALUE] = APG_TRUE;
  spSemantic[RULE_SIP_HNV_UNRESERVED] = (SEM_CALLBACK)sip_hnv_unreserved;
  ulpAst[RULE_SIP_HNV_UNRESERVED] = APG_TRUE;
  spSemantic[RULE_SIP_REQUEST_URI] = (SEM_CALLBACK)sip_requestURI;
  ulpAst[RULE_SIP_REQUEST_URI] = APG_TRUE;
  spSemantic[RULE_SIP_ABSOLUTEURI] = (SEM_CALLBACK)sip_absolutURI;
  ulpAst[RULE_SIP_ABSOLUTEURI] = APG_TRUE;
  spSemantic[RULE_SIP_HIER_PART] = (SEM_CALLBACK)sip_hier_part;
  ulpAst[RULE_SIP_HIER_PART] = APG_TRUE;
  spSemantic[RULE_SIP_NET_PATH] = (SEM_CALLBACK)sip_net_path;
  ulpAst[RULE_SIP_NET_PATH] = APG_TRUE;
  spSemantic[RULE_SIP_ABS_PATH] = (SEM_CALLBACK)sip_abs_path;
  ulpAst[RULE_SIP_ABS_PATH] = APG_TRUE;
  spSemantic[RULE_SIP_OPAQUE_PART] = (SEM_CALLBACK)sip_opaque_part;
  ulpAst[RULE_SIP_OPAQUE_PART] = APG_TRUE;
  spSemantic[RULE_SIP_URIC] = (SEM_CALLBACK)sip_uric;
  ulpAst[RULE_SIP_URIC] = APG_TRUE;
  spSemantic[RULE_SIP_URIC_NO_SLASH] = (SEM_CALLBACK)sip_uric_no_slash;
  ulpAst[RULE_SIP_URIC_NO_SLASH] = APG_TRUE;
  spSemantic[RULE_SIP_PATH_SEGMENTS] = (SEM_CALLBACK)sip_path_segments;
  ulpAst[RULE_SIP_PATH_SEGMENTS] = APG_TRUE;
  spSemantic[RULE_SIP_SEGMENT] = (SEM_CALLBACK)sip_segment;
  ulpAst[RULE_SIP_SEGMENT] = APG_TRUE;
  spSemantic[RULE_SIP_PARAM] = (SEM_CALLBACK)sip_param;
  ulpAst[RULE_SIP_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_PCHAR] = (SEM_CALLBACK)sip_pchar;
  ulpAst[RULE_SIP_PCHAR] = APG_TRUE;
  spSemantic[RULE_SIP_SCHEME] = (SEM_CALLBACK)sip_scheme;
  ulpAst[RULE_SIP_SCHEME] = APG_TRUE;
  spSemantic[RULE_SIP_AUTHORITY] = (SEM_CALLBACK)sip_authority;
  ulpAst[RULE_SIP_AUTHORITY] = APG_TRUE;
  spSemantic[RULE_SIP_SRVR] = (SEM_CALLBACK)sip_srvr;
  ulpAst[RULE_SIP_SRVR] = APG_TRUE;
  spSemantic[RULE_SIP_REG_NAME] = (SEM_CALLBACK)sip_reg_name;
  ulpAst[RULE_SIP_REG_NAME] = APG_TRUE;
  spSemantic[RULE_SIP_QUERY] = (SEM_CALLBACK)sip_query;
  ulpAst[RULE_SIP_QUERY] = APG_TRUE;
  spSemantic[RULE_SIP_IPV4ADDRESS] = (SEM_CALLBACK)sip_ipv4Address;
  ulpAst[RULE_SIP_IPV4ADDRESS] = APG_TRUE;
  spSemantic[RULE_SIP_IPV6REFERENCE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_IPV6REFERENCE] = APG_TRUE;
  spSemantic[RULE_SIP_IPV6ADDRESS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_IPV6ADDRESS] = APG_TRUE;
  spSemantic[RULE_SIP_MESSAGE_HEADER] = (SEM_CALLBACK)sip_messageHeader;
  ulpAst[RULE_SIP_MESSAGE_HEADER] = APG_TRUE;
  spSemantic[RULE_SIP_ACCEPT] = (SEM_CALLBACK)sip_accept;
  ulpAst[RULE_SIP_ACCEPT] = APG_TRUE;
  spSemantic[RULE_SIP_ACCEPT_RANGE] = (SEM_CALLBACK)sip_accept_range;
  ulpAst[RULE_SIP_ACCEPT_RANGE] = APG_TRUE;
  spSemantic[RULE_SIP_MEDIA_RANGE] = (SEM_CALLBACK)sip_media_range;
  ulpAst[RULE_SIP_MEDIA_RANGE] = APG_TRUE;
  spSemantic[RULE_SIP_ACCEPT_PARAM] = (SEM_CALLBACK)sip_accept_param;
  ulpAst[RULE_SIP_ACCEPT_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_QVALUE] = (SEM_CALLBACK)sip_qvalue;
  ulpAst[RULE_SIP_QVALUE] = APG_TRUE;
  spSemantic[RULE_SIP_GENERIC_PARAM] = (SEM_CALLBACK)sip_generic_param;
  ulpAst[RULE_SIP_GENERIC_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_GEN_VALUE] = (SEM_CALLBACK)sip_gen_value;
  ulpAst[RULE_SIP_GEN_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_ACCEPT_ENCODING] = (SEM_CALLBACK)sip_accept_encoding;
  ulpAst[RULE_SIP_ACCEPT_ENCODING] = APG_TRUE;
  spSemantic[RULE_SIP_ENCODING] = (SEM_CALLBACK)sip_encoding;
  ulpAst[RULE_SIP_ENCODING] = APG_TRUE;
  spSemantic[RULE_SIP_CODINGS] = (SEM_CALLBACK)sip_codings;
  ulpAst[RULE_SIP_CODINGS] = APG_TRUE;
  spSemantic[RULE_SIP_CONTENT_CODING] = (SEM_CALLBACK)sip_coding;
  ulpAst[RULE_SIP_CONTENT_CODING] = APG_TRUE;
  spSemantic[RULE_SIP_ACCEPT_LANGUAGE] = (SEM_CALLBACK)sip_accept_language;
  ulpAst[RULE_SIP_ACCEPT_LANGUAGE] = APG_TRUE;
  spSemantic[RULE_SIP_LANGUAGE] = (SEM_CALLBACK)sip_language;
  ulpAst[RULE_SIP_LANGUAGE] = APG_TRUE;
  spSemantic[RULE_SIP_LANGUAGE_RANGE] = (SEM_CALLBACK)sip_language_range;
  ulpAst[RULE_SIP_LANGUAGE_RANGE] = APG_TRUE;
  spSemantic[RULE_SIP_ALERT_INFO] = (SEM_CALLBACK)sip_alert_info;
  ulpAst[RULE_SIP_ALERT_INFO] = APG_TRUE;
  spSemantic[RULE_SIP_ALERT_PARAM] = (SEM_CALLBACK)sip_alert_param;
  ulpAst[RULE_SIP_ALERT_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_ALLOW] = (SEM_CALLBACK)sip_allow;
  ulpAst[RULE_SIP_ALLOW] = APG_TRUE;
  spSemantic[RULE_SIP_AUTHORIZATION] = (SEM_CALLBACK)sip_authorization;
  ulpAst[RULE_SIP_AUTHORIZATION] = APG_TRUE;
  spSemantic[RULE_SIP_CREDENTIALS] = (SEM_CALLBACK)sip_credentials;
  ulpAst[RULE_SIP_CREDENTIALS] = APG_TRUE;
  spSemantic[RULE_SIP_DIGEST_RESPONSE] = (SEM_CALLBACK)sip_digest_response;
  ulpAst[RULE_SIP_DIGEST_RESPONSE] = APG_TRUE;
  spSemantic[RULE_SIP_DIG_RESP] = (SEM_CALLBACK)sip_dig_resp;
  ulpAst[RULE_SIP_DIG_RESP] = APG_TRUE;
  spSemantic[RULE_SIP_USERNAME] = (SEM_CALLBACK)sip_username;
  ulpAst[RULE_SIP_USERNAME] = APG_TRUE;
  spSemantic[RULE_SIP_USERNAME_VALUE] = (SEM_CALLBACK)sip_username_value;
  ulpAst[RULE_SIP_USERNAME_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_DIGEST_URI] = (SEM_CALLBACK)sip_digest_uri;
  ulpAst[RULE_SIP_DIGEST_URI] = APG_TRUE;
  spSemantic[RULE_SIP_DIGEST_URI_VALUE] = (SEM_CALLBACK)sip_digest_uri_value;
  ulpAst[RULE_SIP_DIGEST_URI_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_MESSAGE_QOP] = (SEM_CALLBACK)sip_message_qop;
  ulpAst[RULE_SIP_MESSAGE_QOP] = APG_TRUE;
  spSemantic[RULE_SIP_CNONCE] = (SEM_CALLBACK)sip_cnonce;
  ulpAst[RULE_SIP_CNONCE] = APG_TRUE;
  spSemantic[RULE_SIP_CNONCE_VALUE] = (SEM_CALLBACK)sip_cnonce_value;
  ulpAst[RULE_SIP_CNONCE_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_NONCE_COUNT] = (SEM_CALLBACK)sip_nonce_count;
  ulpAst[RULE_SIP_NONCE_COUNT] = APG_TRUE;
  spSemantic[RULE_SIP_NC_VALUE] = (SEM_CALLBACK)sip_nc_value;
  ulpAst[RULE_SIP_NC_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_DRESPONSE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DRESPONSE] = APG_TRUE;
  spSemantic[RULE_SIP_REQUEST_DIGEST] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_REQUEST_DIGEST] = APG_TRUE;
  spSemantic[RULE_SIP_AUTH_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_AUTH_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_AUTH_PARAM_NAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_AUTH_PARAM_NAME] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_RESPONSE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_OTHER_RESPONSE] = APG_TRUE;
  spSemantic[RULE_SIP_AUTH_SCHEME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_AUTH_SCHEME] = APG_TRUE;
  spSemantic[RULE_SIP_AUTHENTICATION_INFO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_AUTHENTICATION_INFO] = APG_TRUE;
  spSemantic[RULE_SIP_AINFO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_AINFO] = APG_TRUE;
  spSemantic[RULE_SIP_NEXTNONCE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_NEXTNONCE] = APG_TRUE;
  spSemantic[RULE_SIP_RESPONSE_AUTH] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RESPONSE_AUTH] = APG_TRUE;
  spSemantic[RULE_SIP_RESPONSE_DIGEST] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RESPONSE_DIGEST] = APG_TRUE;
  spSemantic[RULE_SIP_CALL_ID] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CALL_ID] = APG_TRUE;
  spSemantic[RULE_SIP_CALLID] = (SEM_CALLBACK)sip_callid;
  ulpAst[RULE_SIP_CALLID] = APG_TRUE;
  spSemantic[RULE_SIP_CALL_INFO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CALL_INFO] = APG_TRUE;
  spSemantic[RULE_SIP_INFO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_INFO] = APG_TRUE;
  spSemantic[RULE_SIP_INFO_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_INFO_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_CONTACT] = (SEM_CALLBACK)sip_contact;
  ulpAst[RULE_SIP_CONTACT] = APG_TRUE;
  spSemantic[RULE_SIP_CONTACT_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CONTACT_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_NAME_ADDR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_NAME_ADDR] = APG_TRUE;
  spSemantic[RULE_SIP_ADDR_SPEC] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ADDR_SPEC] = APG_TRUE;
  spSemantic[RULE_SIP_DISPLAY_NAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DISPLAY_NAME] = APG_TRUE;
  spSemantic[RULE_SIP_D_NAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_D_NAME] = APG_TRUE;
  spSemantic[RULE_SIP_CONTACT_PARAMS] = (SEM_CALLBACK)sip_contact_params;
  ulpAst[RULE_SIP_CONTACT_PARAMS] = APG_TRUE;
  spSemantic[RULE_SIP_C_P_Q] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_C_P_Q] = APG_TRUE;
  spSemantic[RULE_SIP_C_P_EXPIRES] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_C_P_EXPIRES] = APG_TRUE;
  spSemantic[RULE_SIP_CONTACT_EXTENSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CONTACT_EXTENSION] = APG_TRUE;
  spSemantic[RULE_SIP_DELTA_SECONDS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DELTA_SECONDS] = APG_TRUE;
  spSemantic[RULE_SIP_CONTENT_DISPOSITION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CONTENT_DISPOSITION] = APG_TRUE;
  spSemantic[RULE_SIP_DISP_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DISP_TYPE] = APG_TRUE;
  spSemantic[RULE_SIP_DISP_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DISP_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_HANDLING_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_HANDLING_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_HANDLING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_OTHER_HANDLING] = APG_TRUE;
  spSemantic[RULE_SIP_DISP_EXTENSION_TOKEN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DISP_EXTENSION_TOKEN] = APG_TRUE;
  spSemantic[RULE_SIP_CONTENT_ENCODING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CONTENT_ENCODING] = APG_TRUE;
  spSemantic[RULE_SIP_CONTENT_LANGUAGE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CONTENT_LANGUAGE] = APG_TRUE;
  spSemantic[RULE_SIP_LANGUAGE_TAG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_LANGUAGE_TAG] = APG_TRUE;
  spSemantic[RULE_SIP_PRIMARY_TAG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PRIMARY_TAG] = APG_TRUE;
  spSemantic[RULE_SIP_SUBTAG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SUBTAG] = APG_TRUE;
  spSemantic[RULE_SIP_CONTENT_LENGTH] = (SEM_CALLBACK)sip_contentLength;
  ulpAst[RULE_SIP_CONTENT_LENGTH] = APG_TRUE;
  spSemantic[RULE_SIP_CONTENT_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CONTENT_TYPE] = APG_TRUE;
  spSemantic[RULE_SIP_MEDIA_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_MEDIA_TYPE] = APG_TRUE;
  spSemantic[RULE_SIP_M_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_M_TYPE] = APG_TRUE;
  spSemantic[RULE_SIP_DISCRETE_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DISCRETE_TYPE] = APG_TRUE;
  spSemantic[RULE_SIP_COMPOSITE_TYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_COMPOSITE_TYPE] = APG_TRUE;
  spSemantic[RULE_SIP_EXTENSION_TOKEN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_EXTENSION_TOKEN] = APG_TRUE;
  spSemantic[RULE_SIP_IETF_TOKEN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_IETF_TOKEN] = APG_TRUE;
  spSemantic[RULE_SIP_X_TOKEN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_X_TOKEN] = APG_TRUE;
  spSemantic[RULE_SIP_M_SUBTYPE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_M_SUBTYPE] = APG_TRUE;
  spSemantic[RULE_SIP_IANA_TOKEN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_IANA_TOKEN] = APG_TRUE;
  spSemantic[RULE_SIP_M_PARAMETER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_M_PARAMETER] = APG_TRUE;
  spSemantic[RULE_SIP_M_ATTRIBUTE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_M_ATTRIBUTE] = APG_TRUE;
  spSemantic[RULE_SIP_M_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_M_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_CSEQ] = (SEM_CALLBACK)sip_cseq;
  ulpAst[RULE_SIP_CSEQ] = APG_TRUE;
  spSemantic[RULE_SIP_DATE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DATE] = APG_TRUE;
  spSemantic[RULE_SIP_SIP_DATE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SIP_DATE] = APG_TRUE;
  spSemantic[RULE_SIP_RFC1123_DATE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RFC1123_DATE] = APG_TRUE;
  spSemantic[RULE_SIP_DATE1] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DATE1] = APG_TRUE;
  spSemantic[RULE_SIP_TIME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TIME] = APG_TRUE;
  spSemantic[RULE_SIP_WKDAY] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WKDAY] = APG_TRUE;
  spSemantic[RULE_SIP_MONTH] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_MONTH] = APG_TRUE;
  spSemantic[RULE_SIP_ERROR_INFO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ERROR_INFO] = APG_TRUE;
  spSemantic[RULE_SIP_ERROR_URI] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ERROR_URI] = APG_TRUE;
  spSemantic[RULE_SIP_EXPIRES] = (SEM_CALLBACK)sip_expires;
  ulpAst[RULE_SIP_EXPIRES] = APG_TRUE;
  spSemantic[RULE_SIP_FROM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_FROM] = APG_TRUE;
  spSemantic[RULE_SIP_FROM_SPEC] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_FROM_SPEC] = APG_TRUE;
  spSemantic[RULE_SIP_FROM_PARAM] = (SEM_CALLBACK)sip_from;
  ulpAst[RULE_SIP_FROM_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_TAG_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TAG_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_IN_REPLY_TO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_IN_REPLY_TO] = APG_TRUE;
  spSemantic[RULE_SIP_MAX_FORWARDS] = (SEM_CALLBACK)sip_maxForwards;
  ulpAst[RULE_SIP_MAX_FORWARDS] = APG_TRUE;
  spSemantic[RULE_SIP_MIME_VERSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_MIME_VERSION] = APG_TRUE;
  spSemantic[RULE_SIP_MIN_EXPIRES] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_MIN_EXPIRES] = APG_TRUE;
  spSemantic[RULE_SIP_ORGANIZATION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ORGANIZATION] = APG_TRUE;
  spSemantic[RULE_SIP_PRIORITY] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PRIORITY] = APG_TRUE;
  spSemantic[RULE_SIP_PRIORITY_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PRIORITY_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_PRIORITY] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_OTHER_PRIORITY] = APG_TRUE;
  spSemantic[RULE_SIP_PROXY_AUTHENTICATE] = (SEM_CALLBACK)sip_proxy_authen;
  ulpAst[RULE_SIP_PROXY_AUTHENTICATE] = APG_TRUE;
  spSemantic[RULE_SIP_CHALLENGE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_CHALLENGE] = APG_TRUE;
  spSemantic[RULE_SIP_OTHER_CHALLENGE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_OTHER_CHALLENGE] = APG_TRUE;
  spSemantic[RULE_SIP_DIGEST_CLN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DIGEST_CLN] = APG_TRUE;
  spSemantic[RULE_SIP_REALM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_REALM] = APG_TRUE;
  spSemantic[RULE_SIP_REALM_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_REALM_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_DOMAIN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DOMAIN] = APG_TRUE;
  spSemantic[RULE_SIP_URI] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_URI] = APG_TRUE;
  spSemantic[RULE_SIP_NONCE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_NONCE] = APG_TRUE;
  spSemantic[RULE_SIP_NONCE_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_NONCE_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_OPAQUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_OPAQUE] = APG_TRUE;
  spSemantic[RULE_SIP_STALE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_STALE] = APG_TRUE;
  spSemantic[RULE_SIP_ALGORITHM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ALGORITHM] = APG_TRUE;
  spSemantic[RULE_SIP_QOP_OPTIONS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_QOP_OPTIONS] = APG_TRUE;
  spSemantic[RULE_SIP_QOP_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_QOP_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_PROXY_AUTHORIZATION] = (SEM_CALLBACK)sip_proxy_authorization;
  ulpAst[RULE_SIP_PROXY_AUTHORIZATION] = APG_TRUE;
  spSemantic[RULE_SIP_PROXY_REQUIRE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PROXY_REQUIRE] = APG_TRUE;
  spSemantic[RULE_SIP_OPTION_TAG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_OPTION_TAG] = APG_TRUE;
  spSemantic[RULE_SIP_RECORD_ROUTE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RECORD_ROUTE] = APG_TRUE;
  spSemantic[RULE_SIP_REC_ROUTE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_REC_ROUTE] = APG_TRUE;
  spSemantic[RULE_SIP_RR_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RR_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_REPLY_TO] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_REPLY_TO] = APG_TRUE;
  spSemantic[RULE_SIP_RPLYTO_SPEC] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RPLYTO_SPEC] = APG_TRUE;
  spSemantic[RULE_SIP_RPLYTO_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RPLYTO_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_REQUIRE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_REQUIRE] = APG_TRUE;
  spSemantic[RULE_SIP_RETRY_AFTER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RETRY_AFTER] = APG_TRUE;
  spSemantic[RULE_SIP_RETRY_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RETRY_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_ROUTE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ROUTE] = APG_TRUE;
  spSemantic[RULE_SIP_ROUTE_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ROUTE_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_SERVER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SERVER] = APG_TRUE;
  spSemantic[RULE_SIP_SERVER_VAL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SERVER_VAL] = APG_TRUE;
  spSemantic[RULE_SIP_PRODUCT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PRODUCT] = APG_TRUE;
  spSemantic[RULE_SIP_PRODUCT_VERSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PRODUCT_VERSION] = APG_TRUE;
  spSemantic[RULE_SIP_SUBJECT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SUBJECT] = APG_TRUE;
  spSemantic[RULE_SIP_SUPPORTED] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SUPPORTED] = APG_TRUE;
  spSemantic[RULE_SIP_TIMESTAMP] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TIMESTAMP] = APG_TRUE;
  spSemantic[RULE_SIP_DELAY] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DELAY] = APG_TRUE;
  spSemantic[RULE_SIP_TO] = (SEM_CALLBACK)sip_to;
  ulpAst[RULE_SIP_TO] = APG_TRUE;
  spSemantic[RULE_SIP_TO_PARAM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TO_PARAM] = APG_TRUE;
  spSemantic[RULE_SIP_UNSUPPORTED] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_UNSUPPORTED] = APG_TRUE;
  spSemantic[RULE_SIP_USER_AGENT] = (SEM_CALLBACK)sip_userAgent;
  ulpAst[RULE_SIP_USER_AGENT] = APG_TRUE;
  spSemantic[RULE_SIP_VIA] = (SEM_CALLBACK)sip_via;
  ulpAst[RULE_SIP_VIA] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_PARM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_PARM] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_PARAMS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_PARAMS] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_TTL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_TTL] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_MADDR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_MADDR] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_RECEIVED] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_RECEIVED] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_BRANCH] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_BRANCH] = APG_TRUE;
  spSemantic[RULE_SIP_VIA_EXTENSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VIA_EXTENSION] = APG_TRUE;
  spSemantic[RULE_SIP_SENT_PROTOCOL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SENT_PROTOCOL] = APG_TRUE;
  spSemantic[RULE_SIP_PROTOCOL_NAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PROTOCOL_NAME] = APG_TRUE;
  spSemantic[RULE_SIP_PROTOCOL_VERSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PROTOCOL_VERSION] = APG_TRUE;
  spSemantic[RULE_SIP_TRANSPORT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TRANSPORT] = APG_TRUE;
  spSemantic[RULE_SIP_SENT_BY] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SENT_BY] = APG_TRUE;
  spSemantic[RULE_SIP_TTL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TTL] = APG_TRUE;
  spSemantic[RULE_SIP_WARNING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WARNING] = APG_TRUE;
  spSemantic[RULE_SIP_WARNING_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WARNING_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_WARN_CODE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WARN_CODE] = APG_TRUE;
  spSemantic[RULE_SIP_WARN_AGENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WARN_AGENT] = APG_TRUE;
  spSemantic[RULE_SIP_WARN_TEXT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WARN_TEXT] = APG_TRUE;
  spSemantic[RULE_SIP_PSEUDONYM] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PSEUDONYM] = APG_TRUE;
  spSemantic[RULE_SIP_WWW_AUTHENTICATE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WWW_AUTHENTICATE] = APG_TRUE;
  spSemantic[RULE_SIP_EXTENSION_HEADER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_EXTENSION_HEADER] = APG_TRUE;
  spSemantic[RULE_SIP_HEADER_NAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_HEADER_NAME] = APG_TRUE;
  spSemantic[RULE_SIP_HEADER_VALUE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_HEADER_VALUE] = APG_TRUE;
  spSemantic[RULE_SIP_MESSAGE_BODY] = (SEM_CALLBACK)sip_messageBody;
  ulpAst[RULE_SIP_MESSAGE_BODY] = APG_TRUE;
  spSemantic[RULE_SIP_TELEPHONE_SUBSCRIBER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TELEPHONE_SUBSCRIBER] = APG_TRUE;
  spSemantic[RULE_SIP_GLOBAL_PHONE_NUMBER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_GLOBAL_PHONE_NUMBER] = APG_TRUE;
  spSemantic[RULE_SIP_BASE_PHONE_NUMBER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_BASE_PHONE_NUMBER] = APG_TRUE;
  spSemantic[RULE_SIP_LOCAL_PHONE_NUMBER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_LOCAL_PHONE_NUMBER] = APG_TRUE;
  spSemantic[RULE_SIP_ISDN_SUBADDRESS] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ISDN_SUBADDRESS] = APG_TRUE;
  spSemantic[RULE_SIP_POST_DIAL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_POST_DIAL] = APG_TRUE;
  spSemantic[RULE_SIP_AREA_SPECIFIER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_AREA_SPECIFIER] = APG_TRUE;
  spSemantic[RULE_SIP_PHONE_CONTEXT_TAG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PHONE_CONTEXT_TAG] = APG_TRUE;
  spSemantic[RULE_SIP_PHONE_CONTEXT_IDENT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PHONE_CONTEXT_IDENT] = APG_TRUE;
  spSemantic[RULE_SIP_NETWORK_PREFIX] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_NETWORK_PREFIX] = APG_TRUE;
  spSemantic[RULE_SIP_GLOBAL_NETWORK_PREFIX] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_GLOBAL_NETWORK_PREFIX] = APG_TRUE;
  spSemantic[RULE_SIP_LOCAL_NETWORK_PREFIX] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_LOCAL_NETWORK_PREFIX] = APG_TRUE;
  spSemantic[RULE_SIP_PRIVATE_PREFIX] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PRIVATE_PREFIX] = APG_TRUE;
  spSemantic[RULE_SIP_SERVICE_PROVIDER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_SERVICE_PROVIDER] = APG_TRUE;
  spSemantic[RULE_SIP_PROVIDER_TAG] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PROVIDER_TAG] = APG_TRUE;
  spSemantic[RULE_SIP_PROVIDER_HOSTNAME] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PROVIDER_HOSTNAME] = APG_TRUE;
  spSemantic[RULE_SIP_FUTURE_EXTENSION] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_FUTURE_EXTENSION] = APG_TRUE;
  spSemantic[RULE_SIP_TOKEN_CHAR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TOKEN_CHAR] = APG_TRUE;
  spSemantic[RULE_SIP_TEL_QUOTED_STRING] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_TEL_QUOTED_STRING] = APG_TRUE;
  spSemantic[RULE_SIP_PHONEDIGIT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PHONEDIGIT] = APG_TRUE;
  spSemantic[RULE_SIP_VISUAL_SEPARATOR] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_VISUAL_SEPARATOR] = APG_TRUE;
  spSemantic[RULE_SIP_PAUSE_CHARACTER] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_PAUSE_CHARACTER] = APG_TRUE;
  spSemantic[RULE_SIP_ONE_SECOND_PAUSE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_ONE_SECOND_PAUSE] = APG_TRUE;
  spSemantic[RULE_SIP_WAIT_FOR_DIAL_TONE] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_WAIT_FOR_DIAL_TONE] = APG_TRUE;
  spSemantic[RULE_SIP_DTMF_DIGIT] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_DTMF_DIGIT] = APG_TRUE;
  spSemantic[RULE_SIP_RFC1035DOMAIN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RFC1035DOMAIN] = APG_TRUE;
  spSemantic[RULE_SIP_RFC1035SUBDOMAIN] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RFC1035SUBDOMAIN] = APG_TRUE;
  spSemantic[RULE_SIP_RFC1035LABEL] = (SEM_CALLBACK)sem_MyFunc;
  ulpAst[RULE_SIP_RFC1035LABEL] = APG_TRUE;


  return spSemantic;
}




