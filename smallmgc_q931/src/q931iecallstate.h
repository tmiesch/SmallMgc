/*
 * q931iecallcstate.h
 *
 *  Created on: 22.03.2009
 *      Author: thomas
 */

#ifndef Q931IECALLCSTATE_H_
#define Q931IECALLCSTATE_H_

#include "linux/types.h"
#include "Q931_ie.h"

enum q931_ie_call_state_coding_standard
{
	Q931_IE_CS_CS_CCITT	= 0x0,
	Q931_IE_CS_CS_RESERVED	= 0x1,
	Q931_IE_CS_CS_NATIONAL	= 0x2,
	Q931_IE_CS_CS_SPECIFIC	= 0x3,
};

enum q931_ie_call_state_value
{
	Q931_IE_CS_N0_NULL_STATE		= 0x00,
	Q931_IE_CS_N1_CALL_INITIATED		= 0x01,
	Q931_IE_CS_N2_OVERLAP_SENDING		= 0x02,
	Q931_IE_CS_N3_OUTGOING_CALL_PROCEEDING	= 0x03,
	Q931_IE_CS_N4_CALL_DELIVERED		= 0x04,
	Q931_IE_CS_N6_CALL_PRESENT		= 0x06,
	Q931_IE_CS_N7_CALL_RECEIVED		= 0x07,
	Q931_IE_CS_N8_CONNECT_REQUEST		= 0x08,
	Q931_IE_CS_N9_INCOMING_CALL_PROCEEDING	= 0x09,
	Q931_IE_CS_N10_ACTIVE			= 0x0A,
	Q931_IE_CS_N11_DISCONNECT_REQUEST	= 0x0B,
	Q931_IE_CS_N12_DISCONNECT_INDICATION	= 0x0C,
	Q931_IE_CS_N15_SUSPEND_REQUEST		= 0x0F,
	Q931_IE_CS_N17_RESUME_REQUEST		= 0x11,
	Q931_IE_CS_N19_RELEASE_REQUEST		= 0x13,
	Q931_IE_CS_N22_CALL_ABORT		= 0x16,
	Q931_IE_CS_N25_OVERLAP_RECEIVING	= 0x19,

	Q931_IE_CS_U0_NULL_STATE		= 0x00,
	Q931_IE_CS_U1_CALL_INITIATED		= 0x01,
	Q931_IE_CS_U2_OVERLAP_SENDING		= 0x02,
	Q931_IE_CS_U3_OUTGOING_CALL_PROCEEDING	= 0x03,
	Q931_IE_CS_U4_CALL_DELIVERED		= 0x04,
	Q931_IE_CS_U6_CALL_PRESENT		= 0x06,
	Q931_IE_CS_U7_CALL_RECEIVED		= 0x07,
	Q931_IE_CS_U8_CONNECT_REQUEST		= 0x08,
	Q931_IE_CS_U9_INCOMING_CALL_PROCEEDING	= 0x09,
	Q931_IE_CS_U10_ACTIVE			= 0x0A,
	Q931_IE_CS_U11_DISCONNECT_REQUEST	= 0x0B,
	Q931_IE_CS_U12_DISCONNECT_INDICATION	= 0x0C,
	Q931_IE_CS_U15_SUSPEND_REQUEST		= 0x0F,
	Q931_IE_CS_U17_RESUME_REQUEST		= 0x11,
	Q931_IE_CS_U19_RELEASE_REQUEST		= 0x13,
	Q931_IE_CS_U25_OVERLAP_RECEIVING	= 0x19,

	Q931_IE_CS_REST0			= 0x00,
	Q931_IE_CS_REST1			= 0x3d,
	Q931_IE_CS_REST2			= 0x3e,
};

struct q931_ie_call_state_onwire_3
{
	union { struct {
	#if __BYTE_ORDER == __BIG_ENDIAN
		__u8 coding_standard:2;
		__u8 value:6;
	#else
		__u8 value:6;
		__u8 coding_standard:2;
	#endif
		}; __u8 raw; };
	} __attribute__ ((__packed__));

class Q931IeCallState : public Q931Ie
{
public:
	Q931IeCallState();
	virtual ~Q931IeCallState();

	virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
	virtual int writeToBuf(__u8 *buf, int max_size);
	void dump(const class Q931Ie *ie,	const char *prefix);
	void ieRegister(class q931_ie_class *ie_class);
	virtual Q931IeCallState* copy();
	const char *codingStandardToText(enum q931_ie_call_state_coding_standard coding_standard);
	const char *valueToText(enum q931_ie_call_state_value value);
	enum q931_ie_call_state_coding_standard getCoding_standard() const
	{
	  return coding_standard;
	}

	void setCoding_standard(enum q931_ie_call_state_coding_standard coding_standard)
	{
	  this->coding_standard = coding_standard;
	}

	enum q931_ie_call_state_value getValue() const
	{
	  return value;
	}

	void setValue(enum q931_ie_call_state_value value)
	{
	  this->value = value;
	}
private:
	class Q931Ie ie;

	enum q931_ie_call_state_coding_standard	coding_standard;
	enum q931_ie_call_state_value	value;
	class  q931_ie_class *my_class;
};

#endif /* Q931IECALLCSTATE_H_ */
