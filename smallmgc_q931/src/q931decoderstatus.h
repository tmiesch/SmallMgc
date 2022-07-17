/*
 * q931decoderstatus.h
 *
 *  Created on: 13.04.2009
 *      Author: thomas
 */

#ifndef Q931DECODERSTATUS_H_
#define Q931DECODERSTATUS_H_
#include <list>
#include "linux/types.h"
#include "q931messagetype.h"
#include "Q931_ie.h"

#define Q931_SOIE_TYPE1 1
#define Q931_SOIE_TYPE2 2
#define Q931_SINGLE_OCTET_ID_MASK 0xF0
#define Q931_SINGLE_OCTET_VALUE_MASK 0x0F
#define Q931_IE_COMPREHENSION_REQUIRED_MASK 0xF0


class Q931_DecoderStatus {
public:
	Q931_DecoderStatus();
	virtual ~Q931_DecoderStatus();
	void decodeIes(class Q931_Message *msg, class Q931_Interface* interface);
	void setDiagnosticsMandatoryIe(class Q931IeCause* ie);
	void setDiagnosticsInvalidMandatoryIe(class Q931IeCause* ie);
	void setDiagnosticsInvalidOptionalIe(class Q931IeCause* ie);
	void setDiagnosticsInvalidAccessIe(class Q931IeCause* ie);
	void setDiagnosticsUnrecognizedIe(class Q931IeCause* ie);


	void addMandatoryIe(class Q931Ie* ie);
	void addMissingMandatoryIe(class Q931Ie* ie);
	void addInvalidMandatoryIe(class Q931Ie* ie);
	void addInvalidOptionalIe(class Q931Ie* ie);
	void addInvalidAccessIe(class Q931Ie* ie);
	void addUnrecognizedIe(class Q931Ie* ie);

	int countMandatoryIe();
	int countMissingMandatoryIe();
	int countInvalidMandatoryIe();
	int countInvalidOptionalIe();
	int countInvalidAccessIe();
	int countUnrecognizedIe();
private:
	int q931_check_ie_size(Q931_Message *msg, Q931Ie *ie_class, int ie_len);

	const struct Q931_Ie_Usage *q931_get_ie_usage(enum q931_message_type message_type,__u8 codeset,	enum q931_ie_id ie_id);
	class Q931Ie* q931GetIeClass(__u8 codeset, __u8 ie_id);
	void decodeIe(class Q931_Message *msg,class Q931_Interface *intf,
		__u8 codeset,__u8 ie_id,__u8 *buf, __u8 len, bool is_vl);
	__u8 q931DecodeShiftIe(class Q931_Message *msg,	__u8 ie_id);
	int q931_get_so_ie_type(__u8 ie_id)
	{
		return ((ie_id & 0x70) == 0x20) ? Q931_SOIE_TYPE2 : Q931_SOIE_TYPE1;
	};


	int q931_get_so_ie_id(__u8 ie_id)
	{
		return (q931_get_so_ie_type(ie_id) == Q931_SOIE_TYPE1) ?
				(ie_id & Q931_SINGLE_OCTET_ID_MASK) :
				ie_id;
	};

	int q931_get_so_ie_type2_value(__u8 ie_id)
	{
		return ie_id & Q931_SINGLE_OCTET_VALUE_MASK;
	};

	int q931_is_so_ie(__u8 ie_id)
	{
		return ie_id & 0x80;
	}

    int q931_ie_comprehension_required(__u8 ie_id)
	{
		return (ie_id & Q931_IE_COMPREHENSION_REQUIRED_MASK) == 0;
	}


	int curie;

	__u8 active_codeset;
	int previous_ie_id[8];

	std::list<class Q931Ie*> mandatory_ies;

	std::list<class Q931Ie*> missing_mand_ies;
	std::list<class Q931Ie*> invalid_mand_ies;
	std::list<class Q931Ie*> invalid_opt_ies;
	std::list<class Q931Ie*> invalid_access_ies;
	std::list<class Q931Ie*> unrecognized_ies;

	std::list<struct Q931_Ie_Usage*>mandatory_ies_usage;

};

#endif /* Q931DECODERSTATUS_H_ */
