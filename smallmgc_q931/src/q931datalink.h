/*
 * q931datalink.h
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#ifndef Q931DATALINK_H_
#define Q931DATALINK_H_

#define LAPD_BROADCAST_TEI	127

enum q931_dlc_status
{
	Q931_DLC_STATUS_DISCONNECTED,
	Q931_DLC_STATUS_AWAITING_CONNECTION,
	Q931_DLC_STATUS_AWAITING_DISCONNECTION,
	Q931_DLC_STATUS_CONNECTED,
};


class Q931_DataLink {
public:
	Q931_DataLink(class Q931_Interface* intf, int tei);
	virtual ~Q931_DataLink();
	void hold();
	void release();
	void releaseTimerExpired();
    class Q931_Interface *getIntf() const
    {
        return m_intf;
    }

    enum q931_dlc_status getStatus() const
    {
        return status;
    }

    void setStatus(enum q931_dlc_status status)
    {
        this->status = status;
    }

    int getTei() const
    {
        return m_tei;
    }

    void setTei(int m_tei)
    {
        this->m_tei = m_tei;
    }

private:
	class Q931_Interface *m_intf;
	int holdcnt;
	int refcnt;

	enum q931_dlc_status status;
	int m_tei;

	//struct q931_timer autorelease_timer;
};

#endif /* Q931DATALINK_H_ */
