/*
 * Q931Stack.cpp
 *
 *  Created on: 09.03.2009
 *      Author: thomas
 */

#include "q931call.h"
#include "Q931_Ies.h"
#include "q931iecallstate.h"
#include "q931ces.h"
#include "q931datalink.h"
#include "q931interface.h"
#include "q931message.h"
#include "q931globalfsm.h"
#include "q931decoderstatus.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;

Q931_Call::Q931_Call()
{
  // TODO Auto-generated constructor stub
  called_Number.clear();
}

Q931_Call::Q931_Call(class Q931_Adapter *adapter, class Q931_Interface *intf, int ref)
    :m_adapter(adapter), interface(intf), refcnt(ref), state(N0_NULL_STATE)
{
  called_Number.clear();
  setup_ies = new Q931Ies();
  disconnect_cause = new Q931Ies();
  saved_cause = new Q931Ies();
  release_cause = new Q931Ies();

  channel = 0;
  direction = Q931_Call::Q931_CALL_DIRECTION_OUTBOUND;
  call_reference = 1;
  m_ces.clear();
  preselected_ces = 0;
  selected_ces = 0;
  m_dlc = 0;
  broadcast_setup = 0;
  if (interface->getType() == LAPD_INTF_TYPE_PRA)
    tones_option = 0;
  else
    tones_option = 1;

  T303_fired = 0;
  T308_fired = 0;
  senq_status_97_98_received = 0;
  senq_cnt = 0;
  disconnect_indication_sent = 0;
  release_complete_received = 0;
  digits_dialled = 0;
  m_channel.clear();
  called_Number.clear();

  pvt = 0;
  m_timer[0] = 0;
  m_timer[1] = 0;
  m_timer[2] = 0;
}


Q931_Call::~Q931_Call()
{
  // TODO Auto-generated destructor stub
  delete setup_ies;
  delete disconnect_cause;
  delete saved_cause;
  delete release_cause;

  if ((m_timer[0] != 0))
  {
    m_adapter->stopTimer(m_timer[0]);
    m_timer[0] = 0;
  }
  if ((m_timer[1] != 0))
  {
    m_adapter->stopTimer(m_timer[1]);
    m_timer[1] = 0;
  }
  if ((m_timer[2] != 0))
  {
    m_adapter->stopTimer(m_timer[2]);
    m_timer[2] = 0;
  }

  //if (preselected_ces != 0)
  //delete preselected_ces;
}
void Q931_Call::addCes(class Q931_Ces *ces)
{
  Application::instance().logger().debug("Call Add Ces");

  m_ces.push_back(ces);
  //selected_ces = ces;
}

void Q931_Call::removeCes(class Q931_Ces *ces)
{
  Application::instance().logger().debug("Call Remove Ces");

  std::list<Q931_Ces*>::iterator ces_iter;
  for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
  {
    if( (*ces_iter) == ces)
    {
      //std::string st(this->q931_ces_state_to_text(ces->getState()));
      //Application::instance().logger().debug("Remove Ces State: " + st);
      m_ces.erase(ces_iter);
      return;
    }
  }
}

int Q931_Call::cesWaitingConnection()
{
  std::list<Q931_Ces*>::iterator ces_iter;
  Application::instance().logger().debug("Call Ces Waiting Connection");

  for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
  {
    if( (*ces_iter)->getState() != I19_RELEASE_REQUEST)
    {
      Application::instance().logger().debug("Call Ces Waiting Connection: " + NumberFormatter::format((*ces_iter)->getState()));

      return 1;
    }
  }
  return 0;
}

void Q931_Call::q931CallPrimitive(enum q931_primitive primitive, class Q931Ies *ies, unsigned long par1, unsigned long par2)
{
  m_adapter->q931_call_primitive(this, primitive, ies, par1, par2);
}

class Q931_Call* Q931_Call::q931CallAllocOut(class Q931Interface *intf)
{
}

const char* Q931_Call::q931CallStateToText(enum Q931CallState state)
{
  switch (state)
  {
    case N0_NULL_STATE:
	return "N0_NULL_STATE";
    case N1_CALL_INITIATED:
	return "N1_CALL_INITIATED";
    case N2_OVERLAP_SENDING:
	return "N2_OVERLAP_SENDING";
    case N3_OUTGOING_CALL_PROCEEDING:
	return "N3_OUTGOING_CALL_PROCEEDING";
    case N4_CALL_DELIVERED:
	return "N4_CALL_DELIVERED";
    case N6_CALL_PRESENT:
	return "N6_CALL_PRESENT";
    case N7_CALL_RECEIVED:
	return "N7_CALL_RECEIVED";
    case N8_CONNECT_REQUEST:
	return "N8_CONNECT_REQUEST";
    case N9_INCOMING_CALL_PROCEEDING:
	return "N9_INCOMING_CALL_PROCEEDING";
    case N10_ACTIVE:
	return "N10_ACTIVE";
    case N11_DISCONNECT_REQUEST:
	return "N11_DISCONNECT_REQUEST";
    case N12_DISCONNECT_INDICATION:
	return "N12_DISCONNECT_INDICATION";
    case N15_SUSPEND_REQUEST:
	return "N15_SUSPEND_REQUEST";
    case N17_RESUME_REQUEST:
	return "N17_RESUME_REQUEST";
    case N19_RELEASE_REQUEST:
	return "N19_RELEASE_REQUEST";
    case N22_CALL_ABORT:
	return "N22_CALL_ABORT";
    case N25_OVERLAP_RECEIVING:
	return "N25_OVERLAP_RECEIVING";
    default: return "*UNKNOWN*";
  }
}

class Q931_Call* Q931_Call::q931GetCallByReference(class Q931Interface *intf,	enum Q931CallDirection direction,
		int call_reference)
{
  if (this->direction == direction &&   this->call_reference == call_reference)
  {
    return this;
  }

  return NULL;
}

void Q931_Call::q931CallReleaseReference()
{
	q931IntfDelCall();
	q931CallPut(__FILE__,  __LINE__);
}

void Q931_Call::q931IntfDelCall()
{
	interface->delCall(this);
}

Q931_Call *Q931_Call::q931CallAllocIn(class Q931Interface *intf,class Q931Dlc *dlc, int call_reference)
{

}

void Q931_Call::q931CallDlEstablishIndication()
{
    std::string st(this->q931CallStateToText(state));

    Application::instance().logger().debug("Call Establish Indication: " + st);

	Q931Ies* ies = new Q931Ies();
	Q931IeCause *cause = new Q931IeCause();
	cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	cause->setLocation(q931IeCauseLocation());
	cause->setValue(Q931_IE_C_CV_TEMPORARY_FAILURE);
        ies->add_put(cause);

	switch (state) {
	case N2_OVERLAP_SENDING: {
		q931CallStopAnyTimer();

		if (tones_option) {
			Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
			pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
			pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
			pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

			ies->add_put(pi);
			q931_call_send_disconnect(ies);
			channel->startTone(Q931_TONE_FAILURE);
			q931CallStartTimer(Q931_T306);
		} else {
			channel->disconnect();
			q931_call_send_disconnect(ies);
			q931CallStartTimer(Q931_T305);
		}

		setState(N12_DISCONNECT_INDICATION);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, NULL);
	}
	break;

	case N25_OVERLAP_RECEIVING:
		if (!broadcast_setup) {
			channel->disconnect();
			q931_call_send_disconnect(ies);
			q931CallStartTimer(Q931_T305);
			setState(N12_DISCONNECT_INDICATION);
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, NULL);
		}
	break;

	default:
		// Do nothing
	break;
	}

	delete ies;
}

void Q931_Call::q931CallDlEstablishConfirm()
{
   std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Establish Confirm: " + st);

	Q931Ies* ies = new Q931Ies();
	Q931IeCause *cause = new Q931IeCause();
	cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	cause->setLocation(q931IeCauseLocation());
	cause->setValue(Q931_IE_C_CV_NORMAL_UNSPECIFIED);
        ies->add_put(cause);

	if (state == N10_ACTIVE) {
		q931CallStopTimer(Q931_T309);
		q931_call_send_call_status(ies);
	}
   delete ies;
}

void Q931_Call::q931CallDlReleaseIndication()
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Release Indication: " + st);

	switch (state) {
	case N0_NULL_STATE:
		// Do nothing
	break;

	case N1_CALL_INITIATED:
	case N2_OVERLAP_SENDING:
	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
	case N6_CALL_PRESENT:
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N11_DISCONNECT_REQUEST:
	case N12_DISCONNECT_INDICATION:
	case N15_SUSPEND_REQUEST:
	case N17_RESUME_REQUEST:
	case N19_RELEASE_REQUEST:
	case N22_CALL_ABORT:
	case N25_OVERLAP_RECEIVING:
		if (!broadcast_setup || state == N6_CALL_PRESENT) {
			q931CallStopAnyTimer();
			if (channel != 0)
			  channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();

			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue(Q931_IE_C_CV_DESTINATION_OUT_OF_ORDER);
			ies->add_put(cause);
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, ies);
		    delete ies;
		}
	break;

	case N10_ACTIVE:
		if (q931CallTimerRunning(Q931_T309)) {
			q931CallStopTimer(Q931_T309);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();

			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue(Q931_IE_C_CV_DESTINATION_OUT_OF_ORDER);
			ies->add_put(cause);
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, ies);
			delete ies;
		} else {
			q931CallStartTimer(Q931_T309);
		}
	break;
	}
}

void Q931_Call::q931CallDlReleaseConfirm()
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Release Confirm: " + st);

  // do nothing
}

void Q931_Call::q931CallStopAnyTimer()
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Stop Any Timer: " + st);

  if (m_timer[0] != 0)
  {
        m_adapter->stopTimer(m_timer[0]);
        m_timer[0] = 0;
  }
  if (m_timer[1] != 0)
  {
    m_adapter->stopTimer(m_timer[1]);
    m_timer[1] = 0;
  }
  if (m_timer[2] != 0)
  {
    m_adapter->stopTimer(m_timer[2]);
    m_timer[2] = 0;
  }
}


