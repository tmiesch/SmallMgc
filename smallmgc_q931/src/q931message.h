/*
 * q931message.h
 *
 *  Created on: 08.04.2009
 *      Author: thomas
 */

#ifndef Q931MESSAGE_H_
#define Q931MESSAGE_H_
#include "linux/types.h"
#include "Q931_Ies.h"
#include "q931messagetype.h"

class Q931_Message {
public:
	Q931_Message(class Q931_DataLink* dlc);
	virtual ~Q931_Message();
    int writeIes (class Q931Ies* user_ies);

    int getRefcnt() const
    {
        return refcnt;
    }

    void put()
    {
      refcnt--;
      if (refcnt == 0)
         delete this;
    }

    Q931_Message* get()
    {
      refcnt++;
      return this;
    }

    void setRefcnt(int refcnt)
    {
        this->refcnt = refcnt;
    }

    void setRawies_len(int rawies_len)
    {
        this->rawies_len = rawies_len;
    }

    void setMessage_type(enum q931_message_type message_type)
    {
        this->message_type = message_type;
    }

    int getRawlen() const
    {
        return rawlen;
    }

    void setRawlen(int rawlen)
    {
        this->rawlen = rawlen;
    }

    void setRaw_message_type(__u8 raw_message_type)
    {
        this->raw_message_type = raw_message_type;
    }

    int getCallref() const
    {
        return callref;
    }

    void setCallref(int callref)
    {
        this->callref = callref;
    }

    int getCallref_len() const
    {
        return callref_len;
    }

    void setCallref_len(int callref_len)
    {
        this->callref_len = callref_len;
    }

    enum q931_callref_flag getCallref_direction() const
    {
        return callref_direction;
    }

    void setCallref_direction(enum q931_callref_flag callref_direction)
    {
        this->callref_direction = callref_direction;
    }

    __u8* getRaw()
    {
        return raw;
    }

    void setRaw(std::vector<__u8> raw, int length)
    {

        for (int i= 0; i < length; i++)
           this->raw[i] = raw[i];
        rawlen = length -4 ;
        //memcpy (this->raw, raw, length);
        //this->raw = raw;
    }

    void setDlc(class Q931_DataLink *m_dlc)
    {
        this->m_dlc = m_dlc;
    }

    void setRawies(__u8 *rawies)
    {
        this->rawies = rawies;
    }

    __u8 *getRawies() const
    {
        return rawies;
    }

    int getRawies_len() const
    {
        return rawies_len;
    }

    __u8 getRaw_message_type() const
    {
        return raw_message_type;
    }

    class Q931_DataLink *getDlc() const
    {
        return m_dlc;
    }

    class Q931Ies* getIes()
    {
        return ies;
    }

    enum q931_message_type getMessage_type()
    {
        return message_type;
    }

private:
	int refcnt;

	__u8 raw[512];
	int rawlen;

	class Q931_DataLink *m_dlc;

	__u8 raw_message_type;
	enum q931_message_type message_type;

	int callref;
	int callref_len;
	enum q931_callref_flag callref_direction;

	__u8 *rawies;
	int rawies_len;

	class Q931Ies* ies;

};

#endif /* Q931MESSAGE_H_ */
