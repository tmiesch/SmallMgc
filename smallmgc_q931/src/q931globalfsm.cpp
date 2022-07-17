/*
 * q931globalfsm.cpp
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#include "q931globalfsm.h"
#include "q931interface.h"
#include "q931iecallstate.h"
#include "Q931_Ies.h"
#include "q931message.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_GlobalFsm::Q931_GlobalFsm(class Q931_Interface *intf) : m_intf(intf)
{
	// TODO Auto-generated constructor stub
	//q931_init_timer(&gc->T316, "T316", q931_global_timer_T316, gc);
	//q931_init_timer(&gc->T317, "T317", q931_global_timer_T317, gc);
    state = Q931_GLOBAL_STATE_NULL;

    T316 = 0;
    T317 = 0;
    T317_expired =  0;

    restart_retransmit_count = 0;
    restart_responded = 0;
    restart_acknowledged= 0;

    restart_indicator = 0;
    restart_reqd_chans.clear();
    restart_acked_chans.clear();

    restart_dlc = 0;

}

Q931_GlobalFsm::~Q931_GlobalFsm() {
	// TODO Auto-generated destructor stub
	if (restart_indicator) {
	                restart_indicator->put();
			//_q931_ie_put(&restart_indicator->ie);
			restart_indicator = NULL;
	}
}

enum q931_ie_cause_location Q931_GlobalFsm::q931IeCauseLocation()
{
  if (m_intf->getNetwork_role() == Q931_INTF_NET_USER)
    return Q931_IE_C_L_USER;
  else if (m_intf->getNetwork_role() == Q931_INTF_NET_PRIVATE)
    return q931IeCauseLocationPrivate(Q931_Call::Q931_CALL_DIRECTION_INBOUND, m_intf->getRole());
  else if (m_intf->getNetwork_role() == Q931_INTF_NET_LOCAL)
    return q931IeCauseLocationLocal(Q931_Call::Q931_CALL_DIRECTION_INBOUND, m_intf->getRole());
  else if (m_intf->getNetwork_role() == Q931_INTF_NET_TRANSIT)
    return Q931_IE_C_L_TRANSIT_NETWORK;
  else if (m_intf->getNetwork_role() == Q931_INTF_NET_INTERNATIONAL)
    return Q931_IE_C_L_INTERNATIONAL_NETWORK;

}

enum q931_ie_cause_location Q931_GlobalFsm::q931IeCauseLocationPrivate(enum Q931_Call::Q931CallDirection direction,
    enum lapd_intf_role role)
{
    if (role == LAPD_INTF_ROLE_NT) {
            if (direction == Q931_Call::Q931_CALL_DIRECTION_INBOUND)
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_LOCAL_USER;
            else
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_REMOTE_USER;
    } else {
            if (direction == Q931_Call::Q931_CALL_DIRECTION_INBOUND)
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_REMOTE_USER;
            else
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_LOCAL_USER;
    }
}

enum q931_ie_cause_location Q931_GlobalFsm::q931IeCauseLocationLocal(enum Q931_Call::Q931CallDirection direction,
    enum lapd_intf_role role)
{
  if (role == LAPD_INTF_ROLE_NT) {
          if (direction == Q931_Call::Q931_CALL_DIRECTION_INBOUND)
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_LOCAL_USER;
          else
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_REMOTE_USER;
  } else {
          if (direction == Q931_Call::Q931_CALL_DIRECTION_INBOUND)
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_REMOTE_USER;
          else
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_LOCAL_USER;
  }
}

void Q931_GlobalFsm::dispatch(class Q931_Message *msg)
{
  switch(msg->getMessage_type())
  {
    case Q931_MT_STATUS:
	handleStatus(msg);
    break;

    case Q931_MT_RESTART:
      handleRestart(msg);
    break;

    case Q931_MT_RESTART_ACKNOWLEDGE:
      handleRestartAcknowledge(msg);
    break;

   default:
   {
     Q931Ies* ies = new Q931Ies();

     Q931IeCause *cause = new Q931IeCause();
     cause->setCoding_standard(Q931_IE_C_CS_CCITT);
     cause->setLocation(q931IeCauseLocation());
     cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
     ies->add_put(cause);

     Q931IeCallState* cs = new Q931IeCallState();
     cs->setCoding_standard(Q931_IE_CS_CS_CCITT);
     cs->setValue((enum q931_ie_call_state_value)stateToIEState(state));
     ies->add_put(cs);


     //q931_global_send_message(msg->dlc, Q931_MT_STATUS, &ies);
     delete ies;
   }
   break;
  }
}

const char *Q931_GlobalFsm::stateToText(enum q931_global_state state)
{
	switch (state) {
	case Q931_GLOBAL_STATE_NULL:
		return "NULL";
	case Q931_GLOBAL_STATE_RESTART_REQUEST:
		return "RESTART REQUEST";
	case Q931_GLOBAL_STATE_RESTART:
		return "RESTART";
	}

	return NULL;
}

void Q931_GlobalFsm::setState(enum q931_global_state state)
{
	// debug log
	this->state = state;
}

void Q931_GlobalFsm::managementRestartRequest(Q931_DataLink *dlc,
	std::set<Q931_Channel*>& chanset, class Q931Ies *user_ies)
{
    std::string st(this->stateToText(state));
    Application::instance().logger().debug("Global Restart Request: " + st);

    // debug output
	switch(state) {
	case Q931_GLOBAL_STATE_NULL: {

		T317_expired = false;
		restart_retransmit_count = 0;
		restart_responded = false;
		restart_acknowledged = 0;

		//Q931_DECLARE_IES(ies);
		Q931Ies* ies = new Q931Ies();
		if (restart_indicator->restart_class ==
			       		Q931_IE_RI_C_INDICATED) {

			restart_acked_chans.clear();
			restart_acked_chans = chanset;
			Q931IeChannelIdentification *ci = new Q931IeChannelIdentification();
			ci->interface_id_present = Q931_IE_CI_IIP_IMPLICIT;
			ci->interface_type = m_intf->getIntftype();
			ci->preferred_exclusive = Q931_IE_CI_PE_EXCLUSIVE;
			ci->coding_standard = Q931_IE_CI_CS_CCITT;
			ci->chanset.clear();
			ci->chanset = restart_reqd_chans;
			//q931_ies_add_put(&ies, &ci->ie);
            ies->add_put(ci);

			Q931IeRestartIndicator *ri = new Q931IeRestartIndicator();
			ri->restart_class = Q931_IE_RI_C_INDICATED;
			//q931_ies_add_put(&ies, &ri->ie);
			ies->add_put(ri);

		} else if (restart_indicator->restart_class == 	Q931_IE_RI_C_SINGLE_INTERFACE)
		{
			Q931IeRestartIndicator *ri = new Q931IeRestartIndicator();
			ri->restart_class = Q931_IE_RI_C_SINGLE_INTERFACE;
			//q931_ies_add_put(&ies, &ri->ie);
            ies->add_put(ri);
		} else if (restart_indicator->restart_class ==	Q931_IE_RI_C_ALL_INTERFACES)
		{
		}

		//Q931_UNDECLARE_IES(ies);

		restart_dlc = dlc; // GET/PUT reference TODO

		//q931_global_send_message(gc, dlc, Q931_MT_RESTART, &ies);
		//q931_global_start_timer(gc, T316);
		setState(Q931_GLOBAL_STATE_RESTART_REQUEST);
        delete ies;
		/*
		struct q931_call *call;
		list_for_each_entry(call, &gc->intf->calls, calls_node) {
#if 0
			if (q931_chanset_contains(chanset, call->channel)) {
				q931_restart_request(call, user_ies);
				gc->restart_request_count++;

				if (!q931_global_timer_running(gc, T317))
					q931_global_start_timer(gc, T317);
			} else {
				gc->restart_request_count++;
				q931_global_restart_confirm(gc, dlc,
						/*FIXME*/ /*NULL);
			}
#endif
		}*/
	}
	break;

	case Q931_GLOBAL_STATE_RESTART_REQUEST:
	break;

	case Q931_GLOBAL_STATE_RESTART:
	break;
	}
}

