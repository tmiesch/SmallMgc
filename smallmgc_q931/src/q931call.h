/*
 * Q931Stack.h
 *
 *  Created on: 09.03.2009
 *      Author: thomas
 */

#ifndef Q931STACK_H_
#define Q931STACK_H_

#include "linux/types.h"
#include "Q931_ie.h"
#include "q931iecallstate.h"
#include "q931ccbmessage.h"
#include "q931interface.h"
#include <list>
#define Q931_MAX_DIGITS 20



class Q931Callref
{

};

/*#define q931_call_get(call) _q931_call_get((call), __FILE__,  __LINE__)
#define q931_call_put_nonull(call) _q931_call_put((call), __FILE__,  __LINE__)
#define q931_call_put(call) do { q931_call_put_nonull(call); call = NULL; } while(0)
*/
/*#define report_call(call, lvl, format, arg...)				\
		q931_report((lvl),						\
			"%s:CALL[%u.%c]: "					\
			format,							\
			(call)->intf->name,					\
			(call)->call_reference,					\
			((call)->direction ==					\
				Q931_CALL_DIRECTION_OUTBOUND ? 'O' : 'I'),	\
			## arg)
 */
/*	#define q931_call_start_timer(call, timer)			\
		do {							\
			_q931_call_start_timer(call,			\
				&(call)->timer,				\
				(call)->intf->timer);			\
			report_call(call, LOG_DEBUG,			\
				"%s:%d Timer %s started (%lld us)\n",	\
				__FILE__,__LINE__,			\
				#timer,					\
				(call)->intf->timer);			\
		} while(0)

	#define q931_call_restart_timer(call, timer)			\
		do {							\
			_q931_call_restart_timer(call,			\
				&(call)->timer,				\
				(call)->intf->timer);			\
			report_call(call, LOG_DEBUG,			\
				"%s:%d Timer %s restarted (%lld us)\n",	\
				__FILE__,__LINE__,			\
				#timer,					\
				(call)->intf->timer);			\
		} while(0)

 *
 */

/*#define q931_call_stop_timer(call, timer)			\
		do {							\
			_q931_call_stop_timer(call, &(call)->timer);	\
			report_call(call, LOG_DEBUG,			\
				"%s:%d Timer %s stopped\n",		\
				__FILE__,__LINE__,			\
				#timer);				\
		} while(0)

	#define q931_call_timer_running(call, timer)		\
			q931_timer_pending(&(call)->timer)	\

	#define q931_call_primitive(call, primitive, ies)	\
			q931_queue_primitive(call, primitive, ies, 0, 0)

	#define q931_call_primitive1(call, primitive, ies, par1)	\
			q931_queue_primitive(call, primitive, ies, par1, 0)

	#define q931_call_primitive2(call, primitive, ies, par1, par2)	\
			q931_queue_primitive(call, primitive, ies, par1, par2)

 *
 */

class Q931_Call {
public:
	Q931_Call();
	~Q931_Call();

	enum Q931CallDirection
	{
		Q931_CALL_DIRECTION_OUTBOUND	= 0x0,
		Q931_CALL_DIRECTION_INBOUND	= 0x1};
    enum Q931CallState{ N0_NULL_STATE, N1_CALL_INITIATED, N2_OVERLAP_SENDING, N3_OUTGOING_CALL_PROCEEDING, N4_CALL_DELIVERED, N6_CALL_PRESENT, N7_CALL_RECEIVED, N8_CONNECT_REQUEST, N9_INCOMING_CALL_PROCEEDING, N10_ACTIVE, N11_DISCONNECT_REQUEST, N12_DISCONNECT_INDICATION, N15_SUSPEND_REQUEST, N17_RESUME_REQUEST, N19_RELEASE_REQUEST, N22_CALL_ABORT, N25_OVERLAP_RECEIVING, NOT_VALID = 0xFF};
    enum Q931SetupConfirmStatus{ Q931_SETUP_CONFIRM_OK, Q931_SETUP_CONFIRM_ERROR};
    enum Q931SetupCompleteIndicationStatus{ Q931_SETUP_COMPLETE_INDICATION_OK, Q931_SETUP_COMPLETE_INDICATION_ERROR};
    enum Q931SuspendConfirmStatus{ Q931_SUSPEND_CONFIRM_OK, Q931_SUSPEND_CONFIRM_ERROR, Q931_SUSPEND_CONFIRM_TIMEOUT};
    enum Q931ReleaseConfirmStatus{ Q931_RELEASE_CONFIRM_OK, Q931_RELEASE_CONFIRM_ERROR};
    enum Q931ResumeConfirmStatus{ Q931_RESUME_CONFIRM_OK, Q931_RESUME_CONFIRM_ERROR, Q931_RESUME_CONFIRM_TIMEOUT};
    enum Q931StatusIndicationStatus{ Q931_STATUS_INDICATION_OK, Q931_STATUS_INDICATION_ERROR};
public:
    Q931_Call(class Q931_Adapter *adapter, class Q931_Interface *intf, int ref);