void Q931_Call::q931IntAlertingIndication(class Q931_Ces *ces,class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Internal Alerting Indication: " + st);

	switch (state) {
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
		// Do nothing
	break;

	case N9_INCOMING_CALL_PROCEEDING:
		q931CallStopTimer(Q931_T310);
		q931CallStartTimer(Q931_T301);
		setState(N7_CALL_RECEIVED);
		q931CallPrimitive(Q931_CCB_ALERTING_INDICATION, ies);
	break;

	case N25_OVERLAP_RECEIVING:
		q931CallStopTimer(Q931_T304);
		q931CallStartTimer(Q931_T301);
		setState(N7_CALL_RECEIVED);
		q931CallPrimitive(Q931_CCB_ALERTING_INDICATION, ies);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931IntConnectIndication(class Q931_Ces *ces, class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Internal Connect Indication: " + st);

	switch (state) {
	case N7_CALL_RECEIVED:
	{
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Int Connect Indication\n");

	        Q931Ies *causes = new Q931Ies();
		if (q931ChannelSelectResponse(ies, causes)) {
			preselected_ces = ces;
			q931CallStopTimer(Q931_T301);
			setState(N8_CONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_CONNECT_INDICATION, ies);
		} else {
			ces->q931_ces_release_request(causes);
		}

		delete causes;
	}
	break;

	case N8_CONNECT_REQUEST:
		// Do nothing
	break;

	case N9_INCOMING_CALL_PROCEEDING:
		q931CallStopTimer(Q931_T310);
		preselected_ces = ces;
		setState(N8_CONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_CONNECT_INDICATION, ies);
	break;

	case N25_OVERLAP_RECEIVING:
		q931CallStopTimer(Q931_T304);
		preselected_ces = ces;
		setState(N8_CONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_CONNECT_INDICATION, ies);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931IntCallProceedingIndication(class Q931_Ces *ces,class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Internal Call Proceeding Indication: " + st);

  switch (state) {
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
		// Do nothing
	break;

	case N25_OVERLAP_RECEIVING:
		q931CallStopTimer(Q931_T304);
		q931CallStartTimer(Q931_T310);
		setState(N9_INCOMING_CALL_PROCEEDING);
		q931CallPrimitive(Q931_CCB_PROCEEDING_INDICATION, ies);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931IntReleaseCompleteIndication(class Q931_Ces *ces,class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Internal Release Complete Indication: " + st);

	switch (state) {
	case N0_NULL_STATE:
	case N6_CALL_PRESENT:
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N10_ACTIVE:
	case N19_RELEASE_REQUEST:
	case N25_OVERLAP_RECEIVING:
		//q931_ces_free(ces);
	        removeCes(ces);
		delete ces;
	    break;
/*	case N11_DISCONNECT_REQUEST:
	case N12_DISCONNECT_INDICATION:
	  if (disconnect_indication_sent)
	  {
	    q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, ies);
	  }
	  channel->release();
	  setState(N0_NULL_STATE);
	  q931CallReleaseReference();
	  break;*/
	case N22_CALL_ABORT:
		//q931_ces_free(ces);
	removeCes(ces);
        delete ces;

		if (m_ces.empty()  /* list_empty(&call->ces)*/)
		{
			if (disconnect_indication_sent) {
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, ies);
			}

			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
		}
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931IntInfoIndication(class Q931_Ces *ces,class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Internal Info Indication: " + st);

	switch (state) {
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N19_RELEASE_REQUEST:
	case N25_OVERLAP_RECEIVING:
		q931CallPrimitive(Q931_CCB_INFO_INDICATION, ies);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931IntProgressIndication(class Q931_Ces *ces,class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Internal Progress Indication: " + st);

	switch (state)
	{
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Int Progress Indication\n");

		q931CallPrimitive(Q931_CCB_PROGRESS_INDICATION, ies);
	break;

	case N9_INCOMING_CALL_PROCEEDING:
		q931CallStopTimer(Q931_T310);
		q931CallPrimitive(Q931_CCB_PROGRESS_INDICATION, ies);
	break;

	case N25_OVERLAP_RECEIVING:
		q931CallPrimitive(Q931_CCB_PROGRESS_INDICATION, ies);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931IntReleaseIndication(class Q931_Ces *ces,class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Internal Release Indication: " + st);

  switch (state)
  {

        if (ies == 0)
	  ies = new Q931Ies();
        case N7_CALL_RECEIVED:
	{
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Release Indication\n");

	        int able_to_proceed = 0;

		// Are other CES able to proceed?
		// FIXME Is this correct? Specs are unclear! FIXME

		std::list<Q931_Ces*>::iterator ces_iter;
		for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
		{
			if(((*ces_iter) != ces) && ((*ces_iter)->getState() != I0_NULL_STATE) &&
					((*ces_iter)->getState() != I19_RELEASE_REQUEST))
			{
				able_to_proceed = 1;
				break;
			}
		}

		if (able_to_proceed)
		{
			saved_cause->merge( ies);
		}
		else
		{
			if (q931CallTimerRunning(Q931_T312))
			{
				saved_cause->merge(ies);
			}
			else
			{
				q931CallStopTimer(Q931_T301);
				channel->release();
				setState(N0_NULL_STATE);
				q931CallReleaseReference();
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,ies);
			}
		}
	}
	break;

	case N8_CONNECT_REQUEST:
	{
		if (preselected_ces)
		{
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue(Q931_IE_C_CV_NON_SELECTED_USER_CLEARING);
                        ies->add_put(cause);
                        std::list<Q931_Ces*>::iterator ces_iter;
                        for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
                        {
                           (*ces_iter)->q931_ces_release_request(iesNew);
                        }

			channel->release();

			if (q931CallTimerRunning(Q931_T312))
			{
				setState(N22_CALL_ABORT);
			}
			else
			{
				setState(N0_NULL_STATE);
				q931CallReleaseReference();
			}

			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,iesNew);
                        delete iesNew;
		}
	}
	break;

	case N9_INCOMING_CALL_PROCEEDING:
	{
		int able_to_proceed1 = 0;

		// Are other CES able to proceed?
		// FIXME Is this correct? Specs are unclear! FIXME
		std::list<Q931_Ces*>::iterator ces_iter1;
		for(ces_iter1 = m_ces.begin(); ces_iter1 != m_ces.end();ces_iter1++)
		{
			if(((*ces_iter1) != ces) && ((*ces_iter1)->getState() != I0_NULL_STATE) &&
				((*ces_iter1)->getState() != I19_RELEASE_REQUEST))
			{
				able_to_proceed1 = 1;
				break;
			}
		}

		if (able_to_proceed1) {
			saved_cause->merge( ies);
		} else {
		  if (q931CallTimerRunning(Q931_T312))
		  {
			saved_cause->merge(ies);
		  }
		  else
		  {
			q931CallStopTimer(Q931_T310);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,ies);
		  }
		}
	}
	break;
	case N10_ACTIVE:
	 Application::instance().logger().trace(" Q931 Call;  N10_ACTIVE; q931IntReleaseIndication\n");

	/*                channel->disconnect();
	                q931SaveCause(disconnect_cause, ies);
	                setState( N11_DISCONNECT_REQUEST);
	                disconnect_indication_sent = true;
	                q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, ies);
	                */
	        break;
	case N25_OVERLAP_RECEIVING:
	{
		int able_to_proceed = 0;

		// Are other CES able to proceed?
		// FIXME Is this correct? Specs are unclear! FIXME
		std::list<Q931_Ces*>::iterator ces_iter;
		for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
		{
			if(((*ces_iter) != ces) && ((*ces_iter)->getState() != I0_NULL_STATE) &&
				((*ces_iter)->getState() != I19_RELEASE_REQUEST))
			{
				able_to_proceed = 1;
				break;
			}
		}

		if (able_to_proceed) {
			saved_cause->merge( ies);
		} else {
		  if (q931CallTimerRunning(Q931_T312))
		  {
			saved_cause->merge(ies);
		  }
		  else
		  {
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,ies);
		  }
		}
	}
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931AlertingRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Alerting Request: " + st);

  switch (state) {
	case N2_OVERLAP_SENDING:
		q931CallStopTimer(Q931_T302);
		q931_call_send_alerting(ies);
		setState(N4_CALL_DELIVERED);
	break;

	case N3_OUTGOING_CALL_PROCEEDING:
		q931_call_send_alerting(ies);
		setState(N4_CALL_DELIVERED);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
  }
}

void Q931_Call::q931DisconnectRequest(class Q931Ies *user_ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Disconnect Request" + st);

  disconnect_indication_sent = true;
  Q931Ies* ies = new Q931Ies(*user_ies);
  //Q931Ies* ies = new Q931Ies();
  switch (state) {
	case N2_OVERLAP_SENDING:
	  q931CallStopTimer(Q931_T302);
      if (tones_option)
      {
    	  Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
    	  pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
    	  pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
    	  pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

          ies->add_put(pi);

          q931_call_send_disconnect(ies);
    	  channel->startTone(Q931_TONE_HANGUP);
    	  q931CallStartTimer(Q931_T306);
      }
      else
      {
		channel->disconnect();
		q931_call_send_disconnect(ies);
		q931CallStartTimer(Q931_T305);
      }

	  setState(N12_DISCONNECT_INDICATION);
	break;

	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
		if (tones_option)
		{
			Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
			pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
			pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
			pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

			ies->add_put(pi);

			q931_call_send_disconnect( ies);
			channel->startTone(Q931_TONE_HANGUP);
			q931CallStartTimer(Q931_T306);
		 }
		 else
		 {
		 	channel->disconnect();
			q931_call_send_disconnect(ies);
			q931CallStartTimer(Q931_T305);
		 }
         setState(N12_DISCONNECT_INDICATION);
		break;

		case N6_CALL_PRESENT:
			q931CallStopTimer(Q931_T303);

			if (broadcast_setup)
			{
				channel->disconnect();
				setState(N22_CALL_ABORT);
			}
			else
			{
				if (tones_option)
				{
				  Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
				  pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
				  pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
				  pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

				  ies->add_put(pi);

				  q931_call_send_disconnect(ies);
				  channel->startTone(Q931_TONE_HANGUP);
				  q931CallStartTimer(Q931_T306);
				}
				else
				{
					channel->disconnect();
					q931_call_send_disconnect(ies);
					q931CallStartTimer(Q931_T305);
				}
				setState(N12_DISCONNECT_INDICATION);
			}
		break;

		case N7_CALL_RECEIVED:
	          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Disconnect Request\n");

			q931CallStopTimer(Q931_T301);
			if (broadcast_setup) {
				channel->release();

				if (q931CallTimerRunning(Q931_T312))
				{
					setState(N22_CALL_ABORT);

					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}
				} else {
					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}

					q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, user_ies);

					/* If we do del call the CES will not be able
					   to receive release complete */

					setState(N0_NULL_STATE);
					q931CallReleaseReference();
				}
			} else {
			        q931_call_send_disconnect(ies);
				q931CallStartTimer(Q931_T305);
				setState(N12_DISCONNECT_INDICATION);
			}
		break;

		case N8_CONNECT_REQUEST:
			if (broadcast_setup)
			{
				channel->release();
				if (q931CallTimerRunning(Q931_T312))
				{
					setState(N22_CALL_ABORT);

					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}
				} else {
					setState( N0_NULL_STATE);
					q931CallReleaseReference();

					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}
					q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,NULL);
				}
			} else {
				q931_call_send_disconnect(ies);
				q931CallStartTimer(Q931_T305);
				setState( N12_DISCONNECT_INDICATION);
			}
		break;

		case N9_INCOMING_CALL_PROCEEDING:
			q931CallStopTimer(Q931_T310);

			if (broadcast_setup) {
				channel->release();

				if (q931CallTimerRunning(Q931_T312)) {
					setState(N22_CALL_ABORT);
					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}
				} else {
					setState(N0_NULL_STATE);
					q931CallReleaseReference();

					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}

					q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,NULL);
				}
			} else {
				q931_call_send_disconnect(ies);
				channel->release();
				q931CallStartTimer(Q931_T305);
				setState(N12_DISCONNECT_INDICATION);
			}
		break;

		case N10_ACTIVE:
			if (tones_option)
			{
			  Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
			  pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
			  pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
			  pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

			  ies->add_put(pi);

              q931_call_send_disconnect(ies);
			  channel->startTone(Q931_TONE_HANGUP);
				q931CallStartTimer(Q931_T306);
			} else {
				channel->disconnect();
				q931_call_send_disconnect(ies);
				q931CallStartTimer(Q931_T305);
			}

			setState(N12_DISCONNECT_INDICATION);
		break;

		case N15_SUSPEND_REQUEST:
			if (tones_option)
			{
              Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
			  pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
			  pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
			  pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

			  q931_call_send_disconnect(ies);
			  channel->startTone(Q931_TONE_HANGUP);
			  q931CallStartTimer(Q931_T306);
			} else {
				channel->disconnect();
				q931_call_send_disconnect(ies);
				q931CallStartTimer(Q931_T305);
			}

			setState(N12_DISCONNECT_INDICATION);
		break;

		case N25_OVERLAP_RECEIVING:
			q931CallStopTimer(Q931_T304);

			if (broadcast_setup) {
				channel->release();

				if (q931CallTimerRunning(Q931_T312)) {
					setState(N22_CALL_ABORT);

					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}

				} else {
					setState(N0_NULL_STATE);
					q931CallReleaseReference();

					std::list<Q931_Ces*>::iterator ces_iter;
					for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
					{
						(*ces_iter)->q931_ces_release_request(user_ies);
					}


					q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,NULL);
				}
			} else {
				q931_call_send_disconnect(ies);
				channel->disconnect();
				q931CallStartTimer( Q931_T305);
				setState(N12_DISCONNECT_INDICATION);
			}
		break;

		default:
			q931CallUnexpectedPrimitive(__FUNCTION__);
		break;
		}
        delete ies;
}

void Q931_Call::q931InfoRequest(class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Info Request: " + st);

	switch (state)
	{
		case N2_OVERLAP_SENDING:
		case N3_OUTGOING_CALL_PROCEEDING:
		case N4_CALL_DELIVERED:
		case N10_ACTIVE:
		case N11_DISCONNECT_REQUEST:
		case N12_DISCONNECT_INDICATION:
			q931_call_send_information(ies);
		break;

		case N6_CALL_PRESENT:
			// Save event until completion of a transition???
			// FIXME TODO XXX
		break;

		case N7_CALL_RECEIVED:
		case N8_CONNECT_REQUEST:
		case N9_INCOMING_CALL_PROCEEDING:
	          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Info Request\n");

		       if (broadcast_setup)
			{
				std::list<Q931_Ces*>::iterator ces_iter;
				for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
				{
				  (*ces_iter)->q931_ces_info_request(ies);
				}
			}
			else
			{
				q931_call_send_information(ies);
			}
		break;

		default:
			q931CallUnexpectedPrimitive(__FUNCTION__);
		break;
		}
}

void Q931_Call::q931MoreInfoRequest(class Q931Ies *ies)
{
    std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call More Info Request: " + st);

	switch (state)
	{
		case N1_CALL_INITIATED:
		{
			Q931Ies* iesNew = new Q931Ies();
			//iesNew->merge(ies);
			q931CallStartTimer(Q931_T302);

			Q931IeChannelIdentification *ci = new Q931IeChannelIdentification();
			ci->setInterface_id_present(Q931_IE_CI_IIP_IMPLICIT);
			ci->setInterface_type(interface->getIntftype());
			ci->setPreferred_exclusive(Q931_IE_CI_PE_EXCLUSIVE);
			ci->setCoding_standard(Q931_IE_CI_CS_CCITT);
			ci->setChannel_indicator(Q931_IE_CI_DCI_IS_NOT_D_CHAN);
			ci->addChannel(channel);
			iesNew->add_put(ci);

			if (tones_option)
			{
				Q931IeProgressIndicator *pi = new Q931IeProgressIndicator();
				pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
				pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
				pi->setProgress_description(Q931_IE_PI_PD_IN_BAND_INFORMATION);

				iesNew->add_put(pi);
			}
			q931_call_send_setup_acknowledge(iesNew);

			// Handle failed connection TODO
			channel->connect();

			if (!digits_dialled && tones_option) {
				channel->startTone(Q931_TONE_DIAL);
			}

			setState(N2_OVERLAP_SENDING);
            delete iesNew;
		}
		break;

		case N25_OVERLAP_RECEIVING:
			if (broadcast_setup)
			{
				std::list<Q931_Ces*>::iterator ces_iter;
				for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
				{
				  (*ces_iter)->q931_ces_info_request(ies);
				}

			}
			else
			{
				q931_call_send_information(ies);
				q931CallStartTimer(Q931_T304);
			}
		break;

		default:
			q931CallUnexpectedPrimitive(__FUNCTION__);
		break;
		}
}

void Q931_Call::q931NotifyRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Notify Request: " + st);

  switch (state)
  {
		case N0_NULL_STATE:
			// Do nothing
		break;

		case N10_ACTIVE:
		case N15_SUSPEND_REQUEST:
			q931_call_send_notify(ies);
		break;

		default:
			q931CallUnexpectedPrimitive(__FUNCTION__);
		break;
		}
}

void Q931_Call::q931ProceedingRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Proceeding Request: " + st);

  switch (state)
  {
	case N1_CALL_INITIATED:
	{
		Q931Ies* iesNew = new Q931Ies();
		iesNew->merge(ies);

		Q931IeChannelIdentification *ci = new Q931IeChannelIdentification();
		ci->setInterface_id_present(Q931_IE_CI_IIP_IMPLICIT);
		ci->setInterface_type(interface->getIntftype());
		ci->setPreferred_exclusive(Q931_IE_CI_PE_EXCLUSIVE);
		ci->setCoding_standard(Q931_IE_CI_CS_CCITT);
		ci->addChannel(channel);
		iesNew->add_put(ci);

		q931_call_send_call_proceeding(iesNew);

		channel->connect();
		setState(N3_OUTGOING_CALL_PROCEEDING);
        delete iesNew;
	}
	break;

	case N2_OVERLAP_SENDING:
		q931CallStopTimer(Q931_T302);
		q931_call_send_call_proceeding(ies);
		setState(N3_OUTGOING_CALL_PROCEEDING);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
  }
}

void Q931_Call::q931ProgressRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Progress Request: " + st);

  switch (state)
  {
	case N2_OVERLAP_SENDING:
	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
		q931_call_send_progress(ies);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
  }
}