void Q931_GlobalFsm::restartConfirm(Q931_DataLink *dlc,Q931_Channel *chan)
{
	// debug output (gc, LOG_DEBUG, "RESTART-CONFIRM\n");
    std::string st(this->stateToText(state));
    Application::instance().logger().debug("Global Restart Confirm: " + st);

	switch(state) {
	case Q931_GLOBAL_STATE_RESTART_REQUEST:
		if (chan) {
			//q931_chanset_del(&gc->restart_reqd_chans, chan);
			restart_reqd_chans.erase(chan);
			//q931_chanset_add(&gc->restart_acked_chans, chan);
			restart_acked_chans.insert(chan);
		}

		if (restart_reqd_chans.size() == 0) {

			//q931_global_stop_timer(gc, T317);

			if (restart_acknowledged) {
				std::list <Q931_Channel> cs;
				//q931_chanset_init(&cs);

				//q931_chanset_intersect(
				//	&gc->restart_reqd_chans,
				//	&gc->restart_acked_chans);

				setState(Q931_GLOBAL_STATE_NULL);
				//q931_global_primitive1(gc,
				//	Q931_CCB_MANAGEMENT_RESTART_CONFIRM,
				//	&cs);
			} else {
				restart_responded = true;
			}
		}
	break;

	case Q931_GLOBAL_STATE_RESTART:
		if (chan) {
			//q931_chanset_del(&gc->restart_reqd_chans, chan);
			restart_reqd_chans.erase(chan);
			//q931_chanset_add(&gc->restart_acked_chans, chan);
			restart_acked_chans.insert(chan);
		}
		if (restart_reqd_chans.size() == 0) {
			//q931_global_stop_timer(gc, T317);
			setState(Q931_GLOBAL_STATE_NULL);

			Q931Ies* ies = new Q931Ies();
			if (restart_indicator->restart_class ==
				       		Q931_IE_RI_C_INDICATED) {

				Q931IeChannelIdentification *ci = new Q931IeChannelIdentification();
				ci->interface_id_present =	Q931_IE_CI_IIP_IMPLICIT;
				ci->interface_type = m_intf->getIntftype();
				ci->coding_standard = Q931_IE_CI_CS_CCITT;
				ci->preferred_exclusive = Q931_IE_CI_PE_EXCLUSIVE;
				ci->chanset.clear();
				ci->chanset = restart_acked_chans;
				ies->add_put(ci);

				Q931IeRestartIndicator *ri = new Q931IeRestartIndicator();
				ri->restart_class = Q931_IE_RI_C_INDICATED;
				ies->add_put(ri);
			} else if (restart_indicator->restart_class ==
				       		Q931_IE_RI_C_SINGLE_INTERFACE) {
				Q931IeRestartIndicator *ri = new Q931IeRestartIndicator();
				ri->restart_class =	Q931_IE_RI_C_SINGLE_INTERFACE;
				ies->add_put(ri);

			} else if (restart_indicator->restart_class ==
				       		Q931_IE_RI_C_ALL_INTERFACES) {
				//assert(0);
			}

			//q931_global_send_message(gc, dlc,
			//	Q931_MT_RESTART_ACKNOWLEDGE, &ies);

			delete ies;
		}
	break;

	case Q931_GLOBAL_STATE_NULL:
		// Unexpected primitive
	break;
	}
}