    enum Q931_Call::Q931CallState getState() const
    {
        return state;
    }

    void setState(enum Q931_Call::Q931CallState state)
    {
        this->state = state;
    }

    enum Q931_Call::Q931CallDirection getDirection() const
    {
        return direction;
    }

    void setDirection(Q931_Call::Q931CallDirection direction)
    {
        this->direction = direction;
    }

    int getCall_reference() const
    {
        return call_reference;
    }

    void setCall_reference(int call_reference)
    {
        this->call_reference = call_reference;
    }

    class Q931_Channel *channelAlloc();
    class Q931_Channel *getChannel() const
    {
        return channel;
    }

    void addCes(class Q931_Ces *ces);
    void removeCes(class Q931_Ces *ces);

    void setChannel(class Q931_Channel *channel)
    {
        this->channel = channel;
    }
    int getCesNumber()
    {
      return m_ces.size();
    }

    int cesWaitingConnection();

    void q931CallPrimitive(enum q931_primitive primitive, class Q931Ies *ies, unsigned long  par1 = 0, unsigned long  par2 = 0);
    class Q931_Call *q931CallAllocOut(class Q931Interface *intf);
    const char *q931CallStateToText(enum Q931CallState state);
    class Q931_Call *q931GetCallByReference(class Q931Interface *intf, enum Q931CallDirection direction, int call_reference);
    void q931CallReleaseReference();
    class Q931_Call *q931CallAllocIn(class Q931Interface *intf, class Q931Dlc *dlc, int call_reference);
    void q931CallDlEstablishIndication();
    void q931CallDlEstablishConfirm();
    void q931CallDlReleaseIndication();
    void q931CallDlReleaseConfirm();
    void q931CallStopAnyTimer();
    void q931HandleAlerting(class Q931_Message *msg);
    void q931HandleCallProceeding(class Q931_Message *msg);
    void q931HandleConnect(class Q931_Message *msg);
    void q931HandleConnectAcknowledge(class Q931_Message *msg);
    void q931HandleProgress(class Q931_Message *msg);
    void q931HandleSetup(class Q931_Message *msg);
    void q931HandleSetupAcknowledge(class Q931_Message *msg);
    void q931SaveCause(class Q931Ies *ies, Q931Ies *src_ies);
    void q931HandleDisconnect(class Q931_Message *msg);
    void q931HandleRelease(class Q931_Message *msg);
    void q931HandleReleaseComplete(class Q931_Message *msg);
    void q931HandleStatus(class Q931_Message *msg);
    void q931HandleStatusEnquiry(class Q931_Message *msg);
    void q931HandleInfo(class Q931_Message *msg);
    void q931HandleNotify(class Q931_Message *msg);
    void q931HandleResume(class Q931_Message *msg);
    void q931HandleSuspendAcknowledge(class Q931_Message *msg);
    void q931HandleSuspendReject(class Q931_Message *msg);
    void q931HandleSuspend(class Q931_Message *msg);
    void q931HandleResumeAcknowledge(class Q931_Message *msg);
    void q931HandleResumeReject(class Q931_Message *msg);
    void q931IntAlertingIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931IntConnectIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931IntCallProceedingIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931IntReleaseCompleteIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931IntInfoIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931IntProgressIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931IntReleaseIndication(class Q931_Ces *ces, class Q931Ies *ies);
    void q931AlertingRequest(class Q931Ies *ies);
    void q931DisconnectRequest(class Q931Ies *ies);
    void q931InfoRequest(class Q931Ies *ies);
    void q931MoreInfoRequest(class Q931Ies *ies);
    void q931NotifyRequest(class Q931Ies *ies);
    void q931ProceedingRequest(class Q931Ies *ies);
    void q931ProgressRequest(class Q931Ies *ies);
    void q931RejectRequest(class Q931Ies *ies);
    void q931ReleaseRequest(class Q931Ies *ies);
    void q931ResumeRequest(class Q931Ies *ies);
    void q931ResumeRejectRequest(class Q931Ies *ies);
    void q931ResumeResponse(class Q931Ies *ies);
    void q931SetupCompleteRequest(class Q931Ies *ies);
    void q931SetupRequest(class Q931Ies *ies);
    void q931SsetupResponse(class Q931Ies *ies);
    void q931StatusEnquiryRequest(class Q931_Ces *ces, class Q931Ies *ies);
    void q931SuspendRejectRequest(class Q931Ies *ies);
    void q931SuspendResponse(class Q931Ies *ies);
    void q931SuspendRequest(class Q931Ies *ies);
    void q931RestartRequest(class Q931Ies *ies);
    int q931DecodeIes(class Q931_Message *msg);
    int q931_timer_pending(int timerid);
    enum q931_ie_cause_location q931IeCauseLocation();
    enum q931_ie_cause_location q931IeCauseLocationPrivate(enum Q931_Call::Q931CallDirection direction, enum lapd_intf_role role);
    enum q931_ie_cause_location q931IeCauseLocationLocal(enum Q931_Call::Q931CallDirection direction, enum lapd_intf_role role);

