/*
 * q931ccbmessage.cpp
 *
 *  Created on: 08.04.2009
 *      Author: thomas
 */

#include "q931ccbmessage.h"
#include "q931call.h"
#include "Q931_Ies.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_Ccb_Message::Q931_Ccb_Message() {
	// TODO Auto-generated constructor stub
  //primitive = 0;

  call=0;
  ies = 0;

  par1 = 0;
  par2 = 0;

}

Q931_Ccb_Message::~Q931_Ccb_Message() {
	// TODO Auto-generated destructor stub
}

void Q931_Ccb_Message::dispatch()
{
  switch (primitive) {
    case Q931_CCB_ALERTING_REQUEST:
            call->q931AlertingRequest(ies);
    break;

    case Q931_CCB_DISCONNECT_REQUEST:
            call->q931DisconnectRequest(ies);
    break;

    case Q931_CCB_INFO_REQUEST:
            call->q931InfoRequest(ies);
    break;

    case Q931_CCB_MORE_INFO_REQUEST:
            call->q931MoreInfoRequest(ies);
    break;

    case Q931_CCB_NOTIFY_REQUEST:
            call->q931NotifyRequest(ies);
    break;

    case Q931_CCB_PROCEEDING_REQUEST:
            call->q931ProceedingRequest(ies);
    break;

    case Q931_CCB_PROGRESS_REQUEST:
            call->q931ProgressRequest(ies);
    break;

    case Q931_CCB_REJECT_REQUEST:
            call->q931RejectRequest(ies);
    break;

    case Q931_CCB_RELEASE_REQUEST:
            call->q931ReleaseRequest(ies);
    break;

    case Q931_CCB_RESUME_REQUEST:
            call->q931ResumeRequest(ies);
    break;

    case Q931_CCB_RESUME_REJECT_REQUEST:
            call->q931ResumeRejectRequest(ies);
    break;

    case Q931_CCB_RESUME_RESPONSE:
            call->q931ResumeResponse(ies);
    break;

    case Q931_CCB_SETUP_COMPLETE_REQUEST:
            call->q931SetupCompleteRequest(ies);
    break;

    case Q931_CCB_SETUP_REQUEST:
            call->q931SetupRequest(ies);
    break;

    case Q931_CCB_SETUP_RESPONSE:
            call->q931SsetupResponse(ies);
    break;

    case Q931_CCB_STATUS_ENQUIRY_REQUEST:
            call->q931StatusEnquiryRequest((Q931_Ces *)par1, ies);
    break;

    case Q931_CCB_SUSPEND_REJECT_REQUEST:
            call->q931SuspendRejectRequest(ies);
    break;

    case Q931_CCB_SUSPEND_RESPONSE:
            call->q931SuspendResponse(ies);
    break;

    case Q931_CCB_SUSPEND_REQUEST:
            call->q931SuspendRequest(ies);
    break;

    default:;
            // Unexpected primitive
    }
}