__u8 Q931_GlobalFsm::stateToIEState(enum q931_global_state state)
{
	switch (state) {
	case Q931_GLOBAL_STATE_NULL:
		return Q931_IE_CS_REST0;
	case Q931_GLOBAL_STATE_RESTART_REQUEST:
		return Q931_IE_CS_REST1;
	case Q931_GLOBAL_STATE_RESTART:
		return Q931_IE_CS_REST2;
	}

	return 0;
}

int Q931_GlobalFsm::decodeIes(Q931_Message *msg)
{
	return 0;
}


void Q931_GlobalFsm::handleStatus(Q931_Message *msg)
{
    std::string st(this->stateToText(state));
    Application::instance().logger().debug("Global Handle Status: " + st);

	switch(state) {
	case Q931_GLOBAL_STATE_NULL:
		// Do nothing
	break;

	case Q931_GLOBAL_STATE_RESTART_REQUEST:
	case Q931_GLOBAL_STATE_RESTART: {
		if (decodeIes(msg) < 0)
			break;

		Q931IeCallState *cs = NULL;

		int i;
		/*for(i=0; i<msg->ies.count; i++) {
			if (msg->ies.ies[i]->cls->id == Q931_IE_CALL_STATE) {
				cs = container_of(msg->ies.ies[i],
					t q931_ie_call_state, ie);
				break;
			}
		}*/

	/*	if (cs->value == globalStateToIeState(state)) {
			// Implementation dependent
		} else {
			//q931_global_primitive(gc,
			//	Q931_CCB_STATUS_MANAGEMENT_INDICATION);
			// WITH ERROR
		}*/
	}
	break;
	}
}

