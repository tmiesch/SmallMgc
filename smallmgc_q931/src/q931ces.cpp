/*
 * q931ces.cpp
 *
 *  Created on: 07.04.2009
 *      Author: thomas
 */

#include "q931ces.h"
#include "q931message.h"
#include "Q931_Ies.h"
#include "Q931_ie.h"
#include "q931call.h"
#include "q931ccbmessage.h"
#include "q931iecallstate.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_Ces::Q931_Ces(class Q931_Adapter* adapter, class Q931_Call *call, class Q931_DataLink* dlc) :
	m_adapter(adapter), m_call(call), m_dlc(dlc),
   m_state(I0_NULL_STATE), m_delete(false)
{
	// TODO Auto-generated constructor stub
      m_T308_fired = 0;
      m_senq_cause_97_98_received = 0;
      m_senq_cnt = 0;

}

Q931_Ces::~Q931_Ces() {
	// TODO Auto-generated destructor stub
}

const char *Q931_Ces::q931_ces_state_to_text(enum q931_ces_state state)
{
	switch (state) {
		case I0_NULL_STATE:
			return "I0_NULL_STATE";
		case I7_CALL_RECEIVED:
			return "I7_CALL_RECEIVED";
		case I8_CONNECT_REQUEST:
			return "I8_CONNECT_REQUEST";
		case I9_INCOMING_CALL_PROCEEDING:
			return "I9_INCOMING_CALL_PROCEEDING";
		case I19_RELEASE_REQUEST:
			return "I19_RELEASE_REQUEST";
		case I25_OVERLAP_RECEIVING:
			return "I25_OVERLAP_RECEIVING";
		default: return "*UNKNOWN*";
	}
}

void Q931_Ces::q931_ces_dispatch_message(class Q931_Message *msg)
{
	switch (msg->getMessage_type()) {
		case Q931_MT_ALERTING:
			q931_ces_handle_alerting(msg);
		break;

		case Q931_MT_CALL_PROCEEDING:
			q931_ces_handle_call_proceeding(msg);
		break;

		case Q931_MT_CONNECT:
			q931_ces_handle_connect(msg);
		break;

		case Q931_MT_PROGRESS:
			q931_ces_handle_progress(msg);
		break;

		case Q931_MT_DISCONNECT:
			q931_ces_handle_disconnect(msg);
		break;

		case Q931_MT_RELEASE:
			q931_ces_handle_release(msg);
		break;

		case Q931_MT_RELEASE_COMPLETE:
			q931_ces_handle_release_complete(msg);
		break;

		case Q931_MT_STATUS:
			q931_ces_handle_status(msg);
		break;

		case Q931_MT_INFORMATION:
			q931_ces_handle_info(msg);
		break;

		case Q931_MT_STATUS_ENQUIRY:
			q931_ces_handle_status_enquiry(msg);
		break;

		case Q931_MT_SETUP:
			// Do nothing
		break;

		case Q931_MT_CONNECT_ACKNOWLEDGE:
		case Q931_MT_SETUP_ACKNOWLEDGE:
		case Q931_MT_RESTART:
		case Q931_MT_RESTART_ACKNOWLEDGE:
		case Q931_MT_USER_INFORMATION:
		case Q931_MT_SEGMENT:
		case Q931_MT_CONGESTION_CONTROL:
		case Q931_MT_FACILITY:
		case Q931_MT_NOTIFY:
		case Q931_MT_HOLD:
		case Q931_MT_HOLD_ACKNOWLEDGE:
		case Q931_MT_HOLD_REJECT:
		case Q931_MT_RETRIEVE:
		case Q931_MT_RETRIEVE_ACKNOWLEDGE:
		case Q931_MT_RETRIEVE_REJECT:
		case Q931_MT_RESUME:
		case Q931_MT_RESUME_ACKNOWLEDGE:
		case Q931_MT_RESUME_REJECT:
		case Q931_MT_SUSPEND:
		case Q931_MT_SUSPEND_ACKNOWLEDGE:
		case Q931_MT_SUSPEND_REJECT:
			q931_ces_message_not_compatible_with_state(msg);
		break;

		default:
			// log message unhandled
		break;
		}
}

void Q931_Ces::q931_ces_dl_establish_indication()
{
   std::string st(this->q931_ces_state_to_text(m_state));
   Application::instance().logger().debug("CES Establish Indication: " + st);

	if (m_state == I25_OVERLAP_RECEIVING)
	{
		Q931Ies* ies = new Q931Ies();
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(m_call->q931IeCauseLocation());
		cause->setValue(Q931_IE_C_CV_TEMPORARY_FAILURE);

		q931_ces_stop_timer(T304);
		q931_ces_send_release(ies);
		q931_ces_start_timer(T308);
		q931_ces_set_state(I19_RELEASE_REQUEST);
		m_call->q931IntReleaseIndication(this, NULL);
		delete ies;
	}
}