void Q931_Call::q931RejectRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Reject Request: " + st);

  switch (state)
  {
	case N1_CALL_INITIATED:
		q931_call_send_release_complete(ies);
		channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931ReleaseRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Release Request: " + st);

  switch (state)
  {
	case N11_DISCONNECT_REQUEST:
	{
		Q931Ies* iesNew = new Q931Ies();
		//iesNew->merge(ies);

		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(q931IeCauseLocation());
        cause->setValue( Q931_IE_C_CV_NORMAL_CALL_CLEARING);

        struct q931_ie_cause_diag_1_2 *diag = (struct q931_ie_cause_diag_1_2 *)cause->getDiagnostics();
        memset(diag, 0x00, sizeof(*diag));
        diag->ext1 = 0;
        diag->attribute_number = 0;
        diag->ext2 = 1;
        diag->condition = Q931_IE_C_D_C_PERMANENT;
        cause->setDiagnostics_len (sizeof(*diag));
        iesNew->add_put(cause);

        q931_call_send_release(iesNew);
		q931CallStartTimer(Q931_T308);
		setState(N19_RELEASE_REQUEST);
        delete iesNew;

	}
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931ResumeRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Resume Request: " + st);

  q931CallUnexpectedPrimitive(__FUNCTION__);
}

void Q931_Call::q931ResumeRejectRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Resume Reject Request: " + st);

  switch (state)
  {
	case N17_RESUME_REQUEST:
		// Ensure the lib's user specified IE cause

		q931_call_send_resume_reject(ies);
		q931CallReleaseReference();
		setState(N0_NULL_STATE);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931ResumeResponse(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Resume Response: " + st);

  switch (state)
  {
	case N17_RESUME_REQUEST:
		q931_call_send_resume_acknowledge(ies);
		setState(N10_ACTIVE);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931SetupCompleteRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Setup Complete Request: " + st);

  switch (state)
  {
	case N8_CONNECT_REQUEST:
		channel->connect();

		if (broadcast_setup) {
			selected_ces = preselected_ces;
			//Q931_DataLink* dlc = preselected_ces->getDlc();

			m_dlc = preselected_ces->getDlc();
			// TODO: q931_dlc_hold(dlc);

			q931_call_send_connect_acknowledge(ies);

			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_NON_SELECTED_USER_CLEARING);
			iesNew->add_put(cause);

			std::list<Q931_Ces*>::iterator ces_iter;
			for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
			{
			  if( (*ces_iter)->getDlc() != selected_ces->getDlc())
				(*ces_iter)->q931_ces_release_request(iesNew);
			}
			delete iesNew;

			// TODO: q931_ces_free(call->selected_ces);
			removeCes(selected_ces);
			delete selected_ces;
		}
		else
		{
			q931_call_send_connect_acknowledge(ies);
		}

		setState(N10_ACTIVE);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931SetupRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Setup Request: " + st);

  switch (state)
  {
	case N0_NULL_STATE:
	{
		// TODO: q931_channel_acquire(call);
		if (channel || interface->getEnable_bumping())
		{
		   q931CallStartTimer(Q931_T303);
                   //setup_ies = ies;
                   setup_ies->copy(ies);
           /* Q931IeChannelIdentification *ci = new Q931IeChannelIdentification();
            ci->setInterface_id_present(Q931_IE_CI_IIP_IMPLICIT);
            ci->setInterface_type(interface->getIntftype());
            ci->setCoding_standard(Q931_IE_CI_CS_CCITT);
            ci->addChannel(channel);
            setup_ies->add_put(ci);

            Q931IeDateTime* dt = new Q931IeDateTime();
			// should be initialized in the constructor	dt->time = time(NULL);
            setup_ies->add_put(dt);
*/
			if (interface->getMode() == LAPD_INTF_MODE_MULTIPOINT)
			{
			  //ci->setPreferred_exclusive(Q931_IE_CI_PE_EXCLUSIVE);
                          broadcast_setup = true;
			  q931CallStartTimer(Q931_T312);
                          q931_call_send_setup_bc(setup_ies);
			}
			else
			{
				//ci->setPreferred_exclusive(Q931_IE_CI_PE_PREFERRED);
				q931_call_send_setup(setup_ies);
			}

			setState(N6_CALL_PRESENT);

		}
		else
		{
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_NO_CIRCUIT_CHANNEL_AVAILABLE);
            iesNew->add_put(cause);

			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, iesNew);
			delete iesNew;
		}

	}
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931SsetupResponse(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Setup Response: " + st);

	Q931Ies* iesNew = new Q931Ies();
	iesNew->merge(ies);

	switch (state)
	{
	case N2_OVERLAP_SENDING:
	{
		Q931IeDateTime* dt = new Q931IeDateTime();
		// should be initialized in the constructor	dt->time = time(NULL);
		iesNew->add_put(dt);

		q931CallStopTimer(Q931_T302);
		q931_call_send_connect(iesNew);
		setState(N10_ACTIVE);
	}
	break;

	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED: {
		Q931IeDateTime* dt = new Q931IeDateTime();
		// should be initialized in the constructor	dt->time = time(NULL);
		iesNew->add_put(dt);

		q931_call_send_connect(iesNew);
		setState(N10_ACTIVE);
	}
	break;
	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
    delete iesNew;
}

void Q931_Call::q931StatusEnquiryRequest(class Q931_Ces *ces,class Q931Ies *ies)
{

   std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Status Enquiry Request: " + st);

  switch (state) {
	case N0_NULL_STATE:
	case N6_CALL_PRESENT:
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N19_RELEASE_REQUEST:
	case N25_OVERLAP_RECEIVING:
		if (broadcast_setup)
		{
		  if (state != N6_CALL_PRESENT)
		  {
			if (ces)
			{
				ces->q931_ces_status_enquiry_request(ies);
			}
			else
			{
			    std::list<Q931_Ces*>::iterator ces_iter;
			  for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
			  {
			  	(*ces_iter)->q931_ces_status_enquiry_request(ies);
			  }

			}
		  }
		}
		else
		{
		  if (!q931CallTimerRunning(Q931_T322))
		  {
			q931_call_send_status_enquiry(ies);
			senq_status_97_98_received = 0;
			senq_cnt = 0;
			q931CallStartTimer(Q931_T322);
		  }
		}
	break;

	default:
		if (!q931CallTimerRunning(Q931_T322)) {
			q931_call_send_status_enquiry(ies);
			senq_status_97_98_received = 0;
			senq_cnt = 0;
			q931CallStartTimer(Q931_T322);
		}
	break;
	}
}

void Q931_Call::q931SuspendRejectRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Suspend Reject Request: " + st);
  switch (state) {
	case N15_SUSPEND_REQUEST:
		q931_call_send_suspend_reject(ies);
		setState(N10_ACTIVE);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931SuspendResponse(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Suspend Response: " + st);

	switch (state) {
	case N15_SUSPEND_REQUEST:
		q931_call_send_suspend_acknowledge(ies);
		q931CallReleaseReference();
		setState(N0_NULL_STATE);
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}
}

void Q931_Call::q931SuspendRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Suspend Request: " + st);

	q931CallUnexpectedPrimitive(__FUNCTION__);
}

void Q931_Call::q931RestartRequest(class Q931Ies *ies)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Restart Request: " + st);

	switch (state) {
	case N0_NULL_STATE:
	case N1_CALL_INITIATED:
	case N2_OVERLAP_SENDING:
	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
	case N6_CALL_PRESENT:
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N10_ACTIVE:
	case N11_DISCONNECT_REQUEST:
	case N12_DISCONNECT_INDICATION:
	case N15_SUSPEND_REQUEST:
	case N17_RESUME_REQUEST:
	case N19_RELEASE_REQUEST:
	case N22_CALL_ABORT:
	case N25_OVERLAP_RECEIVING: {
		Q931_Channel *callchannel = channel;

		q931CallStopAnyTimer();
		channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, ies);
		// interface->global_call->q931_global_restart_confirm(dlc,callchannel);
		interface->getGlobalCall()->restartConfirm(m_dlc, callchannel);
	}
	break;
	}
}

class Q931_Call* Q931_Call::q931CallGet(const char *file, int line)
{

}

int Q931_Call::q931DecodeIes(class Q931_Message *msg)
{
	Q931_DecoderStatus* ds = new Q931_DecoderStatus();
    ds->decodeIes(msg, interface);

	if (ds->countMandatoryIe() != 0) {

		/*struct q931_ieid_entry *entry;
		list_for_each_entry(entry, &ds.mandatory_ies, node) {

			const struct q931_ie_class *ie_class;
			ie_class = q931_get_ie_class(entry->codeset, entry->id);

			assert(ie_class);

			report_msg_cont(msg, LOG_INFO,
				"Mandatory IE %d:%d (%s) is missing\n",
				entry->codeset,
				entry->id,
				ie_class->name);
		}*/
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_MANDATORY_INFORMATION_ELEMENT_IS_MISSING);
        ds->setDiagnosticsMandatoryIe(cause);

		/*cause->diagnostics_len = sizeof(cause->diagnostics);

		q931_ieid_fill_diagnostic_from_list(
			cause->diagnostics, &cause->diagnostics_len,
			&ds.mandatory_ies);
        */
		Q931Ies* ies = new Q931Ies();
		ies->add_put(cause);

		switch(msg->getMessage_type()) {
		case Q931_MT_SETUP:
		case Q931_MT_RELEASE:
			q931_call_send_release_complete(ies);

			delete ies;

			goto do_not_process_message;
		break;

		case Q931_MT_DISCONNECT:
		break;

		case Q931_MT_RELEASE_COMPLETE:
			/* Do nothing and let message pass assuming it was ok */
		break;

		default:
			q931_call_send_status(ies);

			delete ies;

			goto do_not_process_message;
		break;
		}

		delete ies;
	}
	if (ds->countInvalidMandatoryIe() != 0) {

		/*struct q931_ieid_entry *entry;
		list_for_each_entry(entry, &ds.mandatory_ies, node) {

			const struct q931_ie_class *ie_class;
			ie_class = q931_get_ie_class(entry->codeset, entry->id);

			assert(ie_class);

			report_msg_cont(msg, LOG_INFO,
				"Mandatory IE %d:%d (%s) is missing\n",
				entry->codeset,
				entry->id,
				ie_class->name);
		}*/
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_INVALID_INFORMATION_ELEMENT_CONTENTS);
		ds->setDiagnosticsInvalidMandatoryIe(cause);

    	Q931Ies* ies = new Q931Ies();
		ies->add_put(cause);


		switch(msg->getMessage_type()) {
		case Q931_MT_SETUP:
		case Q931_MT_RELEASE:
			q931_call_send_release_complete(ies);
            delete ies;
			goto do_not_process_message;
		break;

		case Q931_MT_DISCONNECT:
		break;

		case Q931_MT_RELEASE_COMPLETE:
			/* Do nothing and let message pass assuming it was ok */
		break;

		default:
			q931_call_send_status(ies);
            delete ies;
			goto do_not_process_message;
		break;
		}
        delete ies;
    }

	if (ds->countInvalidAccessIe() != 0) {
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
     	cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_ACCESS_INFORMATION_DISCARDED);
		ds->setDiagnosticsInvalidAccessIe(cause);

		Q931Ies* ies = new Q931Ies();
		ies->add_put(cause);



/*
		q931_ieid_fill_diagnostic_from_list(
			cause->diagnostics, &cause->diagnostics_len,
			&ds.invalid_access_ies);
*/
		q931_call_send_status(ies);
        delete ies;
	}

	if (ds->countInvalidOptionalIe() != 0) {

		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
     	cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_INVALID_INFORMATION_ELEMENT_CONTENTS);
		ds->setDiagnosticsInvalidOptionalIe(cause);

		Q931Ies* ies = new Q931Ies();
		ies->add_put(cause);
/*
		q931_ieid_fill_diagnostic_from_list(
			cause->diagnostics, &cause->diagnostics_len,
			&ds.invalid_access_ies);
*/
		q931_call_send_status(ies);
        delete ies;
	}

	if (ds->countUnrecognizedIe() != 0) {

		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
     	cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_INFORMATION_ELEMENT_NON_EXISTENT);
		ds->setDiagnosticsInvalidOptionalIe(cause);

		Q931Ies* ies = new Q931Ies();
		ies->add_put(cause);
/*
		q931_ieid_fill_diagnostic_from_list(
			cause->diagnostics, &cause->diagnostics_len,
			&ds.invalid_access_ies);
*/
		switch(msg->getMessage_type()) {
		case Q931_MT_SETUP: {
			q931_call_send_release_complete(ies);
            delete ies;
			goto do_not_process_message;
		}
		break;

		case Q931_MT_DISCONNECT:
		break;

		case Q931_MT_RELEASE: {
			q931_call_send_release_complete(ies);
            delete ies;
			goto do_not_process_message;
		}
		break;

		case Q931_MT_RELEASE_COMPLETE:
			// Ignore content errors in RELEASE_COMPLETE
		break;

		default:
			q931_call_send_status(ies);
            delete ies;
			goto do_not_process_message;
		break;
		}
        delete ies;
	}

	delete ds;

	return 0;

do_not_process_message:

	delete ds;

	return -1;
}

class Q931_Ces* Q931_Call::getCes(Q931_DataLink* dlc)
{
  Application::instance().logger().debug("Call Get CES");

  std::list<Q931_Ces*>::iterator ces;
  for(ces=m_ces.begin(); ces != m_ces.end(); ++ces)
  {
    if (dlc == (*ces)->getDlc())
      {
        return (*ces);
      }
  }
  return 0;
}


int Q931_Call::q931_timer_pending(int timerid)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer Pending: " + st);

	return 0;
}

int Q931_Call::q931CallTimerRunning( Q931Timers timerId)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer Running: " + st + NumberFormatter::format(timerId));

  return 0;
}

Q931_Call* Q931_Call::q931CallAlloc(Q931Interface* intf)
{
	/*Q931Call* call;

	call = new Q931Call(intf, 1);
	if (!call)
		return 0;

	call->setState(Q931_Call::N0_NULL_STATE);

	/*INIT_LIST_HEAD(&call->ces);

	q931_ies_init(&call->setup_ies);

	q931_ies_init(&call->disconnect_cause);
	q931_ies_init(&call->saved_cause);
	q931_ies_init(&call->release_cause);

	q931_init_timer(&call->T301, "T301", q931_timer_T301, call);
	q931_init_timer(&call->T302, "T302", q931_timer_T302, call);
	q931_init_timer(&call->T303, "T303", q931_timer_T303, call);
	q931_init_timer(&call->T304, "T304", q931_timer_T304, call);
	q931_init_timer(&call->T305, "T305", q931_timer_T305, call);
	q931_init_timer(&call->T306, "T306", q931_timer_T306, call);
	q931_init_timer(&call->T308, "T308", q931_timer_T308, call);
	q931_init_timer(&call->T309, "T309", q931_timer_T309, call);
	q931_init_timer(&call->T310, "T310", q931_timer_T310, call);
	q931_init_timer(&call->T312, "T312", q931_timer_T312, call);
	q931_init_timer(&call->T313, "T313", q931_timer_T313, call);
	q931_init_timer(&call->T314, "T314", q931_timer_T314, call);
	q931_init_timer(&call->T316, "T316", q931_timer_T316, call);
	q931_init_timer(&call->T318, "T318", q931_timer_T318, call);
	q931_init_timer(&call->T319, "T319", q931_timer_T319, call);
	q931_init_timer(&call->T320, "T320", q931_timer_T320, call);
	q931_init_timer(&call->T321, "T321", q931_timer_T321, call);
	q931_init_timer(&call->T322, "T322", q931_timer_T322, call);

	return call;
*/
}