int Q931_GlobalFsm::channelIsRestartable(Q931_Channel *chan, Q931_DataLink *dlc)
{
	/*return chan->state != Q931_CHANSTATE_MAINTAINANCE &&
		chan->state != Q931_CHANSTATE_LEASED;*/
/*
 * Telecom Italia needs all requested channels to be restarted, even if no
 * calls are active on them
 *
 *		 && (!chan->call || chan->call->dlc == dlc);
*/
}

void Q931_GlobalFsm::handleRestart(class Q931_Message *msg)
{
   std::string st(this->stateToText(state));
   Application::instance().logger().debug("Global Handle Restart: " + st);

	/*switch(gc->state) {
	case Q931_GLOBAL_STATE_NULL:
		if (q931_gc_decode_ies(gc, msg) < 0)
			break;

		gc->T317_expired = FALSE;
		gc->restart_retraQ931_GlobalFsm::nsmit_count = 0;
		gc->restart_responded = FALSE;
		gc->restart_acknowledged = 0;
		gc->restart_dlc = msg->dlc;
		q931_chanset_init(&gc->restart_reqd_chans);
		q931_chanset_init(&gc->restart_acked_chans);

		struct q931_ie_channel_identification *ci = NULL;
		int i;
		for (i=0; i<msg->ies.count; i++) {
			if (msg->ies.ies[i]->cls->id ==
					Q931_IE_CHANNEL_IDENTIFICATION) {

				ci = container_of(msg->ies.ies[i],
					struct q931_ie_channel_identification,
						ie);
Q931_GlobalFsm::
				int j;
				for (j=0; j<q931_chanset_count(&ci->chanset);
									j++) {
					if (q931_channel_is_restartable(
							ci->chanset.chans[j],
							msg->dlc)) {
						q931_chanset_add(
							&gc->restart_reqd_chans,
							ci->chanset.chans[j]);
					}
				}
			} else if (msg->ies.ies[i]->cls->id ==
					Q931_IE_RESTART_INDICATOR) {

				if (gc->restart_indicator) {
				        gc->restart_indicator->put();
					//_q931_ie_put(&gc->restart_indicator->ie);
					gc->restart_indicator = NULL;
				}

				gc->restart_indicator =
					container_of(q931_ie_get(
							msg->ies.ies[i]),
					struct q931_ie_restart_indicator,
					ie);
					gc->restart_indicator->get();
			}
		}

		if (!ci) {
			for (i=0; i<gc->intf->n_channels; i++) {
				if (q931_channel_is_restartable(
					&gc->intf->channels[i], msg->dlc)) {
					q931_chanset_add(
						&gc->restart_reqd_chans,
						&gc->intf->channels[i]);
				}
			}
		}

		// Any indicated channel allowed to be restarted?
		if (q931_chanset_count(&gc->restart_reqd_chans)) {
			q931_global_set_state(gc, Q931_GLOBAL_STATE_RESTART);

			/* Ugly hack to safely delete current within loop */
			/*struct q931_chanset cs;
			q931_chanset_init(&cs);
			q931_chanset_copy(&cs, &gc->restart_reqd_chans);

			for (i=0;i<q931_chanset_count(&cs); i++) {

				if (cs.chans[i]->call) {
					q931_restart_request(
						cs.chans[i]->call,
						&msg->ies);

					if (!q931_global_timer_running(gc,
									T317))
						q931_global_start_timer(gc,
									T317);
				} else {
					q931_global_restart_confirm(gc,
						msg->dlc,
						cs.chans[i]);
				}
			}
		} else {
			Q931_DECLARE_IES(ies);

			struct q931_ie_cause *cause = q931_ie_cause_alloc();
			cause->coding_standard = Q931_IE_C_CS_CCITT;
			cause->location = q931_ie_cause_location_gc(gc);
			cause->value =
				Q931_IE_C_CV_IDENTIFIED_CHANNEL_DOES_NOT_EXIST;
			q931_ies_add_put(&ies, &cause->ie);

			q931_global_send_message(gc, msg->dlc,
				Q931_MT_STATUS, &ies);

			Q931_UNDECLARE_IES(ies);
		}
	break;

	case Q931_GLOBAL_STATE_RESTART_REQUEST:
	case Q931_GLOBAL_STATE_RESTART: {
		Q931_DECLARE_IES(ies);
		struct q931_ie_cause *cause = q931_ie_cause_alloc();
		cause->coding_standard = Q931_IE_C_CS_CCITT;
		cause->location = q931_ie_cause_location_gc(gc);
		cause->value = Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE;
		q931_ies_add_put(&ies, &cause->ie);

		q931_global_send_message(gc, msg->dlc, Q931_MT_STATUS, &ies);

		Q931_UNDECLARE_IES(ies);
	}
	break;
	}*/
}

