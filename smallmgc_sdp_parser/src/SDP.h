/********************************************************************
  APG - an ABNF Parser Generator
  Copyright (C) 2009 Coast to Coast Research, Inc.
 
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
  
	 created: Sun May 31 10:48:45 2009
	filename: SDP.h
	  author: Lowell Thomas
	          lowell@coasttocoastresearch.com
	          http://www.coasttocoastresearch.com

	 purpose: Header file for parser SDP.

*********************************************************************/
#ifndef SDP_h
#define SDP_h

// rule name IDs
#define RULE_SDP_ANNOUNCEMENT 0
#define RULE_SDP_SESSION_SECTION 1
#define RULE_SDP_PROTO_VERSION 2
#define RULE_SDP_ORIGIN_FIELD 3
#define RULE_SDP_SESSION_NAME_FIELD 4
#define RULE_SDP_INFORMATION_FIELD 5
#define RULE_SDP_URI_FIELD 6
#define RULE_SDP_EMAIL_FIELD 7
#define RULE_SDP_PHONE_FIELD 8
#define RULE_SDP_CONNECTION_FIELD 9
#define RULE_SDP_BANDWIDTH_FIELD 10
#define RULE_SDP_TIME_FIELDS 11
#define RULE_SDP_REPEAT_FIELDS 12
#define RULE_SDP_ZONE_ADJUSTMENTS 13
#define RULE_SDP_KEY_FIELD 14
#define RULE_SDP_ATTRIBUTE_FIELD 15
#define RULE_SDP_MEDIA_SECTION 16
#define RULE_SDP_MEDIA_FIELD 17
#define RULE_SDP_MEDIA 18
#define RULE_SDP_FMT 19
#define RULE_SDP_PROTO 20
#define RULE_SDP_MEDIAPORT 21
#define RULE_SDP_PORTCOUNT 22
#define RULE_SDP_USERNAME 23
#define RULE_SDP_SESS_ID 24
#define RULE_SDP_SESS_VERSION 25
#define RULE_SDP_NETTYPE 26
#define RULE_SDP_ADDRTYPE 27
#define RULE_SDP_EMAIL_ADDRESS 28
#define RULE_SDP_ADDRESS_AND_COMMENT 29
#define RULE_SDP_DISPNAME_AND_ADDRESS 30
#define RULE_SDP_PHONE_NUMBER 31
#define RULE_SDP_PHONE 32
#define RULE_SDP_BWTYPE 33
#define RULE_SDP_BANDWIDTH 34
#define RULE_SDP_START_TIME 35
#define RULE_SDP_STOP_TIME 36
#define RULE_SDP_TIME 37
#define RULE_SDP_REPEAT_INTERVAL 38
#define RULE_SDP_TYPED_TIME 39
#define RULE_SDP_FIXED_LEN_TIME_UNIT 40
#define RULE_SDP_KEY_TYPE 41
#define RULE_SDP_BASE64 42
#define RULE_SDP_BASE64_UNIT 43
#define RULE_SDP_BASE64_PAD 44
#define RULE_SDP_BASE64_CHAR 45
#define RULE_SDP_ATTRIBUTE 46
#define RULE_SDP_ATT_FIELD 47
#define RULE_SDP_ATT_VALUE 48
#define RULE_SDP_CONNECTION_TYPE 49
#define RULE_SDP_ANY_CONNECTION 50
#define RULE_SDP_FQDN 51
#define RULE_SDP_EXTN_ADDR 52
#define RULE_SDP_IPV4CONNECTION 53
#define RULE_SDP_IPV4TYPE 54
#define RULE_SDP_IPV4MULTICON 55
#define RULE_SDP_TTL 56
#define RULE_SDP_ADDRESSCOUNT 57
#define RULE_SDP_IPV6CONNECTION 58
#define RULE_SDP_IPV6TYPE 59
#define RULE_SDP_IPV6MULTCON 60
#define RULE_SDP_TEXT 61
#define RULE_SDP_BYTE_STRING 62
#define RULE_SDP_NON_WS_STRING 63
#define RULE_SDP_TOKEN_CHAR 64
#define RULE_SDP_TOKEN 65
#define RULE_SDP_EMAIL_SAFE 66
#define RULE_SDP_ALPHA_NUMERIC 67
#define RULE_SDP_URI_REFERENCE 68
#define RULE_SDP_DEC_OCTET 69
#define RULE_SDP_IPV4MULTICAST 70
#define RULE_SDP_IPV4UNICAST 71
#define RULE_SDP_INT224 72
#define RULE_SDP_IPV6MULTICAST 73
#define RULE_SDP_IPV6UNICAST 74
#define RULE_SDP_ADDR_SPEC 75
#define RULE_SDP_LOCAL_PART 76
#define RULE_SDP_DOMAIN 77
#define RULE_SDP_DOMAIN_LITERAL 78
#define RULE_SDP_DCONTENT 79
#define RULE_SDP_DTEXT 80
#define RULE_SDP_NO_WS_CTL 81
#define RULE_SDP_ATEXT 82
#define RULE_SDP_ATOM 83
#define RULE_SDP_DOT_ATOM 84
#define RULE_SDP_DOT_ATOM_TEXT 85
#define RULE_SDP_QTEXT 86
#define RULE_SDP_QCONTENT 87
#define RULE_SDP_QUOTED_STRING 88
#define RULE_SDP_QUOTED_PAIR 89
#define RULE_SDP_OBS_QP 90
#define RULE_SDP_OBS_LOCAL_PART 91
#define RULE_SDP_OBS_DOMAIN 92
#define RULE_SDP_WORD 93
#define RULE_SDP_FWS 94
#define RULE_SDP_CTEXT 95
#define RULE_SDP_CCONTENT 96
#define RULE_SDP_COMMENT 97
#define RULE_SDP_CFWS 98
#define RULE_SDP_OBS_FWS 99
#define RULE_SDP_SP 100
#define RULE_SDP_HTAB 101
#define RULE_SDP_DQUOTE 102
#define RULE_SDP_CRLF 103
#define RULE_SDP_ALPHA 104
#define RULE_SDP_DIGIT 105
#define RULE_SDP_VCHAR 106
#define RULE_SDP_WSP 107
#define RULE_SDP_HEXDIG 108
#define RULE_SDP_POS_DIGIT 109
#define RULE_COUNT_SDP 110

// uniquely named pointer to parser initialization data
extern void* vpParserInit_SDP;

#endif // SDP_h