void Q931_Call::q931CallRestartTimer(Q931Timers timerId)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Restart Timer: " + st);

  q931CallStopTimer(timerId);
  q931CallStartTimer(timerId);
}

void Q931_Call::q931CallStartTimer(Q931Timers timerId)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Start Timer: " + st + NumberFormatter::format(timerId));

  if (m_timer[0] == 0)
    m_timer[0] = m_adapter->startTimer(q931CallTimerDuration(timerId),this,timerId);
  else if (m_timer[1] == 0)
      m_timer[1] = m_adapter->startTimer(q931CallTimerDuration(timerId),this,timerId);
  else if (m_timer[2] == 0)
      m_timer[2] = m_adapter->startTimer(q931CallTimerDuration(timerId),this,timerId);

}

void Q931_Call::q931CallStopTimer(Q931Timers timerId)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Stop Timer: " + st + NumberFormatter::format(timerId));

  if ((m_timer[0] != 0) && (m_timer[0]->getTimerId() == timerId))
  {
      m_adapter->stopTimer(m_timer[0]);
      m_timer[0] = 0;
  }
  else if ((m_timer[1] != 0) && (m_timer[1]->getTimerId() == timerId))
  {
    m_adapter->stopTimer(m_timer[1]);
    m_timer[1] = 0;
  }
  else if ((m_timer[2] != 0) && (m_timer[2]->getTimerId() == timerId))
  {
    m_adapter->stopTimer(m_timer[2]);
    m_timer[2] = 0;
  }
}

void Q931_Call::q931CallClearTimer(Q931Timers timerId)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Clear Timer: " + st + NumberFormatter::format(timerId));

  if ((m_timer[0] != 0) && (m_timer[0]->getTimerId() == timerId))
  {
      m_timer[0] = 0;
  }
  else if ((m_timer[1] != 0) && (m_timer[1]->getTimerId() == timerId))
  {
    m_timer[1] = 0;
  }
  else if ((m_timer[2] != 0) && (m_timer[2]->getTimerId() == timerId))
  {
    m_timer[2] = 0;
  }
}


int Q931_Call::q931CallTimerDuration(Q931Timers timerId)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer Duration: " + st + NumberFormatter::format(timerId));

  switch (timerId)
   {
     case Q931_T301:
       return (180 * 1000);
       break;
     case Q931_T302:
       return (12 * 1000);
       break;
     case Q931_T303:
       return (4 * 1000);
       break;
     case Q931_T304:
       return (20 * 1000);
       break;
     case Q931_T305:
       return (30 * 1000);
       break;
     case Q931_T306:
       return (30 * 1000);
       break;
     case Q931_T308:
       return (4 * 1000);
       break;
     case Q931_T309:
       return (6 * 1000);
       break;
     case Q931_T310:
       return (35 * 1000);
       break;
     case Q931_T312:
       return (Q931_T303 + 2 * 1000);
       break;
     case Q931_T313:
       return(0);
       break;
     case Q931_T314:
       return (4 * 1000);
       break;
     case Q931_T316:
       return (120 * 1000);
       break;
    /* case Q931_T317:
       return(60 * 1000);
       break; */
     case Q931_T318:
       return(0);
       break;
     case Q931_T319:
       return(0);
       break;
     case Q931_T320:
       return (30 * 1000);
       break;
     case Q931_T321:
       return (30 * 1000);
       break;
     case Q931_T322:
       return (4 * 1000);
       break;
   }

}

void Q931_Call::q931DispatchMessage(class Q931_Message *msg)
{


	switch (msg->getMessage_type()) {
	case Q931_MT_ALERTING:
		q931HandleAlerting(msg);
	break;

	case Q931_MT_CALL_PROCEEDING:
		q931HandleCallProceeding( msg);
	break;

	case Q931_MT_CONNECT:
		q931HandleConnect( msg);
	break;

	case Q931_MT_CONNECT_ACKNOWLEDGE:
		q931HandleConnectAcknowledge( msg);
	break;

	case Q931_MT_PROGRESS:
		q931HandleProgress( msg);
	break;

	case Q931_MT_SETUP:
		q931HandleSetup( msg);
	break;

	case Q931_MT_SETUP_ACKNOWLEDGE:
		q931HandleSetupAcknowledge( msg);
	break;

	case Q931_MT_DISCONNECT:
		q931HandleDisconnect( msg);
	break;

	case Q931_MT_RELEASE:
		q931HandleRelease( msg);
	break;

	case Q931_MT_RELEASE_COMPLETE:
		q931HandleReleaseComplete( msg);
	break;

	case Q931_MT_STATUS:
		q931HandleStatus( msg);
	break;

	case Q931_MT_STATUS_ENQUIRY:
		q931HandleStatusEnquiry( msg);
	break;

#if 0
	case Q931_MT_USER_INFORMATION:
		q931_handle_user_information( msg);
	break;

	case Q931_MT_SEGMENT:
		q931_handle_segment( msg);
	break;

	case Q931_MT_CONGESTION_CONTROL:
		q931_handle_congestion_control( msg);
	break;
#endif

	case Q931_MT_INFORMATION:
		q931HandleInfo( msg);
	break;

/*	case Q931_MT_FACILITY:
		q931_handle_facility( msg);
	break;

	case Q931_MT_REGISTER:
		q931_handle_register( msg);
	break;
	*/

	case Q931_MT_NOTIFY:
		q931HandleNotify( msg);
	break;


/*	case Q931_MT_HOLD:
		q931_handle_hold( msg);
	break;

	case Q931_MT_HOLD_ACKNOWLEDGE:
		q931_handle_hold_acknowledge( msg);
	break;

	case Q931_MT_HOLD_REJECT:
		q931_handle_hold_reject( msg);
	break;

	case Q931_MT_RETRIEVE:
		q931_handle_retrieve( msg);
	break;

	case Q931_MT_RETRIEVE_ACKNOWLEDGE:
		q931_handle_retrieve_acknowledge( msg);
	break;

	case Q931_MT_RETRIEVE_REJECT:
		q931_handle_retrieve_reject( msg);
	break;*/

	case Q931_MT_RESUME:
		q931HandleResume( msg);
	break;

	case Q931_MT_RESUME_ACKNOWLEDGE:
		q931HandleResumeAcknowledge( msg);
	break;

	case Q931_MT_RESUME_REJECT:
		q931HandleResumeReject( msg);
	break;

	case Q931_MT_SUSPEND:
		q931HandleSuspend(msg);
	break;

	case Q931_MT_SUSPEND_ACKNOWLEDGE:
		q931HandleSuspendAcknowledge( msg);
	break;

	case Q931_MT_SUSPEND_REJECT:
		q931HandleSuspendReject( msg);
	break;

	default: {
       std::string st(this->q931CallStateToText(state));

		Application::instance().logger().debug("Call Dispatch Default Branch: " + st);

		Q931Ies* ies = new Q931Ies();
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(q931IeCauseLocation());
        cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
        ies->add_put(cause);

		if (state == N0_NULL_STATE) {
			q931_call_send_release(ies);
			q931CallStartTimer(Q931_T308);
			setState(N19_RELEASE_REQUEST);
		} else {
			Q931Ies* ies1 = new Q931Ies();
			Q931IeCause *cause1 = new Q931IeCause();
			cause1->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause1->setLocation(q931IeCauseLocation());
			cause1->setValue( Q931_IE_C_CV_MESSAGE_TYPE_NON_EXISTENT_OR_IMPLEMENTED);
			ies1->add_put(cause1);

			q931_call_send_call_status(ies1);
            delete ies1;

		}

		delete ies;


	}
	break;
	}

  //delete msg;
}

enum q931_ie_cause_location Q931_Call::q931IeCauseLocation()
{
  if (interface->getNetwork_role() == Q931_INTF_NET_USER)
    return Q931_IE_C_L_USER;
  else if (interface->getNetwork_role() == Q931_INTF_NET_PRIVATE)
    return q931IeCauseLocationPrivate(direction, interface->getRole());
  else if (interface->getNetwork_role() == Q931_INTF_NET_LOCAL)
    return q931IeCauseLocationLocal(direction, interface->getRole());
  else if (interface->getNetwork_role() == Q931_INTF_NET_TRANSIT)
    return Q931_IE_C_L_TRANSIT_NETWORK;
  else if (interface->getNetwork_role() == Q931_INTF_NET_INTERNATIONAL)
    return Q931_IE_C_L_INTERNATIONAL_NETWORK;

}

enum q931_ie_cause_location Q931_Call::q931IeCauseLocationPrivate(enum Q931_Call::Q931CallDirection direction,
    enum lapd_intf_role role)
{
    if (role == LAPD_INTF_ROLE_NT) {
            if (direction == Q931_CALL_DIRECTION_INBOUND)
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_LOCAL_USER;
            else
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_REMOTE_USER;
    } else {
            if (direction == Q931_CALL_DIRECTION_INBOUND)
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_REMOTE_USER;
            else
                    return Q931_IE_C_L_PRIVATE_NETWORK_SERVING_LOCAL_USER;
    }
}

enum q931_ie_cause_location Q931_Call::q931IeCauseLocationLocal(enum Q931_Call::Q931CallDirection direction,
    enum lapd_intf_role role)
{
  if (role == LAPD_INTF_ROLE_NT) {
          if (direction == Q931_CALL_DIRECTION_INBOUND)
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_LOCAL_USER;
          else
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_REMOTE_USER;
  } else {
          if (direction == Q931_CALL_DIRECTION_INBOUND)
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_REMOTE_USER;
          else
                  return Q931_IE_C_L_PUBLIC_NETWORK_SERVING_LOCAL_USER;
  }
}

enum q931_ie_call_state_value Q931_Call::q931CallStateToIeState(enum Q931_Call::Q931CallState)
{
	switch (state) {
	case N0_NULL_STATE:
		return Q931_IE_CS_N0_NULL_STATE;
	case N1_CALL_INITIATED:
		return Q931_IE_CS_N1_CALL_INITIATED;
	case N2_OVERLAP_SENDING:
		return Q931_IE_CS_N2_OVERLAP_SENDING;
	case N3_OUTGOING_CALL_PROCEEDING:
		return Q931_IE_CS_N3_OUTGOING_CALL_PROCEEDING;
	case N4_CALL_DELIVERED:
		return Q931_IE_CS_N4_CALL_DELIVERED;
	case N6_CALL_PRESENT:
		return Q931_IE_CS_N6_CALL_PRESENT;
	case N7_CALL_RECEIVED:
		return Q931_IE_CS_N7_CALL_RECEIVED;
	case N8_CONNECT_REQUEST:
		return Q931_IE_CS_N8_CONNECT_REQUEST;
	case N9_INCOMING_CALL_PROCEEDING:
		return Q931_IE_CS_N9_INCOMING_CALL_PROCEEDING;
	case N10_ACTIVE:
		return Q931_IE_CS_N10_ACTIVE;
	case N11_DISCONNECT_REQUEST:
		return Q931_IE_CS_N11_DISCONNECT_REQUEST;
	case N12_DISCONNECT_INDICATION:
		return Q931_IE_CS_N12_DISCONNECT_INDICATION;
	case N15_SUSPEND_REQUEST:
		return Q931_IE_CS_N15_SUSPEND_REQUEST;
	case N17_RESUME_REQUEST:
		return Q931_IE_CS_N17_RESUME_REQUEST;
	case N19_RELEASE_REQUEST:
		return Q931_IE_CS_N19_RELEASE_REQUEST;
	case N22_CALL_ABORT:
		return Q931_IE_CS_N22_CALL_ABORT;
	case N25_OVERLAP_RECEIVING:
		return Q931_IE_CS_N25_OVERLAP_RECEIVING;
	}

	return Q931_IE_CS_N0_NULL_STATE;
}

void Q931_Call::q931_call_send_call_status(class Q931Ies *user_ies)
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Send Call Status: " + st);

	Q931Ies* ies = new Q931Ies(*user_ies);
	Q931IeCallState* cs = new Q931IeCallState();
	cs->setCoding_standard(Q931_IE_CS_CS_CCITT);
	cs->setValue(q931CallStateToIeState(state));
	ies->add(cs);
	q931_call_send_status(ies);
	delete ies;
}

void Q931_Call::q931CallUnexpectedPrimitive(const char *event)
{
	// log error
}

//_q931_call_unexpected_timer((call), __FUNCTION__)

void Q931_Call::q931CallUnexpectedTimer(const char *event)
{
	q931CallStateToText(state);
}

void Q931_Call::q931CallMessageNotCompatibleWithState(Q931_Message *msg,
	const char *event,	enum Q931_Call::Q931CallState new_state)
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Message not Compatible with State: " + st);
    // log error
	Q931Ies* ies = new Q931Ies();
	Q931IeCause *cause = new Q931IeCause();
	cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	cause->setLocation(q931IeCauseLocation());

	if (state == N0_NULL_STATE) {
		cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
		ies->add_put(cause);
		q931_call_send_release(ies);
		q931CallStartTimer(Q931_T308);
		setState(N19_RELEASE_REQUEST);
	} else {
		cause->setValue(Q931_IE_C_CV_MESSAGE_NOT_COMPATIBLE_WITH_CALL_STATE);
		__u8 test = msg->getRaw_message_type();
		memcpy(cause->getDiagnostics(), &(test),sizeof(msg->getRaw_message_type()));
		cause->setDiagnostics_len (sizeof(msg->getRaw_message_type()));

		ies->add_put(cause);

		if (new_state != NOT_VALID)
		{
			Q931IeCallState *cs = new Q931IeCallState();
			cs->setCoding_standard(Q931_IE_CS_CS_CCITT);
			cs->setValue(q931CallStateToIeState(state));
			ies->add_put(cs);
		}
		q931_call_send_call_status(ies);
	}
    delete ies;
}