void Q931_GlobalFsm::handleRestartAcknowledge(class Q931_Message *msg)
{
    std::string st(this->stateToText(state));
    Application::instance().logger().debug("Global Handle Restart Acknowledge: " + st);

	/*switch(gc->state) {
	case Q931_GLOBAL_STATE_RESTART_REQUEST:
		if (q931_gc_decode_ies(gc, msg) < 0)
			break;

		q931_global_stop_timer(gc, T316);

		if (gc->restart_responded || gc->T317_expired) {

			struct q931_chanset cs;
			q931_chanset_init(&cs);

			q931_chanset_intersect(
				&gc->restart_reqd_chans,
				&gc->restart_acked_chans);

			q931_global_primitive1(gc,
				Q931_CCB_MANAGEMENT_RESTART_CONFIRM, &cs);

			q931_global_set_state(gc, Q931_GLOBAL_STATE_NULL);
		} else if (!gc->T317_expired) {

			// Store channel id received in restart ack
			int i;
			for (i=0; i<msg->ies.count; i++) {
				if (msg->ies.ies[i]->cls->id ==
					Q931_IE_CHANNEL_IDENTIFICATION) {

					struct q931_ie_channel_identification *ci =
						container_of(msg->ies.ies[i],
						struct q931_ie_channel_identification, ie);

					q931_chanset_merge(
						&gc->restart_acked_chans,
						&ci->chanset);

					break;
				}
			}

			gc->restart_acknowledged = TRUE;
		}
	break;

	case Q931_GLOBAL_STATE_NULL:
	case Q931_GLOBAL_STATE_RESTART: {
		Q931_DECLARE_IES(ies);
		struct q931_ie_cause *cause = q931_ie_cause_alloc();
		cause->coding_standard = Q931_IE_C_CS_CCITT;
		cause->location = q931_ie_cause_location_gc(gc);
		cause->value = Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE;
		q931_ies_add_put(&ies, &cause->ie);

		q931_global_send_message(gc, msg->dlc, Q931_MT_STATUS, &ies);

		Q931_UNDECLARE_IES(ies);
	}
	break;
	}*/
}

void Q931_GlobalFsm::timerT316(void *data)
{
    std::string st(this->stateToText(state));
    Application::instance().logger().debug("Global Timer T316: " + st);

	/*struct q931_global_call *gc = data;
	assert(data);

	switch(gc->state) {
	case Q931_GLOBAL_STATE_RESTART_REQUEST:
		gc->restart_retransmit_count++;

		if (gc->restart_retransmit_count >= 2) {
			q931_global_set_state(gc, Q931_GLOBAL_STATE_NULL);

			// Indicate that restart has failed
			q931_global_primitive1(gc,
				Q931_CCB_MANAGEMENT_RESTART_CONFIRM, NULL);
		} else {
			Q931_DECLARE_IES(ies);

			if (gc->restart_indicator->restart_class ==
				       		Q931_IE_RI_C_INDICATED) {

				struct q931_ie_channel_identification *ci =
					q931_ie_channel_identification_alloc();

				ci->interface_id_present =
					Q931_IE_CI_IIP_IMPLICIT;
				ci->interface_type =
					q931_ie_channel_identification_intftype(
						gc->intf);
				ci->preferred_exclusive =
					Q931_IE_CI_PE_EXCLUSIVE;
				ci->coding_standard = Q931_IE_CI_CS_CCITT;
				q931_chanset_init(&ci->chanset);
				q931_chanset_copy(&ci->chanset,
					&gc->restart_reqd_chans);
				q931_ies_add_put(&ies, &ci->ie);

				struct q931_ie_restart_indicator *ri =
					q931_ie_restart_indicator_alloc();
				ri->restart_class = Q931_IE_RI_C_INDICATED;
				q931_ies_add_put(&ies, &ri->ie);

			} else if (gc->restart_indicator->restart_class ==
				       		Q931_IE_RI_C_SINGLE_INTERFACE) {

				struct q931_ie_restart_indicator *ri =
					q931_ie_restart_indicator_alloc();
				ri->restart_class =
					Q931_IE_RI_C_SINGLE_INTERFACE;
				q931_ies_add_put(&ies, &ri->ie);

			} else if (gc->restart_indicator->restart_class ==
				       		Q931_IE_RI_C_ALL_INTERFACES) {
			} else {
				assert(0);
			}

			assert(gc->restart_dlc);
			q931_global_send_message(gc, gc->restart_dlc,
				Q931_MT_RESTART, &ies);

			q931_global_start_timer(gc, T316);

			Q931_UNDECLARE_IES(ies);
		}
	break;

	case Q931_GLOBAL_STATE_NULL:
	case Q931_GLOBAL_STATE_RESTART:
		// Unexpected timer
	break;
	}*/
}

