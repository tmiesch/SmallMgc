/*
 * q931globalfsm.h
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#ifndef Q931GLOBALFSM_H_
#define Q931GLOBALFSM_H_

#include "linux/types.h"
#include "set"
#include "q931channel.h"
#include "q931call.h"

enum q931_global_state
{
	Q931_GLOBAL_STATE_NULL,
	Q931_GLOBAL_STATE_RESTART_REQUEST,
	Q931_GLOBAL_STATE_RESTART
};

class Q931_GlobalFsm {
public:
	Q931_GlobalFsm(class Q931_Interface *intf);
	virtual ~Q931_GlobalFsm();

	void dispatch(class Q931_Message *msg);

	void restartConfirm(class Q931_DataLink *dlc, class Q931_Channel *chan);

	void managementRestartRequest(class Q931_DataLink *dlc,	std::set<Q931_Channel*>& chanset,
		class Q931Ies *user_ies);
	const char* stateToText(enum q931_global_state state);
	void setState(enum q931_global_state state);
	__u8 stateToIEState(enum q931_global_state state);
	void handleStatus(class Q931_Message *msg);
	int decodeIes(class Q931_Message *msg);
	int channelIsRestartable(class Q931_Channel *chan, class Q931_DataLink *dlc);
	void handleRestart(class Q931_Message *msg);
	void handleRestartAcknowledge(class Q931_Message *msg);
	void timerT316(void *data);
	void timerT317(void *data);
	void dispatchMessage(class Q931_Message *msg);

        enum q931_ie_cause_location q931IeCauseLocation();
	enum q931_ie_cause_location q931IeCauseLocationPrivate(enum Q931_Call::Q931CallDirection direction, enum lapd_intf_role role);
	enum q931_ie_cause_location q931IeCauseLocationLocal(enum Q931_Call::Q931CallDirection direction, enum lapd_intf_role role);
    class Q931_Interface *getIntf() const
    {
        return m_intf;
    }

    enum q931_global_state getState() const
    {
        return state;
    }

private:
	class Q931_Interface *m_intf;

	enum q931_global_state state;

	int T316;
	int T317;
	int T317_expired;

	int restart_retransmit_count;
	int restart_responded;
	int restart_acknowledged;

	class Q931IeRestartIndicator *restart_indicator;
	std::set<Q931_Channel*> restart_reqd_chans;
	std::set<Q931_Channel*> restart_acked_chans;

	class Q931_DataLink *restart_dlc;
};

#endif /* Q931GLOBALFSM_H_ */