int Q931_Call::q931ChannelSelectResponse(class Q931Ies *ies, class Q931Ies *causes)
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Channel Select Response: " + st);
	Q931IeChannelIdentification *ci = NULL;

	int i;
	for (i=0; i<ies->get_count(); i++) {
	  if (ies->getIe(i)->getId() == Q931_IE_CHANNEL_IDENTIFICATION) {
		ci = (Q931IeChannelIdentification *)ies->getIe(i);
	    break;
	  }
	}

	if (!ci)
	{
	  // No channel identification IE
	  if (!channel)
	  {
	     Application::instance().logger().debug("No Channel Identification IE and no proposed");
          }
	  else
	  {
	     Application::instance().logger().debug("No Channel Identification IE using proposed");
             channel->setCall(this);
             return true;
	  }
	}

	if (!channel)
	{
	  // Ok, we did not indicate a channel so, attempt to use what
	  // other party requests in his response
          std::set<Q931_Channel*> chanset = ci->getChanset();
          for ( std::set< Q931_Channel* >::iterator iter = chanset.begin();
              iter != chanset.end(); iter++)
          {
            if( (*iter)->getState() == Q931_CHANSTATE_AVAILABLE)
            {
	      // Nice, the channel is available
	      Application::instance().logger().debug("No Channel proposed in SETUP, using idicated B channel");
              // report_call(call, LOG_DEBUG,"No channel proposed in setup, ""using indicated channel B%d\n",ci->chanset.chans[i]->id+1);
              channel = (*iter);
              channel->setCall(this);
              channel->setState(Q931_CHANSTATE_SELECTED);
              return true;
	    }
	    else
	    {
	      Application::instance().logger().debug("No Channel proposed in SETUP but indicated channel is unavailable");
	      //report_call(call, LOG_DEBUG,"No channel proposed in setup, ""but indicated channel B%d is ""unavailable\n",ci->chanset.chans[i]->id+1);
	    }
	  }
	  Q931IeCause *cause = new Q931IeCause();
	  cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	  cause->setLocation(q931IeCauseLocation());
	  cause->setValue(Q931_IE_C_CV_CHANNEL_UNACCEPTABLE);
	  causes->add_put(cause);
	  return false;
	}

	// Uh oh, we already indicated the same channel
	//if (q931_chanset_contains(&ci->chanset, call->channel))
	if (channel == *(ci->getChanset().begin()))
	{
	  // Good, the channel is compatible, we will use it
          if (ci->getPreferred_exclusive() == Q931_IE_CI_PE_PREFERRED)
          {
	    // The other party shoudn't have responded
	    // with acceptable alternatives but we are
            // tolerant.
            Application::instance().logger().debug("Party responded with channel alternatives, this is out of standard but tolerable");
            //report_call(call, LOG_WARNING,"Party responded with channel"" alternatives, this is out of"" standard, but tolerable\n");
	   }
           return true;
	 }
	 else
	 {
	   /* Uh, well, we proposed another channel but the other party doesn't agree. We are nice
            * and use her choice. */
	   std::set<Q931_Channel*> chanset = ci->getChanset();
	   for ( std::set< Q931_Channel* >::iterator iter = chanset.begin();
	         iter != chanset.end(); iter++)
	   {
	      if( (*iter)->getState() == Q931_CHANSTATE_AVAILABLE)
	      {
         	if (channel)
         	  channel->release();

		channel = (*iter);
		channel->setCall(this);
                channel->setState(Q931_CHANSTATE_SELECTED);
		return true;
	      }
	    }
            // The other channel is not available too
	    // We're out of luck
	    Q931IeCause *cause = new Q931IeCause();
	    cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	    cause->setLocation(q931IeCauseLocation());
	    cause->setValue(Q931_IE_C_CV_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE);
	    causes->add_put(cause);

	    return false;
	}
	return false;
}

class Q931_Channel *Q931_Call::q931ChannelSelectSetup(class Q931Ies *setup_ies,
	class Q931Ies *causes, int *bumping_needed)
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Channel Select Setup: " + st);
  *bumping_needed = false;

  Q931IeChannelIdentification *ci = NULL;

  int i;
  for(i=0; i<setup_ies->get_count(); i++)
  {
    if (setup_ies->getIe(i)->getId() ==	Q931_IE_CHANNEL_IDENTIFICATION)
    {
      ci = (Q931IeChannelIdentification *)setup_ies->getIe(i);
      break;
    }
  }

  if (!ci)
  {
    // No Channel Identification IE, assuming any channel is ok
    // Presence of CI n->u is already checked in q931_decode_ies
    // report_call(call, LOG_DEBUG,"No channel identification IE,"" assuming any channel available\n");
    int i;
    for (i=0; i<interface->getChannels(); i++)
    {
      int chan = 0;
      if (interface->getChannels() > 2)
        chan = i+1;
      else
        chan = i;
      if (interface->getChannelById(chan)->getState() ==  Q931_CHANSTATE_AVAILABLE)
      {
	return interface->getChannelById(chan);
      }
    }
    // report_call(call, LOG_DEBUG,"No channel identification IE,"" and no channel available\n");
    Q931IeCause *cause = new Q931IeCause();
    cause->setCoding_standard(Q931_IE_C_CS_CCITT);
    cause->setLocation(q931IeCauseLocation());
    cause->setValue( Q931_IE_C_CV_NO_CIRCUIT_CHANNEL_AVAILABLE);
    causes->add_put(cause);

    return NULL;
  }

  if (!ci->getChanset().size())
  {
    *bumping_needed = true;
    return NULL;
  }

  std::set<Q931_Channel*> chanset = ci->getChanset();
  for ( std::set< Q931_Channel* >::iterator iter = chanset.begin();
      iter != chanset.end(); iter++)
  {
    if( (*iter)->getState() == Q931_CHANSTATE_AVAILABLE)
    {
      //report_call(call, LOG_DEBUG,"Requested channel B%d available\n",ci->chanset.chans[i]->id+1);
      return (*iter);
    }

    // The party indicated an unavailable channel
    if (ci->getPreferred_exclusive() == Q931_IE_CI_PE_EXCLUSIVE)
    {
      // Uuops, the party offers no alternative
      // report_call(call, LOG_DEBUG,"Requested channel B%d unavailable and no ""alternatives offered\n",ci->chanset.chans[i]->id+1);
      Q931IeCause *cause = new Q931IeCause();
      cause->setCoding_standard(Q931_IE_C_CS_CCITT);
      cause->setLocation(q931IeCauseLocation());
      cause->setValue( Q931_IE_C_CV_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE);
      causes->add_put(cause);
      return NULL;
    }
  }

  // Let's see any alternative is ok
  std::set<Q931_Channel*> chanset1 = ci->getChanset();
  for ( std::set< Q931_Channel* >::iterator iter1 = chanset1.begin();
     iter1 != chanset1.end(); iter1++)
  {
    if( (*iter1)->getState() == Q931_CHANSTATE_AVAILABLE)
    {
      //report_call(call, LOG_DEBUG,"Requested channel unavailable but "	"alternative B%d is ok\n",call->intf->channels[i].id+1);
      return (*iter1);
    }
  }
  //report_call(call, LOG_DEBUG, "No channel available\n");
  Q931IeCause *cause = new Q931IeCause();
  cause->setCoding_standard(Q931_IE_C_CS_CCITT);
  cause->setLocation(q931IeCauseLocation());
  cause->setValue( Q931_IE_C_CV_NO_CIRCUIT_CHANNEL_AVAILABLE);
  causes->add_put(cause);
  return NULL;
}

void Q931_Call::q931CallPut(const char *file,	int line)
{
	//refcnt--;
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Put: " + st);
	if (!refcnt)
	{
	  if (m_dlc)
	  {
		m_dlc->release();
		// TODO: m_dlc->q931_dlc_put();
	  }
      // free(call);
	}
}

void Q931_Call::q931_timer_T301()
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Tiner T301: " + st);
	switch (state) {
	case N7_CALL_RECEIVED:
	{
		Q931Ies* iesNew = new Q931Ies();
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
		memcpy(cause->getDiagnostics(), "301", 3);
		cause->setDiagnostics_len (3);
		iesNew->add_put(cause);

		if (broadcast_setup)
		{
			std::list<Q931_Ces*>::iterator ces_iter;
			for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
			{
				(*ces_iter)->q931_ces_release_request(iesNew);
			}

			channel->release();

			if (q931CallTimerRunning(Q931_T312)) {
				setState(N22_CALL_ABORT);
			} else {
				setState(N0_NULL_STATE);
				q931CallReleaseReference();
			}
		} else {
			q931_call_send_disconnect(iesNew);
			q931CallStartTimer(Q931_T305);
			setState(N12_DISCONNECT_INDICATION);
		}

		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, NULL);
        delete iesNew;
	}
	break;

	default:
		q931CallUnexpectedPrimitive(__FUNCTION__);
	break;
	}

	q931CallPut(__FILE__, __LINE__);
}

void Q931_Call::q931_timer_T302()
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T302: " + st);
	switch (state) {
	case N2_OVERLAP_SENDING:
	{
		/* Simulates a sending complete to the CCB */
		Q931Ies* iesNew = new Q931Ies();
		Q931IeSendingComplete *sc = new Q931IeSendingComplete();
		iesNew->add_put(sc);

		q931CallPrimitive(Q931_CCB_INFO_INDICATION, iesNew);
		delete iesNew;
	}
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	q931CallPut(__FILE__, __LINE__);
}

void Q931_Call::q931_timer_T303()
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer T303: " + st);

   switch (state) {
	case N6_CALL_PRESENT:
		if (!T303_fired) {
			if (broadcast_setup) {
				if (release_complete_received) {
					channel->release();
					setState(N22_CALL_ABORT);
					q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,NULL);
				} else {
					q931_call_send_setup_bc(setup_ies);
					q931CallStartTimer(Q931_T303);
					q931CallRestartTimer(Q931_T312);
				}
			} else {
				q931_call_send_setup(setup_ies);
				q931CallStartTimer(Q931_T303);
			}
		} else {
			if (broadcast_setup) {
				channel->release();
				setState(N22_CALL_ABORT);

				Q931Ies* iesNew = new Q931Ies();
				Q931IeCause *cause = new Q931IeCause();
				cause->setCoding_standard(Q931_IE_C_CS_CCITT);
				cause->setLocation(q931IeCauseLocation());
				cause->setValue( Q931_IE_C_CV_NO_USER_RESPONDING);
				iesNew->add_put(cause);

				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, NULL);
			} else {
				q931CallStartTimer(Q931_T305);

				Q931Ies* iesNew = new Q931Ies();
				Q931IeCause *cause = new Q931IeCause();
				cause->setCoding_standard(Q931_IE_C_CS_CCITT);
				cause->setLocation(q931IeCauseLocation());
				cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
				memcpy(cause->getDiagnostics(), "303", 3);
				cause->setDiagnostics_len (3);
				iesNew->add_put(cause);

				q931_call_send_disconnect(iesNew);
				setState(N12_DISCONNECT_INDICATION);
				q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION,NULL);
                delete iesNew;
			}
		}
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	T303_fired = true;

	q931CallPut(__FILE__, __LINE__);
}
void Q931_Call::q931_timer_T304()
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer T304: " + st);

	switch (state) {
	case N25_OVERLAP_RECEIVING:
		if (broadcast_setup) {
			channel->release();

			if (q931CallTimerRunning(Q931_T312)) {
				setState(N22_CALL_ABORT);
			} else {
				setState(N0_NULL_STATE);
				q931CallReleaseReference();
			}
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "304", 3);
			cause->setDiagnostics_len (3);
			iesNew->add_put(cause);

			std::list<Q931_Ces*>::iterator ces_iter;
			for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
			{
				(*ces_iter)->q931_ces_release_request(iesNew);
			}
			delete iesNew;

		} else {
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "304", 3);
			cause->setDiagnostics_len (3);
			iesNew->add_put(cause);

			q931_call_send_disconnect(iesNew);
			channel->disconnect();
			q931CallStartTimer(Q931_T305);
			setState(N12_DISCONNECT_INDICATION);
            delete iesNew;
		}

		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, NULL);
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	q931CallPut(__FILE__, __LINE__);
}

