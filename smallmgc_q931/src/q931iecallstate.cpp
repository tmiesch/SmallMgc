/*
 * q931iecallstate.cpp
 *
 *  Created on: 22.03.2009
 *      Author: thomas
 */

#include "q931iecallstate.h"

Q931IeCallState::Q931IeCallState() {
	// TODO Auto-generated constructor stub
  type           = Q931_IE_TYPE_VL;
  max_len        = 3;
  max_occur      = 1;
  network_type   = Q931_NT_ETSI;
  codeset        = 0;
  id             = Q931_IE_CALL_STATE;
  name           = "Call State";
}

Q931IeCallState::~Q931IeCallState() {
	// TODO Auto-generated destructor stub
}

void Q931IeCallState::ieRegister(class q931_ie_class *ie_class)
{
	my_class = ie_class;
}

 int Q931IeCallState::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
	//Q931_IeCallState *ie =
	//	container_of(abstract_ie,
	//		struct q931_ie_call_state, ie);

	int nextoct = 0;
	if (len < 1) {
		// error log
		return false;
	}

	struct q931_ie_call_state_onwire_3 *oct_3 =
		(struct q931_ie_call_state_onwire_3 *)
		(buf + nextoct++);

	if (oct_3->coding_standard != Q931_IE_CS_CS_CCITT) {
		//report_ie(abstract_ie, LOG_ERR, "coding stanrdard != CCITT\n");
		return false;
	}

	coding_standard = (enum q931_ie_call_state_coding_standard)oct_3->coding_standard;
	value = (enum q931_ie_call_state_value)oct_3->value;

	return true;
}

int Q931IeCallState::writeToBuf(__u8 *buf, int max_size)
{
	int len = 0;
	//struct q931_ie_call_state *ie =
	//	container_of(abstract_ie, struct q931_ie_call_state, ie);

	struct q931_ie_call_state_onwire_3 *oct_3 =
		(struct q931_ie_call_state_onwire_3 *)
		(buf + len);
	oct_3->raw = 0;
	oct_3->coding_standard = coding_standard;
	oct_3->value = value;
	len++;

	return len;
}

Q931IeCallState* Q931IeCallState::copy()
{
  Q931IeCallState* ie = new Q931IeCallState();
  ie->setValues(this);
  ie->coding_standard = coding_standard;
  ie->value = value;

  return ie;

}

const char *Q931IeCallState::codingStandardToText(enum q931_ie_call_state_coding_standard coding_standard)
{
	switch(coding_standard) {
	case Q931_IE_CS_CS_CCITT:
		return "CCITT";
	case Q931_IE_CS_CS_RESERVED:
		return "Reserved";
	case Q931_IE_CS_CS_NATIONAL:
		return "National";
	case Q931_IE_CS_CS_SPECIFIC:
		return "Specific";
	default:
		return "*INVALID*";
	}
}

const char *Q931IeCallState::valueToText(enum q931_ie_call_state_value value)
{
	switch(value) {
	case Q931_IE_CS_N0_NULL_STATE:
		return "0_NULL_STATE";
	case Q931_IE_CS_N1_CALL_INITIATED:
		return "1_CALL_INITIATED";
	case Q931_IE_CS_N2_OVERLAP_SENDING:
		return "2_OVERLAP_SENDING";
	case Q931_IE_CS_N3_OUTGOING_CALL_PROCEEDING:
		return "3_OUTGOING_CALL_PROCEEDING";
	case Q931_IE_CS_N4_CALL_DELIVERED:
		return "4_CALL_DELIVERED";
	case Q931_IE_CS_N6_CALL_PRESENT:
		return "6_CALL_PRESENT";
	case Q931_IE_CS_N7_CALL_RECEIVED:
		return "7_CALL_RECEIVED";
	case Q931_IE_CS_N8_CONNECT_REQUEST:
		return "8_CONNECT_REQUEST";
	case Q931_IE_CS_N9_INCOMING_CALL_PROCEEDING:
		return "9_INCOMING_CALL_PROCEEDING";
	case Q931_IE_CS_N10_ACTIVE:
		return "10_ACTIVE";
	case Q931_IE_CS_N11_DISCONNECT_REQUEST:
		return "11_DISCONNECT_REQUEST";
	case Q931_IE_CS_N12_DISCONNECT_INDICATION:
		return "12_DISCONNECT_INDICATION";
	case Q931_IE_CS_N15_SUSPEND_REQUEST:
		return "15_SUSPEND_REQUEST";
	case Q931_IE_CS_N17_RESUME_REQUEST:
		return "17_RESUME_REQUEST";
	case Q931_IE_CS_N19_RELEASE_REQUEST:
		return "19_RELEASE_REQUEST";
	case Q931_IE_CS_N22_CALL_ABORT:
		return "22_CALL_ABORT";
	case Q931_IE_CS_N25_OVERLAP_RECEIVING:
		return "25_OVERLAP_RECEIVING";
	case Q931_IE_CS_REST1:
		return "REST0";
	case Q931_IE_CS_REST2:
		return "REST0";
	default:
		return "*INVALID*";
	}
}

void Q931IeCallState::dump(const Q931Ie *abstract_ie, const char *prefix)
{
	//struct q931_ie_call_state *ie =
	//	container_of(abstract_ie, struct q931_ie_call_state, ie);

//	report_ie_dump(abstract_ie,
//		"%sCoding standard = %s (%d)\n", prefix,
//		q931_ie_call_state_coding_standard_to_text(
//			ie->coding_standard),
//		ie->coding_standard);
//
//	report_ie_dump(abstract_ie,
//		"%sState value = %s (%d)\n", prefix,
//		q931_ie_call_state_value_to_text(
//			ie->value),
//		ie->value);
}