void Q931_Ces::q931_ces_unexpected_primitive()
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().trace("CES Unexpected Primitive: " + st);
  // log error
}

void Q931_Ces::q931_ces_dl_release_indication()
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Release Indication: " + st);

  if (m_state != I0_NULL_STATE)
  {
	q931_ces_stop_any_timer();
	m_call->q931IntReleaseIndication(this, NULL);
	m_call->q931IntReleaseCompleteIndication(this, NULL);
  }
}

void Q931_Ces::q931_ces_dl_establish_confirm()
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Establish Confirm: " + st);

  // do nothing
}

void Q931_Ces::q931_ces_dl_release_confirm()
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Release Confirm: " + st);

  // do nothing
}

void Q931_Ces::q931_ces_alerting_request(class Q931Ies *user_ies)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Alerting Request: " + st);

  switch (m_state)
  {
	case I0_NULL_STATE:
		q931_ces_set_state(I7_CALL_RECEIVED);
	break;

	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
	case I19_RELEASE_REQUEST:
	case I25_OVERLAP_RECEIVING:
		q931_ces_unexpected_primitive();
	break;
  }
}

void Q931_Ces::q931_ces_connect_request(class Q931Ies *user_ies)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Connect Request: " + st);

	switch (m_state)
	{
	  case I0_NULL_STATE:
			q931_ces_set_state(I8_CONNECT_REQUEST);
	  break;

	  case I7_CALL_RECEIVED:
	  case I8_CONNECT_REQUEST:
	  case I9_INCOMING_CALL_PROCEEDING:
	  case I19_RELEASE_REQUEST:
	  case I25_OVERLAP_RECEIVING:
		q931_ces_unexpected_primitive();
      break;
	}
}

void Q931_Ces::q931_ces_call_proceeding_request(class Q931Ies *user_ies)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Call Proceeding Request: " + st);

  switch (m_state)
  {
	case I0_NULL_STATE:
		q931_ces_set_state(I9_INCOMING_CALL_PROCEEDING);
	break;

	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
	case I19_RELEASE_REQUEST:
	case I25_OVERLAP_RECEIVING:
		q931_ces_unexpected_primitive();
	break;
  }
}

void Q931_Ces::q931_ces_setup_ack_request(class Q931Ies *user_ies)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Setup Ack Request: " + st);

  switch (m_state)
  {
	case I0_NULL_STATE:
		q931_ces_start_timer(T304);
		q931_ces_set_state(I25_OVERLAP_RECEIVING);
	break;

	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
	case I19_RELEASE_REQUEST:
	case I25_OVERLAP_RECEIVING:
		q931_ces_unexpected_primitive();
	break;
  }
}

void Q931_Ces::q931_ces_release_request(class Q931Ies *ies)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Release Request: " + st);

  switch (m_state)
  {
	case I0_NULL_STATE:
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
		q931_ces_start_timer(T308);
		q931_ces_send_release(ies);
		q931_ces_set_state(I19_RELEASE_REQUEST);
	break;

	case I9_INCOMING_CALL_PROCEEDING:
		q931_ces_start_timer(T308);
		q931_ces_send_release(ies);
		q931_ces_set_state(I19_RELEASE_REQUEST);
	break;

	case I25_OVERLAP_RECEIVING:
		q931_ces_stop_timer(T304);
		q931_ces_start_timer(T308);
		q931_ces_send_release(ies);
		q931_ces_set_state(I19_RELEASE_REQUEST);
	break;

	case I19_RELEASE_REQUEST:
		q931_ces_unexpected_primitive();
	break;

  }
}

void Q931_Ces::q931_ces_info_request(class Q931Ies *ies)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Info Request: " + st);

  switch (m_state)
  {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
		q931_ces_send_information(ies);
	break;

	case I25_OVERLAP_RECEIVING:
		q931_ces_stop_timer(T304);
		q931_ces_send_information(ies);
		q931_ces_start_timer(T304);
	break;

	case I0_NULL_STATE:
	case I19_RELEASE_REQUEST:
	default:
		q931_ces_unexpected_primitive();
	break;
  }
}

void Q931_Ces::q931_ces_status_enquiry_request(class Q931Ies *ies)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Enquiry Request: " + st);

  switch (m_state)
  {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
	case I25_OVERLAP_RECEIVING:
	case I19_RELEASE_REQUEST:
		if (!m_call->q931_timer_pending(T322)) {
			q931_ces_send_status_enquiry(ies);
			q931_ces_start_timer(T322);
		}
	break;
  }
}