void Q931_Call::q931_timer_T305()
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T305: " + st);

	switch (state)
	{
		case N12_DISCONNECT_INDICATION:
			q931_call_send_release(disconnect_cause);
			q931CallStartTimer(Q931_T308);
			setState(N19_RELEASE_REQUEST);
		break;

    	default:
			q931CallUnexpectedTimer(__FUNCTION__);
		break;
	}

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T306()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T306: " + st);

	switch (state) {
	case N12_DISCONNECT_INDICATION:
		channel->stopTone();
		q931_call_send_release(disconnect_cause);
		q931CallStartTimer(Q931_T308);
		setState(N19_RELEASE_REQUEST);
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T308()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T308: " + st);

	switch (state) {
	case N19_RELEASE_REQUEST: {
		if (T308_fired) {
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "308", 3);
			cause->setDiagnostics_len (3);
			iesNew->add_put(cause);
            iesNew->merge(release_cause);

			q931_call_send_release(iesNew);
			q931CallStartTimer(Q931_T308);
            delete iesNew;
		} else {
			if (channel) {
				if (interface->getMode() ==	LAPD_INTF_MODE_MULTIPOINT) {
					channel->release();
				} else {
					channel->setState(Q931_CHANSTATE_MAINTAINANCE);
				}
			}

			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_CONFIRM,	NULL, Q931_RELEASE_CONFIRM_OK);
		}
	}
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	T308_fired = true;

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T309()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T309: " + st);

	switch (state) {
	case N10_ACTIVE:
		channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, NULL);
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T310()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T310: " + st);

	switch (state) {
	case N9_INCOMING_CALL_PROCEEDING:
		if (broadcast_setup) {
			channel->release();

			if (q931CallTimerRunning(Q931_T312)) {
				setState(N22_CALL_ABORT);
			} else {
				setState(N0_NULL_STATE);
				q931CallReleaseReference();
			}
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
     		cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "310", 3);
			cause->setDiagnostics_len (3);
			iesNew->add_put(cause);
	        iesNew->merge(release_cause);

			std::list<Q931_Ces*>::iterator ces_iter;
			for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
			{
				(*ces_iter)->q931_ces_release_request(iesNew);
			}
			delete iesNew;

		} else {
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
     		cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "310", 3);
			cause->setDiagnostics_len (3);
			iesNew->add_put(cause);
			q931_call_send_disconnect(iesNew);
			channel->release();
			q931CallStartTimer(Q931_T305);
			setState( N12_DISCONNECT_INDICATION);
            delete iesNew;
		}

		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, NULL);
	break;

	default:
		q931CallUnexpectedTimer(__FUNCTION__);
	break;
	}

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T312()
{
   std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Timer T312: " + st);

	switch (state) {
	case N7_CALL_RECEIVED: {
		int able_to_proceed = 0; //int able_to_proceed = 1;
		std::list<Q931_Ces*>::iterator ces_iter;
		for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
		{
			if(((*ces_iter)->getState() != I0_NULL_STATE) && ((*ces_iter)->getState() != I19_RELEASE_REQUEST))
			{
				able_to_proceed = 1;
				break;
			}
		}

		if (!able_to_proceed) {
			q931CallStopTimer(Q931_T301);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,	NULL);
		}
	}
	break;

	case N9_INCOMING_CALL_PROCEEDING: {
		int able_to_proceed = 0;
		std::list<Q931_Ces*>::iterator ces_iter;
		for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
		{
			if(((*ces_iter)->getState() != I0_NULL_STATE) && ((*ces_iter)->getState() != I19_RELEASE_REQUEST))
			{
				able_to_proceed = 1;
				break;
			}
		}
		if (!able_to_proceed) {
			q931CallStopTimer(Q931_T310);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,NULL);
		}
	}
	break;

	case N22_CALL_ABORT:
		if (m_ces.empty()) {

			/* This condition is probably wrong, FIXME */
			if (disconnect_indication_sent)
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,NULL);

			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
		}
	break;

	case N25_OVERLAP_RECEIVING: {
		int able_to_proceed = 0;

		// Are other CES able to proceed?
		// FIXME Is this correct? Specs are unclear! FIXME
		std::list<Q931_Ces*>::iterator ces_iter;
		for(ces_iter = m_ces.begin(); ces_iter != m_ces.end();ces_iter++)
		{
		    if(((*ces_iter)->getState() != I0_NULL_STATE) && ((*ces_iter)->getState() != I19_RELEASE_REQUEST))
			{
				able_to_proceed = 1;
				break;
			}
		}

		if (!able_to_proceed) {
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION,	NULL);
		}
	}
	break;

	default:
		// Do nothing
	break;
	}

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T313()
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer T313: " + st);

	q931CallUnexpectedTimer(__FUNCTION__);
	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T314()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T314: " + st);

	// unexpected timer
	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T316()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T316: " + st);

	// unexpected timer
	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T318()
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T318: " + st);

	q931CallUnexpectedTimer(__FUNCTION__);
	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T319()
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Timer T319: " + st);

	if (state == N15_SUSPEND_REQUEST) {
		q931CallPrimitive(Q931_CCB_SUSPEND_CONFIRM,	NULL, Q931_SUSPEND_CONFIRM_TIMEOUT);
		setState(N10_ACTIVE);
	} else {
		q931CallUnexpectedTimer(__FUNCTION__);
	}

	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T320()
{
   std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Timer T320: " + st);

	// unexpected timer
	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T321()
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer T321: " + st);

    // unexpected timer
	q931CallPut(__FILE__, __LINE__);

}
void Q931_Call::q931_timer_T322()
{
  std::string st(this->q931CallStateToText(state));
  Application::instance().logger().debug("Call Timer T322: " + st);

       if (state == N0_NULL_STATE) {
		q931CallUnexpectedTimer(__FUNCTION__);

		q931CallPut(__FILE__, __LINE__);
		return;
	}

	// Status received
	if (senq_status_97_98_received) {
		// Implementation dependent
	} else {
		if (senq_cnt > 3) {
			Q931Ies* iesNew = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_TEMPORARY_FAILURE);
			iesNew->add_put(cause);
			q931_call_send_release(iesNew);
			channel->release();
			q931CallStartTimer(Q931_T308);
			setState( N19_RELEASE_REQUEST);
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, iesNew); // With error?
			delete iesNew;

		} else {
			q931_call_send_status_enquiry(NULL);
			q931CallStartTimer(Q931_T322);
			senq_cnt++;
		}
	}

	q931CallPut(__FILE__, __LINE__);

}



void Q931_Call::q931HandleAlerting(class Q931_Message *msg)
{
   std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Handle Alerting: " + st);

	switch (state) {
	case N6_CALL_PRESENT:
	  Application::instance().logger().trace(" Q931 Call;  N6_CALL_PRESENT; Handle Alerting\n");
		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStopTimer(Q931_T303);
				q931CallStartTimer(Q931_T301);
				setState(N7_CALL_RECEIVED);
				q931CallPrimitive(Q931_CCB_ALERTING_INDICATION, msg->getIes());
				ces->q931_ces_alerting_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
                        delete causes;
		} else {
			q931CallStopTimer(Q931_T303);
			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStartTimer(Q931_T301);
				setState(N7_CALL_RECEIVED);
				q931CallPrimitive(Q931_CCB_ALERTING_INDICATION, msg->getIes());
			} else {
				q931_call_send_release(causes);
				q931CallStartTimer(Q931_T308);
				setState(N19_RELEASE_REQUEST);
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, causes);
			}

		    delete causes;
		}
	break;

	case N7_CALL_RECEIVED:
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Handle Alerting\n");

		if (broadcast_setup) {
			if (q931DecodeIes(msg) < 0)
				break;

			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				ces->q931_ces_alerting_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
            delete causes;

		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	case N8_CONNECT_REQUEST:
	         Application::instance().logger().trace(" Q931 Call;  N8_CONNECT_REQUEST; Handle Alerting\n");

		if (broadcast_setup) {
			if (q931DecodeIes(msg) < 0)
				break;

			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_NON_SELECTED_USER_CLEARING);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
            delete ies;

		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N9_INCOMING_CALL_PROCEEDING; Handle Alerting\n");

	       if (q931DecodeIes( msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStopTimer(Q931_T310);
				q931CallStartTimer(Q931_T301);
				setState( N7_CALL_RECEIVED);
				q931CallPrimitive(Q931_CCB_ALERTING_INDICATION,msg->getIes());
				ces->q931_ces_alerting_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}

			delete causes;
		} else {
			q931CallStopTimer(Q931_T310);
			q931CallStartTimer(Q931_T301);
			setState(N7_CALL_RECEIVED);
			q931CallPrimitive(Q931_CCB_ALERTING_INDICATION, msg->getIes());
		}
	break;

	case N22_CALL_ABORT:
	  {

	          Application::instance().logger().trace(" Q931 Call;  N22_CALL_ABORT; Handle Alerting\n");
				Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
				Q931_Ces *cesOld = this->getCes(msg->getDlc());
				if (cesOld != 0)
				{ this->removeCes(cesOld);
				  delete cesOld;
				}
				addCes(ces);

		// FIXME (see 5.2.5.4 5.3.2(e))
		// NOTE 1. THE CAUSE SENT DEPENDS ON WHETHER T312 IS STILL
		//	   RUNNING OR NOT, WHICH IS INDICATED BY T312 FLAG.
		//	 IF T312 IS STILL RUNNING, THE CAUSE SENT ALSO
		//	   DEPENDS ON WHETHER A NETWORK DISCONNECT INDICATION
		//	   HAS BEEN RECEIVED OR NOT.

		if (q931CallTimerRunning(Q931_T312)) {

			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);

			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;

		} else {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		}
	}
	break;

	case N25_OVERLAP_RECEIVING:
          Application::instance().logger().trace(" Q931 Call;  N25_OVERLAP_RECEIVING; Handle Alerting\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStartTimer(Q931_T301);
				setState( N7_CALL_RECEIVED);
				q931CallPrimitive(Q931_CCB_ALERTING_INDICATION,msg->getIes());
				ces->q931_ces_alerting_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}

            delete causes;
		} else {
			q931CallStopTimer(Q931_T304);
			q931CallStartTimer(Q931_T301);
			setState(N7_CALL_RECEIVED);
			q931CallPrimitive(Q931_CCB_ALERTING_INDICATION,	msg->getIes());
		}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleCallProceeding(class Q931_Message *msg)
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Handle Call Proceeding: " + st);
	switch (state) {
	case N6_CALL_PRESENT:
		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
     			q931CallStopTimer(Q931_T303);
				q931CallStartTimer(Q931_T310);
				setState( N7_CALL_RECEIVED);
				q931CallPrimitive(Q931_CCB_PROCEEDING_INDICATION,msg->getIes());
				ces->q931_ces_call_proceeding_request(msg->getIes());
				setState( N9_INCOMING_CALL_PROCEEDING);
			} else {
				ces->q931_ces_release_request(causes);
			}
            delete causes;
		} else {
			q931CallStopTimer(Q931_T303);
			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStartTimer(Q931_T310);
				setState( N9_INCOMING_CALL_PROCEEDING);
				q931CallPrimitive(Q931_CCB_PROCEEDING_INDICATION,msg->getIes());
			} else {
				q931_call_send_release(causes);
				q931CallStartTimer(Q931_T308);
				setState(N19_RELEASE_REQUEST);
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, causes);
			}
			delete causes;
		}
	break;

	case N7_CALL_RECEIVED:
	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Call Proceeding\n");

		if (broadcast_setup) {
			if (q931DecodeIes(msg) < 0)
				break;
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				ces->q931_ces_call_proceeding_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;
		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	case N8_CONNECT_REQUEST:
		if (broadcast_setup) {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_NON_SELECTED_USER_CLEARING);
			ies->add_put(cause);
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);
            ces->q931_ces_release_request(ies);

			delete ies;
		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	case N22_CALL_ABORT: {
		Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
		Q931_Ces *cesOld = this->getCes(msg->getDlc());
		if (cesOld != 0)
		{ this->removeCes(cesOld);
		  delete cesOld;
		}
		addCes(ces);

		// FIXME (see 5.2.5.4 5.3.2(e))
		// NOTE 1. THE CAUSE SENT DEPENDS ON WHETHER T312 IS STILL
		//	   RUNNING OR NOT, WHICH IS INDICATED BY T312 FLAG.
		//	 IF T312 IS STILL RUNNING, THE CAUSE SENT ALSO
		//	   DEPENDS ON WHETHER A NETWORK DISCONNECT INDICATION
		//	   HAS BEEN RECEIVED OR NOT.

		if (q931CallTimerRunning(Q931_T312)) {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		} else {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		}
	}
	break;

	case N25_OVERLAP_RECEIVING:
		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStartTimer(Q931_T310);
			    setState(N9_INCOMING_CALL_PROCEEDING);
				q931CallPrimitive(Q931_CCB_ALERTING_INDICATION, msg->getIes());
				ces->q931_ces_call_proceeding_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}

			delete causes;
		} else {
			q931CallStopTimer(Q931_T304);
			q931CallStartTimer(Q931_T310);
			setState(N9_INCOMING_CALL_PROCEEDING);
			q931CallPrimitive(Q931_CCB_PROCEEDING_INDICATION,msg->getIes());
		}
	break;


	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleConnect(class Q931_Message *msg)
{
    std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Handle Connect: " + st);

	switch (state) {
	case N6_CALL_PRESENT:
          Application::instance().logger().trace(" Q931 Call;  N6_CALL_PRESENT; Handle Connect\n");

		if (q931DecodeIes(msg) < 0)
			break;
		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStopTimer(Q931_T303);
				preselected_ces = ces;
				setState( N8_CONNECT_REQUEST);
				q931CallPrimitive(Q931_CCB_SETUP_CONFIRM,msg->getIes(),Q931_SETUP_CONFIRM_OK);
				ces->q931_ces_connect_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;
		} else {
			q931CallStopTimer(Q931_T303);
			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				setState( N8_CONNECT_REQUEST);
				q931CallPrimitive(Q931_CCB_SETUP_CONFIRM,msg->getIes(), Q931_SETUP_CONFIRM_OK);
			} else {
				q931_call_send_release(causes);
				q931CallStartTimer(Q931_T308);
				setState(N19_RELEASE_REQUEST);
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, causes);
			}
			delete causes;
		}
	break;

	case N7_CALL_RECEIVED:
        Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Handle Connect :"+ interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStopTimer(Q931_T301);
				preselected_ces = ces;
				setState( N8_CONNECT_REQUEST);
				q931CallPrimitive(Q931_CCB_CONNECT_INDICATION,msg->getIes());
				ces->q931_ces_connect_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;
		} else {
			q931CallStopTimer(Q931_T301);
			setState(N8_CONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_CONNECT_INDICATION,msg->getIes());
		}
	break;

	case N8_CONNECT_REQUEST:
	        Application::instance().logger().trace(" Q931 Call;  N8_CONNECT_REQUEST; Handle Connect\n");

		 if (broadcast_setup) {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_NON_SELECTED_USER_CLEARING);
			ies->add_put(cause);
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);
            ces->q931_ces_release_request(ies);

			delete ies;
	           } else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		   }

	break;

	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N9_INCOMING_CALL_PROCEEDING; Handle Connect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStopTimer(Q931_T310);
				preselected_ces = ces;
				setState( N8_CONNECT_REQUEST);
				q931CallPrimitive(Q931_CCB_CONNECT_INDICATION,msg->getIes());
				ces->q931_ces_connect_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;
		} else {
			q931CallStopTimer(Q931_T310);
			setState(N8_CONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_CONNECT_INDICATION,msg->getIes());
		}
	break;

	case N22_CALL_ABORT: {
	        Application::instance().logger().trace(" Q931 Call;  N22_CALL_ABORT; Handle Connect\n");

			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);
		// FIXME (see 5.2.5.4 5.3.2(e))
		// NOTE 1. THE CAUSE SENT DEPENDS ON WHETHER T312 IS STILL
		//	   RUNNING OR NOT, WHICH IS INDICATED BY T312 FLAG.
		//	 IF T312 IS STILL RUNNING, THE CAUSE SENT ALSO
		//	   DEPENDS ON WHETHER A NETWORK DISCONNECT INDICATION
		//	   HAS BEEN RECEIVED OR NOT.
		if (q931CallTimerRunning(Q931_T312)) {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		} else {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		}
	}
	break;

	case N25_OVERLAP_RECEIVING:
          Application::instance().logger().trace(" Q931 Call;  N25_OVERLAP_RECEIVING; Handle Connect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				preselected_ces = ces;
			    setState(N8_CONNECT_REQUEST);
				q931CallPrimitive(Q931_CCB_CONNECT_INDICATION, msg->getIes());
				ces->q931_ces_connect_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}

			delete causes;
		} else {
			q931CallStopTimer(Q931_T304);
			setState(N8_CONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_CONNECT_INDICATION,msg->getIes());
		}
	break;


	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleConnectAcknowledge(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Handle Connect Acknowledge: " + st);

	//q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);

}