void Q931_GlobalFsm::timerT317(void *data)
{
    std::string st(this->stateToText(state));
    Application::instance().logger().debug("Global Timer T317: " + st);

	/*struct q931_global_call *gc = data;
	assert(data);

	switch(gc->state) {
	case Q931_GLOBAL_STATE_NULL:
		// Unexpected timer
	break;

	case Q931_GLOBAL_STATE_RESTART_REQUEST:
		if (gc->restart_acknowledged) {
			struct q931_chanset cs;
			q931_chanset_init(&cs);

			q931_chanset_intersect(
				&gc->restart_reqd_chans,
				&gc->restart_acked_chans);

			q931_global_set_state(gc, Q931_GLOBAL_STATE_NULL);

			q931_global_primitive1(gc,
				Q931_CCB_MANAGEMENT_RESTART_CONFIRM, &cs);
		} else {
			gc->T317_expired = TRUE;
		}
	break;

	case Q931_GLOBAL_STATE_RESTART:
		// Any channel restarted?
		if (q931_chanset_count(&gc->restart_acked_chans)) {
			Q931_DECLARE_IES(ies);

			struct q931_ie_channel_identification *ci =
				q931_ie_channel_identification_alloc();
			ci->interface_id_present = Q931_IE_CI_IIP_IMPLICIT;
			ci->interface_type =
				q931_ie_channel_identification_intftype(
							gc->intf);
			ci->preferred_exclusive = Q931_IE_CI_PE_EXCLUSIVE;
			ci->coding_standard = Q931_IE_CI_CS_CCITT;
			q931_chanset_init(&ci->chanset);
			q931_chanset_copy(&ci->chanset,
				&gc->restart_acked_chans);
			q931_ies_add_put(&ies, &ci->ie);

			struct q931_ie_restart_indicator *ri =
				q931_ie_restart_indicator_alloc();
			ri->restart_class = Q931_IE_RI_C_INDICATED;
			q931_ies_add_put(&ies, &ri->ie);

			assert(gc->restart_dlc);
			q931_global_send_message(gc, gc->restart_dlc,
				Q931_MT_RESTART, &ies);

			Q931_UNDECLARE_IES(ies);
		}

		q931_global_set_state(gc, Q931_GLOBAL_STATE_NULL);
		q931_global_primitive(gc,
			Q931_CCB_TIMEOUT_MANAGEMENT_INDICATION);
	break;
	}*/
}

void Q931_GlobalFsm::dispatchMessage(class Q931_Message *msg)
{
	/*switch(msg->message_type) {
	case Q931_MT_STATUS:
		q931_global_handle_status(gc, msg);
	break;

	case Q931_MT_RESTART:
		q931_global_handle_restart(gc, msg);
	break;

	case Q931_MT_RESTART_ACKNOWLEDGE:
		q931_global_handle_restart_acknowledge(gc, msg);
	break;

	default: {
		Q931_DECLARE_IES(ies);
		struct q931_ie_cause *cause = q931_ie_cause_alloc();
		cQ931_GlobalFsm::ause->coding_standard = Q931_IE_C_CS_CCITT;
		cause->location = q931_ie_cause_location_gc(gc);
		cause->value = Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE;
		q931_ies_add_put(&ies, &cause->ie);

		struct q931_ie_call_state *cs = q931_ie_call_state_alloc();
		cs->coding_standard = Q931_IE_CS_CS_CCITT;
		cs->value = q931_global_state_to_ie_state(gc->state);
		q931_ies_add_put(&ies, &cs->ie);

		q931_global_send_message(gc, msg->dlc, Q931_MT_STATUS, &ies);

		Q931_UNDECLARE_IES(ies);
	}
	break;
	}*/
}

