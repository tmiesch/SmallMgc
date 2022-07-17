/*
 * q931ces.h
 *
 *  Created on: 07.04.2009
 *      Author: thomas
 */

#ifndef Q931CES_H_
#define Q931CES_H_
#include "q931adapter.h"
#include "q931messagetype.h"
#include "q931call.h"

enum q931_ces_timer
{
	T301,
	T302,
	T303,
	T304,
	T305,
	T306,
	T308,
	T309,
	T310,
	T312,
	T322
};

enum q931_ces_state
{
	I0_NULL_STATE,
	I7_CALL_RECEIVED,
	I8_CONNECT_REQUEST,
	I9_INCOMING_CALL_PROCEEDING,
	I19_RELEASE_REQUEST,
	I25_OVERLAP_RECEIVING,
};

class Q931_Ces {

public:
	Q931_Ces(class Q931_Adapter* adapter, class Q931_Call *call, class Q931_DataLink* dlc);
	virtual ~Q931_Ces();

	const char *q931_ces_state_to_text(enum q931_ces_state state);

	void q931_ces_dispatch_message(class Q931_Message *msg);
    void q931_ces_handle_status_enquiry(class Q931_Message *msg);
    void q931_ces_handle_info(class Q931_Message *msg);
    void q931_ces_handle_status(class Q931_Message *msg);
    void q931_ces_handle_release_complete(class Q931_Message *msg);
    void q931_ces_handle_release(class Q931_Message *msg);
    void q931_ces_handle_disconnect(class Q931_Message *msg);
    void q931_ces_handle_progress(class Q931_Message *msg);
    void q931_ces_handle_connect(class Q931_Message *msg);
    void q931_ces_handle_call_proceeding(class Q931_Message *msg);
    void q931_ces_handle_alerting(class Q931_Message *msg);
    void q931_ces_message_not_compatible_with_state(class Q931_Message *msg);
    void q931_ces_dl_establish_indication();
    void q931_ces_dl_release_indication();
    void q931_ces_dl_establish_confirm();
    void q931_ces_dl_release_confirm();
    void q931_ces_alerting_request(class Q931Ies *user_ies);
    void q931_ces_connect_request(class Q931Ies *user_ies);
    void q931_ces_call_proceeding_request(class Q931Ies *user_ies);
    void q931_ces_setup_ack_request(class Q931Ies *user_ies);
    void q931_ces_release_request(class Q931Ies *user_ies);
    void q931_ces_info_request(class Q931Ies *user_ies);
    void q931_ces_status_enquiry_request(class Q931Ies *user_ies);
    void q931_ces_stop_timer(int timerId);
    void q931_ces_start_timer(int timerId);
    void q931_ces_stop_any_timer();
    __u8 q931_ces_state_to_ie_state(enum q931_ces_state state);
    void q931_ces_unexpected_primitive();
    int  q931_ces_must_be_deleted()
    {
    	return m_delete;
    }
    enum q931_ces_state getState() const
    {
        return m_state;
    }

    class Q931_DataLink *getDlc() const
    {
        return m_dlc;
    }

    inline int q931_ces_send_alerting(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_ALERTING, ies);
    }

    inline int q931_ces_send_call_proceeding(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_CALL_PROCEEDING, ies);
    }

    inline int q931_ces_send_connect(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_CONNECT, ies);
    }

    inline int q931_ces_send_connect_acknowledge(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_CONNECT_ACKNOWLEDGE, ies);
    }

    inline int q931_ces_send_disconnect(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_DISCONNECT, ies);
    }

    inline int q931_ces_send_information(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_INFORMATION, ies);
    }

    inline int q931_ces_send_notify(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_NOTIFY, ies);
    }

    inline int q931_ces_send_progress(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_PROGRESS, ies);
    }

    inline int q931_ces_send_release(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_RELEASE, ies);
    }

    inline int q931_ces_send_release_complete(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_RELEASE_COMPLETE, ies);
    }

    inline int q931_ces_send_resume(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_RESUME, ies);
    }

    inline int q931_ces_send_resume_acknowledge(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_RESUME_ACKNOWLEDGE, ies);
    }

    inline int q931_ces_send_resume_reject(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_RESUME_REJECT, ies);
    }

    inline int q931_ces_send_setup(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_SETUP, ies);
    }

    inline int q931_ces_send_setup_acknowledge(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_SETUP_ACKNOWLEDGE, ies);
    }

    inline int q931_ces_send_status(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_STATUS, ies);
    }

    inline int q931_ces_send_status_enquiry(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_STATUS_ENQUIRY, ies);
    }

    inline int q931_ces_send_suspend(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_SUSPEND, ies);
    }

    inline int q931_ces_send_suspend_acknowledge(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_SUSPEND_ACKNOWLEDGE, ies);
    }

    inline int q931_ces_send_suspend_reject(class Q931Ies *ies)
    {
        return m_adapter->q931_call_send_message(m_call, m_dlc, Q931_MT_SUSPEND_REJECT, ies);
    }

    void q931_ces_set_state(enum q931_ces_state state)
    {
        m_state = state;
    }

private:
    class Q931_Call *m_call;
    class Q931_DataLink *m_dlc;
    class Q931_Adapter *m_adapter;
    enum q931_ces_state m_state;

	int m_T308_fired;

	int m_senq_cause_97_98_received;
	int m_senq_cnt;
	int m_delete;

};

#endif /* Q931CES_H_ */