void Q931_Ces::q931_ces_stop_timer(int timerId)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Stop Timer: " + st);

}

void Q931_Ces::q931_ces_start_timer(int timerId)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Start Timer: " + st);

}

void Q931_Ces::q931_ces_stop_any_timer()
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().debug("CES Stop Any Timer: " + st);

}
__u8 Q931_Ces::q931_ces_state_to_ie_state(enum q931_ces_state state)
{
	switch (m_state) {
	case I0_NULL_STATE:
		return Q931_IE_CS_N0_NULL_STATE;
	case I7_CALL_RECEIVED:
		return Q931_IE_CS_N7_CALL_RECEIVED;
	case I8_CONNECT_REQUEST:
		return Q931_IE_CS_N8_CONNECT_REQUEST;
	case I9_INCOMING_CALL_PROCEEDING:
		return Q931_IE_CS_N9_INCOMING_CALL_PROCEEDING;
	case I19_RELEASE_REQUEST:
		return Q931_IE_CS_N19_RELEASE_REQUEST;
	case I25_OVERLAP_RECEIVING:
		return Q931_IE_CS_N25_OVERLAP_RECEIVING;
	default:
		return 0;
	}
}
void Q931_Ces::q931_ces_handle_alerting(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Alerting: " + st);

	switch (m_state) {
	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_set_state(I7_CALL_RECEIVED);
		m_call->q931IntAlertingIndication((Q931_Ces*)this, msg->getIes());
	break;

	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		q931_ces_set_state(I7_CALL_RECEIVED);
		m_call->q931IntAlertingIndication((Q931_Ces*)this, msg->getIes());
	break;

	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_call_proceeding(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Call Proceeding: " + st);

	switch (m_state) {
	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		q931_ces_set_state(I9_INCOMING_CALL_PROCEEDING);
		m_call->q931IntCallProceedingIndication(this, msg->getIes());
	break;

	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_connect(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Connect: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_set_state(I8_CONNECT_REQUEST);
		m_call->q931IntConnectIndication(this, msg->getIes());
	break;

	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_set_state(I8_CONNECT_REQUEST);
		m_call->q931IntConnectIndication(this, msg->getIes());
	break;

	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		q931_ces_set_state(I8_CONNECT_REQUEST);
		m_call->q931IntConnectIndication(this, msg->getIes());
	break;

	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_progress(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Progress: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		m_call->q931IntProgressIndication(this, msg->getIes());
	break;

	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		m_call->q931IntProgressIndication(this, msg->getIes());
	break;

	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_disconnect(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Disconnect: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_send_release(NULL);
		q931_ces_start_timer(T308);
		q931_ces_set_state(I19_RELEASE_REQUEST);
		m_call->q931IntReleaseIndication(this, msg->getIes());
	break;

	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_send_release(NULL);
		q931_ces_start_timer(T308);
		q931_ces_set_state(I19_RELEASE_REQUEST);
		m_call->q931IntReleaseIndication(this, msg->getIes());
	break;

	case I19_RELEASE_REQUEST:
		// Do nothing
	break;

	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		q931_ces_send_release(NULL);
		q931_ces_start_timer(T308);
		q931_ces_set_state(I19_RELEASE_REQUEST);
		m_call->q931IntReleaseIndication(this, msg->getIes());
	break;

	case I0_NULL_STATE:
	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_release(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Release: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_send_release_complete(NULL);
		m_call->q931IntReleaseIndication(this, msg->getIes());
		m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
	break;

	case I19_RELEASE_REQUEST:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T308);
		m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
	break;

	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		q931_ces_send_release_complete(NULL);
		m_call->q931IntReleaseIndication(this, msg->getIes());
		m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
	break;

	case I0_NULL_STATE:
	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_release_complete(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Release Complete: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		m_call->q931IntReleaseIndication(this, msg->getIes());
		m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
	break;

	case I19_RELEASE_REQUEST:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T308);
		m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
	break;

	case I25_OVERLAP_RECEIVING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		q931_ces_stop_timer(T304);
		m_call->q931IntReleaseIndication(this, msg->getIes());

		// Implementation dependend
		/*
		q931_ces_stop_timer(ces, T304);
		q931_send_release(ces->dlc);

		q931_ces_start_timer(ces, T308);
		*/

		m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
	break;

	case I0_NULL_STATE:
		m_call->removeCes(this);
		m_delete = true;
	break;
	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_status(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Status: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
	case I25_OVERLAP_RECEIVING: {
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		Q931IeCallState *cs = NULL;
		Q931IeCause *cause = NULL;
        Q931Ies* ies = msg->getIes();
		int i;
		for (i=0; i<ies->get_count(); i++) {
			if (ies->getIe(i)->getId() == Q931_IE_CALL_STATE) {
				cs = (Q931IeCallState *)ies->getIe(i);
			} else if (ies->getIe(i)->getId() == Q931_IE_CAUSE) {
				cause = (Q931IeCause *)ies->getIe(i);
			}
		}

		if (m_call->q931_timer_pending(T322)) {
			if (cause->getValue() == Q931_IE_C_CV_MESSAGE_TYPE_NON_EXISTENT_OR_IMPLEMENTED) {
				// Save cause and call state (!?!?!?)
				return;
			} else if(cause->getValue() == Q931_IE_C_CV_RESPONSE_TO_STATUS_ENQUIRY) {
				q931_ces_stop_timer(T322);
			}
		}

		if (cs && cs->getValue() == q931_ces_state_to_ie_state(I0_NULL_STATE)) {
			q931_ces_stop_any_timer();
			m_call->q931IntReleaseIndication(this, msg->getIes());
			m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
		} else if (cs && cs->getValue() == q931_ces_state_to_ie_state(m_state)) {
				// NOTE 1. FURTHER ACTIONS ARE AN IMPLEMENTATION OPTION.
		} else {
			q931_ces_stop_any_timer();

			//Q931_DECLARE_IES(ies);
			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(m_call->q931IeCauseLocation());
			cause->setValue(Q931_IE_C_CV_MESSAGE_NOT_COMPATIBLE_WITH_CALL_STATE);
			//q931_ies_add_put(&ies, &cause->ie);
			ies->add_put(cause);
			q931_ces_send_release(ies);

			q931_ces_start_timer(T308);
			q931_ces_set_state(I19_RELEASE_REQUEST);
			m_adapter->q931_call_primitive(m_call, Q931_CCB_STATUS_INDICATION,
				msg->getIes(), Q931_Call::Q931_STATUS_INDICATION_ERROR, 0);
			m_call->q931IntReleaseIndication(this, msg->getIes());
			//Q931_UNDECLARE_IES(ies);Q931_Call::Q931CallState
			delete ies;
		}
	}
	break;

	case I19_RELEASE_REQUEST: {
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		Q931IeCallState *cs = NULL;
		Q931Ies* ies = msg->getIes();
		int i;
		for (i=0; i<ies->get_count(); i++) {
			if (ies->getIe(i)->getId() == Q931_IE_CALL_STATE) {
				cs = (Q931IeCallState *)ies->getIe(i);
			}
		}

		if (cs && cs->getValue() == q931_ces_state_to_ie_state(I0_NULL_STATE)) {
			q931_ces_stop_timer(T308);
			m_call->q931IntReleaseCompleteIndication(this, msg->getIes());
		}
	}
	break;

	case I0_NULL_STATE:
	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_info(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Info: " + st);

	switch (m_state) {
	case I7_CALL_RECEIVED:
	case I8_CONNECT_REQUEST:
	case I9_INCOMING_CALL_PROCEEDING:
		if (m_call->q931DecodeIes(msg) < 0)
			break;

		m_call->q931IntInfoIndication(this, msg->getIes());
	break;

	case I19_RELEASE_REQUEST:
		// Do nothing
		// NOTE 1. THE INDIVIDUAL PROCESS MAY PASS AN INT. INFO
		//	 INDICATION TO THE GLOBAL PROCESS AT THIS POINT.
		//	 (REF. SECTION 5.0)

	break;

	case I25_OVERLAP_RECEIVING:
	break;

	case I0_NULL_STATE:
	default:
		q931_ces_message_not_compatible_with_state(msg);
	break;
	}
}

void Q931_Ces::q931_ces_handle_status_enquiry(class Q931_Message *msg)
{
	std::string st(this->q931_ces_state_to_text(m_state));
    Application::instance().logger().debug("CES Handle Status Enquiry: " + st);

	//Q931_DECLARE_IES(ies);
	Q931Ies* ies = new Q931Ies();
	Q931IeCause *cause = new Q931IeCause();
	//struct q931_ie_cause *cause = q931_ie_cause_alloc();
	cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	cause->setLocation(m_call->q931IeCauseLocation());
	cause->setValue(Q931_IE_C_CV_RESPONSE_TO_STATUS_ENQUIRY);
	//q931_ies_add_put(&ies, &cause->ie);
	ies->add_put(cause);
	q931_ces_send_status(ies);
	//Q931_UNDECLARE_IES(ies);
	delete ies;
}

void Q931_Ces::q931_ces_message_not_compatible_with_state(class Q931_Message *msg)
{
  std::string st(this->q931_ces_state_to_text(m_state));
  Application::instance().logger().trace("CES Message Not Compatible with State: "+ st);
  //Application::instance().logger().trace( this->q931_ces_state_to_text(m_state));

}