    void q931_call_send_call_status(class Q931Ies *user_ies);
    void q931CallPut(const char *file, int line);
    void q931_timer_T301();
    void q931_timer_T302();
    void q931_timer_T303();
    void q931_timer_T304();
    void q931_timer_T305();
    void q931_timer_T306();
    void q931_timer_T308();
    void q931_timer_T309();
    void q931_timer_T310();
    void q931_timer_T312();
    void q931_timer_T313();
    void q931_timer_T314();
    void q931_timer_T316();
    void q931_timer_T318();
    void q931_timer_T319();
    void q931_timer_T320();
    void q931_timer_T321();
    void q931_timer_T322();
    Q931_Interface *getInterface() const
    {
        return interface;
    }

    class Q931_DataLink *getDlc() const
    {
        return m_dlc;
    }

    class Q931_Ces* getCes(Q931_DataLink* dlc);
    class Q931_Ces *getPreselected_ces() const
    {
        return preselected_ces;
    }

    class Q931_Ces *getSelected_ces() const
    {
        return selected_ces;
    }
    int q931_call_send_release(class Q931Ies *Ies);
    int q931_call_send_release_complete(class Q931Ies *Ies);
    void q931CallStartTimer(Q931Timers timerId);
    void q931DispatchMessage(class Q931_Message *msg);
    std::string getCalled_Number() const
    {
        return called_Number;
    }

    void setCalled_Number(std::string called_Number)
    {
        this->called_Number = called_Number;
    }

    void addDigit(std::string digit);

    void setDlc(class Q931_DataLink *m_dlc)
    {
        this->m_dlc = m_dlc;
    }
    void q931CallClearTimer(Q931Timers timerId);
private:
    enum q931_ie_call_state_value q931CallStateToIeState(enum Q931_Call::Q931CallState);
    void q931IntfDelCall();
    void q931CallUnexpectedPrimitive(const char *event);
    void q931CallUnexpectedTimer(const char *event);
    void q931CallMessageNotCompatibleWithState(Q931_Message *msg, const char *event, enum Q931_Call::Q931CallState new_state = Q931_Call::NOT_VALID);
    int q931ChannelSelectResponse(class Q931Ies *ies, class Q931Ies *causes);
    class Q931_Channel *q931ChannelSelectSetup(class Q931Ies *setup_ies, class Q931Ies *causes, int *bumping_needed);
    Q931_Call *q931CallGet(const char *file, int line);
    Q931_Call *q931CallAlloc(Q931Interface *intf);
    void q931CallRestartTimer(Q931Timers timerId);
    void q931CallStopTimer(Q931Timers timerId);
    int q931CallTimerDuration(Q931Timers timerId);
    int q931CallTimerRunning(Q931Timers timerId);
    int q931_call_send_alerting(class Q931Ies *Ies);
    int q931_call_send_call_proceeding(class Q931Ies *Ies);
    int q931_call_send_connect(class Q931Ies *Ies);
    int q931_call_send_connect_acknowledge(class Q931Ies *Ies);
    int q931_call_send_disconnect(class Q931Ies *Ies);
    int q931_call_send_information(class Q931Ies *Ies);
    int q931_call_send_notify(class Q931Ies *Ies);
    int q931_call_send_progress(class Q931Ies *Ies);
    int q931_call_send_resume(class Q931Ies *Ies);
    int q931_call_send_resume_acknowledge(class Q931Ies *Ies);
    int q931_call_send_resume_reject(class Q931Ies *Ies);
    int q931_call_send_setup(class Q931Ies *Ies);
    int q931_call_send_setup_bc(class Q931Ies *Ies);
    int q931_call_send_setup_acknowledge(class Q931Ies *Ies);
    int q931_call_send_status(class Q931Ies *Ies);
    int q931_call_send_status_enquiry(class Q931Ies *Ies);
    int q931_call_send_suspend(class Q931Ies *Ies);
    int q931_call_send_suspend_acknowledge(class Q931Ies *Ies);
    int q931_call_send_suspend_reject(class Q931Ies *Ies);

private:
    int refcnt;
    Q931_Interface *interface;
    class Q931_Channel *channel;
    class Q931_Adapter *m_adapter;
    enum Q931_Call::Q931CallState state;
    enum Q931_Call::Q931CallDirection direction;
    int call_reference;
    std::list<class Q931_Ces*> m_ces;
    class Q931_Ces *preselected_ces;
    class Q931_Ces *selected_ces;
    class Q931_DataLink *m_dlc;
    int broadcast_setup;
    int tones_option;
    int T303_fired;
    int T308_fired;
    int senq_status_97_98_received;
    int senq_cnt;
    int disconnect_indication_sent;
    int release_complete_received;
    int digits_dialled;
    class Q931Ies *setup_ies;
    class Q931Ies *disconnect_cause;
    class Q931Ies *saved_cause;
    class Q931Ies *release_cause;
    std::list<class Q931_Channel*> m_channel;
    std::string called_Number;

	void *pvt;
    class TimerQ931* m_timer[3];

};

#endif /* Q931STACK_H_ */