void Q931_Call::q931HandleProgress(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Handle Progress: " + st);


	switch (state) {
	case N7_CALL_RECEIVED:
		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			q931CallPrimitive(Q931_CCB_PROGRESS_INDICATION,	msg->getIes());
		}
	break;

	case N9_INCOMING_CALL_PROCEEDING:
		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			q931CallStopTimer(Q931_T310);
			q931CallPrimitive(Q931_CCB_PROGRESS_INDICATION, msg->getIes());
		}
	break;

	case N25_OVERLAP_RECEIVING:
		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			q931CallPrimitive(Q931_CCB_PROGRESS_INDICATION,msg->getIes());
		}
	break;


	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}


void Q931_Call::q931HandleSetup(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Handle Setup: " + st);

	switch (state) {
	case N0_NULL_STATE: {
		if (q931DecodeIes(msg) < 0)
			break;

		int i;
		for (i=0; i<msg->getIes()->get_count(); i++) {
			if (msg->getIes()->getIe(i)->getId() ==	Q931_IE_CALLED_PARTY_NUMBER) {
				Q931IeCalledPartyNumber *cdpn;
				cdpn = (Q931IeCalledPartyNumber*)(msg->getIes()->getIe(i));
                std::string number = cdpn->getNumber();
				if (!number.empty())
					digits_dialled = true;
			}
		}

		Q931Ies* ies = new Q931Ies();

		int bumping_needed;
		Q931_Channel *chan = q931ChannelSelectSetup(msg->getIes(), ies,	&bumping_needed);

		if (chan) {
			channel = chan;
			chan->setCall(this);
			chan->setState(	Q931_CHANSTATE_SELECTED);
                        setup_ies->copy(msg->getIes());

			setState(N1_CALL_INITIATED);
			q931CallPrimitive(Q931_CCB_SETUP_INDICATION, msg->getIes());
		} else {
			q931_call_send_release_complete(ies);
			q931CallReleaseReference();
		}

		delete ies;
	}
	break;

	default:
		// Do nothing
	break;
	}
}

void Q931_Call::q931HandleSetupAcknowledge(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Handle Setup Acknowledge: " + st);

   switch (state) {
	case N6_CALL_PRESENT: {
		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			Q931Ies* causes = new Q931Ies();
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStopTimer(Q931_T303);
				q931CallStartTimer(Q931_T304);
				setState( N25_OVERLAP_RECEIVING);
				q931CallPrimitive(Q931_CCB_MORE_INFO_INDICATION,msg->getIes());
				ces->q931_ces_setup_ack_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;

		} else {
			q931CallStopTimer(Q931_T303);
			Q931Ies* causes = new Q931Ies();

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				q931CallStartTimer(Q931_T304);
				setState( N25_OVERLAP_RECEIVING);
				q931CallPrimitive(Q931_CCB_MORE_INFO_INDICATION,msg->getIes());
			} else {
				q931_call_send_release(causes);
				q931CallStartTimer(Q931_T308);
				setState(N19_RELEASE_REQUEST);
				q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, causes);
			}
			delete causes;
		}
	}
	break;

	case N7_CALL_RECEIVED:
	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Setup Acknowledge\n");

		if (broadcast_setup) {
			if (q931DecodeIes(msg) < 0)
				break;

			Q931Ies* causes = new Q931Ies();
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				ces->q931_ces_setup_ack_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;
		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	case N8_CONNECT_REQUEST:
		if (broadcast_setup) {
			if (q931DecodeIes(msg) < 0)
				break;

			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_NON_SELECTED_USER_CLEARING);
			ies->add_put(cause);
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);
			ces->q931_ces_release_request(ies);
			delete ies;

		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	case N22_CALL_ABORT: {
		Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
		Q931_Ces *cesOld = this->getCes(msg->getDlc());
		if (cesOld != 0)
		{ this->removeCes(cesOld);
		  delete cesOld;
		}
		addCes(ces);
		// FIXME (see 5.2.5.4 5.3.2(e))
		// NOTE 1. THE CAUSE SENT DEPENDS ON WHETHER T312 IS STILL
		//	   RUNNING OR NOT, WHICH IS INDICATED BY T312 FLAG.
		//	 IF T312 IS STILL RUNNING, THE CAUSE SENT ALSO
		//	   DEPENDS ON WHETHER A NETWORK DISCONNECT INDICATION
		//	   HAS BEEN RECEIVED OR NOT.
		if (q931CallTimerRunning(Q931_T312)) {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		} else {
			Q931Ies* ies = new Q931Ies();

			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY);
			memcpy(cause->getDiagnostics(), "312", 3);
			cause->setDiagnostics_len (3);
			ies->add_put(cause);
			ces->q931_ces_release_request(ies);
			delete ies;
		}
	}
	break;

	case N25_OVERLAP_RECEIVING:
		if (broadcast_setup) {
			if (q931DecodeIes(msg) < 0)
				break;

			Q931Ies* causes = new Q931Ies();
			Q931_Ces *ces = new Q931_Ces(m_adapter, this, msg->getDlc());
			Q931_Ces *cesOld = this->getCes(msg->getDlc());
			if (cesOld != 0)
			{ this->removeCes(cesOld);
			  delete cesOld;
			}
			addCes(ces);

			if (q931ChannelSelectResponse(msg->getIes(), causes)) {
				ces->q931_ces_setup_ack_request(msg->getIes());
			} else {
				ces->q931_ces_release_request(causes);
			}
			delete causes;
		} else {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
		}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931SaveCause(class Q931Ies *ies, Q931Ies *src_ies)
{
	int i;
	for (i=0; i<src_ies->get_count(); i++) {
		if (src_ies->getIe(i)->getId() == Q931_IE_CAUSE)
			ies->add(src_ies->getIe(i));
	}
}

void Q931_Call::q931HandleDisconnect(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
	Application::instance().logger().debug("Call Handle Disconnect: " + st);

	switch (state) {
	case N1_CALL_INITIATED:
          Application::instance().logger().trace(" Q931 Call;  N1_CALL_INITIATED; Handle Disconnect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->disconnect();
		q931SaveCause(disconnect_cause, msg->getIes());
		setState(N11_DISCONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
	break;

	case N2_OVERLAP_SENDING:
          Application::instance().logger().trace(" Q931 Call;  N2_OVERLAP_SENDING; Handle Disconnect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T302);
		channel->disconnect();
		q931SaveCause(disconnect_cause, msg->getIes());
		setState(N11_DISCONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
	break;

	case N3_OUTGOING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N3_OUTGOING_CALL_PROCEEDING; Handle Disconnect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->disconnect();
		q931SaveCause(disconnect_cause, msg->getIes());
		setState(N11_DISCONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
	break;

	case N4_CALL_DELIVERED:
          Application::instance().logger().trace(" Q931 Call;  N4_CALL_DELIVERED; Handle Disconnect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->disconnect();
		q931SaveCause(disconnect_cause, msg->getIes());
		setState(N11_DISCONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
	break;

	case N7_CALL_RECEIVED:

          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Handle Disconnect\n");

		/*if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__, Q931_Call::N6_CALL_PRESENT);
		} else {*/
			if (q931DecodeIes(msg) < 0)
				break;

			q931CallStopTimer(Q931_T301);
			channel->disconnect();
			q931SaveCause(disconnect_cause, msg->getIes());
			setState(N11_DISCONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
		//}
	break;

	case N8_CONNECT_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N8_CONNECT_REQUEST; Handle Disconnect\n");

		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__, Q931_Call::N6_CALL_PRESENT);
		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			channel->release();
			q931SaveCause(disconnect_cause, msg->getIes());
			setState(N11_DISCONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
		}
	break;

	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N9_INCOMING_CALL_PROCEEDING; Handle Disconnect\n");

		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__, Q931_Call::N6_CALL_PRESENT);

		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			q931CallStopTimer(Q931_T310);
			channel->disconnect();
			q931SaveCause(disconnect_cause, msg->getIes());
			setState( N11_DISCONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
		}
	break;

	case N10_ACTIVE:
          Application::instance().logger().trace(" Q931 Call;  N10_ACTIVE; Handle Disconnect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->disconnect();
		q931SaveCause(disconnect_cause, msg->getIes());
		setState( N11_DISCONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
	break;

	case N12_DISCONNECT_INDICATION:
          Application::instance().logger().trace(" Q931 Call;  N12_DISCONNECT_INDICATION; Handle Disconnect\n");

		q931CallStopTimer(Q931_T305);
		q931CallStopTimer(Q931_T306);
		channel->stopTone();
		q931_call_send_release(disconnect_cause);
		q931CallStartTimer( Q931_T308);
		setState(N19_RELEASE_REQUEST);
	break;

	case N17_RESUME_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N17_RESUME_REQUEST; Handle Disconnect\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->disconnect();
		q931SaveCause(disconnect_cause, msg->getIes());
		setState(N11_DISCONNECT_REQUEST);
		q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
	break;

	case N19_RELEASE_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N19_RELEASE_REQUEST; Handle Disconnect\n");

		// Do nothing
	break;

	case N25_OVERLAP_RECEIVING:
          Application::instance().logger().trace(" Q931 Call;  N25_OVERLAP_RECEIVING; Handle Disconnect\n");

		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__, Q931_Call::N6_CALL_PRESENT);
		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			q931CallStopTimer(Q931_T304);
			channel->disconnect();
			q931SaveCause(disconnect_cause, msg->getIes());
			setState(N11_DISCONNECT_REQUEST);
			q931CallPrimitive(Q931_CCB_DISCONNECT_INDICATION, msg->getIes());
		}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleRelease(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Release: " + st);

	switch (state) {
	case N0_NULL_STATE:
	{
          Application::instance().logger().trace(" Q931 Call;  N0_NULL_STATE; Handle Release: "+interface->getName()+"\n");

		Q931Ies* ies = new Q931Ies();
		Q931IeCause *cause = new Q931IeCause();
		cause->setCoding_standard(Q931_IE_C_CS_CCITT);
		cause->setLocation(q931IeCauseLocation());
		cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
		ies->add_put(cause);
		q931_call_send_release_complete(ies);
		q931CallReleaseReference();
		delete ies;
	}
	break;

	case N1_CALL_INITIATED:
	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
          Application::instance().logger().trace(" Q931 Call;  N1--N4; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;
	        Application::instance().logger().trace(" Q931 Call;  N1--N4; Handle Release 2: "+interface->getName()+"\n");

		channel->release();
		q931_call_send_release_complete(NULL);
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N2_OVERLAP_SENDING:
          Application::instance().logger().trace(" Q931 Call;  N2_OVERLAP_SENDING; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T302);
		channel->release();
		q931_call_send_release_complete(NULL);
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N6_CALL_PRESENT:
          Application::instance().logger().trace(" Q931 Call;  N6_CALL_PRESENT; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931_call_send_release_complete(NULL);

		if (!broadcast_setup) {
			q931CallStopTimer(Q931_T303);
			channel->release();
			q931CallReleaseReference();
			setState(N0_NULL_STATE);
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N7_CALL_RECEIVED:
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Handle Release\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931_call_send_release_complete(NULL);

		if (!broadcast_setup) {
			q931CallStopTimer(Q931_T302);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N8_CONNECT_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N8_CONNECT_REQUEST; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931_call_send_release_complete(NULL);

		if (!broadcast_setup) {
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N9_INCOMING_CALL_PROCEEDING; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931_call_send_release_complete(NULL);

		if (!broadcast_setup) {
			q931CallStopTimer(Q931_T310);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N10_ACTIVE:
	case N11_DISCONNECT_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N10--N11; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->release();
		q931_call_send_release_complete(NULL);
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N12_DISCONNECT_INDICATION:
          Application::instance().logger().trace(" Q931 Call;  N12_DISCONNECT_INDICATION; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T305);
		q931CallStopTimer(Q931_T306);
		channel->stopTone();
		channel->release();
		q931_call_send_release_complete(NULL);
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N19_RELEASE_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N19_RELEASE_REQUEST; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T308);
		if (channel != 0)
		  channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_CONFIRM, msg->getIes(),Q931_RELEASE_CONFIRM_OK);
	break;

	case N22_CALL_ABORT:
          Application::instance().logger().trace(" Q931 Call;  N22_CALL_ABORT; Handle Release: "+interface->getName()+"\n");

		q931_call_send_release_complete(NULL);
	break;

	case N25_OVERLAP_RECEIVING:
          Application::instance().logger().trace(" Q931 Call;  N25_OVERLAP_RECEIVING; Handle Release: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
			q931_call_send_release_complete(NULL);
		} else {
			q931CallStopTimer(Q931_T304);
			channel->release();
			q931_call_send_release_complete(NULL);
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	default:
          Application::instance().logger().trace(" Q931 Call;  Default; Handle Release: "+interface->getName()+"\n");

		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}

}

void Q931_Call::q931HandleReleaseComplete(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Release Complete: " + st);

	release_complete_received = true;

	switch (state) {
	case N0_NULL_STATE:
          Application::instance().logger().trace(" Q931 Call;  N0_NULL_STATE; Handle Release Complete: "+interface->getName()+"\n");

		// Do nothing
	break;

	case N1_CALL_INITIATED:
          Application::instance().logger().trace(" Q931 Call;  N1_CALL_INITATED; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->release();
		q931CallReleaseReference();
		setState(N0_NULL_STATE);
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N2_OVERLAP_SENDING:
          Application::instance().logger().trace(" Q931 Call;  N2_OVERLAP_SENDING; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T302);
		channel->release();
		q931CallReleaseReference();
		setState( N0_NULL_STATE);
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
          Application::instance().logger().trace(" Q931 Call;  N3--N4; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->release();
		q931CallReleaseReference();
		setState(N0_NULL_STATE);
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N6_CALL_PRESENT:
          Application::instance().logger().trace(" Q931 Call;  N6_CALL_PRESENT; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (broadcast_setup) {
		  // TODO: save cause doesn't work if I enable that statement I have crashes
		  //saved_cause = msg->getIes();
		} else {
			q931CallStopTimer(Q931_T303);
			channel->release();
			q931CallReleaseReference();
			setState( N0_NULL_STATE);

			/* EN 300 403-2 says that the primitive should be
			 * RELEASE-INDICATION, instead, REJECT-INDICATION seems
			 * more sensible:
			 *
			 * q931CallPrimitive(call,
			 *	Q931_CCB_RELEASE_INDICATION, msg->getIes());
			 *
			 */

			q931CallPrimitive(Q931_CCB_REJECT_INDICATION, msg->getIes());
		}
	break;

	case N7_CALL_RECEIVED:
          Application::instance().logger().trace(" Q931 Call;  N7_CALL_RECEIVED; Handle Release Complete\n");

		if (q931DecodeIes( msg) < 0)
			break;

		if (!broadcast_setup) {
			q931CallStopTimer(Q931_T301);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N8_CONNECT_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N8_CONNECT_REQUEST; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (!broadcast_setup) {
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N9_INCOMING_CALL_PROCEEDING:
          Application::instance().logger().trace(" Q931 Call;  N9_INCOMING_CALL_PROCEEDING; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (!broadcast_setup) {
			q931CallStopTimer(Q931_T310);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	case N10_ACTIVE:
	case N11_DISCONNECT_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N10--N11; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N12_DISCONNECT_INDICATION:
          Application::instance().logger().trace(" Q931 Call;  N12_DISCONNECT_INDICATION; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T305);
		q931CallStopTimer(Q931_T306);
		channel->stopTone();
		channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
	break;

	case N19_RELEASE_REQUEST:
          Application::instance().logger().trace(" Q931 Call;  N19_RELEASE_REQUEST; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T308);
		if (channel != 0)
		  channel->release();
		setState(N0_NULL_STATE);
		q931CallReleaseReference();
		q931CallPrimitive(Q931_CCB_RELEASE_CONFIRM, msg->getIes(),Q931_RELEASE_CONFIRM_OK);
	break;

	case N22_CALL_ABORT:
          Application::instance().logger().trace(" Q931 Call;  N22_CALL_ABORT; Handle Release Complete: "+interface->getName()+"\n");

		// Do nothing
	break;

	case N25_OVERLAP_RECEIVING:
          Application::instance().logger().trace(" Q931 Call;  N25_OVERLAP_RECEIVING; Handle Release Complete: "+interface->getName()+"\n");

		if (q931DecodeIes(msg) < 0)
			break;

		if (!broadcast_setup) {
			q931CallStopTimer(Q931_T304);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
	break;

	default:
          Application::instance().logger().trace(" Q931 Call;  DEFAULT; Handle Release Complete: "+interface->getName()+"\n");

		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleStatus(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Status: " + st);

	switch (state) {
	case N0_NULL_STATE: {
		if (q931DecodeIes(msg) < 0)
			break;

		Q931IeCallState *cs;

		int i;
		for (i=0; i<msg->getIes()->get_count(); i++) {
			if (msg->getIes()->getIe(i)->getId() == Q931_IE_CALL_STATE) {
				cs = (Q931IeCallState *) msg->getIes()->getIe(i);
			}
		}

		if (cs->getValue() != q931CallStateToIeState(N0_NULL_STATE)) {
			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_MESSAGE_NOT_COMPATIBLE_WITH_CALL_STATE);
			__u8 test = msg->getRaw_message_type();
			memcpy(cause->getDiagnostics(), &(test),sizeof(msg->getRaw_message_type()));
			cause->setDiagnostics_len (sizeof(msg->getRaw_message_type()));
			ies->add_put(cause);
			q931_call_send_release(ies);
			setState( N19_RELEASE_REQUEST);
			q931CallStartTimer(Q931_T308);
			delete ies;
		}
	}
	break;

	case N19_RELEASE_REQUEST: {
		if (q931DecodeIes(msg) < 0)
			break;

		Q931IeCallState *cs;

		int i;
		for (i=0; i<msg->getIes()->get_count(); i++) {
			if (msg->getIes()->getIe(i)->getId() == Q931_IE_CALL_STATE) {
				cs = (Q931IeCallState *) msg->getIes()->getIe(i);
			}
		}

		if (cs->getValue() != q931CallStateToIeState(N0_NULL_STATE)) {
			q931CallStopTimer(Q931_T308);
			channel->release();
			setState(N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(Q931_CCB_STATUS_INDICATION, msg->getIes(),Q931_STATUS_INDICATION_ERROR);
		}
	}
	break;

	case N1_CALL_INITIATED:
	case N2_OVERLAP_SENDING:
	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
	case N6_CALL_PRESENT:
	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N10_ACTIVE:
	case N11_DISCONNECT_REQUEST:
	case N12_DISCONNECT_INDICATION:
	case N15_SUSPEND_REQUEST:
	case N17_RESUME_REQUEST:
	case N22_CALL_ABORT:
	case N25_OVERLAP_RECEIVING: {
		if (q931DecodeIes(msg) < 0)
			break;

		Q931Ies* ies = new Q931Ies();

		Q931IeCallState *cs;

		int i;
		for (i=0; i<msg->getIes()->get_count(); i++) {
		    if (msg->getIes()->getIe(i)->getId() == Q931_IE_CALL_STATE) {
			cs = (Q931IeCallState *) msg->getIes()->getIe(i);
			}
		    else if(msg->getIes()->getIe(i)->getId() == Q931_IE_CAUSE) {
		    	ies->add_put(msg->getIes()->getIe(i)->copy());
		    }
		}

		if (q931CallTimerRunning(Q931_T322)) {
			/* Todo: if (ies-q931_ies_contain_cause(&ies,
				Q931_IE_C_CV_MESSAGE_TYPE_NON_EXISTENT_OR_IMPLEMENTED) ||
			    q931_ies_contain_cause(&ies,
				Q931_IE_C_CV_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_STATE)) {
				// Save cause and call state (!?!?!?)
				goto skip;
			} else if(q931_ies_contain_cause(&ies,
					Q931_IE_C_CV_RESPONSE_TO_STATUS_ENQUIRY)) {
				q931CallStopTimer(call, T322);
			}
			*/
		}

skip:
		delete ies;

		if (cs->getValue() != q931CallStateToIeState(N0_NULL_STATE)) {
			q931CallStopAnyTimer();
			channel->release();
			setState( N0_NULL_STATE);
			q931CallReleaseReference();
			q931CallPrimitive(	Q931_CCB_STATUS_INDICATION, msg->getIes(),Q931_STATUS_INDICATION_ERROR);
			q931CallPrimitive(	Q931_CCB_RELEASE_INDICATION, msg->getIes());
		}
		else if (cs->getValue() ==	q931CallStateToIeState(state)) {
				// NOTE 1. FURTHER ACTIONS ARE AN
				// IMPLEMENTATION OPTION.
		} else {
			q931CallStopAnyTimer();

			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_MESSAGE_NOT_COMPATIBLE_WITH_CALL_STATE);
			__u8 test = msg->getRaw_message_type();
			memcpy(cause->getDiagnostics(), &(test),sizeof(msg->getRaw_message_type()));
			cause->setDiagnostics_len (sizeof(msg->getRaw_message_type()));
			ies->add_put(cause);
			q931_call_send_release(ies);
			q931CallStartTimer(Q931_T308);
			setState( N19_RELEASE_REQUEST);
			q931CallPrimitive(Q931_CCB_STATUS_INDICATION, msg->getIes(),Q931_STATUS_INDICATION_ERROR);
			q931CallPrimitive(Q931_CCB_RELEASE_INDICATION, msg->getIes());

			delete ies;

		}
	}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleStatusEnquiry(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Status Enquiry: " + st);

	Q931Ies* ies = new Q931Ies();
	Q931IeCause *cause = new Q931IeCause();
	cause->setCoding_standard(Q931_IE_C_CS_CCITT);
	cause->setLocation(q931IeCauseLocation());
	cause->setValue( Q931_IE_C_CV_RESPONSE_TO_STATUS_ENQUIRY);
	ies->add_put(cause);
	Q931IeCallState *cs = new Q931IeCallState();
    cs->setCoding_standard(Q931_IE_CS_CS_CCITT);
    cs->setValue(q931CallStateToIeState(state));
    ies->add_put(cs);

    q931_call_send_status(ies);
	delete ies;
}


void Q931_Call::q931HandleInfo(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Info: " + st);

	switch (state) {
	case N2_OVERLAP_SENDING:
		if (q931DecodeIes(msg) < 0)
			break;

		q931CallStopTimer(Q931_T302);
		channel->stopTone();
		q931CallStartTimer(Q931_T302);
		q931CallPrimitive(Q931_CCB_INFO_INDICATION, msg->getIes());
	break;

	case N3_OUTGOING_CALL_PROCEEDING:
	case N4_CALL_DELIVERED:
	case N10_ACTIVE:
	case N11_DISCONNECT_REQUEST:
	case N12_DISCONNECT_INDICATION:
	case N19_RELEASE_REQUEST:
		if (q931DecodeIes(msg) < 0)
			break;

		q931CallPrimitive(Q931_CCB_INFO_INDICATION, msg->getIes());
	break;

	case N7_CALL_RECEIVED:
	case N8_CONNECT_REQUEST:
	case N9_INCOMING_CALL_PROCEEDING:
	case N25_OVERLAP_RECEIVING:
		if (broadcast_setup) {
			q931CallMessageNotCompatibleWithState(msg, __FUNCTION__, Q931_Call::N6_CALL_PRESENT);
		} else {
			if (q931DecodeIes( msg) < 0)
				break;

			q931CallPrimitive(Q931_CCB_INFO_INDICATION, msg->getIes());
		}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleNotify(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Notify: " + st);

	switch (state) {
	case N10_ACTIVE:
		if (q931DecodeIes( msg) < 0)
			break;

		q931CallPrimitive(Q931_CCB_NOTIFY_INDICATION, msg->getIes());
	break;

	default:
		// Page 154 subclause 5.9 in ETSI EN 300 403-1 V1.3.2 permits
		// the reception of NOTIFY in any state

		// So, do nothing
	break;
	}
}

void Q931_Call::q931HandleResume(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Handle Resume: " + st);

	switch (state) {
	case N0_NULL_STATE:
		if (interface->getType() != LAPD_INTF_TYPE_BRA) {
			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
			ies->add_put(cause);

			q931_call_send_release(ies);
			q931CallStartTimer(Q931_T308);
			setState(N19_RELEASE_REQUEST);
            delete ies;
		} else {
			if (q931DecodeIes(msg) < 0)
				break;

			setState(N17_RESUME_REQUEST);
			q931CallPrimitive(Q931_CCB_RESUME_INDICATION, msg->getIes());
		}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleResumeAcknowledge(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Resume Acknowledge: " + st);

	switch (state) {
	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleResumeReject(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Resume Reject: " + st);

	switch (state) {
		default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleSuspend(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Suspend: " + st);

	switch (state) {
	case N10_ACTIVE: {
		if (interface->getType() != LAPD_INTF_TYPE_BRA) {
			Q931Ies* ies = new Q931Ies();
			Q931IeCause *cause = new Q931IeCause();
			cause->setCoding_standard(Q931_IE_C_CS_CCITT);
			cause->setLocation(q931IeCauseLocation());
			cause->setValue( Q931_IE_C_CV_FACILITY_REJECTED);
			ies->add_put(cause);

			q931_call_send_suspend_reject(ies);
	        delete ies;
		} else {
			if (q931DecodeIes( msg) < 0)
				break;

			setState( N15_SUSPEND_REQUEST);
			q931CallPrimitive(Q931_CCB_SUSPEND_INDICATION, msg->getIes());
		}
	}
	break;

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleSuspendAcknowledge(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
    Application::instance().logger().debug("Call Handle Suspend Acknowledge: " + st);

	switch (state) {

	default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

void Q931_Call::q931HandleSuspendReject(class Q931_Message *msg)
{
	  std::string st(this->q931CallStateToText(state));
   Application::instance().logger().debug("Call Handle Suspend Reject: " + st);

	switch (state) {
		default:
		q931CallMessageNotCompatibleWithState(msg, __FUNCTION__);
	break;
	}
}

int Q931_Call::q931_call_send_alerting(class Q931Ies *ies)
{
   Application::instance().logger().debug("Call Send Alerting: ");

	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_ALERTING, ies);
}

int Q931_Call::q931_call_send_call_proceeding(class Q931Ies *ies)
{
   Application::instance().logger().debug("Call Send Call Proceeding: ");
   return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_CALL_PROCEEDING, ies);
}

int Q931_Call::q931_call_send_connect(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Connect: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_CONNECT, ies);
}

int Q931_Call::q931_call_send_connect_acknowledge(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Connect Acknowledge: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_CONNECT_ACKNOWLEDGE, ies);
}

int Q931_Call::q931_call_send_disconnect(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Disconnect: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_DISCONNECT, ies);
}

int Q931_Call::q931_call_send_information(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Information: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_INFORMATION, ies);
}

int Q931_Call::q931_call_send_notify(class Q931Ies *ies)
{
    Application::instance().logger().debug("Call Send Notify: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_NOTIFY, ies);
}

int Q931_Call::q931_call_send_progress(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Progress: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_PROGRESS, ies);
}

int Q931_Call::q931_call_send_release(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Release: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_RELEASE, ies);
}

int Q931_Call::q931_call_send_release_complete(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Release Complete: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_RELEASE_COMPLETE, ies);
}

int Q931_Call::q931_call_send_resume(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Resume: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_RESUME, ies);
}

int Q931_Call::q931_call_send_resume_acknowledge(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Resume Acknowledge: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_RESUME_ACKNOWLEDGE, ies);
}

int Q931_Call::q931_call_send_resume_reject(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Resume Reject: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_RESUME_REJECT, ies);
}

int Q931_Call::q931_call_send_setup(class Q931Ies *ies)
{
   Application::instance().logger().debug("Call Send Setup: ");
   Q931_DataLink* dlc = this->getInterface()->getDlc(0);
   return m_adapter->q931_call_send_message(this, dlc, Q931_MT_SETUP, ies);
}

int Q931_Call::q931_call_send_setup_bc(class Q931Ies *ies)
{
  Application::instance().logger().debug("Call Send Setup Broadcast: ");
  interface->setSetupSendAsBroadCast(true);
  return m_adapter->q931_call_send_message_bc(this, this->getInterface()->getBc_dlc(), Q931_MT_SETUP, ies);
}

int Q931_Call::q931_call_send_setup_acknowledge(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Setup Acknowledge: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_SETUP_ACKNOWLEDGE, ies);
}

int Q931_Call::q931_call_send_status(class Q931Ies *ies)
{
  Application::instance().logger().debug("Call Send Status: ");
  return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_STATUS, ies);
}

int Q931_Call::q931_call_send_status_enquiry(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Status Enquiry: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_STATUS_ENQUIRY, ies);
}

int Q931_Call::q931_call_send_suspend(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Suspend: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_SUSPEND, ies);
}

int Q931_Call::q931_call_send_suspend_acknowledge(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Suspend Acknowledge: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_SUSPEND_ACKNOWLEDGE, ies);
}

int Q931_Call::q931_call_send_suspend_reject(class Q931Ies *ies)
{
	Application::instance().logger().debug("Call Send Suspend Reject: ");
	return m_adapter->q931_call_send_message(this, this->getDlc(), Q931_MT_SUSPEND_REJECT, ies);
}

void Q931_Call::addDigit(std::string digit)
{
  this->called_Number.append(digit);
}
